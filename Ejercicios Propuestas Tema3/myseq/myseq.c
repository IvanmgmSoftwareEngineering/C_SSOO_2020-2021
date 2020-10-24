/* Título: myseq
 * Autor: Iván Martín Gómez
 *
 * Sintaxis: mysec [first [incre]] last
 *
 * Argumentos Entrada: (los que estan entre [] son opcionales)
 * 		Argumento 1: [first] --> Primer elemento de la secuencia
 * 		Argumento 2: [incre] --> Incremento entre elementos sucesivos de la secuencia
 * 		Argumento 3: last    --> Último elemento de la secuencia
 *
 * Salida: Por pantalla, un elemento de la secuencia en cada línea,
 * 		   ordenados de menor (arriba) a mayor (abajo) en el caso last>0
 * 		   y ordenados de mayor (arriba) a menor (abajo) en caso last<0.
 *
 * Errores:
 * 		Error 1: No se recibe ningún Arguemento de entrada
 * 		Error 2: Se reciben 4 o más argumentos
 * 		Error 3: Alguno de los argumentos no es de tipo float, double o int
 * 		Error 4: Se reciben 3 argumentos, si incre < 0 y last>first , entonces  error
 * 		Error 5: Se reciben 3 argumentos, si incre > 0 y last<first , entonces  error
 *   	Error 6: Se reciben 3 argumentos, si incre = 0, entonces error
 *
 * 	Descripción: Este programa intenta simular el comportamiento
 * 				 del mandato 'seq'. El comportamiento del mandato 'seq'
 * 				 es: muestra por pantalla (un número en cada línea) una
 * 				 secuencia de números.
 *
 * 				 --> 1 Argumento: (last = Último elemento de la secuencia) OBLIGATORIO!!
 *
 * 				 		Por defecto:
 * 				 			- Primer elemento de la secuencia = '1'
 * 				 			- Incremento/decremento =
 * 				 				caso 1) '1' si último numero secuencia >= 0
 * 								caso 2) '-1' si último numero secuencia < 0
 * 						(Ejemplos funcionamiento casos especiales)
 * 				 		- Recibe como argumento last= '1'  => muestra el 1
 * 				 		- Recibe como argumento last=  '0'  => muestra el 1 y el 0
 * 				 		- Recibe como argumento last=  '-1' => muestra el 1, 0 y -1
 *
 *
 * 				--> 2 Argumentos: (first = Primer elemento de la secuencia y last)
 * 						Por defecto:
 * 							- Incremento/decremento =
 * 				 				caso 1) '1' si último numero secuencia >= 0
 * 								caso 2) '-1' si último numero secuencia < 0
 *
 * 						(Ejemplos funcionamiento casos especiales)
 *
 * 						- Recibe dos argumentos iguales first=10 y last=10  => muestra el 10
 *
 * 				--> 3 Argumentos: (first = Primer elemento de la secuencia, incre = incremento y last)
 *
 * 						(Ejemplos funcionamiento casos especiales)
 *
 * 						- Recibe argumentos iguales first=10 y last=10  => muestra el 10 (da igual lo valga incre) 						 														 		 														 incremento es '1'
 */
//----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <xlocale.h>


/*
 *EMPIEZA EL PROGRAMA
 */
int main (int argc, char *argv[], char *envp[]){

//ZONA DE DECLARACION DE VARIABLES
	float last_float;
	float first_float;
	float incre_float;
	char *pend;
	char *last_string[100];
	char *first_string[100];
	char *incre_string[100];
	char *cero[100];
	int ret;


//FIN ZONA DE DECLARACION DE VARIABLES




//Control Errores Número de Argumentos y Parser del mandato y argumentos

	if((argc < 2)||(argc > 4)){
		printf("Error de uso: seq [first [incre]] last\n");
		return 1;
	}else{
		printf("Parseamos el Mandato:\n");
		printf("--Se han recibido %i Argumento/s:\n",argc-1);
		for (int i=0; i<argc;i=i+1){
			if(i==0){
				printf("---- El nombre del mandato es: %s\n",argv[0]);
			}
			else{
				printf("---- Argumento %i:             %s\n",i,argv[i]);
			}
		}
	}
	printf("---------------------------------------------\n");
	printf("\n");
//Control Errores Forma Argumentos: deben ser tipo float

	//Comprobamos que el tipo de argumentos recibidos son de tipo numérico y tienen el siguiente formato:
	// n, -n, nn, -nn, nnn, n.n, n...n, n.n...n, -n...n, -n.n...n
	// La letra n representa un numero entero
	// No confindir el '.' con '...', un '.' encerrado entre dos números
	// representa al numero decimal n.n, mientras que los tres puntos '...'
	//representan que puede haber a continuación tantos número como se desee n...n
	//Empezamos comprobando el número de argumentos recibidos y
	//despues comprobamos respetando el orden primero last, despues first y por último incre

	if(argc==2){//Hemos recibido 1 Argumento:  last
		// Chekeamos last: almacenado en la posicion 1 de argv[]

			strcpy(last_string,argv[1]);
			strcpy(cero,"0");
			ret=strcmp(last_string,cero);

			if(ret==0){
				last_float=0;
				printf("last_float = %f\n",last_float);
			}else{
				last_float= strtof(last_string,&pend);
				if(!(last_float==0.000000)){
					printf("last_float = %f\n",last_float);
				}else{
					printf("Error1 seq: invalid floating point argument: %s\n",argv[1]);
					return 2;
				}
			}

	}else if(argc==3){ //Hemos recibido 2 Argumentos en este orden: first  last
		// Primero chekeamos last: almacenado en la posicion 2 de argv[]
					strcpy(last_string,argv[2]);
					strcpy(cero,"0");
					ret=strcmp(last_string,cero);

					if(ret==0){
						last_float=0;
						printf("last_float = %f",last_float);
					}else{
						last_float= strtof(last_string,&pend);
						if(!(last_float==0.000000)){
							printf("last_float = %f\n",last_float);
						}else{
							printf("Error1 seq: invalid floating point argument: %s\n",argv[2]);
							return 2;
						}
					}
		// Por último chekeamos first: almacenado en la posicion 1 de argv[]

					strcpy(first_string,argv[1]);
					strcpy(cero,"0");
					ret=strcmp(first_string,cero);

					if(ret==0){
						first_float=0;
						printf("first_float = %f\n",first_float);
					}else{
						first_float= strtof(first_string,&pend);
						if(!(first_float==0.000000)){
							printf("first_float = %f\n",first_float);
						}else{
							printf("Error1 seq: invalid floating point argument: %s\n",argv[1]);
							return 2;
						}
					}

	}else{//Hemos recibido 3 Argumentos en este orden: first incre last
		// Primero chekeamos last: almacenado en la posicion 3 de argv[]
							strcpy(last_string,argv[3]);
							strcpy(cero,"0");
							ret=strcmp(last_string,cero);

							if(ret==0){
								last_float=0;
								printf("last_float = %f\n",last_float);
							}else{
								last_float= strtof(last_string,&pend);
								if(!(last_float==0.000000)){
									printf("last_float = %f\n",last_float);
								}else{
									printf("Error1 seq: invalid floating point argument: %s\n",argv[3]);
									return 2;
								}
							}
	//A continuacion chekeamos first: almacenado en la posicion 1 de argv[]

							strcpy(first_string,argv[1]);
							strcpy(cero,"0");
							ret=strcmp(first_string,cero);

							if(ret==0){
								first_float=0;
								printf("first_float = %f\n",first_float);
							}else{
								first_float= strtof(first_string,&pend);
								if(!(first_float==0.000000)){
									printf("first_float = %f\n",first_float);
								}else{
									printf("Error1 seq: invalid floating point argument: %s\n",argv[1]);
									return 2;
								}
							}
		// Por ultimo chekeamos incre: almacenado en la posicion 2 de argv[]
							strcpy(incre_string,argv[2]);
							strcpy(cero,"0");
							ret=strcmp(incre_string,cero);

							if(ret==0){
								incre_float=0;
								printf("last_float = %f\n",incre_float);
							}else{
								incre_float= strtof(incre_string,&pend);
								if(!(incre_float==0.000000)){
									printf("incre_float = %f\n",incre_float);
								}else{
									printf("Error1 seq: invalid floating point argument: %s\n",argv[2]);
									return 2;
								}
							}
		}

	//Control Errores Argumentos: Error 4, 5 y 6

		if(argc==4){
			if(incre_float ==0){
				printf("Error1 seq: zero increment\n");
				return 3;
			}
			if(((incre_float>0)&&(last_float<first_float))){
				printf("Error1 seq: needs negative decrement\n");
				return 4;
			}
			if(((incre_float<0)&&(last_float>first_float))){
				printf("Error1 seq: needs positive increment\n");
				return 5;
			}


		}

//Fin Contro Errores: faltaría controlar si un argumento recibe en su primera posición
//					  y después letras la funcion de biblioteca strtof devuelve
//					  como número el primero que está almacenado en x_string.

//Empieza funcionalidad del programa


		if(argc==2){// Se recibe 1 Argumento: last
			if(last_float>0){
				for (int i=1;i<=last_float;i++){
					printf("%i\n",i);
				}
			}else{
				for (int i=1;i>=last_float;i--){
					printf("%i\n",i);
				}
			}







		}else if(argc==3){// Se reciben 2 Argumentos: first last
			if(first_float > 0 && last_float > 0 ){//first>0 y last>0
				for (float i=first_float;i<=last_float;i++){
					printf("%f\n",i);
				}
			}else if(first_float > 0 && last_float < 0 ){//first>0 y last<0
				for (float i=first_float;i>=last_float;i--){
					printf("%f\n",i);
				}
			}else if(first_float < 0 && last_float > 0){//first<0 y last>0
				for (float i=first_float;i<=last_float;i++){
					printf("%f\n",i);
				}
			}else{//first<0 y last<0
				for (float i=first_float;i>=last_float;i--){
					printf("%f\n",i);
				}
			}

		}else if(argc==4){// Se reciben 3 Argumentos first incre last
			if(first_float > last_float){//first>0 y incre<0 y last>0
				for (float i=first_float;i>=last_float;i=i+incre_float){//notasr que incre tiene signo negativo por eso se suma
				printf("%f\n",i);
				}
			}else if(first_float < last_float ){//first>0 y last<0
				for (float i=first_float;i>=last_float;i=i+incre_float){
					printf("%f\n",i);
				}
			}
		}


//FIN PROGRAMA
}

