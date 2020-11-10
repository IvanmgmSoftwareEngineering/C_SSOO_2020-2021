/* TÍTULO: main.c
 * AUTOR:  Iván Martín Gómez
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
 * Descripción:
 *
 *
 */
//----------------------------------------------------------------------------
//LIBRERÍAS ESTANDAR DE C (entre < >). YA PREDEFINIDAS EN EL SISTEMA. El compilador toma el código que hay en stdio.h y lo pega en el Fichero.c donde hayamos hecho el '#include'

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

//LIBRERÍAS PROPIAS (entre " ")
//#include "lireria.h"



//(1)Mandatos, (2)Llamadas al Sistema, (3)Funciones de Biblioteca

//Cuando queremos agregar una biblioteca con extensión .a utilizar INSTRUCCIONES_ECLIPSE.rtf



// CABECERAS FUNCIONES UTILIZADAS DENTRO DE MAIN
	// CABECERA FUNCION 1
		//char *nombre_funcion_1(char *argumento_in);
	// FIN CABECERA FUNCION 1
	//---------------------------------
	// CABECERA FUNCION 2
		//tipo_out nombreFuncion2(tipo_in Arg1_in, Arg2_in,..., Argn_in);

	// FIN CABECERA FUNCION 2
	//---------------------------------
	// CABECERA FUNCION N
		//tipo_out nombreFuncionN(tipo_in Arg1_in, Arg2_in,..., Argn_in);

	// FIN CABECERA FUNCION N
// FIN CABECERAS FUNCIONES UTILIZADAS DENTRO DE MAIN
//---------------------------------

// EMPIEZA FUNCION MAIN

int main (int argc, char *argv[], char *envp[]){

	//Notas :
	//	Nota 1: **argv=*argv[]=argv[][]
	// 	Nota 2: true=1, false =0
	//	Nota 3: NULL es una constante. var=NULL ==> if(!var) ==>0
	//  Nota 4: Violaciónes de Segmento típicas: Tipo 1: intentar acceder a un Apuntador que esta apuntando a NULL
	//						 					 Tipo 2: intentar indexar un vector o array en una posición que no existe

		printf("EMPIEZA EL PROGRAMA\n");

// ZONA DE DECLARACION DE VARIABLES GLOBALES
		int i;
		int status;
		int exit_status;
		char *lista[argc];
		char *lista_aux;
		pid_t pid_fork;



// FIN ZONA DE DECLARACION DE VARIABLES GLOBALES
//------------------------------------
// ZONA DE PARSEO (Parsear Mandato = Tokenizar Mandato)

//		- Un token es un elemento atómico

//		- Tokenizar consiste en dividir un String en
//		  varios Tokens separados por un delimitador

//		- Un tokens debe cumplir con una Gramática y para comprobar que
//		  un Token pertenece a un cierto Lenguaje es necesario utilizar un
//		  analizador Léxico (¿Quizás relacionado con AFD y AFN: Asignatura LenguajesFormales)?)

//		- Hay una Función de Biblioteca strtok(3) que es muy útil para tokenizar.
//			char *str;
//			char *token;
//			str = PATH; PATH=string_1:string_2:...:string_n
//			token=strtok(str,:);  Almacena en 'token' string_1
//			token=strtok(NULL,:); Almacena en 'token' string_2. Lo que pasa aquí es que 
//					       el Indicador de Posición se ha quedado en el primer ':' después 
//					       de llamar por primera vez a strtok(), y con NULL le indicamos
//					       que se fije donde está apuntando el Indicador de Posición.

//			if(token==NULL);       Si token=NULL significa que hemos llegado al final de lo que contien str, es decir,
//					       al final de lo que contiene la variable PATH. No confundir este
//					       NULL que devuelve la Función strtok(), con el NULL de antes que le pasamos como
//					       Argumento de entrada a la función strtok().



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
		if(argc==1){ //Hemos recibido 0 Argumentos:
			printf("usage: e1: mandato [-Opcion 1] ... [-Opcion n]");
			return 1;
		}
		if(argc==2){ //Hemos recibido 1 Argumento:
			/*printf("Error nombre_programa: explicación error: %s\n",argv[1]);
			return 2;	*/
		}
		if(argc==3){//Hemos recibido 2 Argumentos:
			/*printf("Error nombre_programa: explicación error: %s\n",argv[1]);
			return 3;	*/
		}
		if(argc > 3){//Hemos recibido más de 3 Argumentos:

		}

// FIN DE CONTROL ERRORES ARGUMENTOS
//---------------------------------
// EMPIEZA FUNCIONALIDAD PROGRAMA


		pid_fork = fork();

		if(pid_fork<0){
			fprintf(stderr,"error: creando el hijo mediante la Lamada al Sistema fork()");
			exit (-1);
		}else if(pid_fork==0){ //Proceso Hijo
			printf("Soy el Hijo\n");
			for (i=1;i<argc;i++){
				lista_aux=argv[i];
				lista[i-1]=lista_aux;
			}
			lista[argc-1]= 0;
			for (i=0;i<argc;i++){
				printf("lista[%i]= %s\n",i,lista[i]);
			}
			execvp(lista[0],lista);
			exit(1);

		}else{//Proceso Padre
			printf("Soy el Padre\n");
			waitpid(pid_fork,&status,0);
			printf("Soy el Padre\n");
			if(WIFEXITED(status) != 0){
				exit_status= WEXITSTATUS(status);
				printf("Error al ejecutar el comando\n");
				//FALTA IMPRIMIR LO QUE DEVUELVE EXECVP
				printf("%s\n",strerror(exit_status));
				printf("El comando no se ejecuto correctamente\n");
			}
			exit(0);

		}


//---------------------------------
}// FIN FUNCIÓN MAIN
//---------------------------------
// IMPLEMENTACIONES FUNCIONES CUYAS CABECERAS ESTAN DEFINIDAS AL PRINCIPIO DE ESTE DOCUMENTO Y QUE SON UTILIZADAS DENTRO DE MAIN
	//IMPORTANTE: 	   No debemos devolver directamente el apuntador *buf, ya que *buf
	// 		   es una variable Local. Haciendo strdup() lo que hacemos es crear una copia
	// 		   de lo que hay donde apunta *buf, es decir, se crea una nueva zona de memoria,
	// 		   y devolvemos la copia. Este problema surge porque C puede trabajar a muy bajo
	// 		   nivel y esto es bueno para algunas cosas pero muy peligrosas para otras, este es
	// 		   un claro ejemplo del cuidado que hay que tener. En otros lenguajes de más alto nivel
	// 		   esta copia que aquí hacemos con strdup() se hace de forma interna si necesidad de
	// 		   explicitarlo en el código que redacta el programador.


	//IMPLEMENTACION FUNCION 1
	char * nombre_funcion_1(char *cmd){
		// ZONA DE DECLARACION DE VARIABLES LOCALES A LA FUNCIÓN 1
				//int i;
				char *buf;
		// FIN ZONA DE DECLARACION DE VARIABLES LOCALES A LA FUNCIÓN 1
		//---------------------------------
		// ZONA DE CONTROL ERRORES ARGUMENTOS

		// FIN DE CONTROL ERRORES ARGUMENTOS
		//---------------------------------
		// ZONA DE FUNCIONALIDAD FUNCION

		// FIN ZONA DE FUNCIONALIDAD FUNCION

		//IMPORTANTE: 	   No debemos devolver directamente el apuntador *buf, ya que *buf
		// 		   es una variable Local. Haciendo strdup() lo que hacemos es crear una copia
		// 		   de lo que hay donde apunta *buf, es decir, se crea una nueva zona de memoria,
		// 		   y devolvemos la copia. Este problema surge porque C puede trabajar a muy bajo
		// 		   nivel y esto es bueno para algunas cosas pero muy peligrosas para otras, este es
		// 		   un claro ejemplo del cuidado que hay que tener. En otros lenguajes de más alto nivel
		// 		   esta copia que aquí hacemos con strdup() se hace de forma interna si necesidad de
		// 		   explicitarlo en el código que redacta el programador.
		return strdup(buf);
	}
	//FIN IMPLEMENTACION FUNCION 1
	//---------------------------------
	//IMPLEMENTACION FUNCION 2
		//tipo_out nombreFuncion2(tipo_in Arg1_in, Arg2_in,..., Argn_in){
			//...
			//return;(si tipo_out =void) ó return algo del mismo tipo_out; (nota: recordar IMPORTANTE)
		//}
	//FIN IMPLEMENTACION FUNCION 2
	//---------------------------------
	//IMPLEMENTACION FUNCION N
		//tipo_out nombreFuncionN(tipo_in Arg1_in, Arg2_in,..., Argn_in){
			//...
			//return;(si tipo_out =void) ó return algo del mismo tipo_out; (nota: recordar IMPORTANTE)
		//}
	//FIN IMPLEMENTACION FUNCION N


// FIN IMPLEMENTACIONES FUNCIONES CUYAS CABAECERAS ESTAN DEFINIDAS AL PRINCIPIO DE ESTE DOCUMENTO UTILIZADAS DENTRO DE MAIN

