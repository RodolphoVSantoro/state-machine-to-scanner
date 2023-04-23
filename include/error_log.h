#ifndef ERROR_H
#define ERROR_H
#include <stdarg.h>
#include <stdio.h>
static void printerrf(char *format, ...) {
    printf("Logged error to ./error.log\n");
    va_list args;
    va_start(args, format);
    FILE *stream = fopen("error.log", "w");
    vfprintf(stream, format, args);
    fclose(stream);
    va_end(args);
}

#endif