#include "data_file.h"


data_file::data_file(std::string data_path){
    path_= data_path;
    open_= false;
    data_file::openFile();
}

data_file::~data_file(){
    data_file::closeFile();
}

bool data_file::openFile(){
    if (path_.at(0)=='~'){
        path_.erase(0,1);
        std::string prefix("/home/");
        prefix+=getenv("LOGNAME");
        prefix+=path_;
        path_= prefix;
    }

    std::string dir= path_.substr(0,path_.find_last_of('/'));
    struct stat st;
    if (stat(dir.c_str(),&st)!=0){
        mkdir(dir.c_str(),S_IRWXU);
    }

    if(!file_.is_open()){
       file_.open(path_.c_str());
    }
    open_=file_.is_open();
    return open_;
}


bool data_file::closeFile(){
    if (open_){
       file_.close();
       open_=file_.is_open();
    }
    return (!open_);
}


bool data_file::header(std::string name){
    if (!open_) return false;

    file_<<name.c_str()<<"\t";
    return true;
}

template <class num>
bool data_file::header(std::string name, std::vector<num> x){
    if (!open_) return false;

    for (unsigned long i=0; i<x.size();i++){
       file_ << name <<"["<<i<<"]" << "\t";
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
       file_<< x.at(i) <<"\t";
    }
    return true;
}
template bool data_file::write<double>(std::vector<double>);
template bool data_file::write<float>(std::vector<float>);
template bool data_file::write<int>(std::vector<int>);



template <class num>
bool data_file::write(num x){
    if (!open_) return false;

   file_ << x <<"\t";
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
