#include <iostream>
#include "data_file.h"

//#include "../util/joystick.h"

void testDataFile(){

    std::string path("~/Desktop/test%/testData%.csv");
    data_file datafile(path,"~/Desktop/num/testnum",1);

//    std::string path("~/Desktop/testData.csv");
//    data_file datafile(path);


    std::vector<double> v(3,-1);
    std::vector<double> x(2,10);
    std::vector<double> hahaha(2,99);
    double y=9;
    datafile.recordTarget("v",v);
    datafile.recordTarget("x",x);
    datafile.recordTarget("y",y);

    datafile.record();
    v.at(1)=100;
    x= hahaha;
    y=9999;
    datafile.record();

//    joystick js(0);
//    js_state state= js.getState();

//    datafile.recordTarget("axis",state.axis);

//    while(!state.button.at(7)){
//        state= js.getState();

//        datafile.record();
//        usleep(1e5);
//    }

}

void testParse(){
    std::string str= " 123, 13, 22.1, 43, 1.53 ";
    printf("Input: %s\n",str.c_str());
    size_t pos=0;
    std::vector<double> vect;
    while( (pos= str.find(',')) != std::string::npos){
        std::string parseStr= str.substr(0,pos);
        double parseVal= std::stod(parseStr);
        vect.push_back(parseVal);
        str.erase(0,pos+1);
    }
    double parseVal= std::stod(str);
    vect.push_back(parseVal);

    for(unsigned int i=0;i<vect.size();i++){
        printf(" %.3f |",vect.at(i));
    }

}

void readFile(){
    data_file file("~/Desktop/knee.csv",0);
    std::vector<std::vector<double> > content= file.getContent();
    std::vector<std::string> header= file.getHeader();

    for(unsigned i=0;i<content.size();i++){
        printf("%s: ",header.at(i).c_str());
        for(unsigned j=0;j<content.at(i).size();j++){
            printf("%.3f, ",content.at(i).at(j));
        }
        printf("\n\n\n");
    }
}

int main(){
    std::cout << "Hello World!" << std::endl;

//    testDataFile();
//    testParse();
    readFile();

    return 0;
}
