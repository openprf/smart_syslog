/**
  @info Wrapper of smart_syslog for using in С++ code

  @author Aleksey Afanasiev (e-mail: afanasiev-sozvezdie@mail.ru)

  @par
  License that can be found in the LICENSE file.
  https://github.com/openprf/smart_syslog
 */

#ifndef PROJECT_AVSKUSYSLOGGER_H
#define PROJECT_AVSKUSYSLOGGER_H

#include "smart_syslog.h"
#include <stdarg.h>
#include <cstdio>
#include <stdlib.h>

#include <map>

/**
 * Helper class for storing logging configuration and logmodules objects
 */
class SmartSyslogHelper{
public:
    /**
     * parse logging configuration
     * @param config_file_name
     */
    SmartSyslogHelper(std::string config_file_name) {
        config = smart_syslog_load_config(config_file_name.c_str());
        if(config){
            for (int i = 0; i <config->moduls_list_size ; i++) {
                modules[std::string(config->moduls_list[i].name)] = &config->moduls_list[i];
            }
        }
    }

    /**
     * free resources
     */
    virtual ~SmartSyslogHelper() {
        free(config);
    }

    /**
     * Get module name
     * @param module_name
     * @return pointer to log module or NULL, if module does not exist
     */
    const ModuleLogger* module(std::string module_name){
        ModuleLogger* ret_module;
        try{
            ret_module = modules.at(module_name);
        }catch(const std::out_of_range& ex){
            ret_module = NULL;
        }
        return ret_module;
    }

     /**
     * Get logging configuration
     * @return pointer to log configuration
     */
    const SmartLoggerConfig* getConfig(){return config;}

private:
    SmartLoggerConfig* config;
    std::map<std::string, ModuleLogger*> modules;
};

/**
 * Logger classs
 */
class SmartSyslogger {
public:
    /**
     * Create logger from file
     * @param config_file_name      logger configuration JSON file name
     * @param _module_name          log module name
     */
    SmartSyslogger(const char *config_file_name, const char* _module_name):isConfigInternal(true) {
        config = smart_syslog_load_config(config_file_name);
        module =  smart_syslog_get_module(config, _module_name);
    }

    /**
     * Create logger from logger config
     * @param _config               pointer to log configuration (if NULL log messages wil be disable)
     * @param _module_name          log module name
     */
    SmartSyslogger(SmartLoggerConfig *_config, const char* _module_name):config(_config), isConfigInternal(false) {
        module =  smart_syslog_get_module(config, _module_name);
    }

    /**
     * Create logger from log module object
     * @param _module
     */
    SmartSyslogger(const ModuleLogger* _module) : config(NULL), module(_module), isConfigInternal(false) {}

    virtual ~SmartSyslogger() {
        if(isConfigInternal && config){
            free(config);
        }
    }

    /**
     * Set new log module name
     * @param new_log_module_name       new log module name
     */
    void setLogModuleName(const char* new_log_module_name){
        module =  smart_syslog_get_module(config, new_log_module_name);
    }

    /**
     * Write lof message
     * Example:
     * @code
     * log(LOG_DEBUG, "log some string %s, log some int %d", some_string, some_int);
     * @endcode
     *
     * @param level     Log level from syslog.h
     * @param format    log messages
     * @param ...       ext parameters
     */
    virtual void log(int level, char *format, ...){
        va_list ap;
        va_start(ap, format);
        smart_syslog_log_va_list(module,level,format, ap);
        va_end(ap);
    };

    virtual void logCrit(char *format, ...){
        va_list ap;
        va_start(ap, format);
        smart_syslog_log_va_list(module,LOG_CRIT,format, ap);
        va_end(ap);
    };

    virtual void logErr(char *format, ...){
        va_list ap;
        va_start(ap, format);
        smart_syslog_log_va_list(module,LOG_ERR,format, ap);
        va_end(ap);
    };

    virtual void logWarn(char *format, ...){
        va_list ap;
        va_start(ap, format);
        smart_syslog_log_va_list(module,LOG_WARNING,format, ap);
        va_end(ap);
    };

    virtual void logInfo(char *format, ...){
        va_list ap;
        va_start(ap, format);
        smart_syslog_log_va_list(module,LOG_INFO,format, ap);
        va_end(ap);
    };

    virtual void logDebug(char *format, ...){
        va_list ap;
        va_start(ap, format);
        smart_syslog_log_va_list(module,LOG_DEBUG,format, ap);
        va_end(ap);
    };

    /**
     * get logger config struct
     * @return pointer to log config struct
     */
    const SmartLoggerConfig* configGet(){return const_cast<SmartLoggerConfig*>(config);}
private:
    SmartLoggerConfig* config;
    const ModuleLogger* module;
    bool isConfigInternal;
};

#endif //PROJECT_AVSKUSYSLOGGER_H
