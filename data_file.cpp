#include "data_file.h"


data_file::data_file(std::string data_path, bool write):
                            path_(data_path), open_(false){
    if (path_.at(0)=='~')
        path_= getenv("HOME")+path_.substr(1,path_.size()-1);
    if (write)
        data_file::openFile();
    else
        data_file::readFile();
}

data_file::data_file(std::string data_path, std::string num_path):
                            path_(data_path),open_(false){
    int i= getFileNum(num_path);
    for (int j=0;j<2;j++){
        std::size_t pos= path_.find('%');
        if (pos!=std::string::npos){
            path_.erase(pos,1);
            path_.insert(pos,std::to_string(i));
        }
    }
    data_file::openFile();
}

data_file::~data_file(){
    data_file::closeFile();
}

int data_file::getFileNum(std::string num_path){
    if (num_path.at(0)=='~')
        num_path= getenv("HOME")+num_path.substr(1,num_path.size()-1);
    std::string dir= num_path.substr(0,num_path.find_last_of('/'));
    struct stat st;
    if (stat(dir.c_str(),&st)!=0)
        mkdir(dir.c_str(),S_IRWXU);

    std::fstream file;
    int i=0;

    file.open(num_path,std::ios::in);
    if (file.is_open()){
        std::string line;
        std::getline(file,line);
        i= std::stoi(line);
        file.close();
    }
    file.open(num_path, std::ios::out|std::ios::trunc);
    file<<i+1;
    file.close();
    return i;
}

bool data_file::openFile(){
    delimiter_= ',';
    if (path_.at(0)=='~')
        path_= getenv("HOME")+path_.substr(1,path_.size()-1);

    std::string dir= path_.substr(0,path_.find_last_of('/'));
    struct stat st;
    if (stat(dir.c_str(),&st)!=0){
        mkdir(dir.c_str(),S_IRWXU);
    }

    if(!file_.is_open()){
       file_.open(path_.c_str(), std::ios::out|std::ios::trunc);
       file_.flush();
       file_.close();
       file_.open(path_.c_str(), std::ios::in|std::ios::out);
    }
    open_=file_.is_open();
    return open_;
}


bool data_file::closeFile(){
    if (open_){
       data_file::rmExtraDelimiter();
       file_.close();
       open_=file_.is_open();
    }
    return (!open_);
}


bool data_file::header(std::string name){
    if (!open_) return false;

    file_<<name.c_str()<< delimiter_<<"\t";
    return true;
}

template <class num>
bool data_file::header(std::string name, std::vector<num> x){
    if (!open_) return false;

    for (unsigned long i=0; i<x.size();i++){
       file_ << name <<"["<<i<<"]" << delimiter_<<"\t";
    }
    return true;
}
template bool data_file::header<double>(std::string,std::vector<double>);
template bool data_file::header<float>(std::string,std::vector<float>);
template bool data_file::header<int>(std::string,std::vector<int>);


template <class num>
bool data_file::write(std::vector<num> x){
    if (!open_) return false;

    for (unsigned long i=0;i<x.size();i++){
        file_<< x.at(i) << delimiter_<<"\t";
    }
    return true;
}
template bool data_file::write<double>(std::vector<double>);
template bool data_file::write<float>(std::vector<float>);
template bool data_file::write<int>(std::vector<int>);



template <class num>
bool data_file::write(num x){
    if (!open_) return false;

    file_ <<x << delimiter_<<"\t";
    return true;
}
template bool data_file::write<double>(double);
template bool data_file::write<float>(float);
template bool data_file::write<int>(int);

bool data_file::endLine(){
    if (!open_)
        return false;
    file_<<std::endl;
    return true;
}

bool data_file::rmExtraDelimiter(){
    file_.seekg(-2,file_.end);
    char c= file_.get();
    if (c==delimiter_){
        file_.seekg(-2,file_.end);
        file_<< ' ';
        return true;
    }
    return false;
}

void data_file::recordTarget(std::string name, double &data){
    data_file::header(name);
    data_file::recordTarget(data);
}

void data_file::recordTarget(std::string name, std::vector<double> &data){
    data_file::header(name,data);
    data_file::recordTarget(data);
}

void data_file::recordTarget(double &data){
    recordBuffer_.push_back(&data);
}

void data_file::recordTarget(std::vector<double> &data){
    recordBuffer_.push_back(&data);
}

void data_file::record(){
    
    data_file::rmExtraDelimiter();
    data_file::endLine();
    for (unsigned int i=0;i<recordBuffer_.size();i++){
        if (recordBuffer_.at(i).type()==typeid(double *)){
            double data= *boost::any_cast<double *>(recordBuffer_.at(i));
            data_file::write(data);
        }
        else if (recordBuffer_.at(i).type()==typeid (std::vector<double> *)){
            std::vector<double> data=
                    *boost::any_cast<std::vector<double> *>(recordBuffer_.at(i));
            data_file::write(data);

        }
    }
}

std::vector<std::vector<double> > data_file::readFile(){
    file_.close();
    file_.open(path_,std::ios::in);
    file_.seekg(0,file_.beg);

    std::string line;
    std::getline(file_,line);

    while( std::getline(file_,line) ){
        std::vector<double> lineVect= data_file::processLine(line);
        if (contentBuf_.size()==0){
            contentBuf_= std::vector<std::vector<double> >
                    (lineVect.size(),std::vector<double>(0,0));
        }
        for (unsigned int i=0;i<lineVect.size();i++){
            contentBuf_.at(i).push_back(lineVect.at(i));
        }
    }

    return contentBuf_;
}

std::vector<double> data_file::processLine(std::string line){
    size_t pos=0;
    std::vector<double> dataVect;
    while( (pos= line.find(',')) != std::string::npos){
        std::string parseStr= line.substr(0,pos);
        double parseVal= std::stod(parseStr);
        dataVect.push_back(parseVal);
        line.erase(0,pos+1);
    }
    double parseVal= std::stod(line);
    dataVect.push_back(parseVal);

    return dataVect;
}

std::vector<std::vector<double> > data_file::getContent(){
    return contentBuf_;
}
