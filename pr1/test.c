/* TÍTULO:
 * AUTOR: Iván Martín Gómez
 *
 * Sintaxis: test [-N lines]
 *
 * Argumentos Entrada: (los que estan entre [] son opcionales)
 * 		Argumento 1: [-N]       --> se muestran las N primeras líneas
 * 									(si se omite este Argumento por defecto N=10)
 *

 *
 * Salida: Muestra por pantalla, en orden, el resultado de intentar recrear el comportamiento de las
 * 	       los mandatos head [-N] , tail [-N]  y el comportamiento de nuestra
 * 	       función propia longlines [-N]
 *
 * Errores:
 * 		Error 1:se recibe como Argumento 1 algo que no es un número natural (cero no es natural)
 * 		Error 2:
 * 		Error 3:
 * 		Error 4:
 * 		Error 5:
 *   	Error 6:
 *
 * 	Descripción:
 *
 *
 */
//----------------------------------------------------------------------------

//LIBRERIA SISTEMA (entre < >)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


//LIBRERIA PROPIAS (entre " ")
#include "libreria.h"




// EMPIEZA EL PROGRAMA

int main (int argc, char *argv[], char *envp[]){

	printf("EMPIEZA EL PROGRAMA\n");

// ZONA DE DECLARACION DE VARIABLES
	int i;
	int N;
	int res;
	int resultado_head;
	int resultado_tail;
	int resultado_longlines;


// FIN ZONA DE DECLARACION DE VARIABLES
//------------------------------------
// ZONA DE INICIALIZACIÓN DE VARIABLES

// FIN ZONA DE DECLARACION DE VARIABLES
//------------------------------------
// ZONA DE PARSEO
		printf("Parseamos el Mandato:\n");
		printf("--> Se han recibido %i Argumento/s:\n",argc-1);
		for ( i=0; i<argc;i=i+1){
			if(i==0){
				printf("---- El nombre del mandato es: %s\n",argv[0]);
			}
			else{
				printf("---- Argumento %i:             %s\n",i,argv[i]);
			}
		}
		printf("------------------------------------------------------\n");
		printf("\n");
// FIN ZONA DE PARSEO
//----------------------------------
// ZONA DE CONTROL ERRORES ARGUMENTOS
		if(argc==1){ // Hemos recibido 0 Argumentos:
			//Por defecto N=10 y file=stdin
				N=10;
		}
		if(argc==2){ // Hemos recibido 1 Argumento:
			res=isnumber(argv[1]);
			if(res != 0){
				printf("head: illegal option algo que NO es un número recibido\n");
				printf("usage: head [-N lines]\n");
				printf("tail: illegal option algo que NO es un número recibido\n");
				printf("usage: tail [-N lines]\n");
				printf("longlines: illegal option algo que NO es un número recibido\n");
				printf("usage: longlines [-N lines]\n");
				return 1;
			}else{
				N=atoi(argv[1]);
				if(N<=0){
					printf("head: illegal option cero o número negativo\n");
					printf("usage: head [-N lines]\n");
					printf("tail: illegal option cero o número negativo\n");
					printf("usage: tail [-N lines]\n");
					printf("longlines: illegal option cero o número negativo\n");
					printf("usage: longlines [-N lines]\n");

					return 2;
				}
			}
		}
		if(argc>2){ // Hemos recibido 2 Argumentos o más:
			printf("usage: head [-N lines]\n");
			printf("usage: tail [-N lines]\n");
			printf("usage: longlines [-N lines]\n");
			return 3;
		}


// FIN DE CONTROL ERRORES ARGUMENTOS
//---------------------------------
// EMPIEZA FUNCIONALIDAD PROGRAMA

		//head [-N] [file ...]
			resultado_head= head(N);
			if(resultado_head!=0){
				printf("Error: función head()\n");
				return 1;
			}
		//FIN head

		//tail [-N] [file ...]
			resultado_tail= tail(N);
			if(resultado_tail!=0){
				printf("Error: función tail()\n");
				return 1;
			}
		//FIN tail

		//longlines [-N] [file ...]
			resultado_longlines= longlines(N);
			if(resultado_longlines!=0){
				printf("Error: función longlines()\n");
				return 1;
			}
		//FIN longlines

		return 0;
//---------------------------------
}//FIN PROGRAMA
