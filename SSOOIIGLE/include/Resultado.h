/******************************************************
 * Project:         Práctica 2 de Sistemas Operativos II
 * 
 * Program name:    Resultado.h
 
 * Author:          Miguel Angel Roldan Mora
 * 
 * Date created:    08/04/2022
 * 
 * Purpose:         Tiene la estructura de la clase resultado, la cual utilizaremos para mostrar de forma adecuada los datos obtenidos del fichero 
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