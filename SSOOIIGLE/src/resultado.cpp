#include <iostream>
#include <queue>
#include "../include/color.h"
#include "../include/Resultado.h"


/*Constructor*/
Resultado::Resultado(int hilo, int primera_linea, int linea_final){
    this -> hilo = hilo; 
    this -> primera_linea = primera_linea;
    this -> linea_final = linea_final; 
}

/*Getters declaration*/
int Resultado::getHilo(){ 
    return hilo; 
}
int Resultado::getLinea_Final(){ 
    return linea_final; 
}
int Resultado::getPrimera_linea(){ 
    return primera_linea; 
}

/*Add elements search to the queue*/
void Resultado::llenar_cola(Datos cont){ 
    queque.push(cont); 
}

/*Show the elements of queue*/
void Resultado::mostrar_resultado(){
    while(!queque.empty()){
        std::cout << "[" << "Hilo " << hilo << " inicio: " << CYAN << primera_linea << RESET<< " - final: " << CYAN << linea_final << RESET << "]" << " :: " << "linea " << RED << queque.front().nline << RESET << " :: " << "... " << queque.front().anterior << " " << GREEN << queque.front().palabra << RESET << " " << queque.front().posterior << " ..." << std::endl;
        queque.pop(); 
    }
}
