/*
 * TÍTULO: libreria.h
 * AUTOR: Iván Martín Gómez
 * DESCRIPCIÓN:	implementación de las funciones,
 * 				cuyas cabeceras hemos definido en libreria.h
 */

/*
 * SINTAXIS FUNCIÓNES: 	head [N], tail [N], longlines [N]
 * ARGUMENTOS ENTRADA: (señalizados entre []'s los argumentos opcionales)
 * 		- ARGUMENTO 1: []       --> N: se muestran las N primeras líneas
 * 									   (si se omite este Argumento por defecto N=10)

 *
 * 	SALIDA:				Se muestra por pantalla, línea por línea, las N primeras líneas
 * 						de Fichero stdin ==> stdin <==
 *
 * 	ERRORES:
 * 		- ERROR 1: error reservando Memoria Dinámica con malloc()
 * 		- ERROR 2:
 */

#include "libreria.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int head(int N){
	// ZONA DE DECLARACION DE VARIABLES
	 int i, z, lineas;
	 int cuenta;
	 char buf[1024];
	 char **lineasTexto;
	 // FIN ZONA DE DECLARACION DE VARIABLES
	 //-------------------------------------
	 // ZONA DE INICIALIZACIÓN DE VARIABLES
	 lineas=N;
	 i=0;
	 cuenta=0;
	 // FIN ZONA DE DECLARACION DE VARIABLES
	 //-------------------------------------
	 // ZONA DE RESERVA MEMORIA DINÁMICA
	 lineasTexto = (char**)malloc(1024*lineas);
	 if (lineasTexto == NULL) {
	   fprintf(stderr, "Error reservando memoria dinámica con malloc()\n");
	   return -3;
	 }
	 while(i<lineas){
	   lineasTexto[i] = (char*)malloc(1024);
	   if (lineasTexto == NULL) {
	   	   fprintf(stderr, "Error reservando memoria dinámica con malloc()\n");
	   	   return -3;
	   	}
	   i++;
	 }
	 // FIN ZONA DE RESERVA MEMORIA DINÁMICA
	 //-------------------------------------
	 // EMPIEZA FUNCIONALIDAD FUNCIÓN
	 printf("EMPIEZA FUNCIÓN HEAD: introduzca por teclado tantas líneas como desee.\n");
	 printf(" 		      Cuando haya terminado de escribir una línea pulse Enter.\n");
	 printf(" 		      Cuando haya terminado de escribir todas las líneas pulse Ctrl + D.\n");
	 printf("");
	 printf(">");
	 while(fgets(buf, 1024, stdin) != NULL){
		if(cuenta<lineas){
			strcpy(lineasTexto[cuenta],buf);
			cuenta++;
		}
	 }

	 printf("------------------------------\n");
	 if(cuenta<lineas){
		 printf("N=%i, pero solo se han recibido %i lineas por stdin\n",lineas,cuenta);
		 printf("Las %i primeras líneas son: \n",cuenta);
	 }else{
		 printf("Las %i primeras líneas son: \n",lineas);
	 }
	 printf("\n");

	 for (z=0;z<cuenta;z++){
	    printf("%s",lineasTexto[z]);
	 }

	 printf("\n");

	 //Liberamos Memoria Dinámica
	 for(z=0;z<lineas;z++){
		free(lineasTexto[z]);
	 }
	 rewind(stdin); // Colocamos el indicador de posición de fichero al principio del Fichero stdin
	 free(lineasTexto); //Liberamos Memoria Dinámica
	 return 0;
	 // FIN FUNCIÓN
}
int tail(int N){
		// ZONA DE DECLARACION DE VARIABLES
		 int i, z, lineas;
		 int cuenta;
		 char buf[1024];
		 char **lineasTexto;
		 // FIN ZONA DE DECLARACION DE VARIABLES
		 //-------------------------------------
		 // ZONA DE INICIALIZACIÓN DE VARIABLES
		 lineas=N;
		 i=0;
		 cuenta=0;
		 // FIN ZONA DE DECLARACION DE VARIABLES
		 //-------------------------------------
		 // ZONA DE RESERVA MEMORIA DINÁMICA
		 lineasTexto = (char**)malloc(1024*lineas);
		 if (lineasTexto == NULL) {
		   fprintf(stderr, "Error reservando memoria dinámica con malloc()\n");
		   return -3;
		 }
		 while(i<lineas){
		   lineasTexto[i] = (char*)malloc(1024);
		   if (lineasTexto == NULL) {
		   	   fprintf(stderr, "Error reservando memoria dinámica con malloc()\n");
		   	   return -3;
		   	}
		   i++;
		 }
		 // FIN ZONA DE RESERVA MEMORIA DINÁMICA
		 //-------------------------------------
		 // EMPIEZA FUNCIONALIDAD FUNCIÓN
		 printf("EMPIEZA FUNCIÓN TAIL: introduzca por teclado tantas líneas como desee.\n");
		 printf(" 		      Cuando haya terminado de escribir una línea pulse Enter.\n");
		 printf(" 		      Cuando haya terminado de escribir todas las líneas pulse Ctrl + D.\n");
		 printf("");
		 printf(">");
		 while(fgets(buf, 1024, stdin) != NULL){
			if(cuenta<lineas){
				strcpy(lineasTexto[cuenta],buf);
				cuenta++;
			}else{
				for(i=0;i<cuenta-1;i++){
					strcpy(lineasTexto[i],lineasTexto[i+1]);
				}
					strcpy(lineasTexto[cuenta-1],buf);
			}

		 }

		 printf("------------------------------\n");
		 if(cuenta<lineas){
		 	printf("N=%i, pero solo se han recibido %i lineas por stdin\n",lineas,cuenta);
		 	printf("Las %i últimas líneas son: \n",cuenta);
		 }else{
		 	printf("Las %i últimas líneas son: \n",lineas);
		 }
		 printf("\n");
		for (z=0;z<cuenta;z++){
			printf("%s",lineasTexto[z]);
		}

		 printf("\n");

		 //Liberamos Memoria Dinámica
		 for(z=0;z<lineas;z++){
			free(lineasTexto[z]);
		 }
		 rewind(stdin); // Colocamos el indicador de posición de fichero al principio del Fichero stdin
		 free(lineasTexto); //Liberamos Memoria Dinámica
		 return 0;
		 // FIN FUNCIÓN


}
int longlines(int N){
			// ZONA DE DECLARACION DE VARIABLES
			 int i, z, lineas, posicion,encontrado;
			 int cuenta;
			 char buf[1024];
			 char **lineasTexto;
			 // FIN ZONA DE DECLARACION DE VARIABLES
			 //-------------------------------------
			 // ZONA DE INICIALIZACIÓN DE VARIABLES
			 lineas=N;
			 i=0;
			 cuenta=0;
			 posicion=0;
			 encontrado=0;
			 // FIN ZONA DE DECLARACION DE VARIABLES
			 //-------------------------------------
			 // ZONA DE RESERVA MEMORIA DINÁMICA
			 lineasTexto = (char**)malloc(1024*lineas);
			 if (lineasTexto == NULL) {
			   fprintf(stderr, "Error reservando memoria dinámica con malloc()\n");
			   return -3;
			 }
			 while(i<lineas){
			   lineasTexto[i] = (char*)malloc(1024);
			   if (lineasTexto == NULL) {
			   	   fprintf(stderr, "Error reservando memoria dinámica con malloc()\n");
			   	   return -3;
			   	}
			   i++;
			 }
			 // FIN ZONA DE RESERVA MEMORIA DINÁMICA
			 //-------------------------------------
			 // EMPIEZA FUNCIONALIDAD FUNCIÓN
			 printf("EMPIEZA FUNCIÓN LONGLINES: introduzca por teclado tantas líneas como desee.\n");
			 printf(" 		           Cuando haya terminado de escribir una línea pulse Enter.\n");
			 printf(" 		           Cuando haya terminado de escribir todas las líneas pulse Ctrl + D.\n");
			 printf("");
			 printf(">");
			 while(fgets(buf, 1024, stdin) != NULL){
				if(cuenta<lineas){
					if(cuenta==0){
						strcpy(lineasTexto[cuenta],buf);
						cuenta++;
					}else if(cuenta==1){
						if(strlen(buf)>strlen(lineasTexto[0])){
							strcpy(lineasTexto[cuenta],lineasTexto[0]);
							strcpy(lineasTexto[0],buf);
						}else{
							strcpy(lineasTexto[cuenta],buf);
						}
						cuenta++;
					}else if(cuenta==2){
						if(strlen(buf)>=strlen(lineasTexto[0])){
							strcpy(lineasTexto[cuenta],lineasTexto[1]);
							strcpy(lineasTexto[1],lineasTexto[0]);
							strcpy(lineasTexto[0],buf);
						}else if(strlen(buf)<=strlen(lineasTexto[1])){
							strcpy(lineasTexto[cuenta],buf);
						}else{
							strcpy(lineasTexto[cuenta],lineasTexto[1]);
							strcpy(lineasTexto[1],buf);
						}
						cuenta++;
					}else if(cuenta>=3){
						if(strlen(buf)>=strlen(lineasTexto[0])){
							for (i=cuenta;i>0;i--){
								strcpy(lineasTexto[i],lineasTexto[i-1]);
							}
							strcpy(lineasTexto[0],buf);
						}else if(strlen(buf)<=strlen(lineasTexto[cuenta-1])){
							strcpy(lineasTexto[cuenta],buf);
						}else{
							posicion=0;
							encontrado=0;
							for (i=1;i<cuenta;i++){
								if(encontrado==0){
									if(strlen(buf)>=strlen(lineasTexto[i])){
										posicion=i;
										encontrado=1;
									}
								}
							}
							for (i=cuenta;i>posicion;i--){
								strcpy(lineasTexto[i],lineasTexto[i-1]);
							}
							strcpy(lineasTexto[posicion],buf);
						}
						cuenta++;
					}

				}else{
					if(strlen(buf)>=strlen(lineasTexto[0])){
						if(cuenta==1){
							strcpy(lineasTexto[0],buf);
						}else{
							for (i=cuenta-1;i>0;i--){
								strcpy(lineasTexto[i],lineasTexto[i-1]);
							}
							strcpy(lineasTexto[0],buf);
						}
					}else if(strlen(buf)<strlen(lineasTexto[cuenta-1])){


					}else{
						if(cuenta!=1){
							posicion=0;
							encontrado=0;
							for (i=1;i<cuenta;i++){
								if(encontrado==0){
									if(strlen(buf)>=strlen(lineasTexto[i])){
										posicion=i;
										encontrado=1;
									}
								}
							}
							for (i=cuenta-1;i>posicion;i--){
								strcpy(lineasTexto[i],lineasTexto[i-1]);
							}
								strcpy(lineasTexto[posicion],buf);
						}

					}

				}

			 }

			 printf("------------------------------\n");
			 if(cuenta<lineas){
			 	printf("N=%i, pero solo se han recibido %i lineas por stdin\n",lineas,cuenta);
			 	printf("Las %i líneas más largas son: \n",cuenta);
			 }else{
			 	printf("Las %i líneas más largas son: \n",lineas);
			 }
			 printf("\n");
			for (z=0;z<cuenta;z++){
				printf("%s",lineasTexto[z]);
			}

			 printf("\n");

			 //Liberamos Memoria Dinámica
			 for(z=0;z<lineas;z++){
				free(lineasTexto[z]);
			 }
			 rewind(stdin); // Colocamos el indicador de posición de fichero al principio del Fichero stdin
			 free(lineasTexto); //Liberamos Memoria Dinámica
			 return 0;
			 // FIN FUNCIÓN

}


