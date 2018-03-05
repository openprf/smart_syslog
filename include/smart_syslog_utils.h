/**
  @info Library private header.
  @author Aleksey Afanasiev (e-mail: afanasiev-sozvezdie@mail.ru)

  @par
  License that can be found in the LICENSE file.
  https://github.com/openprf/smart_syslog
 */

#ifndef SMART_SYSLOG_UTILS_H
#define SMART_SYSLOG_UTILS_H

#define	SMART_SYSLOG_FLAG_EMERG	1	/* system is unusable */
#define	SMART_SYSLOG_FLAG_ALERT	2	/* action must be taken immediately */
#define	SMART_SYSLOG_FLAG_CRIT	4	/* critical conditions */
#define	SMART_SYSLOG_FLAG_ERR	8		/* error conditions */
#define	SMART_SYSLOG_FLAG_WARNING	16	/* warning conditions */
#define	SMART_SYSLOG_FLAG_NOTICE	32	/* normal but significant condition */
#define	SMART_SYSLOG_FLAG_INFO	64	/* informational */
#define	SMART_SYSLOG_FLAG_DEBUG	128	/* debug-level messages */

#define	SMART_SYSLOG_TEXT_EMERG	    "emerg"	/* system is unusable */
#define	SMART_SYSLOG_TEXT_ALERT	    "alert"	/* action must be taken immediately */
#define	SMART_SYSLOG_TEXT_CRIT	    "crit"	/* critical conditions */
#define	SMART_SYSLOG_TEXT_ERR	    "error"		/* error conditions */
#define	SMART_SYSLOG_TEXT_WARNING	"warnings"	/* warning conditions */
#define	SMART_SYSLOG_TEXT_NOTICE	"notice"	/* normal but significant condition */
#define	SMART_SYSLOG_TEXT_INFO	    "info"	/* informational */
#define	SMART_SYSLOG_TEXT_DEBUG	    "debug"	/* debug-level messages */

unsigned int getLogLevelFlag(unsigned int log_level);

unsigned int getLogLevelFlagFromTest(char *text);

char* logLevelToString(unsigned int log_level);

unsigned  int upToMask(unsigned int log_level);

unsigned int getLogLevelFromTest(const char *text);

#endif //SMART_SYSLOG_UTILS_H
