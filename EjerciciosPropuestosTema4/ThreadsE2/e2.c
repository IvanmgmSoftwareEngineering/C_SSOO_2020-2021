/* TÍTULO: e2.c
 * AUTOR:  Iván Martín Gómez
 * FECHA:  Thu Nov 26 22:35:29 CET 2020
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
 * Descripción: PROBLEMA ESCRITO-LECTOR (simplificación problema PRODUCTOR-CONSUMIDOR, ya que no debemos controlar si el buffer esta lleno o vacio)
 *										Sólo será necesario un MUTEX (sin condiciones) para controlar la Exclusión Mutua de la Sección Crítica (buf)
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
	void *funcion_hebra1_escritor_sin_mutex(void);
	void *funcion_hebra2_lector_sin_mutex(void);
	void *funcion_hebra1_escritor_con_mutex(void);
	void *funcion_hebra2_lector_con_mutex(void);
// FIN CABECERAS FUNCIONES UTILIZADAS DENTRO DE MAIN
//---------------------------------

// ZONA DECLARACION VARIABLE GLOBALES (fuera de la función main())
		#define BUFF_SIZE 10000
		int buf[BUFF_SIZE];        //Buffer con 10000 enteros Declarado de forma Global para que se pueda acceder desde las funciones que ejecutan cada una de las Hebras que son externas a la función Main()
		pthread_mutex_t mutex; // Variable de tipo MUTEX(Mutual Exclusion)=SEMÁFORO BINARIO
		//pthread_cond_t full;   // Variable de tipo CONDICIÓN de un MUTEX: Buffer lleno ==> el Productor no puede producir nada
		//pthread_cond_t empty;  // Variable de tipo CONDICIÓN de un MUTEX Buffer vacio ==> el Consumidor no puede consumir nada

// FIN ZONA DECLARACION VARIABLE GLOBALES
//---------------------------------
// EMPIEZA FUNCION MAIN
int main (int argc, char *argv[], char *envp[]){

printf("------------------------------------------------------\n");
printf("EMPIEZA EL PROGRAMA\n");

// ZONA DE DECLARACION DE VARIABLES LOCALES FUNCIÓN MAIN()
	int i;
	pthread_t tid_hebra1_escritor; //Declaración hebra 1
	pthread_t tid_hebra2_lector; //Declaración hebra 2
// FIN ZONA DE DECLARACION DE VARIABLES LOCALES FUNCIÓN MAIN()
//------------------------------------
// ZONA DE PARSEO (Parsear Mandato = Tokenizar Mandato)

		printf("------------------------------------------------------\n");
		printf("Tokenizamos el Mandato:\n");
		printf("--> Se han recibido %i Argumento/s:\n",argc-1);
		for ( i=0; i<argc;i=i+1){
			if(i==0){
				printf("---- El nombre del mandato es: %s\n",argv[0]);
			}else{
				printf("---- Argumento %i:             %s\n",i,argv[i]);
			}
		}
		printf("------------------------------------------------------\n");
		printf("\n");
// FIN ZONA DE PARSEO
//----------------------------------
// ZONA DE CONTROL ERRORES ARGUMENTOS
		if(argc>2){ //Hemos recibido 2 ó más Argumentos:
			printf("usage: %s [Argumento1]\n",argv[0]);
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



			//PROBLEMA DEL ESCRITOR-LECTOR DESDE DOS PUNTOS DE VISTA:
			// Idea esta es una versión simplificada del Problema del PRODUCTOR-CONSUMIDOR, ya que en este problema sólo hay que asegurarse de que se cumpla la Exclusión Mutua en la Sección Crítica y no debemos preocuparnos de sí el buffer esta lleno o vacio
			//	-Punto de Vista 1: sin controlar el acceso a la Sección Crítica
			//	-Punto de Vista 2: controlando el acceso a la Sección Crítica mediante el uso de MUTEX

			if(argc==1){// Implementación SIN control de Acceso a la Sección Crítica
				printf("Se Ejecutará la opción SIN control de Acceso a la Sección Crítica\n");
				printf("\n");
				pthread_create(&tid_hebra1_escritor,NULL,funcion_hebra1_escritor_sin_mutex,NULL);//Creamos la Hebra 1: PRODUCTOR
				pthread_create(&tid_hebra2_lector,NULL,funcion_hebra2_lector_sin_mutex,NULL);//Creamos la Hebra 2: CONSUMIDOR
				pthread_join(tid_hebra1_escritor,NULL);//Sincronizamos Hebra 1: PRODUCTOR
				pthread_join(tid_hebra2_lector,NULL);//Sincronizamos Hebra 2: CONSUMIDOR




			}else if(argc==2){//Implementación CON control de Acceso a la Sección Crítica utilizando MUTEX(Mutual Exclude=Semáforo Binario). Aunque la palabra Semáforo aparece, no se utilizarán funciones de la Librería semaphore.h para implementar al mutex, si no que se utilizaran funciones de la Librería pthread.h
				printf("Se Ejecutará la opción CON control de Acceso a la Sección Crítica mediante el uso de un MUTEX\n");
				printf("\n");
				pthread_mutex_init(&mutex,NULL);//Creamos el mutex
				pthread_create(&tid_hebra1_escritor,NULL,funcion_hebra1_escritor_con_mutex,NULL);//Creamos la Hebra 1: PRODUCTOR
				pthread_create(&tid_hebra2_lector,NULL,funcion_hebra2_lector_con_mutex,NULL);//Creamos la Hebra 2: CONSUMIDOR
				pthread_join(tid_hebra1_escritor,NULL);//Sincronizamos Hebra 1: PRODUCTOR
				pthread_join(tid_hebra2_lector,NULL);//Sincronizamos Hebra 2: CONSUMIDOR
				pthread_mutex_destroy(&mutex);//Destruimos el mutex

			}
			return 0;
//---------------------------------
}// FIN FUNCIÓN MAIN
//---------------------------------
// IMPLEMENTACIONES FUNCIONES CUYAS CABECERAS ESTAN DEFINIDAS AL PRINCIPIO DE ESTE DOCUMENTO Y QUE SON UTILIZADAS DENTRO DE MAIN
	void *funcion_hebra1_escritor_sin_mutex(void){
		int valor_que_escribe_escritor=1;//En cada pasada debe aumentar en uno
		int i;
		while (valor_que_escribe_escritor<11){
			for(i=0;i<BUFF_SIZE;i++){
				buf[i]=valor_que_escribe_escritor;
			}
			valor_que_escribe_escritor++;
		}
		pthread_exit(NULL);
	}
	void *funcion_hebra2_lector_sin_mutex(void){
		int numero_veces_lector_revisa_lo_que_el_escritor_ha_escrito=11;//En cada pasada debe aumentar en uno
		int contador=1;
		int valor_referencia;
		int fallo_encontrado=0;
		int i;
		while (contador<numero_veces_lector_revisa_lo_que_el_escritor_ha_escrito){
			for(i=1;i<BUFF_SIZE;i++){
					valor_referencia=buf[0];
					if(buf[i] != valor_referencia){
						fallo_encontrado=1;
					}
			}
			if(fallo_encontrado==0){
				printf("Comprobación %i. Correcto: El Escritor SI ha realizado bien su trabajo\n",contador);
			}else if (fallo_encontrado==1){
				printf("Comprobación %i. Error: El Escritor NO ha realizado bien su trabajo\n",contador);
			}
			fallo_encontrado=0;
			contador++;
		}
		pthread_exit(NULL);
	}

	void *funcion_hebra1_escritor_con_mutex(void){
		int valor_que_escribe_escritor=1;//En cada pasada debe aumentar en uno
		int i;
		while (valor_que_escribe_escritor<11){
			pthread_mutex_lock(&mutex);//Bloqueamos el acceso a la Sección Crítica (buf) por parte de otras Hebras
			for(i=0;i<BUFF_SIZE;i++){
					buf[i]=valor_que_escribe_escritor;
					break;
			}
			pthread_mutex_unlock(&mutex);//Bloqueamos el acceso a la Sección Crítica (buf) por parte de otras Hebras
			valor_que_escribe_escritor++;
		}
		pthread_exit(NULL);
	}
	void *funcion_hebra2_lector_con_mutex(void){
		int numero_veces_lector_revisa_lo_que_el_escritor_ha_escrito=11;//En cada pasada debe aumentar en uno
		int contador=1;
		int valor_referencia;
		int fallo_encontrado=0;
		int i;
		while (contador<numero_veces_lector_revisa_lo_que_el_escritor_ha_escrito){
			pthread_mutex_lock(&mutex);//Bloqueamos el acceso a la Sección Crítica (buf) por parte de otras Hebras
			for(i=1;i<BUFF_SIZE;i++){
					valor_referencia=buf[0];
					if(buf[i] != valor_referencia){
						fallo_encontrado=1;
						break;
					}
			}
			if(fallo_encontrado==0){
				printf("Comprobación %i. Correcto: El Escritor SI ha realizado bien su trabajo\n",contador);
			}else if (fallo_encontrado==1){
				printf("Comprobación %i. Error: El Escritor NO ha realizado bien su trabajo\n",contador);
			}
			fallo_encontrado=0;
			contador++;
			pthread_mutex_unlock(&mutex);//Bloqueamos el acceso a la Sección Crítica (buf) por parte de otras Hebras
		}
		pthread_exit(NULL);
	}

// FIN IMPLEMENTACIONES FUNCIONES CUYAS CABECERAS ESTAN DEFINIDAS AL PRINCIPIO DE ESTE DOCUMENTO UTILIZADAS DENTRO DE MAIN
