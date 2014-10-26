#ifndef __EXCEPTIONS_H__
#define __EXCEPTIONS_H__

#include <exception>

class Logic_Calculation_Exception:public std::exception
{
};

class Expression_Exception:public std::exception
{
};

#endif