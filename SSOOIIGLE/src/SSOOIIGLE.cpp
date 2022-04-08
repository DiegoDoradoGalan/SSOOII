#include <iostream>
#include <thread>
#include <string>
#include <algorithm> //std::for_each
#include <vector>
#include <queue>
#include <fstream> 
#include <mutex>
#include <locale>   //std::tolower
#include <functional>
#include <sstream>  //std::cadenatringstream 
#include <cctype>   //ispunct
#include <clocale>  //std::setlocale

#include "../include/color.h" 
#include "../include/Resultado.h"
#include "../include/Datos.h"




#define  NUM_CARACTER_ERASE 1 //Number of characters erase when find symbols

/*Functions declaration*/
std::string               convertir_a_minuscula(std::string s);
std::vector<std::string>  dividir_linea_actuals(std::string linea);
void                      linea_actuals_por_hilo(int nHilos, int nLinea_actuals, std::string fichero, std::string palabra); 
void                      crear_hilos(); 
void                      optener_linea_actual(int hilo, std::string fichero, std::string palabra);
void                      buscar_palabra(std::vector<std::string> v_palabra, std::string palabra, int index, int linea_actual);
void                      esperar_hilos();
void                      coger_argumentos(int argc, char *argv[], std::string &fichero, std::string &palabra,int &hilos,int &nLineas_actuales);
int                       datosar_linea_actuals(std::string nombre_fichero);


/*Globals variables*/
std::vector<std::thread>  g_hilos;   //Vector of threads
std::vector<Resultado>    g_resultado;    //Vector of search result 
std::mutex                g_semaforo;
int                       g_total = 0;  //Number of times appear the word searched






int main(int argc, char *argv[]){

    std::string fichero;
    std::string palabra;
    int nHilos;
    int nLineas_actuales;
    coger_argumentos(argc,argv,fichero, palabra,nHilos,nLineas_actuales);
    linea_actuals_por_hilo(nHilos, nLineas_actuales,fichero, palabra); 
    esperar_hilos(); 
 
    std::cout << "\nLa palabra " << CYAN <<  palabra << RESET;
    std::cout << " aparecio " << CYAN << g_total << " veces\n" << RESET << std::endl;

    return EXIT_SUCCESS; 

}  

void coger_argumentos(int argc, char *argv[], std::string &fichero, std::string &palabra,int &hilos,int &nLineas_actuales){
    if(argc!=4){ 
        std::cerr << "El número de argumentos es incorrecto" <<std::endl; 
        exit(EXIT_FAILURE);
    }    

    fichero = argv[1];
    palabra = convertir_a_minuscula(argv[2]);
    hilos = atoi(argv[3]);
    nLineas_actuales = datosar_linea_actuals(fichero);
}

/**********************
 * 
 * Nombre: datosar_linea_actuals
 * 
 * Argumentos de entrada: nombre_fichero: El nombre de un fichero
 * 
 * Descripción: Método que cuenta el número de líneas de un fichero 
 *              que se le indique
 * 
 * ********************/
int datosar_linea_actuals(std::string nombre_fichero){

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
 * Argumentos de entrada: string s: cadena de texto
 * 
 * Descripción: Método que busca una palabra en las linea_actuals que tiene
 *              adjudicadas
 * 
 * ********************/
void buscar_palabra(std::vector<std::string> v_palabra, std::string palabra, int index, int linea_actual) {
    
    Datos datos;

    for(unsigned i = 0; i < v_palabra.size(); i++){
        if(v_palabra.at(i).compare(palabra) == 0){
            datos.nline = linea_actual;
            datos.palabra = v_palabra.at(i); 

            /*Check the inicio linea*/
            if(i == 0){
                datos.anterior = "_Es el inicio de la linea_actual_"; 
            }else{
                datos.anterior = v_palabra.at(i-1); 
            }

            /*Check the final linea*/
            if(i == v_palabra.size()-1){
                datos.posterior = "_Es el final de la linea_actual_";
            }else{
                datos.posterior = v_palabra.at(i+1); 
            }

            /*Add the result inside the vector of results*/   
            std::lock_guard<std::mutex> semaforo_lock(g_semaforo); 
            g_total++;
            g_resultado.at(index).llenar_cola(datos);
        }
    }

}


void linea_actuals_por_hilo(int nHilos, int nLinea_actuals, std::string fichero, std::string palabra){
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
        std::unique_lock<std::mutex> ul(g_semaforo); 
        g_resultado.push_back(sr);  
        ul.unlock(); 
        g_hilos.push_back(std::thread(optener_linea_actual, hilo, fichero, palabra));

    }
}


void optener_linea_actual(int hilo, std::string fichero, std::string palabra){
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
            v_palabra  = dividir_linea_actuals(linea); 
            buscar_palabra(v_palabra, palabra, index, linea_actual);
        } 
        linea_actual++; 
    }
}


std::vector<std::string> dividir_linea_actuals(std::string linea){

    std::vector<std::string>  v_palabras;
    std::string               token; 
    std::istringstream        cadena (linea, std::istringstream::in); 

    while(std::getline(cadena, token, ' ')){
        v_palabras.push_back(token); 
    }

    return v_palabras;
}


void esperar_hilos(){
    
    std::for_each(g_hilos.begin(), g_hilos.end(), std::mem_fn(&std::thread::join)); 

    for(unsigned i = 0; i < g_hilos.size(); i++){
        std::lock_guard<std::mutex> semaforo_lock(g_semaforo);
        g_resultado.at(i).mostrar_resultado();
        
    }
    
}
