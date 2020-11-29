/* TÍTULO: e5.c
 * AUTOR:  Iván Martín Gómez
 * FECHA:  Wed Nov 25 13:13:00 CET 2020
 *
 * Sintaxis: e5 Argumento1 [Argumento2] ... [Argumento_n-1 [Argumento_n]]
 *
 * Argumentos Entrada:
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
//LIBRERÍAS ESTANDAR DE C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // para Llamada al Sistema fork()
#include <signal.h> // para Llamada al Sistema signal()
#include <sys/types.h>
#include <sys/errno.h>// para variable errno


// CABECERAS FUNCIONES UTILIZADAS DENTRO DE MAIN
	// CABECERA FUNCION 1: Manejo señales Procesos
		void manejador_padre(int signal);
	// CABECERA FUNCION 2: Manejo señales Procesos
		void manejador_hijo_1(int signal);
	// CABECERA FUNCION 3: Manejo señales Procesos
		void manejador_hijo_2(int signal);

// FIN CABECERAS FUNCIONES UTILIZADAS DENTRO DE MAIN
//---------------------------------

// ZONA DECLARACION VARIABLE GLOBALES (fuera de la función main())
int fd_tuberia_del_padre_al_hijo1[2]; // GLOBAL PORQUE SE GESTIONA EN función fuera del main() (manejadora de señales)
int fd_tuberia_del_padre_al_hijo2[2]; // GLOBAL PORQUE SE GESTIONA EN función fuera del main() (manejadora de señales)
int fd_tuberia_del_hijo2_al_hijo1[2]; // GLOBAL PORQUE SE GESTIONA EN función fuera del main() (manejadora de señales)
// FIN ZONA DECLARACION VARIABLE GLOBALES
//---------------------------------
// EMPIEZA FUNCION MAIN
int main (int argc, char *argv[]){

printf("------------------\n");
printf("EMPIEZA EL PROGRAMA\n");

// ZONA DE DECLARACION DE VARIABLES LOCALES FUNCIÓN MAIN()
		int status1;
		int status2;
		char recepcion_datos_hijo2_desde_padre[512];
		char recepcion_datos_hijo1_desde_padre[512];
		char mandato1[100]="ls -la";
		char mandato2[100]="tr \"d\" \"D\"";
		char linea[200]="ls -la |tr \"d\" \"D\"";
		pid_t pid1_fork;// Lo que devuelve la Llamada al Sistema fork() no es un int (aunque lo parezca), si no una variable de tipo pid_t. Lo que tiene de especial la variable de tipo pid_t, es que la variable pid_t cambia su valor dependiendo de cuál sea el Proceso desde el que la estamos consultando
		pid_t pid2_fork;
		pid_t PID_unico_padre;
		pid_t PID_unico_hijo1;
		pid_t PID_unico_hijo2;


// FIN ZONA DE DECLARACION DE VARIABLES LOCALES FUNCIÓN MAIN()
//------------------------------------
// ZONA DE CONTROL ERRORES ARGUMENTOS
		if(argc>1){ //Hemos recibido 1 Argumento ó más:
			printf("usage: %s\n",argv[0]);
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

		//IDEAS CLAVE DEL EJERCICIO:
		//	Idea 1: execvp(3) solo acepta cómo argumentos un único mandato (junto con sus argumentos)
		//	Idea 2: Usamos dup(2) ó duo2(2) para redirigir la Salida Estándar del Hijo1 que hace execvp(3) del primer mandato de la línea(visto de iquierda a derecha) a la Entrada Estandar del Hijo2 que hace execvp(3) del segundo mandato de la línea
		//	Idea 3: Misma idea que en E4. Almaceno en una variable Global vector los PIDs únicos de los Hijos para que entre ellos puedan enviarse señales, y así por ejemplo un hijo pueda sacar a otro del estado de pause.
		//	Idea 4: Varias Tuberias para comunicar procesos:
		//			Tuberia 1: Padre-->Hijo1, envio de mandato1
		//			Tuberia 2: Padre-->Hijo2, envio de mandato2
		//			Tuberia 3: Hijo1-->Hijo2, envio de Salida Estandar de execvp(3) del Hijo 1 a la Entrada Estandar de execvp(3) del Hijo2
		//------------>Estructura básica para saber en que proceso estamos
				printf("Queremos Ejecutar la línea: %s\n",linea);
				printf("Tokenizamos línea... \n");
				printf("Mandato 1 = %s \n",mandato1);
				printf("Mandato 2 = %s \n",mandato2);

				pipe(fd_tuberia_del_padre_al_hijo1); // Padre-->Hijo1.Tubería para comunicación entre el Padre y el Hijo 1. Para el envio del mandato 2 desde el Padre al Hijo 1
				pipe(fd_tuberia_del_padre_al_hijo2); // Padre-->Hijo2.Tubería para comunicación entre el Padre y el Hijo 2. Para el envio del mandato 1 desde el Padre al Hijo 2
				pipe(fd_tuberia_del_hijo2_al_hijo1); // Hijo2-->Hijo1.Tubería para comunicación entre el Hijo 2 y el Hijo 1. Para el envio de Salida Estandar de execvp(3) del Hijo 2 a la Entrada Estandar de execvp(3) del Hijo1
				pid1_fork=fork();

				if(pid1_fork<0){ // ERROR AL HACER EL primer fork()
					printf("Error: al hacer el primer fork()");
				}else if(pid1_fork==0){// ESTAMOS EN EL HIJO 1
					signal(SIGUSR1,manejador_hijo_1);
					PID_unico_hijo1=getpid();
					close(fd_tuberia_del_padre_al_hijo2[0]);//Padre---->>Hijo2.El Hijo 1 no escribe en esta tubería
					close(fd_tuberia_del_padre_al_hijo2[1]);//Padre---->>Hijo2.El Hijo 1 no lee en esta tubería
					close(fd_tuberia_del_padre_al_hijo1[1]);//Padre---->>Hijo1. El Hijo 1 solo lee, no escribe, en esta tubería. Cierro fd_tph2[1] desde el Hijo 1.
					close(fd_tuberia_del_hijo2_al_hijo1[1]);//Hijo2---->>Hijo1. El Hijo 1 solo lee, no escribe, en esta tubería. Cierro fd_th2h1[1] desde el Hijo 1.
					printf("El Hijo 1 se queda en pause a la espera de recibir seál desde el Hijo 2\n");
					pause();//Ponemos al Hijo 1 en estado de pause, en espera de recibir una señal por parte del Padre
					printf("El Hijo 1 sale del pause porque ha recibido una señal del Padre\n");
					read(fd_tuberia_del_padre_al_hijo1[0],recepcion_datos_hijo1_desde_padre,512);
					printf("El Hijo 1 lee de la Tuberia Padre-->Hijo1. Mensaje recibido: %s\n",recepcion_datos_hijo1_desde_padre);
					printf("El Hijo 1 redirige la Entrada Estandar desde el Teclado(2) al extremo de lectura de la Tuberia Hijo2-->Hijo1 haciendo uso de de close(2) y dup(2)\n");
					close(0);//Cierro del Descriptor de Fichero asociado al Teclado (0) por lo que después de hacer el close(), el Descriptor de Fichero 0 estará Libre. Recordar que dup() asigna por defecto el número de Descriptor de Fichero libre más pequeño disponible
					dup(fd_tuberia_del_hijo2_al_hijo1[0]);//Haciendo dup() estoy: 1º) Redirigiendo la Entrada Estandar del Teclado (Descriptor de Fichero =0) al extremo de lectura de la Tuberia Hijo2-->Hijo1. 2º) Lee en la Tubería (me ahorro el read)
					printf("El Hijo 1 Ejecuta el mandato (de tipo Filtro) con execlp() recibido desde el Padre y habiendo redirigido previamente la Entrada Estandar.\n");
					execlp("tr","tr","\"d\"","\"D\"",NULL);
					//Si llegamos hasta aquí es que algo ha ido mal en el execlp().
					printf("Error execlp() del Hijo 1: %s",strerror(errno));
					exit(1);//El Hijo 1 muere. Si llega hasta aquí es porque algo no ha ido bien en el excelp()
				}else if (pid1_fork>0){// ESTAMOS EN EL PADRE
					PID_unico_hijo1=pid1_fork;
					pid2_fork=fork();
					if(pid2_fork<0){// ERROR AL HACER EL segundo fork()
						printf("Error: al hacer el segundo fork()");
					}else if(pid2_fork==0){// ESTAMOS EN EL HIJO 2
						//Desde el Hijo 2 si concozco el PID úncio del Hijo 1, pero al revés no.
						signal(SIGUSR1,manejador_hijo_2);
						PID_unico_hijo2=getpid();
						close(fd_tuberia_del_padre_al_hijo1[0]);//Padre---->>Hijo1.El Hijo 2 no escribe en esta tubería
						close(fd_tuberia_del_padre_al_hijo1[1]);//Padre---->>Hijo1.El Hijo 2 no lee en esta tubería
						close(fd_tuberia_del_padre_al_hijo2[1]);//Padre---->>Hijo2. El Hijo 2 solo lee, no escribe, en esta tubería. Cierro fd_tph2[1] desde el Hijo 2.
						close(fd_tuberia_del_hijo2_al_hijo1[0]);//Hijo2---->>Hijo1. El Hijo 2 solo escribe, no lee, en esta tubería. Cierro fd_th2h1[0] desde el Hijo 2.
						printf("El Hijo 2 se queda en pause a la espera de recibir señal desde el Padre\n");
						pause(); //Ponemos al Hijo 2 en estado de pause, en espera de recibir una señal por parte del Padre
						printf("El Hijo 2 sale del pause porque ha recibido una señal del Padre\n");
						printf("El Hijo 2 lee de la Tubería Padre-->Hijo2\n");
						read(fd_tuberia_del_padre_al_hijo2[0],recepcion_datos_hijo2_desde_padre,512);
						printf("El Hijo 2 ha recibido a través de la Tubería Padre-->Hijo2: %s\n",recepcion_datos_hijo2_desde_padre);
						printf("El Hijo 2 redirige la Salida Estandar al extremo de escritura de la Tuberia Hijo2-->Hijo1 haciendo uso de de close() y dup()\n");
						close(1);//Cierro del Descriptor de Fichero asociado a la Pantalla (1) por lo que después de hacer el close(), el Descriptor de Fichero 1 estará Libre. Recordar que dup() asigna por defecto el número de Descriptor de Fichero libre más pequeño disponible
						dup(fd_tuberia_del_hijo2_al_hijo1[1]);//Haciendo dup() estoy: 1º) Redirigiendo la Salida Estandar de la pantalla (Descriptor de Fichero =1) al extremo de escritura de la Tuberia Hijo2-->Hijo1. 2º) Escribiendo en la Tubería (me ahorro el write)
						//printf("El Hijo 2 Ejecuta el mandato recibido con execvp(3)\n");
						execlp("ls","ls","-la",NULL);
						//Si llegamos hasta aquí es que algo ha ido mal en el execlp().
						printf("Error execlp() del Hijo 1: %s",strerror(errno));
						exit(1);//El Hijo 2 muere. Si llegamos hasta aquí es que algo ha ido mal en el execlp().
					}else if(pid2_fork>0){// ESTAMOS EN EL PADRE
						PID_unico_hijo2=pid2_fork;
						PID_unico_padre=getpid();
						close(fd_tuberia_del_padre_al_hijo1[0]);//Padre---->>Hijo1. El Padre solo escribe, no lee en esta tubería. Cierro fd_tph1[0] desde el Padre.
						close(fd_tuberia_del_padre_al_hijo2[0]);//Padre---->>Hijo2. El Padre solo escribe, no lee en esta tubería. Cierro fd_tph2[0] desde el Padre.
						close(fd_tuberia_del_hijo2_al_hijo1[0]);// El Padre no escribe en esta tuberia
						close(fd_tuberia_del_hijo2_al_hijo1[1]);// El Padre no lee en esta Tuberia
						sleep(2);
						printf("-----------------\n");
						printf("El Padre se despierta\n");
						printf("El Padre escribe en la Tuberia Padre-->Hijo1, el mandato que quiere que el Hijo 1 ejecute con execvp(3):(segundo mandato de la línea) %s\n", mandato2);
						write(fd_tuberia_del_padre_al_hijo1[1],mandato2,100);
						printf("El Padre escribe en la Tuberia Padre-->Hijo2, el mandato que quiere que el Hijo 2 ejecute con execvp(3):(primer mandato de la línea) %s\n", mandato1);
						write(fd_tuberia_del_padre_al_hijo2[1],mandato1,100);
						printf("El Padre envia una señal al Hijo 2 para que salga del pause\n");
						kill(PID_unico_hijo2,SIGUSR1); //Enviamos señal al Hijo 1 para que salga del pause(3)
						printf("El Padre hace el primer waitpid(1) en espera de que el Hijo 2 muera\n");
						waitpid(PID_unico_hijo2,&status1,0);//Esperamos a que muera el Hijo 2 para salir del wait(1)
						printf("El Padre sale del primer wait después de que el Hijo 2 haya muerto\n");
						printf("El Padre comprueba como ha ido la ejecución del execlp() en el Hijo 2 haciendo uso de las Macros WIFEXITED y WEXITSTATUS\n");
						if(WIFEXITED(status1)!= 0){ //Si es distinta de cero el Hijo 2 ha hecho exit() o bien desde el execlp() o bien desde su propio código
							if(WEXITSTATUS(status1)!=0){ //Si es distinto de cero algo ha ido mal en el execlp() del Hijo 2. Esta macro sólo es consultable en el caso de que WIFEXITED sea distinta de cero (es decir, el Hijo 2 haya terminado)
								printf("El Hijo 2 ya esta muerto. Algo ha ido mal en el execlp() del Hijo 2.\n");

							}else{
								printf("El Hijo 2 ya ha muerto. En el padre se ha comprobado que la ejecución del execlp() del Hijo 2 ha ido bien.\n");
								printf("El Padre envia una señal al Hijo 1 para que salga del pause\n");
								kill(PID_unico_hijo1,SIGUSR1); //El Hijo 1 envia una señal al Hijo 2 para que salga del pause.
							}
						}

						printf("El Padre hace el segundo waitpid(1) en espera de que el Hijo 1 muera\n");
						waitpid(PID_unico_hijo1,NULL,0);//Esperamos a que muera el Hijo 2 para salir del wait(1)
						printf("El Padre sale del segundo wait después de que el Hijo 1 haya muerto\n");
						printf("El Padre comprueba como ha ido la ejecución del execlp() en el Hijo 2 haciendo uso de las Macros WIFEXITED y WEXITSTATUS\n");
						if(WIFEXITED(status2)!= 0){ //Si es distinta de cero el Hijo 2 ha hecho exit() o bien desde el execlp() o bien desde su propio código
							if(WEXITSTATUS(status2)!=0){ //Si es distinto de cero algo ha ido mal en el execlp() del Hijo 2. Esta macro sólo es consultable en el caso de que WIFEXITED sea distinta de cero (es decir, el Hijo 2 haya terminado)
								printf("El Hijo 1 ya esta muerto. Algo ha ido mal en el execlp() del Hijo 1.\n");

							}else{
								printf("El Hijo 1 ya ha muerto. En el padre se ha comprobado que la ejecución del execlp() del Hijo 1 ha ido bien.\n");
							}
						}
						printf("El Padre muere\n");
						exit(0); //El proceso Padre muere
					}
				}

//---------------------------------
}// FIN FUNCIÓN MAIN
//---------------------------------
// IMPLEMENTACIONES FUNCIONES CUYAS CABECERAS ESTAN DEFINIDAS AL PRINCIPIO DE ESTE DOCUMENTO Y QUE SON UTILIZADAS DENTRO DE MAIN
	//IMPLEMENTACION FUNCION 1
		void manejador_padre(int signal){
			if(signal == SIGINT){
				//printf("Señal=%i recibida en el Padre\n",signal);
			}else if(signal ==SIGUSR1){
				//printf("Señal=%i recibida en el Padre\n",signal);
			}else if(signal ==SIGUSR2){
				//printf("Señal=%i recibida en el Padre\n",signal);
			}
		}

	//IMPLEMENTACION FUNCION 2
		void manejador_hijo_1(int signal){
			if(signal == SIGINT){
				//printf("Señal=%i recibida en el Hijo 1\n",signal);
			}else if(signal ==SIGUSR1){
				//printf("Señal=%i recibida en el Hijo 1\n",signal);
			}else if(signal ==SIGUSR2){
				//printf("Señal=%i recibida en el Hijo 1\n",signal);
			}
		}

	//IMPLEMENTACION FUNCION 3
		void manejador_hijo_2(int signal){
			if(signal == SIGINT){
				//printf("Señal=%i recibida en el Hijo 2\n",signal);
			}else if(signal ==SIGUSR1){
				//printf("Señal=%i recibida en el Hijo 2\n",signal);
			}else if(signal ==SIGUSR2){
				//printf("Señal=%i recibida en el Hijo 2\n",signal);
			}
		}

// FIN IMPLEMENTACIONES FUNCIONES CUYAS CABECERAS ESTAN DEFINIDAS AL PRINCIPIO DE ESTE DOCUMENTO UTILIZADAS DENTRO DE MAIN
