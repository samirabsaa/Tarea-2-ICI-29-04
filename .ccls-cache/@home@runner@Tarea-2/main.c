#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>
#include "list.h"
#include "map.h"
#include "stack.h"
#define BARRA printf(" ------------------------------------------------------\n")

//NOMBRE, PUNTOS DE HABILIDAD, #ITEMS
typedef struct{
  char nombre[100];
  int PH;
  int num_items;
  List *items;
  Stack * accion;
}Perfil; 

typedef struct{
  char nombre[100];
  char item[100];
  int accion; //la accion sera 1 por si se añade item, 2 por si se elimina item y 3 por si se añade PH
  int PH_an; //puntos añadidos, si es que se realiza la accion de añadir PH
}Pila;

// Mapa de jugadores con el item en especifico
typedef struct
{
  char *key;
  const void *data;
}JCI;

//FUNCIONES ADICIONALES 
void * busqueda (char *nombreJ, List *lista){
  Perfil *aux = firstList(lista);
  while(aux != NULL){
    if (strcmp(nombreJ, aux->nombre) == 0) return aux;
    aux = nextList(lista);
  }
  return NULL;
}
  

//1.CREAR PERFIL
 void crear_perfil(List *lista){
   Perfil *datos;
   datos = (Perfil*)malloc(sizeof(Perfil));
   printf("Ingrese el nombre del jugador\n");
   getchar();
   scanf("%99[^\n]s",datos->nombre);
   getchar();
   datos->PH = 0;
   datos->num_items = 0;
   datos->items = createList();
   pushFront(lista, datos);
 }

 //2.MOSTRAR PERFIL
 void mostrar_perfil(char *nJugador, List *lista){
   Perfil *aux;
   aux = (Perfil*)malloc(sizeof(Perfil));
   //búsqueda jugador en lista 
   if(lista != NULL)
     aux = busqueda(nJugador, lista);
   
   if(aux == NULL){
     printf("Tal jugador no se encontró en la lista de jugadores\n");
     return;
   }
   
   char *items = firstList(aux->items);
   printf("\nPerfil jugador:\n");
   printf(" * Nombre jugador: %s \n", aux->nombre);
   printf(" * Puntaje habilidades: %d\n", aux->PH);
   if(aux->num_items == 0){
     printf("Jugador/a no tiene ningún item\n");   
   }
   else{
      printf(" * Inventario de items obtenidos:\n");
      while(items != NULL){
        printf("     - %s\n",items);
        items = nextList(aux->items);
      }
   }
 }

 //3. AGREGAR ITEM JUGADOR 
 void agregar_item(char *nombre,List *lista,Pila *accion){
  char *nomI;
  char lectura[100];
  
  printf("Ingrese el nombre del ítem que desea añadir\n");
  scanf("%99[^\n]s",lectura);
  getchar();
  nomI = malloc(sizeof(char)*strlen(lectura));
  strcpy(nomI, lectura);
  Perfil *aux = busqueda(nombre, lista);
  if(aux == NULL)
  {
    printf("El jugador no existe\n");
    return;
  }
  if(aux->items == NULL)
  {
    aux->items = createList();
  }
  aux->num_items++;
  pushFront(aux->items, nomI);
  strcpy(accion->nombre, nombre);
  strcpy(accion->item, nomI);
  accion->accion = 1;
 }

 //4. ELIMINAR ITEM JUGADOR
 void eliminar_item(List *lista,Pila *accion){
  Perfil* jugador = firstList(lista);
  char lectura[100];
  char * nombre;
  char *item;
 
  printf("Ingrese nombre jugador al que desea eliminar ítem\n");
  getchar();
  scanf("%99[^\n]s", lectura);
  nombre = malloc(sizeof(char)*strlen(lectura));
  strcpy(nombre, lectura);
  jugador = busqueda(nombre, lista);
  if(jugador == NULL){
    printf("NO SE PUDO ELIMINAR ÍTEM, PUES NO EXISTE JUGADOR\n");
    return; 
  } 
  
  printf("Ingrese el nombre del ítem que desea eliminar\n");
  getchar();
  scanf("%99[^\n]s", lectura);
  item = malloc(sizeof(char)*strlen(lectura));
  strcpy(item, lectura);
  if(firstList(jugador -> items)  == NULL){
    printf("el jugador no posee ningun objeto\n");
    return;
  }

   if(busqueda(item, jugador -> items) != NULL)
   {
     popCurrent(jugador -> items);
     jugador -> num_items--;
     printf("se elimino el objeto en cuestion\n");
     strcpy(accion->nombre,nombre);
     strcpy(accion->item,item);
     accion->accion = 2;
     return;
   }else{
     printf("el objeto en cuestion no lo posee el jugador\n");
   }

  printf("\n");
  BARRA;
  
 }

 //5. AGREGAR PUNTOS HABILIDAD 
 void agregar_ph(char *nombre, List *lista,Pila *accion){
   int puntos;
   Perfil * aux;
   aux = (Perfil *) malloc(sizeof(Perfil));
   printf("Ingrese la cantidad de puntos obtenidos\n");
   scanf("%d", &puntos);
   
   if(lista != NULL)
     aux = busqueda(nombre, lista);
   
   if(aux != NULL){
     aux->PH += puntos;
   }
   strcpy(accion->nombre, nombre);
   accion->PH_an = puntos;
   accion->accion = 3;
   //printf("puntajenombreombreombre %d", aux->PH);*/
 }


 /*//6. MOSTRAR JUGADORES ITEM ESPECIFICO 
 void mostrar_iEspecifico(item){
 
 }

//7. DESHACER ÚLTIMA ACCIÓN
 void deshacer_uAccion(char *nombre){
   La idea seria que: 
   si la pila esta vacia, quiere decir que no hay ninguna acción ( agregar/eliminar item y/o agregar puntos) para deshacer, sino que se elimine lo último que se guardó en la pila y se disminuya el top en 1
   
 }
 //8. EXPORTAR ARCHIVO
 void exportar_datos(nombre_archivo){}

 //9. CARGAR DATOS
 void cargar_datos(nombre_archivo){}*/ 


int main()
{
  List *lista = NULL;
  bool listacreada = false;
  Pila *ult_accion;
  ult_accion = (Pila*)malloc(sizeof(Pila));
  Perfil *aux;
  aux = (Perfil*)malloc(sizeof(Perfil));
  while(true)
  {
    int comando;
    BARRA;
    printf("|             💻¿QUÉ ACCIÓN DESEA REALIZAR?💻          |\n");
    BARRA;
    printf("|1. Crear perfil de jugador/a                          |\n");
    printf("|2. Mostrar perfil de jugador/a                        |\n");
    printf("|3. Agregar item a jugador/a                           |\n");
    printf("|4. Eliminar item a jugador/a                          |\n");
    printf("|5. Agregar puntos de habilidad a el/la jugador/a      |\n");
    printf("|6. Mostrar jugadores con item específico              |\n");
    printf("|7. Deshacer última acción de jugador/a                |\n");
    printf("|8. Exportar datos de jugadores a archivo de texto     |\n");
    printf("|9. Cargar datos de jugadores desde un archivo de texto|\n");
    printf("|0. Terminar                                           |\n");  
    BARRA;
    
    scanf("%i",&comando);
    
    char nombreJ [100];
    
    switch(comando){
      case 1:
        printf("Crear perfil de jugador/a\n");
        if(listacreada == false)
        {
          lista = createList();
          listacreada = true;
        }
         crear_perfil(lista);
        printf("Perfil creado\n");
        BARRA;
        break;
      
      case 2:
        printf("Mostrar perfil de jugador/a\n");
        if(lista == NULL){
          printf("No hay jugadores\n\n");
          break;
        }
        printf("Ingrese el nombre del jugador/a\n");
        getchar();
        scanf("%99[^\n]s", nombreJ);
        getchar();
        mostrar_perfil(nombreJ, lista);
        BARRA;
        break;
      
      case 3:
        printf("Agregar item a jugador/a\n");
        if(lista == NULL){
          printf("\nNo hay jugadores para agregar item\n");
          break;
        }
        printf("Ingrese nombre de jugador al que desea agregar item\n");
        getchar();
        scanf("%99[^\n]s", nombreJ);
        getchar();
        if(busqueda(nombreJ, lista)== NULL)
        {
          printf("El jugador no existe\n");
          break;
        }
        agregar_item(nombreJ, lista, ult_accion);
        BARRA;
        break;
      
      case 4:
        printf("Eliminar item de jugador/a\n");
        if(lista == NULL){
          printf("No hay jugadores\n");
          break;
        }
        eliminar_item(lista, ult_accion);
        BARRA;
        break;
      
      case 5:
        printf("Agregar puntos de habilidad a el/la jugador/a\n");
        if(lista == NULL){
          printf("No hay jugadores\n");
          break;
        }  
        printf("Ingrese nombre de jugador al que desea agregarle puntos de habilidad\n");
        getchar();
        scanf("%99[^\n]s", nombreJ);
        getchar();
        if(busqueda(nombreJ, lista) == NULL){
          printf("Jugador/a no se encontró en la lista\n");
          break;
        }
        agregar_ph(nombreJ, lista,ult_accion);
        BARRA;
        break;
      
      case 6:
        printf("Mostrar jugadores con item específico\n");
        break;
      
      case 7:
        printf("Deshacer última acción de jugador/a\n");
        if(lista == NULL){
          printf("No hay jugadores\n");
          break;
        }
        
        /*switch(ult_accion->accion)
          {
            case 1:
            break;
            case 2:
            break;
            case 3:
            
            
          }
        printf("Ingrese el nombre de el/la jugador/a\n");
        getchar();
        scanf("%99[^\n]s", nombreJ);
        getchar();
        if(busqueda(nombreJ, lista) == NULL){
          printf("Tal jugador/a no existe\n");
          break;
        }
        //deshacer_uAccion(nombreJ);*/
        BARRA;
        break;
      
      case 8:
        printf("Exportar datos de jugadores a archivo de texto\n");
        
        break;
      
      case 9:
        printf("Cargar datos de jugadores desde un archivo de texto\n");
        //lista = 
        break;
      case 0:
        printf("░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░\n");
        printf("░░██████░░██████░░░█████░░░██████░██░░█████░░███████░\n");
        printf("░██░░░░░░░██░░░██░██░░░██░██░░░░░░██░██░░░██░██░░░░░░ \n");
        printf("░██░░░███░██████░░███████░██░░░░░░██░███████░███████░ \n");
        printf("░██░░░░██░██░░░██░██░░░██░██░░░░░░██░██░░░██░░░░░░██░ \n");
        printf("░░██████░░██░░░██░██░░░██░░██████░██░██░░░██░███████░ \n");
        printf("░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░\n");
        printf("\n");
        printf("                    ATTE: 'G'\n");
          
        return 0;
      default:
        printf("¡¡¡ADVETENCIA!!!\n");
        printf("¡OPCIÓN NO CORRESPONDE A VALORES DEL MENÚ, POR FAVOR INGRESE NÚMERO VÁLIDO(0-9)!\n\n");
        BARRA;
        continue;
    }
  }
}