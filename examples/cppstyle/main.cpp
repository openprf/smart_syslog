#include <iostream>
#include "CppStyleLog.h"
#include <memory>

int main() {
    std::cout << "Welcome to avsku_syslog example" << std::endl;

    //C++ SmartLogger example

    //parse logger file
    auto logger_conf = std::make_shared<SmartSyslogHelper>("../cppstyle.json");

    //Create class witch we want to log
    auto logedClass = std::make_shared<CppStyleLog>(logger_conf->getConfig());
    //call some methods
    logedClass->someMethod();
    logedClass->someVirtualMethod();

    //Create a child class
    auto loggerChild = std::make_shared<CppStyleChild>(logger_conf->getConfig());
    //Call parent method
    loggerChild->someMethod();
    //call overwrite method
    loggerChild->someVirtualMethod();
    //call ouwn method
    loggerChild->actionChild();

    //create another child
    auto loggerChild2 = std::make_shared<CppStyleChild>(logger_conf->getConfig());
    //set log module name
    loggerChild2->setLogModuleName("CppStyleChild2");
    //call some methods
    loggerChild2->someMethod();
    loggerChild2->actionChild();

    //Create class from module
    auto logedClass2 = std::make_shared<CppStyleLog2>(logger_conf->module("CppStyleLog2"));
    //call some methods
    logedClass2->someMethod();
    logedClass2->someVirtMethod();

    return 0;
}

