/* Título: invert
 * Autor: Iván Martín Gómez
 *
 * Sintaxis: myinvert Arg1 [Arg2] ... [Argn]
 *
 * Argumentos Entrada: (los que estan entre [] son opcionales)
 * 		Argumento 1:  Arg1  --> string
 * 		Argumento 2: [Arg2] --> string
 * 		Argumento n: [Argn] --> string
 *
 * Salida: Por pantalla, en la misma línea, cada uno de los argumentos
 * 		   con sus caracteres en orden inverso, colocados en orden inverso
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



/*
 *EMPIEZA EL PROGRAMA
 */
int main (int argc, char *argv[], char *envp[]){

//ZONA DE DECLARACION DE VARIABLES

	int numero_argumentos=argc-1;
	int tamano_argumentos[argc-1];
	int tamano=0;

	//FORMA 1 COMO LO HICE
		char *caracter;
		char **argumentos;
		char **argumentos_invert;
		char **argumentos_invert_2;


	//FORMA 2 COMO LO HICE
	/*typedef char argumento[100];
	argumento *argumentos;
	argumento *argumentos_invert;
	argumento *argumentos_invert_2;*/






//FIN ZONA DE DECLARACION DE VARIABLES

	//FORMA 1 COMO LO HICE
	//argumentos =  malloc((argc-1)*100*sizeof(char));
	//argumentos_invert =  malloc((argc-1)*100*sizeof(char));
	//argumentos_invert_2 =  malloc((argc-1)*100*sizeof(char));

	//FORMA 2 COMO LO HICE

	for (int i=0;i<argc;i++){
		tamano_argumentos[i]=sizeof(argv[i+1]);
	}

	argumentos =  malloc(numero_argumentos*sizeof(char));
	if(argumentos == NULL){
		printf("Error: reservando memoria dinámica de número_argumentos para apuntador argumento\n");
		return 1;
	}
	argumentos_invert =  malloc(numero_argumentos*sizeof(char));
	if(argumentos_invert == NULL){
		printf("Error: reservando memoria dinámica de número_argumentos para apuntador argumentos_invert\n");
		return 1;
	}
	argumentos_invert_2 =  malloc(numero_argumentos*sizeof(char));
	if(argumentos_invert_2 == NULL){
		printf("Error: reservando memoria dinámica de número_argumentos para apuntador argumentos_invert_2\n");
		return 1;
	}

	for(int i=0; i<numero_argumentos;i++){
		argumentos[i]= malloc(tamano_argumentos[i]*sizeof(char));
		if(argumentos[i] == NULL){
				printf("Error: reservando memoria dinámica de tamano_argumentos[%i]= %i para apuntador argumento[%i]\n",i,tamano_argumentos[i],i);
				return 2;
		}
		argumentos_invert[i]= malloc(tamano_argumentos[i]*sizeof(char));
		if(argumentos_invert[i] == NULL){
				printf("Error: reservando memoria dinámica de tamano_argumentos[%i]= %i para apuntador argumentos_invert[%i]\n",i,tamano_argumentos[i],i);
				return 2;
		}
		argumentos_invert_2[i]= malloc(tamano_argumentos[i]*sizeof(char));
		if(argumentos_invert_2[i] == NULL){
				printf("Error: reservando memoria dinámica de tamano_argumentos[%i]= %i para apuntador argumentos_invert_2[%i]\n",i,tamano_argumentos[i],i);
				return 2;
		}

	//Ahora hacemos que los apuntadores "apunten"


	}

//Control Errores

	if((argc < 2)){
		printf("Error de uso: seq Arg1\n");
		return 7;
	}else{
		printf("Parseamos el Mandato:\n");
		printf("--Se han recibido %i Argumento/s:\n",argc-1);
		for (int i=0; i<argc;i=i+1){
			if(i==0){
				printf("---- El nombre del mandato es: %s\n",argv[0]);
			}
			else{
				printf("---- Argumento %i:              %s\n",i,argv[i]);
			}
		}
	}
	printf("---------------------------------------------\n");
	printf("\n");

// Fin control Errores y Parseo

//Empieza funcionalidad programa
	printf("Guardo el contenido de los argumentos recibidos en *argv[] en Memoria dinámica *argumentos[]\n");
	printf("  1º--Instanciado **argumentos\n");
	printf("  2º--Reservada memoria dinámica con malloc: argumentos =  malloc((%i)*100*sizeof(char)) \n",argc-1);
	for (int i=0;i<=argc-2;i++){
		argumentos[i]=argv[i+1];
		printf("    ----Contenido de a lo que apunta argv[%i]= %s\n",i+1,argv[i+1]);
		printf("    ----Guardo dirección memoria almacenada en argv[%i] en argumentos[%i]\n",i+1,i);
		printf("    ----Contenido de a lo que apunta argumentos[%i]= %s\n",i,argumentos[i]);
		printf("    ----Muestro direcciones de memoria para verificarlo:\n");
		printf("    -------Dirección memoria donde apunta argv[%i] = %p\n",i+1,argv[i+1]);
		printf("    -------Dirección memoria donde apunta argumentos[%i] = %p\n",i,argumentos[i]);
	}
	printf("---------------------------------------------\n");

	//invertimos orden de los argumentos, es decir, Arg1 Arg2 ... Argn
	//pasa a ser Argn ... Arg2 Arg1 y lo almecenamos en argumentos_invert.
	// Además inicializamos la matriz argumentos_invert_2[][]
	printf("Invierto el orden los argumentos: Arg1 Arg2 ... Argn ==> Argn ... Arg2 Arg1\n");
	printf("  1ºInstanciado **argumentos_invert\n");
	printf("  2ºReservada memoria dinámica con malloc: argumentos_invert =  malloc((%i)*100*sizeof(char)) \n",argc-1);
	for (int i=0;i<argc-1;i++){
		printf("    ----Contenido de a lo que apunta argumentos[%i]= %s\n",argc-2-i,argumentos[argc-2-i]);
		printf("    ----Guardo dirección memoria almacenada en argumentos[%i] en argumentos_invert[%i]\n",argc-2-i,i);
		argumentos_invert[i]=argumentos[argc-2-i];
		printf("    ----Contenido de a lo que apunta argumentos_invert[%i]= %s\n",i,argumentos_invert[i]);
		printf("    ----Muestro direcciones de memoria para verificarlo:\n");
		printf("    -------Dirección memoria donde apunta argumentos[%i] = %p\n",argc-2-i,argumentos[argc-2-i]);
		printf("    -------Dirección memoria donde apunta argumentos_invert[%i] = %p\n",i,argumentos_invert[i]);
		argumentos_invert[i]=argumentos[argc-2-i];
		argumentos_invert_2[i]=argumentos[argc-2-i];
	}
	printf("--------------\n");
	//Ahora vamos cambiando el orden de los caracteres de cada uno de los
	//argumentos almacenados en argumentos_invert



	for (int i=0;i<argc-1;i++){
		printf("Analizamos argument_invert[%i]\n",i);
		printf("  ---> Contenido de a lo que apunta argument_invert [%i] = %s\n",i,argumentos_invert[i]);
		printf("  ---> Tamaño de a lo que apunta argument_invert [%i] = %i\n",i,strlen(argumentos_invert[i]));
		tamano=strlen(argumentos_invert[i])-1;
		printf("\n");
		printf("  Cambiamos el orden de los caracteres\n");
		printf("    1ºInstanciado **argumentos_invert_2\n");
		printf("    2ºReservada memoria dinámica con malloc: argumentos_invert_2 =  malloc((%i)*100*sizeof(char)) \n",argc-1);
		printf("\n");
		for(int j=0;j<tamano+1;j++){
			printf("      ----Contenido de a lo que apunta argument_invert[%i][%i]= %c\n",i,tamano-j,argumentos_invert[i][tamano-j]);
			printf("      ----Guardo dirección memoria almacenada en argument_invert[%i][%i] en argumentos_invert_2[%i][%i]\n",i,tamano-j,i,j);
			argumentos_invert_2[i][j]=argumentos_invert[i][tamano-j];

			printf("      ----Contenido de a lo que apunta argumentos_invert_2[%i][%i]= %c\n",i,j,argumentos_invert[i][j]);
			printf("      ----Muestro direcciones de memoria para verificarlo:\n");
			printf("      -------Dirección memoria donde apunta argument_invert[%i][%i] = %p\n",i,tamano-j,argumentos_invert[i][tamano-j]);
			printf("      -------Dirección memoria donde apunta argumentos_invert_2[%i][%i] = %p\n",i,j,argumentos_invert_2[i][j]);
			//argumentos_invert[i][tamano-j]='\0';
			printf("\n");
		}
		printf("\n");
		printf("  ---> Contenido de a lo que apunta argument_invert [%i] = %s\n",i,argumentos_invert[i]);

		argumentos_invert_2[i][tamano+1]='\0';

		printf("\n");

	}
	//Imprimimos por pantalla el resultado esperado
	for (int i=0;i<argc-1;i++){
		printf("%s ",argumentos_invert_2[i]);
	}





free(argumentos);
free(argumentos_invert);
free(argumentos_invert_2);
//FIN PROGRAMA
}
