#ifndef DF_FILE_H
#define DF_FILE_H

//For Writing data
#include <fstream>
#include <sys/stat.h>

#include <vector>
#include <boost/any.hpp>
#include <map>

class df_file{
private:
    std::fstream file_;
    std::string path_;
    bool fopen_;

    std::string commentStr_;
    std::string transposeStr_;
    std::string nameDelimiter_;
    std::string vectDelimiter_;
    std::string matDelimiter_;

    std::string processPath(std::string path);
    void init();

    std::vector<std::pair<std::string, boost::any> > elements_;

    std::string strBuff_;
    void processElementStr(std::string str, bool fmat);
    std::vector<double> str2vect(std::string valStr);
    std::vector<std::vector<double> > str2mat(std::string valStr);
    std::vector<std::vector<double> > transpose(std::vector<std::vector<double> >mat);

    void writeVal(std::string name, boost::any val);
    void writeVect(std::string name, boost::any vect);
    void writeMat(std::string name, boost::any mat);

public:
    df_file();
    df_file(std::string path);
    ~df_file();
    bool closeFile();

    void setPath(std::string path);
    bool openForWrite();
    int readFile();

    double getElementVal(std::string name);
    std::vector<double> getElementVect(std::string name);
    std::vector<std::vector<double> > getElementMat(std::string name);
    int getElementType(std::string name);

    void addElement(std::string name, double x);
    void addElement(std::string name, std::vector<double> x);
    void addElement(std::string name, std::vector<std::vector<double> > x);

    int changeElement(std::string name, double x);
    int changeElement(std::string name, std::vector<double> x);
    int changeElement(std::string name, std::vector<std::vector<double> > x);

    void clearElements();
    void writeElements();

};

#endif // DF_FILE_H
