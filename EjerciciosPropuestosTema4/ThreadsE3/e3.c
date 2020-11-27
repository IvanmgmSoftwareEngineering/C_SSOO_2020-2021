/* TÍTULO: e3.c
 * AUTOR:  Iván Martín Gómez
 * FECHA:  Thu Nov 26 23:50:52 CET 2020
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
 * Descripción: CENA DE LOS FILÓSOFOS con tratamiento de Interbloqueo utilizando Solución Asimétrica
 *
 *
 */
//----------------------------------------------------------------------------
//LIBRERÍAS ESTANDAR DE C (entre < >). YA PREDEFINIDAS EN EL SISTEMA. El compilador toma el código que hay en stdio.h y lo pega en el Fichero.c donde hayamos hecho el '#include'
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>// para Llamada al Sistema fork()
#include <signal.h>// para Llamada al Sistema signal()
#include <ctype.h>// para Función de Biblioteca toupper(); pasa un char de lower case to upper case
#include <pthread.h>// Para el uso de Threads(Hebras) y MUTEX(Semáforos Binarios). Para utilizar la API Pthread útil para la Creación y Sincronización de Hebras. Para Compilar haciendo uso de la Librería ptherad.h se debe incluir "-lpthread" en la compilación (Ej: gcc fila.c -pthread -o programa)
#include <semaphore.h>// Para el uso de Semáforos

//LIBRERÍAS PROPIAS (entre " ")
//#include "lireria.h"
//Cuando queremos agregar una Librería con extension .a utilizar manual INSTRUCCIONES_ECLIPSE.rtf situado en el Directorio WORKFLOWProgramacion


//ALGUNAS SECCIONES DEL MANUAL QUE ES CONVENIENTE SABER:
// Para consultar el Manual hacer: man 
//  Sección (1)Mandatos,
//	Sección (2)Llamadas al Sistema,
//	Sección (3)Funciones de Biblioteca


// CABECERAS FUNCIONES UTILIZADAS DENTRO DE MAIN
	void *funcion_hebra_filosofo();
	void manejadorSenal(int signal);
// FIN CABECERAS FUNCIONES UTILIZADAS DENTRO DE MAIN
//---------------------------------
// ZONA DECLARACION VARIABLE GLOBALES (fuera de la función main())
	pthread_mutex_t vector_mutexs_palillos[5];// Variable de tipo MUTEX declarada de forma Global para que puede ser vista desde funciones externas que utiliza la hebra para su ejecución. Típicamente esta función externa que ejecuta la hebra llama a las funciones sem_wait(&semaforo) y sem_post(&semaforo) de la librería semaphore.h
// FIN ZONA DECLARACION VARIABLE GLOBALES
//---------------------------------
// EMPIEZA FUNCION MAIN
int main (int argc, char *argv[], char *envp[]){

printf("------------------------------------------------------\n");
printf("EMPIEZA EL PROGRAMA\n");

// ZONA DE DECLARACION DE VARIABLES LOCALES FUNCIÓN MAIN()
		pthread_t tid_hebra1_filosofo0;// Thread Identifier único de la Hebra1:Filósofo0
		pthread_t tid_hebra2_filosofo1;// Thread Identifier único de la Hebra2:Filósofo1
		pthread_t tid_hebra3_filosofo2;// Thread Identifier único de la Hebra3:Filósofo2
		pthread_t tid_hebra4_filosofo3;// Thread Identifier único de la Hebra4:Filósofo3
		pthread_t tid_hebra5_filosofo4;// Thread Identifier único de la Hebra5:Filósofo4
		int id_filosofo0=0; //Identificador del Filósofo 0
		int id_filosofo1=1; //Identificador del Filósofo 1
		int id_filosofo2=2; //Identificador del Filósofo 2
		int id_filosofo3=3; //Identificador del Filósofo 3
		int id_filosofo4=4; //Identificador del Filósofo 4


// FIN ZONA DE DECLARACION DE VARIABLES LOCALES FUNCIÓN MAIN()
//------------------------------------

// ZONA DE CONTROL ERRORES ARGUMENTOS
		if(argc>1){ //Hemos recibido 0 Argumentos:
			printf("usage: %s\n",argv[0]);
			return 1;
		}
// FIN DE CONTROL ERRORES ARGUMENTOS
//---------------------------------
// EMPIEZA FUNCIONALIDAD PROGRAMA

		//ZONA DE HEBRAS ("THREADS"="PROCESOS LIGEROS")
			// -Identificador Único de una Hebra: pthread_t TID (Thread Identifier)
			// -Creación de Hebras:		      pthread_create (pthread_t *tid, pthread_attr_t *attr, void *funcion, void *param)
			// 	-pthread_t *tid == Identificador Único de la Hebra. Hay que usar '&'.
			// 	-pthread_attr_t **attr == NULL
			// 	-void *funcion == Función que se llamará y será el código que ejecute la hebra. Está Función deberá ser implementada fuera de la función main
			// 	-void *param == Argumentos que recibe la función anterior
			// -Sincronización de Hebras:	      pthread_join (pthread_t tid, int **res); La Hebra que llama a esta función, espera a que finalice la hebra llamada (indicada en el primer argumento de esta función), antes de continuar
			// 	-pthread_t tid == Identificador Único de la Hebra
			// 	-int **res == devuelve el valor de retorno ¿de la función que se ejecuta en la hebra llamada?
			// -Finalización de una Hebra: 	      pthread_exit(void *res). Toda Hebra debe finalizar su ejecución con esta función.
			// 	-void *res = ¿¿Por qué aquí es apuntador simple y en join es apuntador doble??
			// -COMENTARIOS SINCRONIZACIÓN:
			// 	(Ejemplo: Productor- Consumidor)
			// 	- CONDICIÓN DE CARRERA: Cuando varios procesos (dos o más) acceden a los mismos datos de forma concurrente y el resultado de la ejecución depende del orden concreto en que se realizasen los accesos, se dice que hay una Condición de Carrera.
			// -SECCIÓN CRÍTICA: Cada hebra tiene una zona de código llamada Sección Crítica donde se accede a variables y recursos compartidos. Cuando una Hebra está ejecutando su Sección Crítica, ninguna otra Hebra puede ejecutar su correspondiente Sección Crítica. El problema de la Sección Crítica consiste en diseñar un Protocolo que permita cooperar. Cualquier diseño del Protocolo para solucionar el Problema de la Sección Crítica debe cumplir:
			// 	-EXCLUSIÓN MUTUA: Si una Hebra está ejecutando su Sección Crítica, ninguna otra Hebra puede ejecutar su correspondiente Sección Critica
			// 	-PROGRESO: Si ninguna Hebra está ejecutando su Sección Crítica, y hay varias Hebras que quieren entrar en su Sección Crítica (están esperando), sólo las Hebras que están en su sección restante (no están esperando para entrar en su Sección Crítica), piden participar en la decisión de quién entra en la Sección Crítica, y esta decisión no puede posponerse indefinidamente.
			// 	-ESPERA LIMITADA: Existe un límite de veces que se permite a otras Hebras entrar en la Sección Crítica después de que otra Hebra lo haya solicitado y antes de que se le conceda.

			// -SEMÁFOROS:
			// 	-Definición: Un Semáforo es una variable entera a la que solamente se accede mediante dos operaciones Atómicas: wait y signal 
			//	-Declaración Global Semáforo: 		sem_t sem
			//	-Inicialización de un Semáforo : 	sem_init (sem_t *sem, int shared, int valor)
			//		-sem_t *sem== Apuntador a una variable de tipo sem_t (hay que utilizar operador &)
			//		-int shared== 0
			//		-int valor== valor inicial
			//	-Destrucción de un Semáforo: 		int sem_destroy(sem_t *sem)
			//	-Operación Atómica wait: 		sem_wait(sem_t *sem)
			//	-Operación Atómica signal: 		sem_post(sem_t *sem)
			//	-Los Semáforos no son parte del estándar Pthread, pero se pueden utilizar de forma conjunta.
			// 	-Operación Atómica: es una operación que garantiza que se ejecutará sin que ningún otro proceso la interrumpa de forma concurrente (Ejemplo: recordar que cuando un proceso 1 varios printf11(), printf12(),... y otro proceso está ejecutando varios printf21(), printf22(),... de forma concurrente, se alternarán los printf's() de ambos procesos por estarse ejecutando de forma concurrente. Pero lo que si sabemos es que un printf() es una operación atómica ya que hasta que no se ejecute el printf() de un proceso, no podrá imprimirse el printf() de otro proceso. 
			// 	-Las operaciones wait y signal deben ser atómicas, por lo que internamente utilizarán inhibidos de Interrupciones (TestAndSet, Swap, etc)
			// 			wait (S){// La librería semaphore.h tiene una implementación de esta función llamada sem_wait(*sem)
			// 				S--
			// 				if(S<0){
			// 					//añadir el proceso a la lista de bloqueados
			// 					block();
			// 				}
			//			}

			// 			signal(S){// La librería semaphore.h tiene una implementación de esta función llamada sem_post(*sem)
			// 				S++
			// 				if(S<=0){
			// 					//sacar el proceso P de la lista
			// 					wakeup();
			// 				}
			// 			}

			// 	-Dos tipos de Semáforos:
			// 		-SEMÁFORO BINARIO (MUTEX = Mutual Exclusion)
			// 			-Sólo puede tomar los valores 0 o 1
			// 			-Permiten resolver el Problema de la Sección crítica
			// 			-Permiten resolver otros Problemas de Sincronización: Ejemplo: garantizar que un proceso se ejecuta antes que otro d
			// 			-INTERBLOQUEOS: dos o más Hebras están esperando indefinidamente por un evento que sólo puede generar uno de las Hebras blóquelas. Ejemplo: 2 Semáforos S1 y S2 inicializados a 1, y 2 Hebras ejecutando H0 y H1-
			// 			-El MUTEX es un mecanismo liviano ideal para Hebras(Procesos Ligeros)
			// 			-Un MUTEX es Semáforo Binario (0 o 1 valores posibles) sin memoria
			// 				-Cerradura cerrada, llave puesta
			// 				-Abro y cierro y me llevo la llave
			// 			-Dos Acciones Atómicas: mutex_lock(m) y mutex_unlock(m)
			// 				mutex_lock(m)
			// 					//Código Sección Critica
			// 				mutex_unlock(m)
			// 				-No se puede hacer un mutex_unlock(m) sin haber hecho previamente un mutex_lock(m). Esto pasa igual que con Tuberías. Si hacemos un close() de una Tubería de la cual no hemos hecho antes un pipe() es un error que es difícil de depurar.
			// 			-MUTEX CON CONDICIÓN:
			// 				-A veces la Hebra que está en la Sección Crítica, no puede continuar, porque no se cumple cierta CONDICIÓN que sólo podría cambiar otra Hebra desde dentro de la Sección Crítica. Para solucionar esto, es preciso:
			// 					-Liberar temporalmente el MUTEX que protege la Sección Crítica mientras se espera a que la CONDICIÓN se cumpla.
			// 					-Sin abandonar la Sección Crítica
			// 					-De forma Atómica
			// 				-La implementación de un MUTEX (==SEMÁFORO BINARIO) no se hace utilizando funciones de la Librería semaphore.h, si no que se implementa haciendo uso de funciones de la Librería pthread.h
			// 				-Declaración MUTEX: 			pthread_mutex_t mutex
			// 				-Inicialización de un MUTEX:		int pthread_mutex_init(pthread_mutex_t *mutex, pthread_mutexattr_t *attr)
			// 					-pthread_mutex_t *mutex=Apuntador a una variable de tipo pthread_mutex_t (debemos utilizar &)
			// 					-pthread_mutexattr_t *attr= atributos MUTEX
			// 				-Destrucción de una MUTEX:		int pthread_mutex_destroy(pthread_mutex_t *mutex)
			// 				-Operaciones básicas con MUTEX:-
			// 					-Competir por coger el MUTEX:	int pthread_mutex_lock(pthread_mutex_t *mutex)
			// 					-Devolver el MUTEX:		int pthread_mutex_unlock(pthread_mutex_t *mutex)

			// 				-Declaración CONDICIÓN MUTEX: 		pthread_cond_t cond_mutex
			// 				-Inicialización CONDICIÓN de un MUTEX:	int pthread_cond_init(pthread_cond_t *cond_mutex, pthread_condattr_t *attr)
			// 					-pthread_cond_t *cond_mutex= apuntador a una variable de tipo condicion_mutex (se debe utilizar operador &)
			// 					-pthread_condattr_t *attr= atributos condición
			// 				-Destrucción de CONDICIÓN de un MUTEX:	int pthread_cond_destroy(pthread_cond_t *cond_mutex)
			// 				-Operaciones básicas con CONDICIONES de MUTEX
			// 					-Operación 1:Sin salir de la Sección Crítica, libera temporalmente el MUTEX que la protege, para esperar a que se cumpla la CONDICIÓN
			// 						int pthread_cond_wait(pthread_cond_t *cond_mutex, pthread_mutex_t *mutex)
			// 					-Operación 2: Señalar un cambio que permita continuar a una las Hebras que espera en la condición
			// 						int pthread_cond_signal(pthread_cond_t *cond_mutex)
			// 					-Operación 3: Señalar un cambio que permita continuar a todos las Hebras que esperan en la CONDICIÓN
			// 						int pthread_cond_broadcast(pthread_cond_t *cond_mutex)

			// 		-SEMÁFORO CONTADOR
			// 			-Pueden tomar valores dentro de un rango
			// 			-Se pueden utilizar para resolver el Problema de control de acceso a un determinado recurso formado por un número finito de instancias (recursos limitados) Ejemplo: Control de acceso a un Parking en base a plazas disponibles

			// 			-Los Semáforos Contadores son mecanismos más pesados, más adecuados para Procesos
			// 	-PROBLEMA PRODUCTOR-CONSUMIDOR: Tres semáforos compartidos por las dos Hebras (Hp=HebraProductor y Hc=HebraConsumidor), S1(Semáforo 1: llamado 'sección' inicializado a 1), S2(Semáforo 2: llamado 'full' inicializado a 0) y S3(Semáforo 3: llamado 'empty' inicializado a N

//-----------------------------------------------------------------------------------
			// CENA DE LOS FILÓSOFOS
		signal(SIGINT,manejadorSenal);
			pthread_mutex_init(&vector_mutexs_palillos[0],NULL);//Inicializamos MUTEX1:Palillo0
			pthread_mutex_init(&vector_mutexs_palillos[1],NULL);//Inicializamos MUTEX2:Palillo1
			pthread_mutex_init(&vector_mutexs_palillos[2],NULL);//Inicializamos MUTEX3:Palillo2
			pthread_mutex_init(&vector_mutexs_palillos[3],NULL);//Inicializamos MUTEX4:Palillo3
			pthread_mutex_init(&vector_mutexs_palillos[4],NULL);//Inicializamos MUTEX5:Palillo4

			pthread_create(&tid_hebra1_filosofo0,NULL,funcion_hebra_filosofo,&id_filosofo0);//Creamos la Hebra1:Filósofo1
			pthread_create(&tid_hebra2_filosofo1,NULL,funcion_hebra_filosofo,&id_filosofo1);//Creamos la Hebra2:Filósofo2
			pthread_create(&tid_hebra3_filosofo2,NULL,funcion_hebra_filosofo,&id_filosofo2);//Creamos la Hebra3:Filósofo3
			pthread_create(&tid_hebra4_filosofo3,NULL,funcion_hebra_filosofo,&id_filosofo3);//Creamos la Hebra4:Filósofo4
			pthread_create(&tid_hebra5_filosofo4,NULL,funcion_hebra_filosofo,&id_filosofo4);//Creamos la Hebra5:Filósofo5

			pthread_join(tid_hebra1_filosofo0,NULL);//Sincronizamos la Hebra1:Filósofo1
			pthread_join(tid_hebra2_filosofo1,NULL);//Sincronizamos la Hebra2:Filósofo2
			pthread_join(tid_hebra3_filosofo2,NULL);//Sincronizamos la Hebra3:Filósofo3
			pthread_join(tid_hebra4_filosofo3,NULL);//Sincronizamos la Hebra4:Filósofo4
			pthread_join(tid_hebra5_filosofo4,NULL);//Sincronizamos la Hebra5:Filósofo5

			pthread_mutex_destroy(&vector_mutexs_palillos[0]);//Destruimos MUTEX1:Palillo1
			pthread_mutex_destroy(&vector_mutexs_palillos[1]);//Destruimos MUTEX2:Palillo2
			pthread_mutex_destroy(&vector_mutexs_palillos[2]);//Destruimos MUTEX3:Palillo3
			pthread_mutex_destroy(&vector_mutexs_palillos[3]);//Destruimos MUTEX4:Palillo4
			pthread_mutex_destroy(&vector_mutexs_palillos[4]);//Destruimos MUTEX5:Palillo5

			while(1); //Bucle infinito como nos dice el enunciado

//---------------------------------
}// FIN FUNCIÓN MAIN
//---------------------------------
// IMPLEMENTACIONES FUNCIONES CUYAS CABECERAS ESTAN DEFINIDAS AL PRINCIPIO DE ESTE DOCUMENTO Y QUE SON UTILIZADAS DENTRO DE MAIN
	void *funcion_hebra_filosofo(int *arg_id_filosofo){
		int id_filosofo= *arg_id_filosofo;
		srand (time(NULL)); // Va cambiando la semilla que utiliza la función rand() para generar el número aleatorio (la hora del sistema es diferente cada vez que ejecutemos el programa), si no hiciesemos esto la función rand() siempre devolveria los mismo numeros aleatorio en distintas ejecuaciones, ya que utiliza siempre la misma semilla
		while(1){
			printf("Filósofo %i pensando\n",id_filosofo);
			sleep((rand() % 5)+1); // (rand() % (N-M+1))+M devuelve un número aleatorio decimal entre M=1 y N=5
			printf("Filósofo %i quiere comer\n",id_filosofo);
			//Solución asimétrica para resolver interbloqueos: imponemos que un filósofo impar coge siempre primero el palillo de su izquierda. Mientras que un Filósofo par(cero incluido) coge siempre el palillo de su derecha.
			if(id_filosofo%2==0){//El Filósofo tiene un número par==> imponemos que el Filósofo con número par coja siempre el palillo de su derecha
				pthread_mutex_lock(&vector_mutexs_palillos[id_filosofo+1 % 5]);//El Filosofo i intenta coger el paillo que tiene a su derecha
				pthread_mutex_lock(&vector_mutexs_palillos[id_filosofo]);//El Filosofo i intenta coger el paillo que tiene a su izquierda
					printf("Filósofo %i comiendo\n",id_filosofo);
					sleep((rand() % 5)+1); // (rand() % (N-M+1))+M devuelve un número aleatorio decimal entre M=1 y N=5
				pthread_mutex_unlock(&vector_mutexs_palillos[id_filosofo+1%5]);//El Filosofo i intenta coger el paillo que tiene a su derecha
				pthread_mutex_unlock(&vector_mutexs_palillos[id_filosofo]);//El Filosofo i intenta coger el paillo que tiene a su izquierda
			}else{//El Filósofo tiene un número impar==>//El Filósofo tiene un número par==> imponemos que el Filósofo con número impar coja siempre el palillo de su izquierda
				pthread_mutex_lock(&vector_mutexs_palillos[id_filosofo]);//El Filosofo i intenta coger el paillo que tiene a su izquierda
				pthread_mutex_lock(&vector_mutexs_palillos[id_filosofo+1 % 5]);//El Filosofo i intenta coger el paillo que tiene a su derecha
					printf("Filósofo %i comiendo\n",id_filosofo);
					sleep((rand() % 5)+1); // (rand() % (N-M+1))+M devuelve un número aleatorio decimal entre M=1 y N=5
				pthread_mutex_unlock(&vector_mutexs_palillos[id_filosofo]);//El Filosofo i intenta coger el paillo que tiene a su izquierda
				pthread_mutex_unlock(&vector_mutexs_palillos[id_filosofo+1%5]);//El Filosofo i intenta coger el paillo que tiene a su derecha

			}


		}


		pthread_exit(NULL);
	}


	void manejadorSenal(int signal){
		if(signal==SIGINT){
			printf("CTRL+C ha sido pulsado. Saliendo Programa. BYE BYE!!");
			exit(signal);
		}
	}

// FIN IMPLEMENTACIONES FUNCIONES CUYAS CABECERAS ESTAN DEFINIDAS AL PRINCIPIO DE ESTE DOCUMENTO UTILIZADAS DENTRO DE MAIN
