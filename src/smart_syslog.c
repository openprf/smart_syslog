/**
  @author Aleksey Afanasiev (e-mail: afanasiev-sozvezdie@mail.ru)

  @par
  License that can be found in the LICENSE file.
  https://github.com/openprf/smart_syslog
 */

#include "smart_syslog.h"
#include "smart_syslog_utils.h"
#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include <json-c/json.h> // sudo apt-get install libjson-c-dev

#define MODULES_LOG_JSON_KEY "ModuleList"
#define MODULES_NAME_JSON_KEY "name"
#define MODULES_INCLUDE_LEVELS_JSON_KEY "levels"
#define MODULES_UPTO "upto"

typedef struct _ModulesList{
    int size;
    int common_mask;
    char **names;
}ModulesList;

int parseModulesKey(SmartLoggerConfig **ppConfig, json_object *modules_object)
{
    if(!ppConfig || !modules_object){
        syslog(LOG_ERR, "smart_syslog: FAIL to use parseModulesKey 0x%X , 0x%X", ppConfig, modules_object);
        return -1;
    }

    int len = json_object_array_length(modules_object); //get mdule list length
 //   syslog(LOG_INFO, "smart_syslog: Module list size: %d", len);

    if(!len){
        syslog(LOG_WARNING, "smart_syslog: No modules for logging!");
        return -2;
    }

    //init modules list
    ModulesList* modulesList = (ModulesList*)calloc(sizeof(ModulesList),len);

    int modules_list_size=0;

    //get modules decriprion
    for (int i = 0; i < len; i++)
    {
        //parse module object
        json_object* module = json_object_array_get_idx(modules_object, i);
        if(!module){
            syslog(LOG_ERR, "smart_syslog: cannot parse module object %d", i);
            *ppConfig = NULL;
            free(modulesList);
            return -3;
        }

        //parse module name
        json_object* name = json_object_new_object();
        if(!json_object_object_get_ex(module, MODULES_NAME_JSON_KEY, &name)){
            syslog(LOG_ERR, "smart_syslog: cannot parse module content %d", i);
            free(modulesList);
            *ppConfig = NULL;
            return -4;
        }

 //       syslog(LOG_INFO, "smart_syslog: Parse modules name... %d", i);

        //check module name type
        if(json_object_is_type(name, json_type_string)){ //Если модуль один
            modulesList[i].names = malloc(sizeof(char*));
            modulesList[i].names[0] = strdup(json_object_get_string(name));
 //           syslog(LOG_INFO, "smart_syslog: strdup ok");
            modulesList[i].size = 1;
            ++modules_list_size;

        }else if(json_object_is_type(name, json_type_array)){ //Если группа модулей
            int names_len = json_object_array_length(name);
        //    syslog(LOG_INFO, "smart_syslog: multi modul (count %d)", names_len);
            modulesList[i].size = 0;
            modulesList[i].names = calloc(sizeof(char*), names_len);
            for (int j=0; j<names_len ; j++) {
                json_object* name_entry = json_object_array_get_idx(name, j);
                if(!json_object_is_type(name_entry, json_type_string)) {
                    break;
                }
                modulesList[i].names[j] = strdup(json_object_get_string(name_entry));
                ++modulesList[i].size;
                ++modules_list_size;
            }
        }else{ //type unknown
            syslog(LOG_ERR, "smart_syslog: wrong module name type %d", i);
            free(modulesList);
            *ppConfig = NULL;
            return -5;
        }

   //     syslog(LOG_INFO, "smart_syslog: Parse modules mask %d", i);

        modulesList[i].common_mask = 0;

        //parse log levels
        json_object* levels = json_object_new_object();
        if(json_object_object_get_ex(module, MODULES_INCLUDE_LEVELS_JSON_KEY, &levels)) {
            int levels_count = json_object_array_length(levels);
       //     syslog(LOG_INFO, "smart_syslog: Module: %d; levels list size: %d", i, levels_count);

            for (int j=0; j<levels_count ; j++) {
                json_object* log_value = json_object_array_get_idx(levels, j);

                if(!log_value){
                    syslog(LOG_ERR, "smart_syslog: Cannot get jason value in list at %d", j);
                    free(modulesList);
                    *ppConfig = NULL;
                    return -6;
                }

                if(json_object_is_type(log_value, json_type_string)){
                    modulesList[i].common_mask |= getLogLevelFlagFromTest(json_object_get_string(log_value));
                }else if(json_object_is_type(log_value, json_type_int)){
                    modulesList[i].common_mask |= getLogLevelFlag(json_object_get_int(log_value));
                }else{
                    syslog(LOG_WARNING, "smart_syslog: Bad levels json type");
                }
                //         syslog(LOG_DEBUG, "smart_syslog: module level mask 0x%X", mlog->levels_mask);
            }
        }
   //     syslog(LOG_WARNING, "smart_syslog: Result %d  levels 0x%X", i, modulesList[i].common_mask );

        //parse log level "upto"
        json_object* upto = json_object_new_object();
        if(json_object_object_get_ex(module, MODULES_UPTO, &upto)) {
          //  syslog(LOG_INFO, "smart_syslog: Module: %d; upto", i);
            if(json_object_is_type(upto, json_type_string)){
                modulesList[i].common_mask |= upToMask(getLogLevelFromTest(json_object_get_string(upto)));;
            }else if(json_object_is_type(upto, json_type_int)){
                modulesList[i].common_mask |= upToMask(json_object_get_int(upto));
            }else{
                syslog(LOG_WARNING, "smart_syslog: Bad upto json type ");
            }
        }
        //syslog(LOG_WARNING, "smart_syslog: Result %d mask 0x%X", i, modulesList[i].common_mask );
    }

    if(modules_list_size && modulesList){
        //Init logger configuration
        SmartLoggerConfig* new_logger = (SmartLoggerConfig* )calloc(sizeof(SmartLoggerConfig)+(modules_list_size)*sizeof(ModuleLogger),sizeof(char));
        new_logger->moduls_list_size = modules_list_size;

        int pos = 0;
        for (int i = 0; i < len; i++) {
            for (int j = 0; j < modulesList[i].size; j++) {
                new_logger->moduls_list[pos].levels_mask = modulesList[i].common_mask;
                strcpy(new_logger->moduls_list[pos].name, modulesList[i].names[j]);
                free(modulesList[i].names[j]);

          //      syslog(LOG_INFO, "smart_syslog: Module %s mask: %X", new_logger->moduls_list[pos].name, new_logger->moduls_list[pos].levels_mask);
                pos++;
            }
            free(modulesList[i].names);
        }

        free(modulesList);
        *ppConfig = new_logger;
    }

    return 0;
}

SmartLoggerConfig* smart_syslog_load_config(const char *config_file_name)
{
    if(config_file_name){
        syslog(LOG_INFO, "smart_syslog: Get config from file %s", config_file_name);

        json_object* log_file = json_object_from_file(config_file_name); //get logger config JSON object from file

        if(!log_file){
            syslog(LOG_WARNING, "smart_syslog: Cannot open config file: %s", config_file_name);
            return NULL;
        }

        SmartLoggerConfig* new_logger = NULL;

        //Parse config object
        json_object* log_module_list = json_object_new_object();
        if(!log_module_list){
            syslog(LOG_ERR, "smart_syslog: Cannot create new json object");
            return NULL;
        }
        json_object_object_get_ex(log_file, MODULES_LOG_JSON_KEY, &log_module_list); //Get modules list
       // parseModulesKey(&new_logger, log_module_list);
        parseModulesKey(&new_logger, log_module_list);

    //    syslog(LOG_WARNING, "smart_syslog: Logger ready 0x%X", new_logger);

        return  new_logger;
    }else{
        syslog(LOG_INFO, "smart_syslog: no file");
        return NULL;
    }
}

const ModuleLogger * smart_syslog_get_module(const SmartLoggerConfig *log_config, const char *module_name)
{

    ModuleLogger* module = NULL;

    if(log_config && module_name){
        for (int i = 0; i < log_config->moduls_list_size; i++) {
            if(strcmp(module_name, log_config->moduls_list[i].name) == 0){
                return &log_config->moduls_list[i];
            }
        }
    }

    return NULL;
}

void smart_syslog_log(const ModuleLogger *logger, unsigned int log_level, const char *format, ...)
{
    if(!logger || !format)
        return;

    va_list ap;
    va_start(ap, format);
    smart_syslog_log_va_list(logger, log_level, format, ap);
    va_end(ap);
}

void smart_syslog_log_va_list(const ModuleLogger *logger, unsigned int log_level, const char *format, va_list ap)
{
    if(!logger || !format)
        return;

    int flags = getLogLevelFlag(log_level);
    if(flags&logger->levels_mask){
        char* lvl_str = logLevelToString(log_level);
        int log_size = strlen(logger->name)+strlen(format)+strlen(lvl_str)+6;
        char* buf = (char*)malloc(log_size);
        sprintf(buf, "%s [%s]: %s", logger->name, lvl_str, format);

        vsyslog((log_level>LOG_DEBUG)?LOG_DEBUG:log_level, buf, ap);

        free(buf);
    }
}