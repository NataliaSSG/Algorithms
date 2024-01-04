#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

// struct del nodo
struct Nodo {
  string mes;
  string dia;
  string hora;
  string IP;
  long long IPint; // para hacer ordenamiendo
  string resto;
  Nodo* izq;
  Nodo* der;
  int accesos;
};


// struct del arbol
struct ABB {
  Nodo* raiz;
  ABB() {raiz = nullptr;}
  //~ABB();
  void insertar(Nodo* nodo);
  void preorden();
  void preordenRecorrido(Nodo* nodo);
};


// metodo para parsear -> complejidad O(n)
Nodo* parseLine(const string& line) {
  Nodo *nodo = new Nodo();
  istringstream iss(line);
  string stringMes, stringDia, stringHora, stringIP, stringResto;
  iss >> stringMes >> stringDia >> stringHora >> stringIP;
  nodo->mes = stringMes;
  nodo->dia = stringDia;
  nodo->hora = stringHora;
  nodo->IP = stringIP;

  string stringIP2 = "";
  for(int i = 0; i < stringIP.length(); i++) {
    if(stringIP[i] == '.') {
      continue;
    }
    else if (stringIP[i] == ':') {
      break;
    }
    else {
      stringIP2 = stringIP2 + stringIP[i];
    }
  }

  nodo->IPint = stoll(stringIP2);
  getline(iss >> ws, nodo->resto, '\n');
  
  nodo->izq = NULL;
  nodo->der = NULL;
  return nodo;
}


void ABB::insertar(Nodo* nodo) {
  if (raiz == nullptr) {
    raiz = nodo;
  }
  else if (nodo->IPint < raiz->IPint) {
    if (raiz->izq == nullptr) {
      raiz->izq = nodo;
    }
    else {
      ABB izq_subarbol;
      izq_subarbol.raiz = raiz->izq;
      izq_subarbol.insertar(nodo);
    }
  }
  else if (nodo->IPint > raiz->IPint) {
    if (raiz->der == nullptr) {
      raiz->der = nodo;
    }
    else {
      ABB der_subarbol;
      der_subarbol.raiz = raiz->der;
      der_subarbol.insertar(nodo);
    }
  } 
  else {
    raiz->accesos++;
  }
}


void ABB::preordenRecorrido(Nodo* nodo) {
    if (nodo != nullptr) {
        std::cout << nodo->accesos;
        std::cout << std::endl;
        preordenRecorrido(nodo->izq);
        preordenRecorrido(nodo->der);
    }
}

void ABB::preorden() {
    preordenRecorrido(raiz);
}


int main() {
  ifstream file("bitacora.txt");
  ABB arbol;
  string line;
  while (getline(file, line)) {
      Nodo *nodo = parseLine(line);
      arbol.insertar(nodo);
  }
  file.close();
  arbol.preorden();
}

