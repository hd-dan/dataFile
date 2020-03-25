#ifndef XML_FILE_H
#define XML_FILE_H

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <boost/optional/optional.hpp>

#include <sys/stat.h>
#include <iostream>


class xml_file{
private:
    boost::property_tree::ptree pt_;
    std::string xml_path_;

    std::string processPath(std::string path);
    void processDirectory(std::string dir);

public:
    xml_file();
    xml_file(std::string xml_path);
    ~xml_file();
    boost::property_tree::ptree readFile(std::string xml_path);
    void writeFile(std::string xml_path, boost::property_tree::ptree pt);

    template <class num>
    num getXmlVal(std::string element_name);
    template <class num>
    num getXmlVal(std::string element_name, num defaultVal);

    template <class num>
    std::vector<num> getXmlVect(std::string element_name);
    template <class num>
    std::vector<num> getXmlVect(std::string element_name, num defaultVal);

    template <class num>
    std::vector<std::vector<num> > getXmlMat(std::string element_name,std::vector<std::string> sub_element);
    template <class num>
    std::vector<std::vector<num> > getXmlMat_append(std::string element_name);

    std::vector<double> getXmlVect_2rad(std::string element_name);
    std::vector<std::vector<double> > getXmlMat_2rad(std::string element_name,std::vector<std::string> sub_element);

    bool checkElementExist(std::string element_name);
    boost::property_tree::ptree getPtree();
};

#endif // XML_FILE_H
