//
// Пример использования avsku_syslog в проекктах написаных на C
//

#include <stdio.h>
#include "smart_syslog.h"
#include <stdlib.h>

SmartLoggerConfig* g_log_config = NULL;


void log1(){
    ModuleLogger* module = smart_syslog_get_module(g_log_config, "cStyle1"); //Получение модуля логирования по имени
    printf("Log 1 module 0x%X\n", module);

    char test[] = "log test";
    smart_syslog_log(module, LOG_DEBUG, "Debug level %d, %X", 42, module);
    smart_syslog_log(module, LOG_ERR, "Error level %s", test);
    printf("Log 1 stop\n");
}

void log2(){
    ModuleLogger* module = smart_syslog_get_module(g_log_config, "cStyle2");
    printf("Log 2 module 0x%X\n", module);

    smart_syslog_log(module, LOG_ERR, "Error level %d", LOG_ERR);
    smart_syslog_log(module, LOG_DEBUG, "Debug level %d", LOG_DEBUG);
    smart_syslog_log(module, 9, "CUSTOM level %d", 9);
    printf("Log 2 stop\n");
}

void log3(){
    ModuleLogger* module = smart_syslog_get_module(g_log_config, "cStyle3");
    printf("Log 3 module 0x%X\n", module);

    smart_syslog_log(module, LOG_ALERT, "lert level %d", LOG_ALERT);
    smart_syslog_log(module, LOG_INFO, "Info level %d", LOG_INFO);
    smart_syslog_log(module, LOG_DEBUG, "Debug level %d", LOG_DEBUG);
    printf("Log 3 stop\n");
}

void cStyleLog() {
    g_log_config = smart_syslog_load_config("../cstyle.json");

    printf("Config pointer 0x%X\n", g_log_config);
    if(g_log_config){
        log1();
        log2();
        log3();

        free(g_log_config);
    }
}