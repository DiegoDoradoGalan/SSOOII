/******************************************************
 * Project:         Práctica 2 de Sistemas Operativos II
 * 
 * Program name:    Resultado.h
 
 * Author:          María Espinosa Astilleros
 * 
 * Date created:    9/3/2020
 * 
 * Purpose:         Contains the structure of the search elements and the definition of methods of the class Resultado 
 * 
 ******************************************************/

#include <iostream>
#include <queue>

struct Datos{
    int          nline; 
    std::string  anterior; 
    std::string  posterior;
    std::string  palabra; 
};

class Resultado{
    private:
        int hilo;
        int primera_linea;
        int linea_final;
        std::queue<Datos> queque;
        
    public:
        Resultado(int hilo, int primera_linea, int linea_final);
        int  getHilo(); 
        int  getLinea_Final(); 
        int  getPrimera_linea(); 
        void llenar_cola(Datos cont); //Add elements search to the queue
        void mostrar_resultado(); //Show the elements of queue
};