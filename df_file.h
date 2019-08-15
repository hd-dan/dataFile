#ifndef DF_FILE_H
#define DF_FILE_H

//For Writing data
#include <fstream>
#include <sys/stat.h>

#include <vector>
#include <boost/any.hpp>

class df_file{
private:
    std::fstream file_;
    std::string path_;
    bool fopen_;
    std::string processPath(std::string path);

    std::pair<std::string, boost::any> buffer_;

    std::string strBuff_;
    void processElementStr(std::string str);


public:
    df_file();
    df_file(std::string path);
    ~df_file();
    void setPath(std::string path);
    bool closeFile();
    bool openForWrite();

    int readFile();

};

#endif // DF_FILE_H
