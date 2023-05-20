#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>
#include "list.h"
//#include "treemap.h"
#include "hashmap.h"

//#include "stack.h"


#define MAX 31

typedef struct
{
  char nombre[MAX];
  int prioridad;
  bool realizada;
  List* precedentes;
}tareita;

void mostrarOpciones()
{
  puts("================================================================");
  puts("\t1.- AGREGAR TAREA");
  puts("\t2.- ESTABLECER PRECEDENCIA DE TAREA");
  puts("\t3.- MOSTRAR TAREAS POR HACER");
  puts("\t4.- MARCAR TAREAS COMPLETADAS");
  puts("================================================================");
  printf("Seleccione una opción (1-4) o ingrese 0 para salir: ");
}

void agregarTarea(HashMap* tareas)
{
  tareita * datos = (tareita*)malloc(sizeof(tareita));
  char nombre[MAX];
  
  getchar();
  printf("Ingrese el nombre de la tarea: ");
  scanf("%[^\n]%*c",nombre);
  
  strcpy(datos->nombre, nombre);
  
  printf("Ingrese la prioridad de la tarea: ");
  scanf("%i",&datos->prioridad);
  
  datos->realizada = false;
  List* precedentes = createList();
  datos->precedentes = precedentes;
  
  insertMap(tareas, datos->nombre, datos);
}

void presedenciaTarea(HashMap* tareas)
{
  char nombrePrimeraTarea[MAX], nombreSegundaTarea[MAX];

  getchar();
  printf("Ingrese el nombre de la primera tarea: ");
  scanf("%[^\n]%*c",nombrePrimeraTarea);

  printf("Ingrese el nombre de la segunda tarea: ");
  scanf("%[^\n]%*c",nombreSegundaTarea);

  if(searchMap(tareas, nombreSegundaTarea)!= NULL){
  
    Pair *buscado = searchMap(tareas, nombrePrimeraTarea);
    tareita* temp = buscado->value;
  
    pushBack(temp->precedentes, searchMap(tareas, nombreSegundaTarea));
    eraseMap(tareas, temp->nombre);
    eraseMap(tareas, nombreSegundaTarea);
    insertMap(tareas, temp->nombre, temp);
    return;
  }
  
  if(searchMap(tareas, nombrePrimeraTarea)== NULL)
  {
    Pair *buscado = firstMap(tareas);
    tareita* temp = buscado->value;
    void* precedentes = firstList(temp->precedentes);
    
    while(temp!=NULL)
    {    
      if(temp->precedentes!= NULL)
      {
        while(precedentes!= NULL)
        {
          if(strcmp(precedentes,nombrePrimeraTarea) == 0)
          {
            pushBack(temp->precedentes, searchMap(tareas, nombreSegundaTarea));
            eraseMap(tareas, nombreSegundaTarea);
            insertMap(tareas, temp->nombre, temp);
            return;
          }
          precedentes= nextList(temp->precedentes);
          
        }
      }
      buscado = nextMap(tareas);
      temp = buscado->value;
      precedentes = firstList(temp->precedentes);
    }
  }
}

/*
Como los datos estan guardados en un 
*/

void marcarTarea(HashMap* mapa)
{
  char nombreTarea[MAX];
  getchar();
  printf("Ingrese el nombre de la primera tarea: ");
  scanf("%[^\n]%*c",nombreTarea);
  
  //en caso de que se la tarea con mayor precedencia 
  if(searchMap(mapa, nombreTarea) != NULL)
  {
    Pair* temp = searchMap(mapa, nombreTarea);
    tareita* tareas = temp->value;
    
    if(tareas->precedentes !=NULL)
    {
      char opcion[MAX];
      printf("Esta tarea tiene prescedencia si quiere eliminarla escriba si, si no quiere eliminarla escriba no: ");
      scanf("%[^\n]%*c",opcion);
  
      if(strcmp(opcion, "si") == 0)
      {
        eraseMap(mapa, opcion);
      }
    }
  }
  if(searchMap(mapa, nombreTarea)== NULL)
  {
    Pair *buscado = firstMap(mapa);
    tareita* temp = buscado->value;
    void* precedentes = firstList(temp->precedentes);
    
    while(temp!=NULL)
    {    
      if(temp->precedentes!= NULL)
      {
        while(precedentes!= NULL)
        {
          if(strcmp(precedentes,nombreTarea) == 0)
          {
            popCurrent(temp->precedentes);
            eraseMap(mapa, nombreTarea);
            insertMap(mapa, temp->nombre, temp);
            return;
          }
          precedentes= nextList(temp->precedentes);
        }
      }
      buscado = nextMap(mapa);
      temp = buscado->value;
      precedentes = firstList(temp->precedentes);
    }
  }
  
}

void menu(HashMap * tareas)
{
  int opcion;

  while (1) 
  {
    mostrarOpciones();
    scanf("%d", &opcion);
    
    switch (opcion) 
    {
      case 1:
        printf("Has seleccionado la opción 1.\n");
        agregarTarea(tareas);
        break;

      case 2:
        printf("Has seleccionado la opción 2.\n");
        presedenciaTarea(tareas);
        break;

      case 3:
        printf("Has seleccionado la opción 3.\n");
        break;

      case 4:
        printf("Has seleccionado la opción 4.\n");
        marcarTarea(tareas);
        break;

      case 0:
        printf("Saliendo del programa...\n");
        return ;

      default:
        printf("Opción inválida.\n");
        break;
    }
  }

  
}

int main(void) 
{
  HashMap* Tareas = createMap(10000);
  menu(Tareas);
  
  
  return 0;
}