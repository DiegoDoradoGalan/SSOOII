#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <cctype>
#include <fstream>
#include <regex>
#include <map>
#include <vector>
#include <thread>
#include <mutex>

std::string convertir_a_minuscula(std::string s);
int contar_lineas(std::string nombre_fichero);


std::mutex semaphore;
std::vector<std::thread> g_hilos;


int main(int argc, char *argv[])
{
    //Verificamos parámetros de entrada
    if(argc!=4){ 
        std::cerr << "El número de argumentos es incorrecto" <<std::endl; 
        exit(EXIT_FAILURE);
    }    

    std::string nombre_fichero = argv[1];
    std::string palabra = convertir_a_minuscula(argv[2]);
    int nHilos = atoi(argv[3]);
    int nLineas = contar_lineas(nombre_fichero);
    
}  


/**********************
 * 
 * Nombre: contar_lineas
 * 
 * Argumentos de entrada: nombre_fichero: El nombre de un fichero
 * 
 * Descripción: Método que cuenta el número de líneas de un fichero 
 *              que se le indique
 * 
 * ********************/
int contar_lineas(std::string nombre_fichero){

    std::ifstream fichero;
    fichero.open(nombre_fichero);

    std::string linea;
    int nLineas = 0;

    if (fichero.fail()){
        std::cerr << "Error al abrir el fichero" <<std::endl;
        fichero.close();
        exit(EXIT_FAILURE);
    }

    while (getline(fichero, linea)){
        nLineas++;
    }
    fichero.close();

    return nLineas;
}

/**********************
 * 
 * Nombre: convertir_a_minuscula
 * 
 * Argumentos de entrada: string s: cadena de texto
 * 
 * Descripción: Método que convierte todas las letras de una cadena
 *              texto en minúsculas
 * 
 * ********************/
std::string convertir_a_minuscula(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
       [](unsigned char c){ return std::tolower(c); }
    );
    return s;
}
