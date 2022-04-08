/******************************************************
 * Project:         Práctica 2 de Sistemas Operativos II
 * 
 * Program name:    SSOOIIGLE.cpp
 
 * Author:          Miguel Angel Roldan Mora
 * 
 * Date created:    08/04/2022
 * 
 * Purpose:         Metodo main del programa contiene los metodos mas importantes del programa, como son el buscar palabra, crear los hilos, dividir el trabajo de los hilos, etc. 
 * 
 ******************************************************/
#include <iostream>
#include <thread>
#include <string>
#include <algorithm> 
#include <vector>
#include <queue>
#include <fstream> 
#include <mutex>
#include <locale>   
#include <functional>
#include <sstream>  
#include <cctype>   
#include <clocale>  

#include "../include/color.h" 
#include "../include/Resultado.h"
#include "../include/Datos.h"




#define  NUM_CARACTER_ERASE 1 //Number of characters erase when find symbols

/*Declaracion de las funciones*/
std::string               convertir_a_minuscula(std::string s);
std::vector<std::string>  dividir_linea_actual(std::string linea);
void                      linea_por_hilo(int nHilos, int nLinea_actuals, std::string fichero, std::string palabra); 
void                      crear_hilos(); 
void                      obtener_linea_actual(int hilo, std::string fichero, std::string palabra);
void                      buscar_palabra(std::vector<std::string> v_palabra, std::string palabra, int index, int linea_actual);
void                      esperar_hilos();
void                      coger_argumentos(int argc, char *argv[], std::string &fichero, std::string &palabra,int &hilos,int &nLineas_actuales);
int                       linea_actual(std::string nombre_fichero);


/*Variables globales*/
std::vector<std::thread>  g_hilos;      //Vector que contine los hilos del programa
std::vector<Resultado>    g_resultado;  //Vector para guarda el resultado de la busqueda 
std::mutex                g_semaforo;
int                       g_total = 0;  //Numero de veces que aparece la palabra que buscamos






int main(int argc, char *argv[]){

    std::string fichero;
    std::string palabra;
    int nHilos;
    int nLineas_actuales;
    coger_argumentos(argc,argv,fichero, palabra,nHilos,nLineas_actuales);
    linea_por_hilo(nHilos, nLineas_actuales,fichero, palabra); 
    esperar_hilos(); 
 
    std::cout << "\nLa palabra " << CYAN <<  palabra << RESET;
    std::cout << " aparecio " << CYAN << g_total << " veces\n" << RESET << std::endl;

    return EXIT_SUCCESS; 

}  

/**********************
 * 
 * Nombre: coger_argumentos
 * 
 * Argumentos de entrada: int argc: entero que indica el numero de cadenas. int argv: cadena de ent
 *                        en el fichero, int hilos: numero de hilos en los que se va a dividir el p
 * 
 * Descripción: Método con el que cogemos todos los datos de entrada el programa y los pasamos a la
 *              
 * 
 * ********************/
void coger_argumentos(int argc, char *argv[], std::string &fichero, std::string &palabra,int &hilos,int &nLineas_actuales){
    if(argc!=4){ 
        std::cerr << "El número de argumentos es incorrecto" <<std::endl; 
        exit(EXIT_FAILURE);
    }    

    fichero = argv[1];
    palabra = convertir_a_minuscula(argv[2]);
    hilos = atoi(argv[3]);
    nLineas_actuales = linea_actual(fichero);
}

/**********************
 * 
 * Nombre: linea_actual
 * 
 * Argumentos de entrada: nombre_fichero: El nombre de un fichero
 * 
 * Descripción: Método que cuenta el número de líneas de un fichero 
 *              que se le indique
 * 
 * ********************/
int linea_actual(std::string nombre_fichero){

    std::ifstream fichero;
    fichero.open(nombre_fichero);

    std::string linea_actual;
    int nLinea_actuals = 0;

    if (fichero.fail()){
        std::cerr << "Error al abrir el fichero" <<std::endl;
        fichero.close();
        exit(EXIT_FAILURE);
    }

    while (getline(fichero, linea_actual)){
        nLinea_actuals++;
    }
    fichero.close();

    return nLinea_actuals;
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

/**********************
 * 
 * Nombre: buscar_palabra
 * 
 * Argumentos de entrada: vector<string> v_palabra: vector que contiene las palabras de las lineas, string palabra: palabra a buscar en el texto, int index: posicion en la que nos encontramos,
 *                        int linea_actual: linea del fichero en la que nos econtramos 
 * 
 * Descripción: Método que busca una palabra en las linea_actuals que tiene
 *              adjudicadas
 * 
 * ********************/
void buscar_palabra(std::vector<std::string> v_palabra, std::string palabra, int index, int linea_actual) {
    
    Datos datos;

    for(unsigned i = 0; i < v_palabra.size(); i++){
        if(v_palabra[i].compare(palabra) == 0){
            datos.nline = linea_actual;
            datos.palabra = v_palabra[i]; 

            if(i == 0){                                               //Comprobamos si se trata de la primera linea, ya que en ese caso no abria una palabra anterior 
                datos.anterior = "_Es el inicio de la linea_actual_"; 
            }else{
                datos.anterior = v_palabra[i-1]; 
            }

            if(i == v_palabra.size()-1){                              //Comprobamos si se trata de la ultima linea, ya que en ese caso no abria una palabra posterior  
                datos.posterior = "_Es el final de la linea_actual_";
            }else{
                datos.posterior = v_palabra[i+1]; 
            }
 
            std::lock_guard<std::mutex> semaforo_lock(g_semaforo); 
            g_total++;                                               //Sumamos las veces que aparece la palabra que buscamos
            g_resultado[index].llenar_cola(datos);                   //Llenamos la cola con los datos de la estructura 
        }
    }

}

/**********************
 * 
 * Nombre: linea_por_hilo
 * 
 * Argumentos de entrada: int nHilo: numero de hilos para ejecutar el programa, int nLinea_actual: linea en la que se encuntra del fichero, string fichero: nombre del fichero,
 *                        string palabra: palabra que se va a buscar
 * 
 * Descripción: Método con el que crearemos los hilos y le adjudicaremos las lineas de trabajo que van a leer 
 * 
 * ********************/

void linea_por_hilo(int nHilos, int nLinea_actuals, std::string fichero, std::string palabra){
    int inicio = 0; 
    int final  = 0;
    int hilo   = 0;  
    int division_linea_actuals  = nLinea_actuals / nHilos;  

    for(unsigned i = 0; i < nHilos; i++){
        inicio = (division_linea_actuals * i) + 1;
        final  = division_linea_actuals * (i + 1); 
        hilo   =  i + 1; 

        if(i == (nHilos -1)){ final = nLinea_actuals; } 

        Resultado sr(hilo, inicio, final); 
        std::unique_lock<std::mutex> semaforo_lock(g_semaforo); 
        g_resultado.push_back(sr);  
        semaforo_lock.unlock(); 
        g_hilos.push_back(std::thread(obtener_linea_actual, hilo, fichero, palabra));

    }
}

/**********************
 * 
 * Nombre: obtener_linea_actual
 * 
 * Argumentos de entrada: int hilo: hilo que se encarga de esa parte del fichero, string fichero: nombre de fichero a tratar, string palabra: palabra que buscamos en el fichero
 * 
 * Descripción: Método que nos indica en la linea en la que nos encontramos en ese momento, tambien obtenemos la linea en la que empieza y finaliza el rango con el que va trabajar el hilo
 * 
 * ********************/
void obtener_linea_actual(int hilo, std::string fichero, std::string palabra){
    int  principio; 
    int  fin; 
    int  index = hilo - 1;    /*Index of the vector of results*/
    int  linea_actual = 1;    /*Number of linea where we work*/
    std::string               linea;                           /*Line where we work*/
    std::vector<std::string>  v_palabra;                     /*Vector that save the number of words that has the linea where we work*/
    std::ifstream             fp(fichero, std::ifstream::in); 

    std::unique_lock<std::mutex> semaforo_unique(g_semaforo);  
    principio = g_resultado.at(index).getPrimera_linea(); 
    fin       = g_resultado.at(index).getLinea_Final(); 
    semaforo_unique.unlock();     

    while (linea_actual <= fin){
        std::getline(fp, linea, '\n'); 
        if(linea_actual >= principio){ 
            linea      = convertir_a_minuscula(linea);    
            v_palabra  = dividir_linea_actual(linea); 
            buscar_palabra(v_palabra, palabra, index, linea_actual);
        } 
        linea_actual++; 
    }
}

/**********************
 * 
 * Nombre: dividir_linea_actual
 * 
 * Argumentos de entrada: string linea: linea del fichero que leemos 
 * 
 * Descripción: Dividimos la linea del fichero en partes, para poder tratar los datos de una manera correcta 
 * 
 * ********************/
std::vector<std::string> dividir_linea_actual(std::string linea){

    std::vector<std::string>  v_palabras;
    std::string               token; 
    std::istringstream        cadena (linea, std::istringstream::in); 

    while(std::getline(cadena, token, ' ')){
        v_palabras.push_back(token); 
    }

    return v_palabras;
}

/**********************
 * 
 * Nombre: esperar_hilos
 * 
 * Descripción: Esperamos que os hilos terminen su trabajo y ademas mostramos por pantalla los datos de la cola 
 * 
 * ********************/
void esperar_hilos(){
    
    std::for_each(g_hilos.begin(), g_hilos.end(), std::mem_fn(&std::thread::join)); 

    for(unsigned i = 0; i < g_hilos.size(); i++){
        std::lock_guard<std::mutex> semaforo_lock(g_semaforo);   //Utilizamos el semaforo para poder imprimir los datos de manera correcta y no intenten imprimir todos a la vez
        g_resultado[i].mostrar_resultado();
        
    }
    
}
