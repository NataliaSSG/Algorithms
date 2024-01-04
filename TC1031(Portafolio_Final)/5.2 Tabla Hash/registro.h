#ifndef __REGISTRO_H_
#define __REGISTRO_H_

#include <iostream>
#include <string>
using namespace std;

struct Registro {
public:
    string mes;
    string dia;
    string hora;
    string IP;
    long long IPint;
    string puerto;
    string resto;
};

#endif // _REGISTRO_H_