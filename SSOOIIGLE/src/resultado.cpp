/******************************************************
 * Project:         Práctica 2 de Sistemas Operativos II
 * 
 * Program name:    resultado.cpp
 
 * Author:          Miguel Angel Roldan Mora
 * 
 * Date created:    08/04/2022
 * 
 * Purpose:         Clase resultado donde tenemos el costructor y el metodo de llenar la cola y mostrar los datos de la cola
 * 
 ******************************************************/
#include <iostream>
#include <queue>
#include "../include/color.h"
#include "../include/Resultado.h"
#include "../include/Datos.h"


/*Constructor de la clase*/
Resultado::Resultado(int hilo, int primera_linea, int linea_final){
    this -> hilo = hilo; 
    this -> primera_linea = primera_linea;
    this -> linea_final = linea_final; 
}

int Resultado::getHilo(){ 
    return hilo; 
}
int Resultado::getLinea_Final(){ 
    return linea_final; 
}
int Resultado::getPrimera_linea(){ 
    return primera_linea; 
}

/**********************
 * 
 * Nombre: llenar_cola
 * 
 * Argumentos de entrada: datos: estructura de entrada que contiene la informacion de la linea que estamos leyendo 
 * 
 * Descripción: Rellenamos la cola de resultados con los datos del fichero 
 * 
 * ********************/
void Resultado::llenar_cola(Datos datos){ 
    queque.push(datos); 
}

/**********************
 * 
 * Nombre: mostrar_resultados
 * 
 * Descripción: Mostramos por pantalla los datos de la cola de resultados hasta que esta se quede vacia 
 * 
 * ********************/
void Resultado::mostrar_resultado(){
    while(!queque.empty()){
        std::cout << "[" << "Hilo " << hilo << " inicio: " << CYAN << primera_linea << RESET<< " - final: " << CYAN << linea_final << RESET << "]" << " :: " << "linea " << RED << queque.front().nline << RESET << " :: " << "... " << queque.front().anterior << " " << GREEN << queque.front().palabra << RESET << " " << queque.front().posterior << " ..." << std::endl;
        queque.pop(); 
    }
}
