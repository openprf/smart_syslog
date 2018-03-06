/*
@author Aleksey Afanasiev (e-mail: afanasiev-sozvezdie@mail.ru)

@par
        License that can be found in the LICENSE file.
        https://github.com/openprf/smart_syslog
*/

#include "CppStyleLog.h"

CppStyleLog::CppStyleLog(char *config_file_name) : SmartSyslogger(config_file_name, "CppStyleLog")
{

}

void CppStyleLog::someMethod() {
    //do something...
    log(10, "parent someMethod 10");
    log(9, "parent someMethod 9");
    log(LOG_DEBUG, "parent someMethod LOG_DEBUG");
    log(LOG_WARNING, "parent someMethod LOG_WARNING");
    log(LOG_ERR, "parent someMethod LOG_ERR");
    log(LOG_CRIT, "parent someMethod LOG_CRIT");
    log(LOG_EMERG, "parent someMethod LOG_EMERG");
}

CppStyleLog::CppStyleLog(SmartLoggerConfig *_config) : SmartSyslogger(_config, "CppStyleLog")
{

}

void CppStyleLog::someVirtualMethod() {
    log(LOG_DEBUG, "virtual parent someVirtualMethod log");
    log(LOG_WARNING, "virtual parent someVirtualMethod log");
    log(LOG_ERR, "virtual parent someVirtualMethod log");
    log(LOG_CRIT, "virtual parent someVirtualMethod log");
}

CppStyleChild::CppStyleChild(SmartLoggerConfig *_config) :
        CppStyleLog(_config),
        other(CppStyleLog(_config)),
        some_int(42),
        some_string("fourty two")
{
    setLogModuleName("CppStyleChild");
}

void CppStyleChild::someVirtualMethod() {
    logDebug("child virtual someVirtualMethod log");
    logWarn("child virtual someVirtualMethod log");
    logErr("child virtual someVirtualMethod log");
    logCrit("child virtual someVirtualMethod log");

    log(LOG_ERR, "Log incapsulated");
    other.someVirtualMethod();
    other.someMethod();
}

void CppStyleChild::actionChild() {
    logDebug("actionChild log %d %X, %s", some_int, some_int, some_string.c_str());
    logWarn("actionChild log %d %X, %s",  some_int, some_int, some_string.c_str());
    logErr("actionChild log %d %X, %s", some_int,some_int, some_string.c_str());
}

CppStyleLog2::CppStyleLog2(ModuleLogger *log_module) : logger(new SmartSyslogger(log_module))
{

}

void CppStyleLog2::someMethod() {
    logger->log(LOG_DEBUG, "incapsulated someMethod log");
    logger->log(LOG_WARNING, "incapsulated someMethod log");
    logger->log(LOG_ERR, "incapsulated someMethod log");
    logger->log(LOG_CRIT, "incapsulated someMethod log");
}

void CppStyleLog2::someVirtMethod() {
    logger->logDebug("incapsulated virtual someVirtualMethod log");
    logger->logWarn("incapsulated virtual someVirtualMethod log");
    logger->logErr("incapsulated virtual someVirtualMethod log");
    logger->logCrit("incapsulated virtual someVirtualMethod log");
}
