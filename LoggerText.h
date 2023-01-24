/* 
 * File:   LoggerText.h
 * Author: firman
 *
 * Created on October 9, 2015, 10:54 AM
 */

#ifndef LOGGERTEXT_H
#define	LOGGERTEXT_H

#include <sstream>
using namespace std;

class LoggerText {
public:
    stringstream stream;

    operator string() const {
        return stream.str();
    }

    template<class T>
    LoggerText& operator<<(T const& VAR) {
        stream << VAR;
        return *this;
    }
private:

};

#endif	/* LOGGERTEXT_H */

