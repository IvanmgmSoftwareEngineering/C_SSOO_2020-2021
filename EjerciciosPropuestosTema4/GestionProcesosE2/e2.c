/* TÍTULO: e2.c
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
#include <unistd.h> // para Llamada al Sistema fork()
#include <signal.h> // para Llamada al Sistema signal()
#include <ctype.h>  // para Funcion de Biblioteca toupper(); pasa un char de lower case to upper case

//LIBRERÍAS PROPIAS (entre " ")
//#include "lireria.h"



//(1)Mandatos, (2)Llamadas al Sistema, (3)Funciones de Biblioteca

//Cuando queremos agregar una biblioteca con extensión .a utilizar INSTRUCCIONES_ECLIPSE.rtf



// CABECERAS FUNCIONES UTILIZADAS DENTRO DE MAIN
	// CABECERA manejador_padre
		void manejador_padre(int sig);
	// FIN CABECERA FUNCION 1
	//---------------------------------
	// CABECERA manejador_hijo
		void manejador_hijo(int sig);

	// FIN CABECERA FUNCION 2
	//---------------------------------
	// CABECERA FUNCION N
		//tipo_out nombreFuncionN(tipo_in Arg1_in, Arg2_in,..., Argn_in);

	// FIN CABECERA FUNCION N
// FIN CABECERAS FUNCIONES UTILIZADAS DENTRO DE MAIN
//---------------------------------

// EMPIEZA FUNCION MAIN

pid_t pid_fork; // Variable global para tratar lo que devuelve la Llamada al Sistema fork(). Se pone aquí fuera porque esta variable será consultada desde las funciones que están fuera del main()

//----------------CONCEPTO DE PIPE---------------------
		//fd=File Descriptor. Este array de 2 enteros se le pasara como argumento a la Llamada al Sistema pipe()
		//TODOS LOS PIPES SE CREAN ANTES DEL fork().
		//CUIDADO AHORA!! ESTE CONCEPTO ES ESCURRIDIZO: como el de la consulta de la variable de tipo pid_t pid_fork que devuelve la LLamada al Sistema fork()
		//											 	recordar que el valor de la variable pid_fork varia dependiendo del Proceso desde el que la estamos consultando.
		//Con la Llamada al Sistema pipe() pasa algo similar.
		//CONCEPTO DE HERENCIA DE PIPES: un pipe creado antes del fork() por un padre, es heredado por su hijo. Entonces, cuando utilizamos fd[0] y fd[1], debemos tener cuidado desde que proceso lo estamos haciendo.
		//  PIPE 1: envio de datos desde el padre al hijo
		//	fd_tph[1]=El padre escribe aqui, estando en el proceso padre
		//	fd_tph[0]=El hijo lee aqui, estando en el proceso hijo
		//Debemos cerrar por no usarlos:
		//	fd_tph[0]=El padre no lee, estando en el padre close (fd_tph[0])
		//	fd_tph[1]=El hijo no ecribe, estando en el hijo  close (fd_tph[1])
		//------------------------------------------------------
		//  PIPE 2: envio de datos desde el hijo al padre
		//	fd_thp[0]=El hijo escribe aqui, estando en el proceso hijo
		//	fd_thp[1]=El padre lee aqui, estando en el proceso padre
		//Debemos cerrar por no usarlos:
		//	fd_thp[1]=El hijo no lee, estando en el hijo  close (fd_thp[1])
		//	fd_thp[0]=El padre no escribe, estando en el padre close (fd_thp[0])
		//------------------------------------------------------
		//fd=File Descriptor. Este array de 2 enteros se le pasara como argumento a la Llamada al Sistema pipe()
		int fd_tuberia_del_padre_al_hijo[2]; //fd_tph GLOBAL PORQUE SE GESTIONA EN funcion fuera del main()
		int fd_tuberia_del_hijo_al_padre[2]; //fd_thp GLOBAL PORQUE SE GESTIONA EN funcion fuera del main()


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
		char buf_padre_envio[1024];    // El lee de teclado con Función de biblioteca fgets() y lo almacena en variable buf_padre_envio
		char buf_hijo_recepcion[1024]; // El Hijo recepciona en buf_hijo_recepcion lo que hay en buf_padre_envio a través del Pipe con flujo Padre---->>Hijo
		char buf_hijo_envio[1024];	   // El Hijo transforma lo que hay en buf_hijo_recepcion en Mayúsculas y lo guarda en la variable buf_hijo_envio
		char buf_padre_recepcion[1024];// El Padre recepciona en buf_padre_recepcion lo que hay en buf_hijo_envio a través del Pipe con flujo Hijo---->>Padre




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
		if(argc>2){ //Hemos recibido 1 o más Argumentos:
			printf("usage: e2: Se han recibido 1 o más Argumentos");
			return -1;
		}

// FIN DE CONTROL ERRORES ARGUMENTOS
//---------------------------------
// EMPIEZA FUNCIONALIDAD PROGRAMA
		pipe(fd_tuberia_del_padre_al_hijo); // creamos el Pipe para enviar datos desde el padre hacia el hijo
		pipe(fd_tuberia_del_hijo_al_padre); // creamos el Pipe para enviar datos desde el padre hacia el hijo
		printf("-----------------EMPIEZA EL PROGRAMA-----------------------\n");
		printf("Instrucciones: Ingrese caracteres por teclado y pulse enter\n");
		printf("	       Para finalizar el programa pulse Ctrl+D (FIN FICHERO)\n");
		printf("-----------------------------------------------------------\n");
		pid_fork=fork();

		if(pid_fork<0){ // ERROR AL HACER EL fork()
			printf("Error: al hacer el fork()");

		}else if(pid_fork==0){// ESTAMOS EN EL HIJO
			signal(SIGUSR1,manejador_hijo);
			signal(SIGUSR2,manejador_hijo);
			close(fd_tuberia_del_padre_al_hijo[1]);//Padre---->>Hijo el hijo solo lee, no escribe. Cierro fd_tph[1].
			close(fd_tuberia_del_hijo_al_padre[0]);//Padre<<----Hijo el hijo solo escribe, no lee. Cierro fd_thp[1].
			pause();
			while(1){ // Así evitamos que el hijo termine
				read(fd_tuberia_del_padre_al_hijo[0], buf_hijo_recepcion, 1024);

				for (i=0;i<strlen(buf_hijo_recepcion);i++){ //Cuidado aqui ¿Donde termina buf_hijo_recepcion? 1024 o después del ultimo carácter que ha recibido por teclado
					buf_hijo_envio[i]=toupper(buf_hijo_recepcion[i]);
				}

				write(fd_tuberia_del_hijo_al_padre[1], buf_hijo_envio, 1024);
				memset(buf_hijo_recepcion, 0, 1024); //Limpiamos: ponemos \0 en los 1024 caracteres
				memset(buf_hijo_envio, 0, 1024);//Limpiamos: ponemos \0 en los 1024 caracteres
				kill(getppid(),SIGUSR1);//Enviamos señal al padre para sacarle del pause
				pause(); //Pausamos al hijo para que deje trabajar al padre
			}

		}else{ // ESTAMOS EN EL PADRE
			signal(SIGUSR1,manejador_padre);
			close(fd_tuberia_del_padre_al_hijo[0]);// Padre---->>Hijo el padre solo escribe, no lee. Cierro fd_tph[0].
			close(fd_tuberia_del_hijo_al_padre[1]);//Padre<<----Hijo el padre solo lee, no escribe. Cierro fd_thp[0].
			while(fgets(buf_padre_envio, 1024, stdin) != NULL){// Bucle que no para de leer lineas hasta que pulsamos ctrl+D

				buf_padre_envio[strcspn(buf_padre_envio, "\r\n")] = 0; // Limpia lo que hemos recibido por stdin quitando saltos de linea y redimensionando el indice maximo del string
				write(fd_tuberia_del_padre_al_hijo[1], buf_padre_envio, 1024);
				kill(pid_fork,SIGUSR2); //Enviamos señal al hijo para sacarle del pause
				pause(); // Pausamos al padre para que deje trabajar al hijo
				read(fd_tuberia_del_hijo_al_padre[0], buf_padre_recepcion, 1024);
				printf("%s\n",buf_padre_recepcion);
				memset(buf_padre_envio, 0, 1024); //Limpiamos: ponemos \0 en los 1024 caracteres
				memset(buf_padre_recepcion, 0, 1024);//Limpiamos: ponemos \0 en los 1024 caracteres
				fflush(stdin); // Limpiamos stdin

			} //Salimos de while por que fgets==NULL, es decir, hemos leido el Final de Fichero (hemos pulsado ctrl+D)

			kill(pid_fork,SIGUSR1); //Enviamos señal al hijo para que muera
			wait(NULL); // Un buen padre espera por su hijo
			close(fd_tuberia_del_padre_al_hijo[1]);//Cerramos los Pipe's antes de abandonar el programa
			close(fd_tuberia_del_hijo_al_padre[0]);//Cerramos los Pipe's antes de abandonar el programa
			printf("(Ctrl+D) (Fin de Archivo)\n");
			exit (0); //Matamos al padre

		}

		//¿¿NO HA SIDO NECESARIO UTILIZAR PAUSE() NI LOS ENVIOS DE SEÑALES ASOCIADOS PARA SACAR DEL PAUSE AL PDRE DESDE EL HIJO Y AL HIJO DESDE EL PADRE (HECHO ASI EN E3, Y TAMBIEN FUNCIONA)?? ES COMO SI SE SINCRONIZASE SOLO CON LOS PIPES



		// Algunas Llamadas al Sistema y funciones necesarias:
			// fork(): crea hijo
			// pipe(): permite a un proceso hijo comunicarse con su proceso padre a tavés de un canal de comunicación y viceversa.
			// fdopen(): abre un FILE
			// close(): cierra un FILE
			// fgets(): lee de un FILE
			// fputs(): escribe en un FILE
			// fflush(): sirve para borrar el FILE stdin y ¿stdout?
			// toupper(): cambia de minúsculas a mayúsculas
			// srtcspn(): redimensiona un string, es decir, si inicialmente tenmos un string de tamaño 1024 y solo almacenamos en el un string de tamaño 2, se redimensiona a tamamño 2.
			// memset(): pone todos los valores de una cadena a un determinado valor

		//Incluir estructura básica para leer de teclado




		//Apuntamos a la primera posición de stdin rewind()
		   rewind(stdin); // Colocamos el indicador de posición de fichero al principio del Fichero stdin


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
	void manejador_padre(int sig){
		// ZONA DE DECLARACION DE VARIABLES LOCALES A LA FUNCIÓN 1

		// FIN ZONA DE DECLARACION DE VARIABLES LOCALES A LA FUNCIÓN 1
		//---------------------------------
		// ZONA DE CONTROL ERRORES ARGUMENTOS

		// FIN DE CONTROL ERRORES ARGUMENTOS
		//---------------------------------
		// ZONA DE FUNCIONALIDAD FUNCION

			//printf("Padre: Recibida señal!! %d\n", sig);
			if(sig==SIGUSR1){//El hijo
				//No hace nada, solo saca del pause al padre
			}else if (sig==SIGINT){ //El hijo no esta muerto, entonces hay que matar al hijo antes de matar al padre para evitar eque el hijo se quede en estado 'zombie'

			}

		// FIN ZONA DE FUNCIONALIDAD FUNCION
	}
	//FIN IMPLEMENTACION FUNCION 1
	//---------------------------------
	//IMPLEMENTACION FUNCION 2
		void manejador_hijo(int sig){
				//printf("Hijo: Recibida señal. %d\n", sig);
				if(sig==SIGUSR1){// El padre ha recibido ctrl+D ==> El padre ha enviado señal al hijo para que muera.
					close(fd_tuberia_del_padre_al_hijo[0]);//Cerramos los Pipe's antes de abandonar el programa
					close(fd_tuberia_del_hijo_al_padre[1]);//Cerramos los Pipe's antes de abandonar el programa
					exit(0);
				}else if(sig==SIGUSR2){// SIGINT=2 , Enviamos señal al padre y matamos al hijo
					// No hacemos nada, solo sacamos del pause al hijo
				}
		}
	//FIN IMPLEMENTACION FUNCION 2
	//---------------------------------
	//IMPLEMENTACION FUNCION N
		//tipo_out nombreFuncionN(tipo_in Arg1_in, Arg2_in,..., Argn_in){
			//...
			//return;(si tipo_out =void) ó return algo del mismo tipo_out; (nota: recordar IMPORTANTE)
		//}
	//FIN IMPLEMENTACION FUNCION N


// FIN IMPLEMENTACIONES FUNCIONES CUYAS CABAECERAS ESTAN DEFINIDAS AL PRINCIPIO DE ESTE DOCUMENTO UTILIZADAS DENTRO DE MAIN
