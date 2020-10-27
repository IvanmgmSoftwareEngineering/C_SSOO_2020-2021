/* Título: mycd
 * Autor: Iván Martín Gómez
 *
 * Sintaxis: mycd [path Aboslute or Relative]
 *
 * Argumentos Entrada: (los que estan entre [] son opcionales)
 * 		Argumento 1: [path]  --> ruta Abosoluta o Relativa
 *
 *
 * Salida: Escribe por pantalla la ruta del nuevo Directorio actual de Trabajo
 *
 * Errores:
 * 		Error 1: No se recibe ningún Arguemento de entrada
 *
 * 	Descripción: Este programa cambia el orden de los argumentos recibidos y los muestra
 * 				 por pantalla, es decir si recibe Arg1 Arg2 ... Argn, mostrará
 * 				 Argn Argn-1 ... Arg2 Arg1. Además invierte el orden de los caracteres
 * 				 de cada uno de los argumentos recibidos, es decir, si el argumento
 * 				 i-ésimo es: Argi="abcdef", en la posición que le corresponda en la
 * 				 salida a Argi, se mostrara "fedcba".		 					 														 		 														 incremento es '1'
 */
//----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

/*
 *EMPIEZA EL PROGRAMA
 */
int main (int argc, char *argv[], char *envp[]){

//ZONA DE DECLARACION DE VARIABLES
 DIR *dir;
 char *pwd;
 int error;
 char *directorio;


//FIN ZONA DE DECLARACION DE VARIABLES

//PARSEO MANDATO


 printf("Parseamos el Mandato recibido:\n");
 	printf("-- Se han recibido %i Argumento/s:\n",argc-1);
 	for (int i=0; i<argc;i=i+1){
 		if(i==0){
 			printf("---- El nombre del mandato es: %s\n",argv[0]);
 		}
 		else{
 			printf("---- Argumento %i:              %s\n",i,argv[i]);
 		}
 	}
 	printf("---------------------------------------------\n");
 	printf("\n");

//FIN PARSEO MANDATO

//ZONA CONTROL ERRORES

 	if (argc == 1){
 		directorio=getenv("HOME");
 	}else if(argc ==2){
 		//Suponemos que la ruta es absoluta
 		directorio=argv[1];
 		dir = opendir(directorio);
 		if(dir == NULL){
 			printf("El Argumento recibido %s no es un Directorio o no tiene permiso de lectura",directorio);
 			return 1;
 		}
 		pwd=getenv("PWD");
 		printf("Valor variable PWD antes de llamar a chdir(): %s\n",pwd);
 		error = chdir(directorio);
 		pwd=getenv("PWD");
 		printf("Valor variable PWD después de llamar a chdir(): %s\n",pwd);

 		if(error == -1){
 			printf("Error: no se puede cambiar al directorio %s. Posible ausencia de permiso",directorio);
 			return 2;
 		}
 		printf("%s", directorio);







 	}else{
 		printf("Error: se han recibido 2 Argumentos o más");
 		return 3;


 	}

//FIN ZONA CONTROL ERRORES


//Empieza funcionalidad Programa


//FIN PROGRAMA
}


