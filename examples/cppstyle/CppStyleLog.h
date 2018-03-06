/*
@author Aleksey Afanasiev (e-mail: afanasiev-sozvezdie@mail.ru)

@par
        License that can be found in the LICENSE file.
        https://github.com/openprf/smart_syslog
*/

#ifndef PROJECT_CPPSTYLELOG_H
#define PROJECT_CPPSTYLELOG_H

#include "SmartSyslogger.h"
#include <string>

class CppStyleLog : public SmartSyslogger {

public:
    CppStyleLog(char *config_file_name);

    CppStyleLog(SmartLoggerConfig *_config);

    void someMethod();
    virtual void someVirtualMethod();
};

class CppStyleChild: public CppStyleLog{
public:
    CppStyleChild(SmartLoggerConfig *_config);
    virtual void someVirtualMethod() final;
    void actionChild();

    CppStyleLog other;

    int some_int;
    std::string some_string;
};


class CppStyleLog2{
public:
    CppStyleLog2(ModuleLogger *log_module);

    void someMethod();
    virtual void someVirtMethod();

private:
    SmartSyslogger* logger;
};
#endif //PROJECT_CPPSTYLELOG_H
