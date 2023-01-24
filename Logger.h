#ifndef LOGGER_H
#define	LOGGER_H

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <functional>
#include "LoggerText.h"

using namespace std;

#define INIT_LOGGER Logger::initialize();
#define LOGGER_PRINT_SRC_CODE_LINE Logger::showSourceCodeLine(true);
#define LOGGER_SET_WORKING_DIR(workdir) Logger::setWorkingDirectory(workdir);
#define INFO(message,cause) Logger::log(__FILE__,__FUNCTION__, __LINE__,(string)(LoggerText() << message),cause,"INFO");
#define DEBUG(message,cause) Logger::log(__FILE__,__FUNCTION__, __LINE__,(string)(LoggerText() << message), cause, "DEBUG");
#define FATAL(message,cause) Logger::log(__FILE__,__FUNCTION__, __LINE__,(string)(LoggerText() << message),cause,"FATAL");
#define WARN(message,cause) Logger::log(__FILE__,__FUNCTION__, __LINE__,(string)(LoggerText() << message),cause,"WARN");
#define TRACE(message,cause) Logger::log(__FILE__,__FUNCTION__, __LINE__,(string)(LoggerText() << message),cause,"TRACE");


#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

class Logger {
private:
    const static string version;
public:
    static void log(string file,string function, int line, string message, string cause, string logLevel);
    static string getEnum(int logLevel);
    static void showSourceCodeLine(bool show);
    static void setWorkingDirectory(string workDir);
    static bool initialize();

    static const string getVersion() {
        return version;
    }



};
#endif	/* LOGGER_H */