/* TÍTULO: prueba.c
 * AUTOR:  Iván Martín Gómez
 * FECHA:  Sun Nov 22 20:59:58 CET 2020
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
#include <ctype.h>  // para Función de Biblioteca toupper(); pasa un char de lower case to upper case

//LIBRERÍAS PROPIAS (entre " ")
//#include "lireria.h"
//Cuando queremos agregar una Librería con extension .a utilizar manual INSTRUCCIONES_ECLIPSE.rtf situado en el Directorio WORKFLOWProgramacion


//ALGUNAS SECCIONES DEL MANUAL QUE ES CONVENIENTE SABER:
// Para consultar el Manual hacer: man 
//  	Sección (1)Mandatos,
//	Sección (2)Llamadas al Sistema,
//	Sección (3)Funciones de Biblioteca


// CABECERAS FUNCIONES UTILIZADAS DENTRO DE MAIN
	// CABECERA FUNCION 1: Manejo señales Procesos
		void manejador_padre(int signal);
	// FIN CABECERA FUNCION 1
	//---------------------------------
	// CABECERA FUNCION 2: Manejo señales Procesos
		void manejador_hijo_1(int signal);
	// FIN CABECERA FUNCION 2
	//---------------------------------
	// CABECERA FUNCION 3: Manejo señales Procesos
		void manejador_hijo_2(int signal);
	// FIN CABECERA FUNCION 3
	//---------------------------------
	// CABECERA FUNCION 4: Manejo señales Procesos
		void manejador_hijo_3(int signal);
	// FIN CABECERA FUNCION 4
	//---------------------------------
	// CABECERA FUNCION 5: Manejo señales Procesos
		void manejador_hijo_4(int signal);
	// FIN CABECERA FUNCION 5
	//---------------------------------
// FIN CABECERAS FUNCIONES UTILIZADAS DENTRO DE MAIN
//---------------------------------

// ZONA DECLARACION VARIABLE GLOBALES (fuera de la función main())
	 //nota: La función main() aún siendo una función especial por ser la función de entrada y de salida del programa,
         //	 es una función más, por lo que las variables declaradas dentro de la función main() serán variables  
	 // 	 locales a la función main(), es decir, no serán accesibles desde funciones externas a la función main().
	 // 	 Si queremos tener acceso a una variable desde cualquier función, debemos declararla aquí.
	 //	 Este comentario es útil para:
	 //		- Funciones manejadoras de señales; utilizan pid_fork
	 //	 	- Funciones que devuelven una variable de tipo apuntador; se debe hacer copia antes de hacer return


pid_t vector_PIDs[5]; //Declaramos e Inicializamos de forma estática la Variable de tipo vector, Global para almacenar lo que devuelven las Llamadas al Sistema fork(2). Esto nos permitirá conocer los PIDs entre Procesos que son Hermanos. Se pone aquí fuera porque esta variable para que pueda ser consultada desde las funciones manejadoras() de señales que están fuera del main()
					  //vector_PIDs[0]= guardamos PID único del padre
					  //vector_PIDs[1]= guardamos PID único del Hijo1
					  //vector_PIDs[2]= guardamos PID único del Hijo2
					  //vector_PIDs[3]= guardamos PID único del Hijo3
					  //vector_PIDs[4]= guardamos PID único del Hijo4
pid_t pid1_fork; // Variable global para tratar lo que devuelve la Llamada al Sistema fork(2). Se pone aquí fuera porque esta variable para que pueda ser consultada desde las funciones manejadoras() de señales que están fuera del main()
pid_t pid2_fork; // Variable global para tratar lo que devuelve la Llamada al Sistema fork(2). Se pone aquí fuera porque esta variable para que pueda ser consultada desde las funciones manejadoras() de señales que están fuera del main()
pid_t pid3_fork; // Variable global para tratar lo que devuelve la Llamada al Sistema fork(2). Se pone aquí fuera porque esta variable para que pueda ser consultada desde las funciones manejadoras() de señales que están fuera del main()
pid_t pid4_fork; // Variable global para tratar lo que devuelve la Llamada al Sistema fork(2). Se pone aquí fuera porque esta variable para que pueda ser consultada desde las funciones manejadoras() de señales que están fuera del main()


// FIN ZONA DECLARACION VARIABLE GLOBALES
//---------------------------------
// EMPIEZA FUNCION MAIN
int main (int argc, char *argv[], char *envp[]){

	//Notas :
	//	Nota 1: **argv=*argv[]=argv[][]
	// 	Nota 2: true=1, false =0
	//	Nota 3: NULL es una constante. var=NULL ==> if(!var) ==>0
	//  	Nota 4: Violaciones de Segmento típicas: una Violación de Segmento se produce cuando intentamos acceder a una zona de memoria a la que no tenemos derecho 
	//						 Tipo 1: intentar acceder a un Apuntador que esta apuntando a NULL
	//						 Tipo 2: intentar indexar un vector o array en una posición que no existe
	//  	Nota 5: Toda variable se deben DECLARAR y INICIALIZAR
	//		Destacamos la Declaración e Inicialización de las variables de tipo Apuntador (el uso de los Apuntadores es una de las característica más relevantes que distingue al Lenguaje de Programación C de otros Lenguajes de Programación de más alto nivel
	//				Declaración: (char *linea ó char linea[]) (Vector) ó (char **lista_lineas ó char lista_lineas[][]) (Matriz) 
	//				             (int *numeros ó int numeros[]) (Vector) ó (int **lista_numeros ó int lista_numeros[][]) (Matriz)
        //			                Inicialización: (3 posibles formas):
	//					- FORMA 1: de forma explícita. linea =['a' 'b' 'c' 'd'] 
	//					- FORMA 2: Asignación Memoria Estática. char *linea [1024]. Esta forma de hacerlo implica hacer la Inicialización en la misma línea que la Declaración 
	//					- FORMA 3: Asignación Memoria Estática. Mediante uso de malloc(3)
	//					  (nota): Cuando asignamos Memoria, ya sea de forma Estática o de forma Dinámica, estamos Inicializando la variable de tipo apuntador, esto se debe que en ese momento la variable apuntador empieza apuntar a una zona de memoria, hasta el momento de la Inicialización, la variable de tipo Apuntador no apuntaba a ninguna parte.

	//	Nota 6: No confundir Reserva de Memoria Dinámica con Reserva de Memoria Estática.

	//		--> Reserva de Memoria ESTÁTICA: se utiliza cuando SI conocemos el tamaño de memoria que vamos a reservar en tiempo de COMPILACIÓN
	//						Ejemplo: *char[argc] No es una asignación de Memoria Estática ya que el valor de argh no lo conocemos en tiempo de Compilación. Aunque C permita hacerlo esto está mal.
	//		--> Reserva de Memoria DINÁMICA: se utiliza cuando NO conocemos el tamaño de memoria que vamos a reservar en tiempo de COMPILACIÓN, pero SI lo conocemos en tiempo de EJECUCIÓN



	//	Nota 7:
printf("---------------------\n");
printf("EMPIEZA EL PROGRAMA\n");

// ZONA DE DECLARACION DE VARIABLES LOCALES FUNCIÓN MAIN()
		int i;
		int status;
		int exit_status;




// FIN ZONA DE DECLARACION DE VARIABLES LOCALES FUNCIÓN MAIN()
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

//			if(token==NULL);       Si token=NULL significa que hemos llegado al final de lo que contiene str, es decir,
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
		if(argc>1){ //Hemos recibido 0 Argumentos:
			printf("usage: %s Argumento1 [Argumento2] ... [Argumento_n-1 [Argumento_n]]",argv[0]);
			return 1;
		}


// FIN DE CONTROL ERRORES ARGUMENTOS
//---------------------------------
// EMPIEZA FUNCIONALIDAD PROGRAMA

		//ZONA DE PROCESOS

			//CONCEPTOS PREVIOS:


//------------>Creación de nuevos Procesos con la Llamada al Sistema fork(2)

				//nota 1): Cuando hacemos un fork() se crea un Proceso Hijo (Copia exacta del código del Padre)

				//nota 2): No confundir lo que devuelve la Llamada al Sistema fork() con el pid (Proccess Identifier -Único- para cada proceso) de un 
				//	  Proceso, aunque típicamente se tiende a llamar a la variable de tipo pid_t, que devuelve el fork(), 'pid', NO ES LO MISMO,
				//	  el pid único para cada Proceso se consulta utilizando la Llamada al Sistema getpid().  Por todo lo dicho anteriormente, 
				//        no llamo a lo que devuelve la Llamada al Sistema fork() 'pid', si no que lo llamo 'pid_fork'

				//nota 3): 


//------------>Recepción y manejo de señales con Función de Biblioteca signal(3)

				//nota 1): La Función de Biblioteca signal(3) se llama desde el código del Proceso que recibe la señal.

				//nota 2): Tiene un sintaxis 'fea': Ejemplo: signal(SIGINT,manejador). La función manejador recibe como Argumento de entrada un 'int' pero aquí 
				//	  no se especifica. Lo que esta pasando internamente es que la Función de Biblioteca signal(3) le está pasando
				//	  como Argumento la señal SIGINT (que es un entero) a la función, implementada por nosotros, void manejador(int signal)

				//nota 3): Hay dos formas de gestionar las señales:

				//	  -Forma 1: Tenemos una única función 'manejador()' que se encarga de gestionar las señales recibidas en el Padre y en el hijo
				//	  	   -Esta forma de hacerlo implica tener un código en la Función manejador() que nos permita saber si estamos en el Padre o en el Hijo
				//	  	   -Esta forma de hacerlo implica también que signal deberá hacerse desde una zona del código compartida por padre e hijo

				//	  -Forma 2:(ESTA ES LA MEJOR FORMA) Tenemos dos funciones manejadoras: manejador_padre() y manejador_hijo()
				//	  	   que se encarga de gestionar las señales recibidas en el Padre y en el hijo
				//	  	   -Esta forma de hacerlo implica que habrá dos signal: uno para escuchar las señales que recibe el Padre y
				//	   	    otro signal para escuchar las señales que recibe el Hijo

				//nota 4): Algunas señales útiles 

				// 	-Señal SIGINT: Ctrl +C
				// 	-Señal SIGUSR1: Para uso del Programador
				// 	-Señal SIGUSR2: Para uso del Programador

				//nota 5): Ctrl+D no es una señal. Esta secuencia de teclas es utilizada para informar de final de fichero. Ejemplo: si estamos leyendo líneas por teclado en la Entrada Estándar mediante la Función de Biblioteca fgets(3) y queremos que fgets devuelva NULL, pulsando ctrl+d lo conseguimos. 

				//nota 6): Hay dos 'Macros' útiles para la gestión de señales:  

				//	  WIFEXITED (status): distinto de cero si el hijo ha terminado

				//	  WESISTATUS (status): nos permite conocer el valor que ha devuelto el hijo mediante el exit(1). Sólo se puede consultar si WIFEXITED es distinta de cero, es decir, si el hijo ha terminado

				//nota 7): 

				//nota 8): Variable stderr, con un número de error asociado errno y la Función de Biblioteca strerror(3) para gestionar: errores de Mandatos, Llamadas al Sistema y Funciones de biblioteca 

//------------>Envío de señales con Mandato Kill(1)

				//nota 1): No hay que malinterpretar la palabra 'Kill'. Este mandato NO se utiliza para matar un Proceso, se utiliza para enviar una señal desde un Proceso 
				//nota 2): El mandato kill(1) se ejecutará desde el código del Proceso que desea enviar la señal. 

				//nota 3): Será útil la Llamada al Sistema getppid(2) para conseguir el pid único del Proceso Padre

				//nota 4): 

//------------>Creación y uso de Tuberías con Comando de Administración de Sistema pipe(8)

				//nota 1): int fd[2]. Este array de 2 enteros se le pasará cómo argumento a pipe()

				//nota 2): La comunicación de una Tubería (una llamada a pipe()) es unidireccional, es decir, si queremos una comunicación bidireccional, entonces se necesitan dos Tuberías (dos llamadas a pipe())

				//nota 3): TODOS LOS PIPES SE CREAN ANTES DEL fork().

				//nota 4): CUIDADO AHORA!! ESTE CONCEPTO ES ESCURRIDIZO:
				//	  CONCEPTO DE HERENCIA DE PIPES: un pipe creado antes del fork() por un padre, es heredado por su hijo. Entonces, cuando utilizamos fd[0] y fd[1], debemos tener cuidado desde que proceso lo estamos haciendo.

				//	  TUBERÍA 1: envío de datos desde el proceso PADRE --> HIJO

				//	  	fd_tuberia_padre_hijo[1]; El padre escribe aquí, estando en el proceso padre

				//	  	fd_tuberia_padre_hijo[0]; El hijo lee aqui, estando en el proceso hijo

				//	  Debemos cerrar por no usarlos:

				//	  	fd_tuberia_padre_hijo[0]; El Padre sólo Envía (escribe), el padre no Recibe (no lee) ==> estando en el padre hacemos close (fd_tuberia_padre_hijo[0])

				//	  	fd_tuberia_padre_hijo[1]; El Hijo sólo Recibe (lee), el Hijo no Envía (no escribe) ==> estando en el hijo hacemos close (fd_tuberia_padre_hijo[1])

				//	  TUBERÍA 2: envío de datos desde el proceso HIJO --> PADRE

				//	  	fd_tuberia_hijo_padre[1]; El Hijo escribe aquí, estando en el proceso hijo

				//	  	fd_tuberia_hijo_padre[0]; El Padre lee aquí, estando en el proceso padre

				//	  Debemos cerrar por no usarlos:

				//	  	fd_tuberia_hijo_padre[0]; El Hijo sólo Envía (escribe), el hijo no Recibe (no lee) ==> estando en el hijo hacemos close (fd_tuberia_hijo_padre[0])

				//	  	fd_tuberia_hijo_padre[1]; El Padre sólo Recibe (lee), el padre no Envía (no escribe) ==> estando en el padre hacemos close (fd_tuberia_hijo_padre[1])

				//nota 5): Las Tuberías se pueden gestionar de dos formas: 

//				- Forma 1: Mediante el Uso de Descriptores de Fichero 
//					   Utilizaremos Llamadas al sistema (2) cómo: write(), read() y close()

//				- Forma 2: Mediante el Uso de File * 
//					   Utilizaremos Funciones de Biblioteca (3) cómo: fopen(), fputs(), fgets() y fclose()

//			nota 6): La lectura del pipe SI es bloqueante. La escritura del pipe NO es bloquearte

//			nota 7): RECORDAR QUE SIMPRE: hay que cerrar los pipes antes salir del programa. Es algo similar a liberar memoria dinámica.

//			nota 8):


//------------>Matar a un Proceso: Utilizamos Mandato exit(1): esta es la única manera de poner un proceso en estado Terminado (muerto)

			  //			Recordar siempre: "Todo buen padre espera por sus hijos". Esta frase es importante tenerla en cuanta para

			  //					   que ningún hijo se quede en esta 'Zombie' \<defunct\>, es decir, si un padre muere antes que su hijo, el hijo se queda en estado 'Zombie'.

//------------>Diferencias entre: wait(1), waitpid(2), pause(3) y sleep(1) 

			  //	- wait(1): wait(NULL) pone en estado de pausa el proceso que la llama hasta que uno (cualquiera) de sus procesos hijos muere

			  //	- waitpid(2): pone en estado de pausa el proceso que la llama hasta que EL (uno en concreto) proceso hijo muere

			  //	- pause(3): no recibe ningún Argumento de entrada y el Proceso que llama se queda en estado de pausa hasta que recibe una señal

			  //	- sleep(1): recibe cómo Argumento de entrada el tiempo que el Proceso debe quedarse en estado de pausa.


//------------>Ejecución de mandatos mediante execvp(3)


//				nota 0): Cuidado aquí entre el concepto de Tubería que creamos mediante pipe(2) para que dos Procesos se comuniquen y el concepto de pipe '|' que utilizamos para redireccionar la Salida Estándar de un Mandato a la Entrada Estándar de otro mandato.
//				nota 1): Siempre que hagamos uso de execvp(3), deberemos crear un Proceso Hijo, y será el Hijo quien haga uso de execvp(3)
//				nota 2): Para comunicar al Proceso Padre y al Proceso Hijo haremos uso de Tuberias (pipe(2))

//				nota 3): La función de Biblioteca execvp(3), sólo admite UN ÚNICO mandato (junto con sus argumentos) en sus Argumentos. No admite redirecciones ni pipes
//				nota 4): Diferenciar entre Redirecciones '>' ó '<' y Pipe '|'
//					(nota: las redireción de salida tiene la siguiente sintaxis mandato>fichero)
//					(nota: las redireción de entrada tiene la siguiente sintaxis fichero<mandato)
//					(nota: los pipes tienen la siguiente sintaxis mandato1|mandato2|...|mandaton)
//					(nota: Con los pies hay que tener cuidado de que mandatos ponemos en cada posición (Concepto de mandatos de tipo Filtro, concepto de que un mandato reciba información a través de Argumentos o a través de la Entrada Estándar.)
//					(nota: Mezcla Redirecciones y pipes: fichero_in < mandato1|mandato2|...|mandaton > fichero_out) En este caso, únicamente la Entrada Estándar del primer mandato que aparece en la cadena de pipes será redirigida a fichero_in, de igual forma, únicamente la Salida Estándar del último mandato que aparece en la cadena de pipes será redirigida a fichero_out 
//					-Redirecciones:
//							Redireccion de la Entrada Estándar: fichero 0< mandato == fichero < mandato. Por defecto, si no especificamos ninguna redirección de entrada, el mandato tiene asociada la Entrada Estándar con el fichero stdin (TECLADO)  

//							Redireccion de la Salida Estándar: mandato 1> fichero == fichero > mandato. Por defecto, si no especificamos ninguna redirección de salida, el mandato tiene asociada la Salida Estándar con el fichero stdout (PANTALLA)  

	//						Redireccion de la Salida Estándar Error: mandato 2> fichero. Por defecto, si no especificamos ninguna redirección de salida, el mandato tiene asociada la Salida Estándar Error con el fichero stdout (PANTALLA)  


//				nota 5): Cuando dentro de un proceso Hijo hacemos un execvp(3), lo que se hace es sustituir el código del Hijo por el Código del mandato que le hemos pasado como argumento a execvp(3). En caso de todo ir bien y no producirse ningún error en la ejecución del mandato que hemos pasado cómo argumento a execvp(3), se hará un exit(0) lo que hará que el Hijo muera. En caso de que algo haya ido mal en el execvp(3), se almacenará en la variable 'errno' el código de error y se seguirá con la siguiente línea de código que el Hijo tenga a continuación de execvp(3). 


//				nota 6): Por la nota1) Concluimos que execvp(3) deber recibir el mandato LIMPIO (mandato + argumentos, nada más, ni redirecciones ni pipes)
//				nota 7): Por la nota 4), si queremos ejecutar una línea que contenga Redirecciones debemos:
//						-Paso 1: Limpiar la línea y quedarnos sólo con el mandato y con sus argumentos Limpios 
//						-Paso 2: Antes de hacer execvp(3) debemos hacer la redirección utilizando dup(2) ó dup2(2)
//						(nota: dup() hace una copia del Descriptor de fichero que recibe como argumento y a la copia le asigna el número de Descriptor de Fichero más bajo disponible.
//						(nota: dup2()=close(1)+dup() hace una copia del Descriptor de fichero que recibe como argumento y a la copia le asigna el número de Descriptor de Fichero que le hemos indicado como Argumento.)
//						(nota: Por qué dup2()=close(1)+dup()? Porque haciendo close(1) dejamos libre el número de Descriptor de fichero 1 y a continuación dup() cómo toma el primer número de Descriptor de Fichero más bajo disponible, sabemos que tomará el 1)


//				nota 8): Por la nota 4), si queremos ejecutar una línea que contenga varios mandatos unidos por pipes '|' (mandato1|mandato2|mandato3|...) debemos:
//						-Paso 1: Parsear la línea: y quedarnos con cada mandato (junto con sus argumentos) de forma individual
//						-Paso 2: Crear Tuberías desde el Padre a cada uno de sus hijos para enviarle un único mandato (junto con sus argumentos)
//						-Paso 3: Creamos tantos Procesos Hijos como mandatos tenga la línea (número de mandatos= número de apariciones símbolo '|' -1). Notar que debemos hacer los fork() de forma controlada para que siempre se hagan en el padre y así asegurarnos que todos los Hijos son Hermanos entre sí.
//						-Paso 4: Para hacer posible la Transferencia de información entre los hermanos, notar que los hermanos deben conocer los números únicos de PID's de sus otros hermanos, para esto nos ayudamos de pid_t *vector_PIDs definido de forma GLOBAL
//						-Paso 5: Debemos crear Tuberías para que los hermanos puedan transferirse información entre ellos.
//						-Paso 6: Haciendo uso de dup() o dup2(). Cada Hijo deberá, antes de utilizar execvp(3), redirigir la Salida Estándar hacia el pipe de escritura 
//						-Paso 7: Haciendo uso de dup() o dup2(). ¿Cada Hijo deberá, antes de utilizar execvp(3), redirigir la Entrada Estándar hacia el pipe de lectura? 
//						-Paso 8: Haciendo uso de dup() o dup2(). Cada Hijo deberá, antes de utilizar execvp(3), redirigir la Salida Estándar Error hacia el pipe de escritura 
//						-Paso 9: Cada hermano ejecutará un único mandato (junto con sus argumentos) haciendo uso de execvp(3)


						pid1_fork=fork(); // Creamos Hijo 1
						if(pid1_fork<0){ // ERROR AL HACER EL primer fork()
							printf("Error: al hacer el primer fork()");
						}else if(pid1_fork==0){// ESTAMOS EN EL HIJO 1
							signal(SIGUSR1,manejador_hijo_1);
							vector_PIDs[0]=getppid();//Guardamos el PID único del Padre en la posicón 0 de la varibale GLOBAL vector_PIDs
							printf("vector_PIDs[0]=%i\n",vector_PIDs[0]);
							vector_PIDs[1]=getpid();//Guardamos el PID único del Hijo1 en la posicón 1 de la varibale GLOBAL vector_PIDs
							printf("vector_PIDs[1]=%i\n",vector_PIDs[1]);
							pause();
							printf("Soy el Hijo 1 (pid=%i), corriendo...\n",vector_PIDs[1]);
							sleep(2);
							printf("Termine! Paso el testigo al Hijo 2 (pid=%i)\n",vector_PIDs[2]);
							kill(vector_PIDs[2],SIGUSR1);//Envio señal al Hijo 2 para que salga del pause y empiece a correr
							pause();
							exit(0);
						}else if (pid1_fork>0){// ESTAMOS EN EL PADRE
							pid2_fork=fork(); // Creamos Hijo 2
							if(pid2_fork<0){ // ERROR AL HACER EL segundo fork()
								printf("Error: al hacer el segundo fork()\n");
							}else if(pid2_fork==0){// ESTAMOS EN EL HIJO 2
								signal(SIGUSR1,manejador_hijo_2);
								vector_PIDs[2]=getpid();//Guardamos el PID único del Hijo2 en la posición 2 de la varibale GLOBAL vector_PIDs
								printf("vector_PIDs[2]=%i\n",vector_PIDs[2]);
								pause();
								printf("Soy el Hijo 2 (pid=%i), corriendo...\n",vector_PIDs[2]);
								sleep(2);
								printf("Termine! Paso el testigo al Hijo 3 (pid=%i)\n",vector_PIDs[3]);
								kill(vector_PIDs[3],SIGUSR1);//Envio señal al Hijo 3 para que salga del pause y empiece a correr
								exit(0);
							}else if (pid2_fork>0){ // ESTAMOS EN EL PADRE
								pid3_fork=fork(); // Creamos Hijo 3
								if(pid3_fork<0){ // ERROR AL HACER EL tercer fork()
									printf("Error: al hacer el segundo fork()\n");
								}else if(pid3_fork==0){// ESTAMOS EN EL HIJO 3
									signal(SIGUSR1,manejador_hijo_3);
									vector_PIDs[3]=getpid();//Guardamos el PID único del Hijo3 en la posicón 3 de la varibale GLOBAL vector_PIDs
									printf("vector_PIDs[3]=%i\n",vector_PIDs[3]);
									pause();
									printf("Soy el Hijo 3 (pid=%i), corriendo...\n",vector_PIDs[3]);
									sleep(2);
									printf("Termine! Paso el testigo al Hijo 4 (pid=%i)\n",vector_PIDs[4]);
									kill(vector_PIDs[4],SIGUSR1);//Envio señal al Hijo 4 para que salga del pause y empiece a correr
									exit(0);
								}else if (pid3_fork>0){ // ESTAMOS EN EL PADRE
									pid4_fork=fork(); // Creamos hijo 4
									if(pid4_fork<0){ // ERROR AL HACER EL cuarto fork()
										printf("Error: al hacer el cuarto fork()\n");
									}else if(pid4_fork==0){// ESTAMOS EN EL HIJO 4
										signal(SIGUSR1,manejador_hijo_4);
										vector_PIDs[4]=getpid();//Guardamos el PID único del Hijo4 en la posicón 4 de la varibale GLOBAL vector_PIDs
										printf("vector_PIDs[4]=%i\n",vector_PIDs[4]);
										pause();
										printf("Soy el Hijo 4 (pid=%i), corriendo...\n",vector_PIDs[4]);
										sleep(2);
										printf("Termine!\n");
										kill(vector_PIDs[1],SIGUSR1);//Enviamos señal desde el Hijo 4 (el último en correr) para que saque al Hijo 1 del segundo pause y a continuación muera.
										exit(0); //Termina Proceso 4

									}else if (pid4_fork>0){// ESTAMOS EN EL PADRE
										printf("Todos los hijos creados.");
										printf("Doy la salida!");
										kill(vector_PIDs[1],SIGUSR1);//Enviamos señal al Hijo1 para que empiece a correr
										waitpid(vector_PIDs[1],&status,0);//Un buen Padre espera por sus hijos, en este caso, el padre esperará unicamente por el Hijo 1 porque tal y cómo esta implementado el código antes de que el Hijo 1 muera el resto de Hijos ya estarán muertos.
										if(WIFEXITED(status) != 0){
											if(WEXITSTATUS(status) != 0){
												exit_status= WEXITSTATUS(status);
												printf("El Hijo 1 no terminó de forma correcta\n");
											}
										}
										exit(0);
									}
								}
							}
						}
//---------------------------------
}// FIN FUNCIÓN MAIN
//---------------------------------
// IMPLEMENTACIONES FUNCIONES CUYAS CABECERAS ESTAN DEFINIDAS AL PRINCIPIO DE ESTE DOCUMENTO Y QUE SON UTILIZADAS DENTRO DE MAIN

//IMPLEMENTACION FUNCION 1
		void manejador_padre(int signal){
			//Señal SIGINT: Ctrl +C
			//Señal SIGUSR1: Para uso del Programador
			//Señal SIGUSR2: Para uso del Programador
			//printf("Padre: Recibida señal. %d\n", signal);

		}
	//FIN IMPLEMENTACION FUNCION 1
	//---------------------------------
	//IMPLEMENTACION FUNCION 2
		void manejador_hijo_1(int signal){
			//Señal SIGINT: Ctrl +C
			//Señal SIGUSR1: Para uso del Programador
			//Señal SIGUSR2: Para uso del Programador
			//printf("Hijo_1: Recibida señal. %d\n", signal);
			 if(signal ==SIGUSR1){
				 printf("Estoy en la función manejador_hijo_1\n ");
				 //No hace nada, sólo queremos que el Hijo 1 salga del pause(3) y empiece a correr
			}
		}
	//FIN IMPLEMENTACION FUNCION 2
	//---------------------------------
	//IMPLEMENTACION FUNCION 3
		void manejador_hijo_2(int signal){
			//Señal SIGINT: Ctrl +C
			//Señal SIGUSR1: Para uso del Programador
			//Señal SIGUSR2: Para uso del Programador
			//printf("Hijo_2: Recibida señal. %d\n", signal);
			if(signal ==SIGUSR1){
				 printf("Estoy en la función manejador_hijo_2\n ");
				 //No hace nada, sólo queremos que el Hijo 1 salga del pause(3) y empiece a correr
			}
		}
	//FIN IMPLEMENTACION FUNCION 3
	//---------------------------------
	//IMPLEMENTACION FUNCION 4
		void manejador_hijo_3(int signal){
			//Señal SIGINT: Ctrl +C
			//Señal SIGUSR1: Para uso del Programador
			//Señal SIGUSR2: Para uso del Programador
			//printf("Hijo_3: Recibida señal. %d\n", signal);
			if(signal ==SIGUSR1){
				 printf("Estoy en la función manejador_hijo_3\n ");
				 //No hace nada, sólo queremos que el Hijo 1 salga del pause(3) y empiece a correr
			}
		}
	//FIN IMPLEMENTACION FUNCION 4
	//---------------------------------
	//IMPLEMENTACION FUNCION 5
		void manejador_hijo_4(int signal){
			//Señal SIGINT: Ctrl +C
			//Señal SIGUSR1: Para uso del Programador
			//Señal SIGUSR2: Para uso del Programador
			//printf("Hijo_4: Recibida señal. %d\n", signal);
			if(signal ==SIGUSR1){
				 printf("Estoy en la función manejador_hijo_4\n ");
				 //No hace nada, sólo queremos que el Hijo 1 salga del pause(3) y empiece a correr
			}
		}
	//FIN IMPLEMENTACION FUNCION 5

// FIN IMPLEMENTACIONES FUNCIONES CUYAS CABECERAS ESTAN DEFINIDAS AL PRINCIPIO DE ESTE DOCUMENTO UTILIZADAS DENTRO DE MAIN
