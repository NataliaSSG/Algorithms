#ifndef __HASH_CHAIN_H_
#define __HASH_CHAIN_H_

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "nodo.h"
#include "registro.h"
using namespace std;

class hashChain {
private:
    int tamanio;
    vector<Nodo*> hashTable;

public:
    hashChain(int tamanio);
    int buscar(long long key);
    void insertar(Registro reg);
    int funcionHash(long long key);
    void imprimir();
    void readFile(string file);
    void imprimirRegistros(string ip);
};

hashChain::hashChain(int size) {
    tamanio = size;
    hashTable.resize(tamanio, nullptr);
}


int hashChain::funcionHash(long long key) {
    return key % tamanio;
}


// FUNCION PARA PARSEAR
Registro parseLine(const string& line) {
  Registro reg = Registro();
  istringstream iss(line);
  string stringMes, stringDia, stringHora, stringIP, stringResto;
  iss >> stringMes >> stringDia >> stringHora >> stringIP;

  reg.mes = stringMes;
  reg.dia = stringDia;
  reg.hora = stringHora;

  string IPtoInt = "";
  string stringIP2 = "";
  string port;
  for(int i = 0; i < stringIP.length(); i++) {
    if(stringIP[i] == '.') {
      stringIP2 = stringIP2 + stringIP[i];
      continue;
    }
    else if (stringIP[i] == ':') {
      port = stringIP.substr(i+1, stringIP.length()-1);
      break;
    }
    else {
      IPtoInt = IPtoInt + stringIP[i];
      stringIP2 = stringIP2 + stringIP[i];
    }
  }
  reg.IP = stringIP2;
  reg.IPint = stoll(IPtoInt);
  reg.puerto = port;

  getline(iss >> ws, reg.resto, '\n');
  return reg;
}


int hashChain::buscar(long long key) {
    int index = funcionHash(key);
    if (hashTable[index] == nullptr) {
        return -1;
    }
    else {
        Nodo* aux = hashTable[index];
        while (aux != nullptr) {
            if (aux->llaveIP == key) {
                return index;
            }
            aux = aux->sig;
        }
    }
    return -1;
}


void hashChain::insertar(Registro reg) {
    long long key = reg.IPint;
    int index = funcionHash(key);

    if (buscar(key) != -1) {
        Nodo* aux = hashTable[index];
        while (aux != nullptr) {
            if (aux->llaveIP == key) {
                aux->registros.push_back(reg);
                return;
            }
            aux = aux->sig;
        }
    } 
    else {
        Nodo* nuevoNodo = new Nodo();
        nuevoNodo->llaveIP = key;
        nuevoNodo->IPstring = reg.IP;
        nuevoNodo->sig = nullptr;
        nuevoNodo->registros.push_back(reg);

        if (hashTable[index] == nullptr) { // si es la cabeza
            hashTable[index] = nuevoNodo;
        } else {
            Nodo* aux = hashTable[index];
            while (aux->sig != nullptr) { // itera hasta encontrar espacio
                aux = aux->sig;
            }
            aux->sig = nuevoNodo;
        }
    }
}


void hashChain::readFile(string file) {
    ifstream archivo(file);
    string line;
    int key;
    while (getline(archivo, line)) {
        Registro registro = parseLine(line);
        insertar(registro);
    }
    archivo.close();
}


void hashChain::imprimir() {
    int i = 0;
    while (i < tamanio) {
        Nodo* aux = hashTable[i];
        while (aux != nullptr) {
          cout << "Llave: " << aux->llaveIP << " " << "Dato: " << aux->IPstring << endl;
          aux = aux->sig;
        }
      i++;
    }
}


void hashChain::imprimirRegistros(string ip) {
    string IPtoInt = "";
    string stringIP = ip;
    for(int i = 0; i < stringIP.length(); i++) {
      if(stringIP[i] == '.') {  
        continue;
      }
      else {
        IPtoInt = IPtoInt + stringIP[i];
      }
    }
    long long IPint = stoll(IPtoInt);
    int index = funcionHash(IPint);
  
    if (hashTable[index] == nullptr) {
        cout << "No existe ese IP en la tabla." << endl;
    }
    else {
        bool encontrado = false; 
        Nodo* aux = hashTable[index];
        while (aux != nullptr) {
            if (aux->llaveIP == IPint) {
                encontrado = true;
                cout << "Se encontraron " << aux->registros.size() << " registros:" << endl;
                for (int j = 0; j < aux->registros.size(); j++) {
                    cout << aux->registros[j].mes << "  ";
                    cout << aux->registros[j].dia << "  ";
                    cout << aux->registros[j].hora << "  ";
                    cout << aux->registros[j].IP << ":";
                    cout << aux->registros[j].puerto << "  ";
                    cout << aux->registros[j].resto << endl;
                }
            }
            aux = aux->sig;  
        }
        if (!encontrado) {
            cout << "No existe ese IP en la tabla." << endl;
        }
    }
}

#endif // __HASH_CHAIN_H_
