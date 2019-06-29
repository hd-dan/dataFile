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
    
    bool rmExtraDelimiter();

public:
    data_file(std::string data_path);
    data_file(std::string data_path, std::string num_path);
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

};

#endif // DATA_FILE_H
