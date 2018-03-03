/**
  @author Aleksey Afanasiev (e-mail: afanasiev-sozvezdie@mail.ru)

  @par
  License that can be found in the LICENSE file.
  https://github.com/openprf/smart_syslog
 */
#include "smart_syslog_utils.h"
#include <syslog.h>
#include <string.h>

unsigned int getLogLevelFlag(unsigned int log_level)
{
    switch(log_level){
        case	LOG_EMERG: return SMART_SYSLOG_FLAG_EMERG;
        case	LOG_ALERT: return SMART_SYSLOG_FLAG_ALERT;
        case	LOG_CRIT: return SMART_SYSLOG_FLAG_CRIT;
        case	LOG_ERR: return SMART_SYSLOG_FLAG_ERR;
        case	LOG_WARNING: return SMART_SYSLOG_FLAG_WARNING;
        case	LOG_NOTICE: return SMART_SYSLOG_FLAG_NOTICE;
        case	LOG_INFO: return SMART_SYSLOG_FLAG_INFO;
        case	LOG_DEBUG: return SMART_SYSLOG_FLAG_DEBUG;
    }
    return (log_level<32)?(1<<log_level):0;
}

unsigned int getLogLevelFlagFromTest(char *text)
{
    if(!text)
        return 0;

    if(!strcmp(text, SMART_SYSLOG_TEXT_EMERG)){
        return SMART_SYSLOG_FLAG_EMERG;
    }else  if(!strcmp(text, SMART_SYSLOG_TEXT_ALERT)){
        return SMART_SYSLOG_FLAG_ALERT;
    }else if(!strcmp(text, SMART_SYSLOG_TEXT_CRIT)){
        return SMART_SYSLOG_FLAG_CRIT;
    }else if(!strcmp(text, SMART_SYSLOG_TEXT_ERR)){
        return SMART_SYSLOG_FLAG_ERR;
    }else if(!strcmp(text, SMART_SYSLOG_TEXT_WARNING)){
        return SMART_SYSLOG_FLAG_WARNING;
    }else if(!strcmp(text, SMART_SYSLOG_TEXT_NOTICE)){
        return SMART_SYSLOG_FLAG_NOTICE;
    }else if(!strcmp(text, SMART_SYSLOG_TEXT_INFO)){
        return SMART_SYSLOG_FLAG_INFO;
    }else if(!strcmp(text, SMART_SYSLOG_TEXT_DEBUG)){
        return SMART_SYSLOG_FLAG_DEBUG;
    }

    return 0;
}

char* logLevelToString(unsigned int log_level)
{
    switch(log_level){
        case	LOG_EMERG: return "EMERG";
        case	LOG_ALERT: return "ALERT";
        case	LOG_CRIT: return "CRIT";
        case	LOG_ERR: return "ERR";
        case	LOG_WARNING: return "WARN";
        case	LOG_NOTICE: return "NOTICE";
        case	LOG_INFO: return "INFO";
        case	LOG_DEBUG: return "DEBUG";
    }
    return "_DEBUG_";
}

unsigned int upToMask(unsigned int log_level) {
    unsigned int mask = 0;
    for (int i = 0; i <= log_level; i++) {
        mask |= getLogLevelFlag(i);
    }
    return mask;
}

unsigned int getLogLevelFromTest(char *text)
{
    if(!text)
        return 0;

    if(!strcmp(text, SMART_SYSLOG_TEXT_EMERG)){
        return LOG_EMERG;
    }else  if(!strcmp(text, SMART_SYSLOG_TEXT_ALERT)){
        return LOG_ALERT;
    }else if(!strcmp(text, SMART_SYSLOG_TEXT_CRIT)){
        return LOG_CRIT;
    }else if(!strcmp(text, SMART_SYSLOG_TEXT_ERR)){
        return LOG_ERR;
    }else if(!strcmp(text, SMART_SYSLOG_TEXT_WARNING)){
        return LOG_WARNING;
    }else if(!strcmp(text, SMART_SYSLOG_TEXT_NOTICE)){
        return LOG_NOTICE;
    }else if(!strcmp(text, SMART_SYSLOG_TEXT_INFO)){
        return LOG_INFO;
    }else if(!strcmp(text, SMART_SYSLOG_TEXT_DEBUG)){
        return LOG_DEBUG;
    }

    return 0;
}