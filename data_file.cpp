#include "data_file.h"


data_file::data_file(std::string path, bool write):path_(path), fopen_(false),openNum_(0){
    if (write)
        data_file::openFile();
    else
        data_file::readFile();
}
data_file::data_file():fopen_(false),openNum_(0){

}

data_file::~data_file(){
    data_file::closeFile();
}

std::string data_file::processPath(std::string path){
    path_= path;
    if (path_.at(0)=='~')
        path_= getenv("HOME")+path_.substr(1);
    if (path_.find("..")==0){
//        std::string pwd= getenv("PWD");
        char tuh[PATH_MAX];
        std::string pwd=getcwd(tuh,sizeof(tuh));
        do{
            pwd= pwd.substr(0,pwd.rfind("/"));
            path_= path_.substr(path_.find("..")+2);
        }while(path_.find("..")<2);
        path_= pwd+path_;

    }else if(path_.at(0)=='.')
        path_= getenv("PWD")+path_.substr(1);
    if (path_.find("/")==path_.npos)
        path_= getenv("PWD")+std::string("/")+path_;

    size_t pos= path_.find("%");
    if (pos==path_.npos){
        fileNum_= data_file::getFileNum(path_);
        size_t dotpos= path_.find(".");
        if (dotpos==std::string::npos)
            dotpos= path_.size();
        path_.insert(dotpos,std::to_string(fileNum_));
    }else{
        fileNum_= data_file::getFileNum(path_.substr(0,pos));
        do {
            path_.erase(pos,1);
            path_.insert(pos,std::to_string(fileNum_));
        }while( (pos=path_.find("%"))!=std::string::npos);
    }
    return path_;
}

int data_file::getFileNum(std::string path){
    std::string pathdir;
    std::string name;
    size_t dirpos= path.rfind("/");
    if (dirpos!=path.npos){
        pathdir= path.substr(0,dirpos);
        name= path.substr(dirpos+1);
        name= name.substr(0,name.find("."));
    }

    int fileNum=0;
    DIR *dir= opendir(pathdir.c_str());
    if (dir){
        struct dirent *ent;
        while((ent=readdir(dir))!=NULL){
            if (!strcmp(ent->d_name,".")) continue;
            if (!strcmp(ent->d_name,"..")) continue;
            if (ent->d_name[0]=='.') continue;

            std::string filename= ent->d_name;
            filename= filename.substr(0,filename.find("."));
            size_t pos= filename.find(name);
            if (pos!=filename.npos){
                filename= filename.substr(pos+name.size());
                int i= extractIntFromStr(filename);
                fileNum= (fileNum<=i)? i+1:fileNum;
            }
        }
    }
    return fileNum;
}

int data_file::extractIntFromStr(std::string numStr){
    unsigned int i=0;
    unsigned int digitSum=0;
    for (unsigned int j=0;j<numStr.size();j++){
        i= (digitSum==0 && isdigit(numStr.at(j)))? j:i;
        if (i!=0 && !isdigit(numStr.at(j)))
            break;
        if (isdigit(numStr.at(j))) digitSum++;
    }
    if (digitSum==0)
        return 0;
    return std::stoi(numStr.substr(i,digitSum));
}

void data_file::processDirectory(std::string dir){
    dir= dir.substr(0,dir.rfind('/'));
    struct stat st;
    if (stat(dir.c_str(),&st)!=0){
        data_file::processDirectory(dir);
        mkdir(dir.c_str(),S_IRWXU);
    }
    return;
}

bool data_file::openFile(){
    return data_file::openFile(path_);
}

bool data_file::openFile(std::string path){
    if (fopen_)
        data_file::closeFile();

    delimiter_= ',';
    path_= data_file::processPath(path);
    data_file::processDirectory(path_);

    if(!file_.is_open()){
       file_.open(path_.c_str(), std::ios::out|std::ios::trunc);
       file_.flush();
       file_.close();
       file_.open(path_.c_str(), std::ios::in|std::ios::out);
    }
    fopen_=file_.is_open();
    openNum_+=float(0.5);
    return fopen_;
}


bool data_file::closeFile(){
    if (fopen_){
       data_file::rmExtraDelimiter();
       file_.close();
       fopen_=file_.is_open();
    }
    return (!fopen_);
}


bool data_file::header(std::string name){
    if (!fopen_) return false;

    std::lock_guard<std::mutex> lockWrite(mtxWrite_);
    file_<< name.c_str()<< delimiter_<<"\t";
    return true;
}

template <class num>
bool data_file::header(std::string name, std::vector<num> x){
    if (!fopen_) return false;

    std::lock_guard<std::mutex> lockWrite(mtxWrite_);
    for (unsigned long i=0; i<x.size();i++){
       file_ << name <<"["<<i<<"]" << delimiter_<<"\t";
    }
    return true;
}
template bool data_file::header<double>(std::string,std::vector<double>);
template bool data_file::header<float>(std::string,std::vector<float>);
template bool data_file::header<int>(std::string,std::vector<int>);


bool data_file::addComment(std::string comment){
    if (!fopen_) return false;
    std::lock_guard<std::mutex> lockWrite(mtxWrite_);
    file_<< "#" << comment << std::endl;
    return true;
}

template <class num>
bool data_file::write(std::vector<num> x){
    if (!fopen_) return false;

    std::lock_guard<std::mutex> lockWrite(mtxWrite_);
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
    if (!fopen_) return false;

    std::lock_guard<std::mutex> lockWrite(mtxWrite_);
    file_ << x << delimiter_<<"\t";
    return true;
}
template bool data_file::write<double>(double);
template bool data_file::write<float>(float);
template bool data_file::write<int>(int);

bool data_file::endLine(){
    if (!fopen_)
        return false;
    std::lock_guard<std::mutex> lockWrite(mtxWrite_);
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

void data_file::recordTarget(std::string name, int &data){
    headerBuf_<< name.c_str()<< delimiter_<<"\t";
    recordBuffer_.push_back(&data);
}

void data_file::recordTarget(std::string name, bool &data){
    headerBuf_<< name.c_str()<< delimiter_<<"\t";
    recordBuffer_.push_back(&data);
}

void data_file::recordTarget(std::string name, double &data){
//    data_file::header(name);
    headerBuf_<< name.c_str()<< delimiter_<<"\t";
    data_file::recordTarget(data);
}

void data_file::recordTarget(std::string name, std::vector<double> &data){
//    data_file::header(name,data);
    for (unsigned long i=0; i<data.size();i++){
        headerBuf_ << name <<"["<<i<<"]" << delimiter_<<"\t";
     }
    data_file::recordTarget(data);
}

void data_file::recordTarget(double &data){
    recordBuffer_.push_back(&data);
}

void data_file::recordTarget(std::vector<double> &data){
    recordBuffer_.push_back(&data);
}

void data_file::record(){
    std::lock_guard<std::mutex> lockWrite(mtxWrite_);
    if ( int(2*openNum_)%2==1 && !headerBuf_.str().empty()){
        file_ << headerBuf_.str();
        openNum_+=float(0.5);
    }

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
    data_file::processHeaders(line);

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
    while( (pos= line.find(delimiter_)) != std::string::npos){
        std::string parseStr= line.substr(0,pos);
        double parseVal= std::stod(parseStr);
        dataVect.push_back(parseVal);
        line.erase(0,pos+1);
    }
    double parseVal= std::stod(line);
    dataVect.push_back(parseVal);

    return dataVect;
}

std::vector<std::string> data_file::processHeaders(std::string line){
    size_t pos=0;
    contentHeader_= std::vector<std::string>(0,"");
    while( (pos= line.find(',')) != std::string::npos){
        std::string parseStr= line.substr(0,pos);
        parseStr.erase(std::remove(parseStr.begin(),parseStr.end(),'\t'),parseStr.end());
        contentHeader_.push_back(parseStr);
        line.erase(0,pos+1);
    }
    line.erase(std::remove(line.begin(),line.end(),'\t'),line.end());
    line.erase(std::remove(line.begin(),line.end(),' '),line.end());
    contentHeader_.push_back(line);

    return contentHeader_;
}

std::vector<std::vector<double> > data_file::getContent(){
    return contentBuf_;
}

std::vector<std::string> data_file::getHeader(){
    return contentHeader_;
}
