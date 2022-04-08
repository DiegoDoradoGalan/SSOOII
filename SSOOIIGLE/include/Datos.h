/******************************************************
 * Project:         Práctica 2 de Sistemas Operativos II
 * 
 * Program name:    Datos.h
 
 * Author:          Miguel Angel Roldan Mora
 * 
 * Date created:    08/04/2022
 * 
 * Purpose:         Esta librería contiene las estructura de datos que utilizaremos posteriormente para guardar los datos del fichero
 * 
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