#define _CRT_SECURE_NO_WARNINGS

#include "util.h"
#include <cstdio>
#include <cstdarg>

static FILE* logFile = nullptr;

void initLog()
{
#ifndef DISABLE_LOGGING
  char filename[] = "ddraw_proxy_log.txt";
  logFile = fopen(filename, "a");
#endif
}

void closeLog()
{
#ifndef DISABLE_LOGGING
  if (!logFile)
    return;

  fclose(logFile);
  logFile = nullptr;
#endif
}

void writeLog(char* fmt, ...)
{
#ifndef DISABLE_LOGGING
  if (!logFile)
    return;

  va_list ap;
  va_start(ap, fmt);

  vfprintf(logFile, fmt, ap);
  fprintf(logFile, "\n");

  va_end(ap);
#endif
}
