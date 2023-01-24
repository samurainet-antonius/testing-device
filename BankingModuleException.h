/* 
 * File:   BankingModuleException.h
 * Author: firman
 *
 * Created on November 12, 2015, 1:36 PM
 */

#ifndef BANKINGMODULEEXCEPTION_H
#define	BANKINGMODULEEXCEPTION_H

#include <string>
using namespace std;

class BankingModuleException : public exception {
public:
    BankingModuleException(const char* msg);
    BankingModuleException(const BankingModuleException& orig);
    virtual ~BankingModuleException();

    virtual const char* what() const throw();

private:
    string msg;
};

#endif	/* BANKINGMODULEEXCEPTION_H */

