#include <iostream>
#include "data_file.h"

//#include "../util/joystick.h"

void testDataFile(){

//    std::string path("~/Desktop/test%/testData%.txt");
//    data_file datafile(path,"~/Desktop/num/testnum");

    std::string path("~/Desktop/testData.csv");
    data_file datafile(path);


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

int main(){
    std::cout << "Hello World!" << std::endl;

    testDataFile();

    return 0;
}
