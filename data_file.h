#ifndef DATA_FILE_H
#define DATA_FILE_H

#include <vector>

#include <iostream>
#include <string>
#include <sstream>

//For Writing data
#include <fstream>
#include <sys/stat.h>

#include <boost/any.hpp>

class data_file{
private:
    std::fstream file_;
    std::string path_;
    bool open_;

    std::vector<boost::any> recordBuffer_;
    int getFileNum(std::string num_path);
    
    char delimiter_;
    bool rmExtraDelimiter();

    std::vector<std::vector<double> > contentBuf_;
    std::vector<std::string> contentHeader_;
    std::vector<double> processLine(std::string line);
    std::vector<std::string> processHeaders(std::string line);

public:
    data_file(std::string data_path, bool write=1);
    data_file(std::string data_path, std::string num_path, bool write=1);
    ~data_file();
    bool openFile();

    bool header(std::string name);
    template <class num>
    bool header(std::string name, std::vector<num> x);


    template <class num>
    bool write(std::vector<num> x);
    template <class num>
    bool write(num x);

    bool endLine();
    bool closeFile();

    void recordTarget(std::string name, double &data);
    void recordTarget(double &data);
    void recordTarget(std::string name, std::vector<double> &data);
    void recordTarget(std::vector<double> &data);
    void record();

    std::vector<std::vector<double> > readFile();
    std::vector<std::vector<double> > getContent();
    std::vector<std::string> getHeader();
};

#endif // DATA_FILE_H
