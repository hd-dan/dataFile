#include "df_file.h"

df_file::df_file():fopen_(false),fcomment_(true){
    df_file::init();
}

df_file::df_file(std::string path,bool fcomment):fopen_(false),fcomment_(fcomment){
    df_file::init();
    df_file::setCommentFlag(fcomment);
    df_file::setPath(path);
}

df_file::~df_file(){
    df_file::closeFile();
}

void df_file::init(){
    commentStr_= "#";
    transposeStr_= "'";
    nameDelimiter_= ":";
    vectDelimiter_= ",";
    matDelimiter_= ";";

    commentBuff_= "";
}

void df_file::setPath(std::string path){
    path_= df_file::processPath(path);
    df_file::readFile();
    return;
}

std::string df_file::processPath(std::string path){
    if (path.at(0)=='~')
        path= getenv("HOME")+path.substr(1);
    if (path.find("..")==0){
        std::string pwd= getenv("PWD");
        do{
            pwd= pwd.substr(0,pwd.rfind("/"));
            path= path.substr(path.find("..")+2);
        }while(path.find("..")<2);
        path= pwd+path;

    }else if(path.at(0)=='.')
        path= getenv("PWD")+path.substr(1);
    if (path.find("/")==path.npos)
        path= getenv("PWD")+std::string("/")+path;

    path_= path;
    return path;
}

bool df_file::closeFile(){
    if (fopen_){
       file_.close();
       fopen_=file_.is_open();
    }
    return (!fopen_);
}

int df_file::readFile(std::string path){
    path_= df_file::processPath(path);
    return df_file::readFile();
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
        ssize_t pos= line.find(commentStr_);
        if (pos!=std::string::npos){
            commentBuff_= commentBuff_+ line.substr(pos,std::string::npos)+"\n";
            line= line.substr(0,pos);
        }

        strBuff_+= line;
        bool fmat= (strBuff_.find('[')!=std::string::npos &&
                strBuff_.find(']')!=std::string::npos);
        bool fval= (strBuff_.find('[')==std::string::npos && strBuff_.size()>0);

        if (fmat||fval){
            if (fcomment_) commentBuff_= commentBuff_+commentStr_+strBuff_+"\n";
            df_file::processElementStr(strBuff_,fmat);
            i++;
        }
    }

    return int(elements_.size());
}

void df_file::processElementStr(std::string str,bool fmat){
    str.erase(std::remove(str.begin(),str.end(),' '));
    ssize_t pos= str.find(nameDelimiter_);
    if (pos==std::string::npos) return;

    std::string name= str.substr(0,pos);
    std::string valStr= str.substr(pos+nameDelimiter_.size(),std::string::npos);
    strBuff_= "";

    if (!fmat){
        double val= std::stod(valStr);
        elements_.push_back( std::make_pair(name,val) );
        return;
    }

    bool ftranspose= (valStr.find(transposeStr_)!=std::string::npos);

    valStr= valStr.substr(0, valStr.find("]"));
    valStr= valStr.substr(valStr.find("[")+1, std::string::npos);


    std::vector<std::vector<double> > elementi= df_file::str2mat(valStr);
    if (elementi.size()==1)
        elements_.push_back( std::make_pair(name, elementi.at(0)) );
    else if (elementi.size()>1){
        if (ftranspose)
            elementi= df_file::transpose(elementi);
        elements_.push_back( std::make_pair(name, elementi) );
    }

    return;
}

std::vector<double> df_file::str2vect(std::string valStr){
    ssize_t pos;
    std::string stri;
    std::vector<double> vect;

    while ( ( pos= valStr.find(vectDelimiter_) )!= std::string::npos ) {
        stri= valStr.substr(0,pos);
        vect.push_back( std::stod(stri) );
        valStr= valStr.substr(pos+vectDelimiter_.size(),std::string::npos);
    }
    vect.push_back(std::stod(valStr));
    return vect;
}

std::vector<std::vector<double> > df_file::str2mat(std::string valStr){
    ssize_t pos;
    std::string stri;
    std::vector<std::vector<double> > mat;
    while ( (pos= valStr.find(matDelimiter_))!= std::string::npos){
        stri= valStr.substr(0,pos);
        mat.push_back( df_file::str2vect(stri) );
        valStr= valStr.substr(pos+matDelimiter_.size(),std::string::npos);
    }
    mat.push_back( df_file::str2vect(valStr) );
    return mat;
}

std::vector<std::vector<double> > df_file::transpose(
                                std::vector<std::vector<double> >mat){

    std::vector<std::vector<double> > matT;
    for (unsigned int i=0;i<mat.at(0).size();i++){
        std::vector<double> vect;
        for(unsigned int j=0;j<mat.size();j++){
            vect.push_back( mat.at(j).at(i) );
        }
        matT.push_back(vect);
    }
    return matT;
}



void df_file::addElement(std::string name, double x){
    elements_.push_back( std::make_pair(name,x) );
}
void df_file::addElement(std::string name, std::vector<double> x){
    elements_.push_back( std::make_pair(name,x) );
}
void df_file::addElement(std::string name, std::vector<std::vector<double> > x){
    elements_.push_back( std::make_pair(name,x) );
}

int df_file::changeElement(std::string name, double x){
    for (unsigned int i=0;i<elements_.size();i++){
        if (!elements_.at(i).first.compare(name)){
            elements_.at(i).second= x;
            return int(i);
        }
    }
    elements_.push_back( std::make_pair(name,x) );
    return elements_.size()-1;
}
int df_file::changeElement(std::string name, std::vector<double> x){
    for (unsigned int i=0;i<elements_.size();i++){
        if (!elements_.at(i).first.compare(name)){
            elements_.at(i).second= x;
            return int(i);
        }
    }
    elements_.push_back( std::make_pair(name,x) );
    return elements_.size()-1;
}
int df_file::changeElement(std::string name, std::vector<std::vector<double> > x){
    for (unsigned int i=0;i<elements_.size();i++){
        if (!elements_.at(i).first.compare(name)){
            elements_.at(i).second= x;
            return int(i);
        }
    }
    elements_.push_back( std::make_pair(name,x) );
    return elements_.size()-1;
}

void df_file::clearElements(){
    elements_.clear();
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

void df_file::writeElements(){
    df_file::closeFile();
    df_file::openForWrite();

    if (commentBuff_.size()>0)
        file_<< commentBuff_ << std::endl;

    for (unsigned int i=0;i<elements_.size();i++){
        std::pair<std::string,boost::any> elementi= elements_.at(i);
        if (elementi.second.type()==typeid(double))
            df_file::writeVal(elementi.first, elementi.second);
        else if (elementi.second.type()==typeid(std::vector<double>))
            df_file::writeVect(elementi.first, elementi.second);
        else if (elementi.second.type()==typeid(std::vector<std::vector<double> >))
            df_file::writeMat(elementi.first, elementi.second);
    }

}

void df_file::writeVal(std::string name, boost::any val){
    double x= boost::any_cast<double>(val);
    file_<< name + nameDelimiter_ + " " << x << std::endl;
}

void df_file::writeVect(std::string name, boost::any vect){
    std::vector<double> x= boost::any_cast<std::vector<double> >(vect);
    file_<< name + nameDelimiter_ + " [";

    for (unsigned int i=0;i<x.size();i++){
        file_<< x.at(i);
        if (i!= x.size()-1)
            file_<< vectDelimiter_ << " ";
    }

    file_<< "]" << std::endl;
}

void df_file::writeMat(std::string name, boost::any mat){
    std::vector<std::vector<double> >x= boost::any_cast<
                    std::vector<std::vector<double> > >(mat);
    file_<< name + nameDelimiter_ + " [";

    for (unsigned int i=0;i<x.size();i++){
        for (unsigned int j=0;j<x.at(i).size();j++){
            file_<< x.at(i).at(j);
            if (j!= x.at(i).size()-1)
                file_<< vectDelimiter_ << " ";
        }
        if (i!= x.size()-1)
            file_<< matDelimiter_ << " ";
    }

    file_<< "]" << std::endl;

}


double df_file::getElementVal(std::string name, double x){
    for (unsigned int i=0;i<elements_.size();i++){
        if (!elements_.at(i).first.compare(name)
            && elements_.at(i).second.type()==typeid(double))
            return boost::any_cast<double> (elements_.at(i).second);
    }
//    printf("Element %s Not Found || Type not comply\n",name.c_str());
    return x;
}
double df_file::getElementVal(std::string name){
    return df_file::getElementVal(name,-1);
}

std::vector<double> df_file::getElementVect(std::string name, std::vector<double> x){
    for (unsigned int i=0;i<elements_.size();i++){
        if (!elements_.at(i).first.compare(name)
            && elements_.at(i).second.type()==typeid(std::vector<double>) )
                return boost::any_cast<std::vector<double> >(elements_.at(i).second);
    }
//    printf("Element %s Not Found || Type not comply\n",name.c_str());
    return x;
}
std::vector<double> df_file::getElementVect(std::string name){
    return df_file::getElementVect(name,std::vector<double>(0,-1));
}

std::vector<std::vector<double> > df_file::getElementMat(std::string name,
                                            std::vector<std::vector<double> > x){
    for (unsigned int i=0;i<elements_.size();i++){
        if (!elements_.at(i).first.compare(name)
            && elements_.at(i).second.type()==typeid(
                        std::vector<std::vector<double> >) )
            return boost::any_cast<std::vector<std::vector<double> > >
                                            (elements_.at(i).second);
    }
//    printf("Element %s Not Found || Type not comply\n",name.c_str());
    return x;
}
std::vector<std::vector<double> > df_file::getElementMat(std::string name){
    return df_file::getElementMat(name,
                    std::vector<std::vector<double> >(0,std::vector<double>(0,-1)));
}


int df_file::getElementType(std::string name){
    for (unsigned int i=0;i<elements_.size();i++){
        if (!elements_.at(i).first.compare(name)){
            if (elements_.at(i).second.type()==typeid(double))
                return 1;
            else if (elements_.at(i).second.type()==typeid(std::vector<double>))
                return 2;
            else if (elements_.at(i).second.type()==typeid(std::vector<
                                                           std::vector<double> >))
                return 3;
            return 0;
        }
    }
//    printf("Element %s Not Found\n",name.c_str());
    return -1;
}

void df_file::setCommentFlag(bool fcomment){
    fcomment_= fcomment;
    return;
}
