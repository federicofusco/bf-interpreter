#ifndef LOG_H
#define LOG_H

void logf_fatal(char *error, ...);
void log_fatal(char *error);
void logf_warn(char *warning, ...);
void log_warn(char *warning);
void logf_info(char *info, ...);
void log_info(char *info);
void logf_verbose(char *info, ...);
void log_verbose(char* info);

#endif