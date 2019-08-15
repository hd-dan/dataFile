#include "df_file.h"

df_file::df_file():fopen_(false){

}

df_file::df_file(std::string path):fopen_(false){
    path_= df_file::processPath(path);
}

df_file::~df_file(){
    df_file::closeFile();
}

void df_file::setPath(std::string path){
    path_= df_file::processPath(path);
    return;
}

std::string df_file::processPath(std::string path){
    if (path.at(0)=='~')
        path= getenv("HOME")+path.substr(1,path.size()-1);
    path_= path;
    return path;
}

bool df_file::openForWrite(){
    std::string dir= path_.substr(0,path_.find_last_of('/'));
    struct stat st;
    if (stat(dir.c_str(),&st)!=0){
        mkdir(dir.c_str(),S_IRWXU);
    }

    if (!file_.is_open()){
       file_.open(path_.c_str(), std::ios::out|std::ios::trunc);
       file_.flush();
       file_.close();
       file_.open(path_.c_str(), std::ios::in|std::ios::out);
    }
    fopen_=file_.is_open();
    return fopen_;
}

bool df_file::closeFile(){
    if (fopen_){
       file_.close();
       fopen_=file_.is_open();
    }
    return (!fopen_);
}


int df_file::readFile(){
    file_.close();
    file_.open(path_,std::ios::in);
    fopen_= file_.is_open();
    if (!fopen_) return -1;

    file_.seekg(0,file_.beg);
    std::string line;
    int i=0;
    while ( std::getline(file_,line)){
        strBuff_+= line;
        if ((strBuff_.find('[')!=std::string::npos && strBuff_.find(']')!=std::string::npos)
                    || (strBuff_.find('[')==std::string::npos && strBuff_.size()>0)){
            printf("%d: %s\n",i,strBuff_.c_str());
            df_file::processElementStr(strBuff_);
            strBuff_= "";
            i++;
        }
    }

    return 1;
}

void df_file::processElementStr(std::string str){

}
