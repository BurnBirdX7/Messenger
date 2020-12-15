//
//
//

#ifndef DBEXCEPTION_H
#define DBEXCEPTION_H

#include <exception>
#include <string>

class DbException
        : public std::exception
{
public:
    DbException(const std::string& str) : mString(str){}

    virtual const char* what() const noexcept override
    {
        return mString.c_str();
    }

private:
    std::string mString;
};


#endif //DBEXCEPTION_H
