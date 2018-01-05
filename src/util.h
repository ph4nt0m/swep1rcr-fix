#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#define DISABLE_LOGGING

void initLog();
void closeLog();
void writeLog(char* fmt, ...);

#endif // UTIL_H_INCLUDED
