#include <stdio.h>
#include <iostream>
#include <iomanip>
#include "Logger.h"
#include <time.h>
#include <string>
#include <ctime>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

#define VERSION "1.1.1"

//------------- GLOBAL VAIABLES --------------
ofstream logFile;
string days[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
int lastSavedDayOfWeek = -1;
string s_now;
struct tm tstruct;
bool show = false;
bool isInitialized = false;

/*
 * Asign default value to log directory variables  
 */
string wdir = ".";
string logDirName = "log";
string fullPathLogDir = "./log";
string currentLog = "CURRENTLOG.log";

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//-------------------------------------------

string getDayOfWeek(int dayOfWeek) {
    return days[dayOfWeek];
}

bool preLog() {
    pthread_mutex_lock(&mutex);
    time_t now = time(0);
    tstruct = *localtime(&now);
    if (lastSavedDayOfWeek != tstruct.tm_wday && lastSavedDayOfWeek >= 0) {
        struct stat attrib; // create a file attribute structure
        if (stat(currentLog.c_str(), &attrib) == 0) {// get the attributes of CURRENTLOG.log
            struct tm fileClock; // create a time structure
            fileClock = *localtime(&(attrib.st_mtime));
            string newName = fullPathLogDir;
            newName.append("/");
            newName.append(getDayOfWeek(fileClock.tm_wday));
            newName.append(".log");
            rename(currentLog.c_str(), newName.c_str());
        }
    }
    logFile.open(currentLog.c_str(), ios_base::app);
    char buf[80];
    strftime(buf, sizeof (buf), "%Y%m%d%H%M%S", &tstruct);
    s_now.assign(buf);
    return true;
}

bool postLog() {
    //--------- save last day of week --------
    lastSavedDayOfWeek = tstruct.tm_wday;
    //----------------------------------------
    logFile.close();
    pthread_mutex_unlock(&mutex);
}

string getColorLevel(string level) {
    if (level.compare("DEBUG") == 0) {
        return ANSI_COLOR_GREEN;
    } else if (level.compare("WARN") == 0) {
        return ANSI_COLOR_YELLOW;
    } else if (level.compare("FATAL") == 0) {
        return ANSI_COLOR_RED;
    } else if (level.compare("TRACE") == 0) {
        return ANSI_COLOR_BLUE;
    } else if (level.compare("INFO") == 0) {
        return ANSI_COLOR_MAGENTA;
    }
}

const string Logger::version = VERSION;

void Logger::log(string file, string function, int line, string message, string cause, string logLevel) {
    if (cause.compare("") != 0) {
        string temp = " -> ";
        temp.append(cause);
        cause.assign(temp);
    }
    if (preLog()) {
        if (!isInitialized) {
            cerr << ANSI_COLOR_RED << " [" << setw(5) << setfill(' ') << logLevel << "] " << ANSI_COLOR_RESET << s_now << " " << left << setw(30) << setfill('.') << file << left << setw(4) << setfill(' ') << line << " " << left << setw(25) << setfill(' ') << function << " " << "LOGGER HAS NOT BEEN INITIALIZED YET.." << " " << cause << endl;
        } else {
            if (show) {
                if (strcmp(logLevel.c_str(), "FATAL") == 0) {
                    cerr << getColorLevel(logLevel) << " [" << setw(5) << setfill(' ') << logLevel << "] " << ANSI_COLOR_RESET << s_now << " " << left << setw(30) << setfill('.') << file << left << setw(4) << setfill(' ') << line << " " << left << setw(25) << setfill(' ') << function << " " << message << " " << cause << endl;
                } else {
                    cout << getColorLevel(logLevel) << " [" << setw(5) << setfill(' ') << logLevel << "] " << ANSI_COLOR_RESET << s_now << " " << left << setw(30) << setfill('.') << file << left << setw(4) << setfill(' ') << line << " " << left << setw(25) << setfill(' ') << function << " " << message << " " << cause << endl;
                }
                logFile << " [" << setw(5) << setfill(' ') << logLevel << "] " << s_now << " " << left << setw(30) << setfill('.') << file << left << setw(4) << setfill(' ') << line << " " << left << setw(25) << setfill(' ') << function << " " << message << " " << cause << endl;
            } else {
                if (strcmp(logLevel.c_str(), "FATAL") == 0) {
                    cout << getColorLevel(logLevel) << " [" << setw(5) << setfill(' ') << logLevel << "] " << ANSI_COLOR_RESET << s_now << " " << message << " " << cause << endl;
                } else {
                    cout << getColorLevel(logLevel) << " [" << setw(5) << setfill(' ') << logLevel << "] " << ANSI_COLOR_RESET << s_now << " " << message << " " << cause << endl;
                }
                logFile << " [" << setw(5) << setfill(' ') << logLevel << "] " << s_now << " " << message << " " << cause << endl;
            }
        }
        postLog();
    }
}

string Logger::getEnum(int logLevel) {
    string en[] = {"INFO", "DEBUG", "FATAL", "WARN", "TRACE"};
    return en[logLevel];
}

void Logger::showSourceCodeLine(bool _show) {
    show = _show;
}

void Logger::setWorkingDirectory(string workDir) {
    wdir = workDir;
}

bool Logger::initialize() {
    fullPathLogDir = "";
    currentLog = "";
    fullPathLogDir.append(wdir);
    fullPathLogDir.append("/");
    fullPathLogDir.append(logDirName);
    currentLog.append(fullPathLogDir);
    currentLog.append("/");
    currentLog.append("CURRENTLOG.log");
    struct stat st = {0};
    if (stat(fullPathLogDir.c_str(), &st) == -1) {
        int res = mkdir(fullPathLogDir.c_str(), ACCESSPERMS);
        if (res == 0 || res == EEXIST) {
            isInitialized = true;
            return true;
        }
    } else {
        isInitialized = true;
        return true;
    }
}

















//#include <stdio.h>
//#include <iostream>
//#include <iomanip>
//#include "Logger.h"
//#include <time.h>
//#include <string>
//#include <ctime>
//#include <sys/stat.h>
//#include <unistd.h>
//#include <string.h>
//#include <pthread.h>
//#include <errno.h>
//
////------------- GLOBAL VAIABLES --------------
//ofstream logFile;
//string days[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
//int lastSavedDayOfWeek = -1;
//string s_now;
//struct tm tstruct;
//bool show = false;
//bool isInitialized = false;
//
///*
// * Asign default value to log directory variables  
// */
//string wdir = ".";
//string logDirName = "log";
//string fullPathLogDir = "./log";
//string currentLog = "CURRENTLOG.log";
//
//pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
////-------------------------------------------
//
//string getDayOfWeek(int dayOfWeek) {
//    return days[dayOfWeek];
//}
//
//bool preLog() {
//    pthread_mutex_lock(&mutex);
//    time_t now = time(0);
//    tstruct = *localtime(&now);
//    if (lastSavedDayOfWeek != tstruct.tm_wday && lastSavedDayOfWeek >= 0) {
//        struct stat attrib; // create a file attribute structure
//        if (stat(currentLog.c_str(), &attrib) == 0) {// get the attributes of CURRENTLOG.log
//            struct tm fileClock; // create a time structure
//            fileClock = *localtime(&(attrib.st_mtime));
//            string newName = fullPathLogDir;
//            newName.append("/");
//            newName.append(getDayOfWeek(fileClock.tm_wday));
//            newName.append(".log");
//            rename(currentLog.c_str(), newName.c_str());
//        }
//    }
//    logFile.open(currentLog.c_str(), ios_base::app);
//    char buf[80];
//    strftime(buf, sizeof (buf), "%Y%m%d%H%M%S", &tstruct);
//    s_now.assign(buf);
//    return true;
//}
//
//bool postLog() {
//    //--------- save last day of week --------
//    lastSavedDayOfWeek = tstruct.tm_wday;
//    //----------------------------------------
//    logFile.close();
//    pthread_mutex_unlock(&mutex);
//}
//
//string getColorLevel(string level) {
//    if (level.compare("DEBUG") == 0) {
//        return ANSI_COLOR_GREEN;
//    } else if (level.compare("WARN") == 0) {
//        return ANSI_COLOR_YELLOW;
//    } else if (level.compare("FATAL") == 0) {
//        return ANSI_COLOR_RED;
//    } else if (level.compare("TRACE") == 0) {
//        return ANSI_COLOR_BLUE;
//    } else if (level.compare("INFO") == 0) {
//        return ANSI_COLOR_MAGENTA;
//    }
//}
//
//const string Logger::version = VERSION;
//
//void Logger::log(string file,string <<"("<<function<<")", int line, string message, string cause, string logLevel) {
//    if (cause.compare("") != 0) {
//        string temp = " -> ";
//        temp.append(cause);
//        cause.assign(temp);
//    }
//    if (preLog()) {
//        if (!isInitialized) {
//            cerr << ANSI_COLOR_RED << " [" << setw(5) << setfill(' ') << logLevel << "] " << ANSI_COLOR_RESET << s_now << " " << left << setw(30) << setfill('.') << file << left << setw(4) << setfill(' ') << line << " " << "LOGGER HAS NOT BEEN INITIALIZED YET.." << " " << cause << endl;
//        } else {
//            if (show) {
//                if (strcmp(logLevel.c_str(), "FATAL") == 0) {
//                    cerr << getColorLevel(logLevel) << " [" << setw(5) << setfill(' ') << logLevel << "] " << ANSI_COLOR_RESET << s_now << " " << left << setw(30) << setfill('.') << file << left << setw(4) << setfill(' ') << line << " " << message << " " << cause << endl;
//                } else {
//                    cout << getColorLevel(logLevel) << " [" << setw(5) << setfill(' ') << logLevel << "] " << ANSI_COLOR_RESET << s_now << " " << left << setw(30) << setfill('.') << file << left << setw(4) << setfill(' ') << line << " " << message << " " << cause << endl;
//                }
//                logFile << " [" << setw(5) << setfill(' ') << logLevel << "] " << s_now << " " << left << setw(30) << setfill('.') << file << left << setw(4) << setfill(' ') << line << " " << message << " " << cause << endl;
//            } else {
//                if (strcmp(logLevel.c_str(), "FATAL") == 0) {
//                    cout << getColorLevel(logLevel) << " [" << setw(5) << setfill(' ') << logLevel << "] " << ANSI_COLOR_RESET << s_now << " " << message << " " << cause << endl;
//                } else {
//                    cout << getColorLevel(logLevel) << " [" << setw(5) << setfill(' ') << logLevel << "] " << ANSI_COLOR_RESET << s_now << " " << message << " " << cause << endl;
//                }
//                logFile << " [" << setw(5) << setfill(' ') << logLevel << "] " << s_now << " " << message << " " << cause << endl;
//            }
//        }
//        postLog();
//    }
//}
//
//string Logger::getEnum(int logLevel) {
//    string en[] = {"INFO", "DEBUG", "FATAL", "WARN", "TRACE"};
//    return en[logLevel];
//}
//
//void Logger::showSourceCodeLine(bool _show) {
//    show = _show;
//}
//
//void Logger::setWorkingDirectory(string workDir) {
//    wdir = workDir;
//}
//
//bool Logger::initialize() {
//    fullPathLogDir = "";
//    currentLog = "";
//    fullPathLogDir.append(wdir);
//    fullPathLogDir.append("/");
//    fullPathLogDir.append(logDirName);
//    currentLog.append(fullPathLogDir);
//    currentLog.append("/");
//    currentLog.append("CURRENTLOG.log");
//    struct stat st = {0};
//    if (stat(fullPathLogDir.c_str(), &st) == -1) {
//        int res = mkdir(fullPathLogDir.c_str(), ACCESSPERMS);
//        if (res == 0 || res == EEXIST) {
//            isInitialized = true;
//            return true;
//        }
//    } else {
//        isInitialized = true;
//        return true;
//    }
//}
//
