/* TÍTULO: prueba1.c
 * AUTOR:  Iván Martín Gómez
 * FECHA:  Thu Nov 26 19:21:27 CET 2020
 *
 * Sintaxis: nombre_programa Argumento1 [Argumento2] ... [Argumento_n-1 [Argumento_n]]
 *
 * Argumentos Entrada: (los que están entre [] son opcionales)
 * 		Argumento 1:    -->
 * 		Argumento 2: [] -->
 * 		Argumento n: [] -->
 *
 * Salida:
 *
 * Errores:
 * 		Error 1:
 * 		Error 2:
 * 		Error n:
 *
 * Descripción: IMPONER PRIORIDAD DE EJECUCIÓN DE UNA HEBRA SOBRE OTRA
 *
 *
 */
//----------------------------------------------------------------------------
//LIBRERÍAS ESTANDAR DE C (entre < >). YA PREDEFINIDAS EN EL SISTEMA. El compilador toma el código que hay en stdio.h y lo pega en el Fichero.c donde hayamos hecho el '#include'
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // para Llamada al Sistema fork()
#include <signal.h> // para Llamada al Sistema signal()
#include <ctype.h>  // para Función de Biblioteca toupper(); pasa un char de lower case to upper case
#include <pthread.h> // para el uso de Threads (Hebras) y MUTEX (Semáforos Binarios)
#include <semaphore.h> // para el uso de Semáforos Contadores o Binarios
//LIBRERÍAS PROPIAS (entre " ")
//#include "lireria.h"
//Cuando queremos agregar una Librería con extension .a utilizar manual INSTRUCCIONES_ECLIPSE.rtf situado en el Directorio WORKFLOWProgramacion


//ALGUNAS SECCIONES DEL MANUAL QUE ES CONVENIENTE SABER:
// Para consultar el Manual hacer: man 
//  Sección (1)Mandatos,
//	Sección (2)Llamadas al Sistema,
//	Sección (3)Funciones de Biblioteca


// CABECERAS FUNCIONES UTILIZADAS DENTRO DE MAIN
	void *funcion_hebra1();
	void *funcion_hebra2();


// ZONA DECLARACION VARIABLE GLOBALES (fuera de la función main())
	sem_t semaforo; // Declaramos un Semáforo de de forma GLOBAL para que pueda ser consultado desde las funciones externas al main que cada Hebra utiliza.

// FIN ZONA DECLARACION VARIABLE GLOBALES
//---------------------------------
// EMPIEZA FUNCION MAIN
int main (int argc, char *argv[], char *envp[]){

printf("------------------------------------------------------\n");
printf("EMPIEZA EL PROGRAMA\n");

// ZONA DE DECLARACION DE VARIABLES LOCALES FUNCIÓN MAIN()
		pthread_t tid_hebra1; //Declaración hebra 1
		pthread_t tid_hebra2; //Declaración hebra 1



// FIN ZONA DE DECLARACION DE VARIABLES LOCALES FUNCIÓN MAIN()
//----------------------------------
// ZONA DE CONTROL ERRORES ARGUMENTOS
		if(argc>1){ //Hemos recibido 1 o más Argumentos:
			printf("usage: %s\n",argv[0]);
			return 1;
		}


// FIN DE CONTROL ERRORES ARGUMENTOS
//---------------------------------
// EMPIEZA FUNCIONALIDAD PROGRAMA

		sem_init(&semaforo,0,0); //Inicializamos el Semáforo a 0. Antes de crear las Hebras
		pthread_create(&tid_hebra1,NULL,funcion_hebra1,NULL);//Creamos la Hebra 1
		pthread_create(&tid_hebra2,NULL,funcion_hebra2,NULL);//Creamos la Hebra 2
		pthread_join(tid_hebra1,NULL);//Sincronizamos Hebra 1. Como todo buen programa principal espero por la Hebra 1
		pthread_join(tid_hebra2,NULL);//Sincronizamos Hebra 2. Como todo buen programa principal espero por la Hebra 2
//---------------------------------
}// FIN FUNCIÓN MAIN
//---------------------------------
// IMPLEMENTACIONES FUNCIONES CUYAS CABECERAS ESTAN DEFINIDAS AL PRINCIPIO DE ESTE DOCUMENTO Y QUE SON UTILIZADAS DENTRO DE MAIN
	void *funcion_hebra1(){
		printf("Aseguro que este mensaje de la Hebra 1 se imprimirá antes que le mensaje de la Hebra 2\n");
		sem_post(&semaforo);
		pthread_exit(NULL);
	}
	void *funcion_hebra2(){
		sem_wait(&semaforo);
		printf("Aseguro que este mensaje de la Hebra 2 se imprimirá después que le mensaje de la Hebra 1\n");
		pthread_exit(NULL);
	}


// FIN IMPLEMENTACIONES FUNCIONES CUYAS CABECERAS ESTAN DEFINIDAS AL PRINCIPIO DE ESTE DOCUMENTO UTILIZADAS DENTRO DE MAIN
