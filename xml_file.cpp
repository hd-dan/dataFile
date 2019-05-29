#include "xml_file.h"
#include <iostream>

xml_file::xml_file(){

}

xml_file::~xml_file(){
}

xml_file::xml_file(std::string xml_path){
    xml_path_= xml_path;
    pt_= xml_file::readFile(xml_path_);
}

boost::property_tree::ptree xml_file::readFile(std::string xml_path){
    xml_path_= xml_path;
    boost::property_tree::read_xml(xml_path,pt_);
    return pt_;
}

void xml_file::writeFile(std::string xml_path,boost::property_tree::ptree pt){
    boost::property_tree::xml_writer_settings<std::string> settings(' ', 2,"ASCII");
    boost::property_tree::write_xml(xml_path,pt,std::locale(),settings);
    xml_path_= xml_path;
    return;
}

bool xml_file::checkElementExist(std::string element_name){
    boost::optional<boost::property_tree::ptree &> child= pt_.get_child_optional(element_name);
    return !(!child);
}

template <class num>
num xml_file::getXmlVal(std::string element_name){
    num val= pt_.get<num>(element_name);
    return val;
}
template int xml_file::getXmlVal<int>(std::string);
template double xml_file::getXmlVal<double>(std::string);
template float xml_file::getXmlVal<float>(std::string);
template std::string xml_file::getXmlVal<std::string>(std::string);


template <class num>
num xml_file::getXmlVal(std::string element_name, num defaultVal){
    num val= pt_.get<num>(element_name, defaultVal);
    return val;
}
template int xml_file::getXmlVal<int>(std::string, int);
template double xml_file::getXmlVal<double>(std::string, double);
template float xml_file::getXmlVal<float>(std::string, float);
template std::string xml_file::getXmlVal<std::string>(std::string, std::string);



template <class num>
std::vector<num> xml_file::getXmlVect(std::string element_name){
    std::vector<num> data;
    BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt_.get_child(element_name)){
        data.push_back( boost::lexical_cast<num>(v.second.data()) );
    }
    return data;
}
template std::vector<int> xml_file::getXmlVect<int>(std::string);
template std::vector<double> xml_file::getXmlVect<double>(std::string);
template std::vector<float> xml_file::getXmlVect<float>(std::string);
template std::vector<std::string> xml_file::getXmlVect<std::string>(std::string);


std::vector<double> xml_file::getXmlVect_2rad(std::string element_name){
    std::vector<double> data_rad;
    BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt_.get_child(element_name)){
        data_rad.push_back( M_PI/180*boost::lexical_cast<double>(v.second.data()) );
    }
    return data_rad;
}

template <class num>
std::vector<std::vector<num> > xml_file::getXmlMat(std::string element_name, std::vector<std::string> sub_element){
    std::vector<std::vector<num> > data;
    BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt_.get_child(element_name)){
        std::vector<num> data_i;
        std::string data_i_name= v.first.data();
        data_i.push_back(pt_.get<num>(element_name + "." + data_i_name + "." + sub_element.at(0)));
        data_i.push_back(pt_.get<num>(element_name + "." + data_i_name + "." + sub_element.at(1)));

        data.push_back(data_i);
    }
    return data;
}
template std::vector<std::vector<int> > xml_file::getXmlMat<int>(std::string, std::vector<std::string>);
template std::vector<std::vector<double> > xml_file::getXmlMat<double>(std::string, std::vector<std::string>);
template std::vector<std::vector<float> > xml_file::getXmlMat<float>(std::string, std::vector<std::string>);
template std::vector<std::vector<std::string> > xml_file::getXmlMat<std::string>(std::string, std::vector<std::string>);


template <class num>
std::vector<std::vector<num> > xml_file::getXmlMat_append(std::string element_name, std::vector<std::string> sub_element){
    std::vector<std::vector<num> > data;
    BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt_.get_child(element_name)){
        std::vector<num> data_i;
//        std::cout << element_name + "." + v.first.data() <<std::endl;
        BOOST_FOREACH(boost::property_tree::ptree::value_type &v1, pt_.get_child(element_name + "." + v.first.data())){
//            std::cout << element_name + "." + v.first.data() + "." + v1.first.data() <<std::endl;
            BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, pt_.get_child(element_name + "." + v.first.data() + "." + v1.first.data())){
                data_i.push_back(boost::lexical_cast<num>(v2.second.data()));
            }
        }
        data.push_back(data_i);
    }
    return data;
}
template std::vector<std::vector<int> > xml_file::getXmlMat_append<int>(std::string, std::vector<std::string>);
template std::vector<std::vector<double> > xml_file::getXmlMat_append<double>(std::string, std::vector<std::string>);
template std::vector<std::vector<float> > xml_file::getXmlMat_append<float>(std::string, std::vector<std::string>);
template std::vector<std::vector<std::string> > xml_file::getXmlMat_append<std::string>(std::string, std::vector<std::string>);


std::vector<std::vector<double> > xml_file::getXmlMat_2rad(std::string element_name, std::vector<std::string> sub_element){
    std::vector<std::vector<double> > data;
    BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt_.get_child(element_name)){
        std::vector<double> data_i;
        std::string data_i_name= v.first.data();
        data_i.push_back(M_PI/180* pt_.get<double>(element_name + "." + data_i_name + "." + sub_element.at(0), -1000000));
        data_i.push_back(M_PI/180* pt_.get<double>(element_name + "." + data_i_name + "." + sub_element.at(1), 1000000));

        data.push_back(data_i);
    }
    return data;
}

boost::property_tree::ptree xml_file::getPtree(){
    return pt_;
}
