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
void logf_cyan(char* cyan, ...);
void log_cyan(char* cyan);
void logf_yellow(char* yellow, ...);
void log_yellow(char* yellow);
void logf_green(char* green, ...);
void log_green(char* green);
void logf_red(char* red, ...);
void log_red(char* red);

#endif