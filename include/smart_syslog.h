#ifndef SMART_SYSLOG_LIBRARY_H
#define SMART_SYSLOG_LIBRARY_H
/**
  @info Extencion of syslog functions aimed at flexible configuration without recompilation.
  Logging configuration sets up in JSON file (library json-c heed to be installed). Configuration file defines
  levels of logging and witch parts of source code have to be logged.

  @author Aleksey Afanasiev (e-mail: afanasiev-sozvezdie@mail.ru)

  @par
  License that can be found in the LICENSE file.
  https://github.com/openprf/smart_syslog
**/


#include <syslog.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif
#define MAX_MODULE_NAME_LEN 64

#pragma pack(push,1)

typedef struct _ModuleLogger {
    unsigned int levels_mask;
    char name[MAX_MODULE_NAME_LEN];
} ModuleLogger;

typedef struct _smartLoggerConfig {
    unsigned int moduls_list_size;
    ModuleLogger moduls_list[1];
} SmartLoggerConfig;
#pragma pack(pop)

/**
 * Parsing of setting file.
 * IMPORTANT: you heed free allocated in smart_syslog_load_config() memory manualy like this:
 * @code
 * int main()
 * {
 *  //...
 *  SmartLoggerConfig *log_config = smart_syslog_load_config("myapplogconfig.json");
 *  //...
 *  //Do something
 *  //...
 *  free(log_config)
 * }
 * If config_file_name NULL or file does not exist or parsing error heppened function return NULL
 * @endcode
 * @param config_file_name log configuration JSON file name
 * @return Pointer at SmartLoggerConfig struct or NULL if something wrong
 */
SmartLoggerConfig *smart_syslog_load_config(const char *config_file_name);

/**
 * Getting of logging module.
 * If input parameters are NULL or module name not found function returns NULL
 * @param log_config        Pointer at SmartLoggerConfig struct
 * @param module_name       Name of logging module in configuration file
 * @return                  Pointer at ModuleLogger or  NULL if something wrong
 */
const ModuleLogger * smart_syslog_get_module(const SmartLoggerConfig *log_config, const char *module_name);

/**
 * Write message to syslog. If Pointer at ModuleLogger is NULL write message skips
 * @param logger            Pointer at ModuleLogge
 * @param log_level         log level
 * @param format            log message
 * @param ...               list of parametrs
 */
void smart_syslog_log(ModuleLogger *logger, unsigned int log_level, char *format, ...);

/**
 * Write message to syslog with a parameter list.
 * Same as smart_syslog_log, but parameters passed to the function as va_list.
 * @param logger            Pointer at ModuleLogge
 * @param log_level         log level
 * @param format            log message
 * @param ap               va_list of parametrs
 */
void smart_syslog_log_va_list(ModuleLogger *logger, unsigned int log_level, char *format, va_list ap);
#ifdef __cplusplus
}
#endif
#endif