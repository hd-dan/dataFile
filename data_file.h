#ifndef DATA_FILE_H
#define DATA_FILE_H

#include <vector>

#include <iostream>
#include <string>
#include <sstream>

//For Writing data
#include <fstream>
#include <sys/stat.h>
#include <dirent.h>

#include <boost/any.hpp>
#include <mutex>

class data_file{
private:
    std::fstream file_;
    std::string path_;
    bool fwrite_;
    bool fopen_;
    float openNum_;

    std::string processPath(std::string path);
    void processDirectory(std::string dir);
    int retrieveFileNum(std::string path);
    int extractIntFromStr(std::string numStr);
    int fileNum_;

    char delimiter_;
    bool rmExtraDelimiter();

    std::vector<boost::any> recordBuffer_;
    std::stringstream headerBuf_;

    std::vector<std::vector<double> > contentBuf_;
    std::vector<std::string> contentHeader_;
    std::vector<double> processLine(std::string line);
    std::vector<std::string> processHeaders(std::string line);

    std::mutex mtxWrite_;

public:
    data_file();
    data_file(std::string path, bool write=1);
    ~data_file();
    bool openFile();
    bool openFile(std::string path);
    std::string getPath();
    int getFileNum();

    bool header(std::string name);
    template <class num>
    bool header(std::string name, std::vector<num> x);

    bool addComment(std::string comment);


    template <class num>
    bool write(std::vector<num> x);
    template <class num>
    bool write(num x);

    bool endLine();
    bool closeFile();

    void recordTarget(std::string name, bool &data);
    void recordTarget(std::string name, int &data);
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
