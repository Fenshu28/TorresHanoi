/*
 * Pila.h
 * Por: Dr. Víctor Alberto Gómez Pérez
 * Universidad de la Sierra Sur
 */
#include <stdio.h>
#define MAXo 8


typedef struct{
	int arreglo[MAXo];
	int tope;
}Pila;

void PilaVacia (Pila *a);
int EsVacia (Pila *a);
int Cima (Pila *a);
void Push (int x, Pila *a);
int Pop (Pila *a); 
int EsLlena(Pila *a);
int NumElem(Pila *a);
