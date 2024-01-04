#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "registro.h"
#include "nodo.h"
#include "hashtable.h"
using namespace std;


int main() {
    hashChain hash(503);
    string file = "bitacora_3.txt";
    hash.readFile(file);
    cout << "Ingrese una direccion IP: ";
    string IP;
    cin >> IP;
    hash.imprimirRegistros(IP);
}