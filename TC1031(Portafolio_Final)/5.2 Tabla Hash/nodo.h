#ifndef __NODO_H_
#define __NODO_H_

#include <iostream>
#include <vector>
#include "registro.h"
using namespace std;

struct Nodo {
public:
    vector<Registro> registros;
    long long llaveIP;
    string IPstring;
    Nodo* sig;
};

#endif // _NODO_H_