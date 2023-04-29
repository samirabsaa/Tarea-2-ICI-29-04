#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>
#include "list.h"
#include "stack.h"
#define BARRA printf(" ------------------------------------------------------\n")

//NOMBRE, PUNTOS DE HABILIDAD, #ITEMS
typedef struct{
  char nombre[100];
  int PH;
  int num_items;
  List *items;
}Perfil; 

typedef struct{
  char nombre[100];
  char item[100];
  int accion; //la accion sera 1 por si se añade item, 2 por si se elimina item y 3 por si se añade PH
  int PH_an; //puntos añadidos, si es que se realiza la accion de añadir PH
}Pila;


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
   //printf("item: %s\n", items);
   printf("\nPERFIL JUGADOR:\n");
   printf(" * Nombre jugador: %s \n", aux->nombre);
   printf(" * Puntaje habilidades: %d\n", aux->PH);
   if(aux->num_items == 0){
     printf("Jugador/a no tiene ningún item\n");   
   }
   else{
     printf(" * Numero de items: %i\n", aux->num_items);
     printf(" * Inventario de items obtenidos:\n");
     while(items != NULL){
       printf("     - %s\n",items);
       items = nextList(aux->items);
     }
   }
 }

 //3. AGREGAR ITEM JUGADOR 
 void agregar_item(char *nombre,List *lista,char *item){
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
  char *itm= malloc(sizeof(char)*strlen(item));
  strcpy(itm,item);
  printf("  (%s)  \n\n",itm);
  pushFront(aux->items, itm);
 }

 //4. ELIMINAR ITEM JUGADOR
 void eliminar_item(List *lista,char *nombre,char *item){
  Perfil* jugador = firstList(lista);
  jugador = busqueda(nombre, lista);
  if(jugador == NULL){
    printf("NO SE PUDO ELIMINAR ÍTEM, PUES NO EXISTE JUGADOR\n");
    return; 
  } 
  
  if(firstList(jugador -> items)  == NULL){
    printf("el jugador no posee ningun objeto\n");
    return;
  }

   if(busqueda(item, jugador -> items) != NULL)
   {
     popCurrent(jugador -> items);
     jugador -> num_items--;
     printf("se eliminó el objeto en cuestion\n");
     return;
   }else{
     printf("el objeto en cuestion no lo posee el jugador\n");
   }

  printf("\n");
  BARRA;
 }

 //5. AGREGAR PUNTOS HABILIDAD 
 void agregar_ph(char *nombre, List *lista, Pila * accion){
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

 //6. MOSTRAR JUGADORES ITEM ESPECIFICO 
void mostrar_iEspecifico(List *lista){
  char lectura[100];
  char *item;
  bool hay = false;
  Perfil *aux = firstList(lista);
  printf("que objeto desea saber quien tiene\n");
  getchar();
  scanf("%99[^\n]s", lectura);
  getchar();
  item = malloc(sizeof(char)*strlen(lectura));
  strcpy(item,lectura);
  while(aux != NULL){
    if(busqueda(item, aux -> items) != NULL){
      if(hay == false){
        printf("Los jugadores con este objeto son:\n");
        hay = true;
      }
      printf("  * %s\n", aux ->nombre);
    } 
    aux = nextList(lista);
  }
  if(hay == false) printf("no existe un jugador con ese objeto\n");
  BARRA;
}

 //8. EXPORTAR ARCHIVO
 void exportar_datos(List *lista){
    char salida[150];
    printf("Ingrese el nombre del archivo que desea crear\n");
    getchar();
    scanf("%149[^\n]s", salida );
    getchar();
    FILE *exit = fopen(salida, "w");
    Perfil *jugador = firstList(lista);
    while(jugador != NULL){
      fprintf(exit,"%s,%i,%i,",jugador ->nombre, jugador -> PH, jugador -> num_items  );
      char *itm = firstList(jugador ->items);
      while(itm != NULL)
      {
        fprintf(exit, "%s,", itm);
        itm = nextList(jugador -> items);
      }
      jugador = nextList(lista);
      fprintf(exit,"\n");
    }
    fclose(exit);
    printf("se creo el archivo exitosamente\n");
    return;
 }

 //9. CARGAR DATOS
//funcion hecha por el profe, si no funciona, culpen al profe.
const char *get_csv_field (char * tmp, int k) {
    int open_mark = 0;
    char* ret=(char*) malloc (100*sizeof(char));
    int ini_i=0, i=0;
    int j=0;
    while(tmp[i+1]!='\0'){
        if(tmp[i]== '\"'){
            open_mark = 1-open_mark;
            if(open_mark) ini_i = i+1;
            i++;
            continue;
        }

        if(open_mark || tmp[i]!= ','){
            if(k==j) ret[i-ini_i] = tmp[i];
            i++;
            continue;
        }

        if(tmp[i]== ','){
            if(k==j) {
               ret[i-ini_i] = 0;
               return ret;
            }
            j++; ini_i = i+1;
        }

        i++;
    }
    if(k==j) {
       ret[i-ini_i] = 0;
       return ret;
    }
    return NULL;
}

 List * cargar_datos(){
  List *listaaux;
  char archivo[100];
  printf("¿Qué archivo desea importar?\n");
  getchar();
  scanf("%99[^\n]",archivo);
  getchar();
  FILE *fp = fopen (archivo, "r");
  if(fp == NULL){
    printf("el archivo no existe\n");
    return NULL; 
  } 
  char linea[1024];
  int i;
  fgets (linea, 1023, fp);
  
  listaaux = createList();
  char *itm = (char*)malloc(sizeof(char)*100);
  //char *aux = (char*)malloc(sizeof(char)*100);
  while (fgets (linea, 1023, fp) != NULL) { // Se lee la linea
    Perfil *datos = malloc(sizeof(Perfil));
    i=0;
    char *aux = (char*)get_csv_field(linea, i);
    while(aux != NULL){
      aux = (char*)get_csv_field(linea, i); // Se obtiene el nombre
      if(datos->items == NULL) datos ->items = createList();
      if(i == 0){
        strcpy(datos ->nombre, aux );
      }
      else if(i == 1)
      {
        datos ->PH = atoi(aux);
      }
      else if(i == 2)
      {
        datos ->num_items = atoi(aux);
      }else{
        if(aux != NULL){
          strcpy(itm, aux);
          //printf("item: %s\n", itm);
          pushFront(datos -> items, itm);
       }
      }
      i++;
    }
    pushFront(listaaux, datos);
  }
  printf("importada de manera exitosa\n");
  return listaaux;     
}
   
   
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
    char nombreI [100];
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
        printf("Ingrese el nombre del ítem que desea añadir\n");
        scanf("%99[^\n]s",nombreI);
        getchar();
        //printf("(1)\n");
        
        agregar_item(nombreJ, lista, nombreI);
        
        strcpy(ult_accion->nombre,nombreJ);
        strcpy(ult_accion->item,nombreI);
        ult_accion->accion = 1;
        BARRA;
        break;
      
      case 4:
        printf("Eliminar item de jugador/a\n");
        if(lista == NULL){
          printf("No hay jugadores\n");
          break;
        }
        printf("¿ A que jugador desea eliminar un item?\n");
        getchar();
        scanf("%99[^\n]s", nombreJ);
        getchar();
        if(busqueda(nombreJ, lista)== NULL)
        {
          printf("El jugador no existe\n");
          break;
        }
        printf("¿Qué item desea eliminar?\n");
        scanf("%99[^\n]s",nombreI);
        getchar();
        eliminar_item(lista,nombreJ,nombreI);
        strcpy(ult_accion->nombre, nombreJ);
        strcpy(ult_accion->item, nombreI);
        ult_accion->accion = 2;
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
        mostrar_iEspecifico(lista);
        break;

      //esto no funciona para 2 acciones seguidas y ademas no hay vuelta atras
      case 7:
        printf("Deshacer última acción de jugador/a\n");
        if(lista == NULL){
          printf("No hay jugadores\n");
          break;
        }
        
        switch(ult_accion->accion)
          {
            case 1:
            eliminar_item(lista, ult_accion->nombre, ult_accion->item);
            printf("Accion deshecha\n");
            break;
            case 2:
            agregar_item(ult_accion->nombre, lista, ult_accion->item);
            printf("Accion deshecha\n");
            break;
            case 3:
            aux = firstList(lista);
            while(aux != NULL)
              {
                if(aux == busqueda(ult_accion->nombre, lista))
                {
                  aux->PH -= ult_accion->PH_an;
                  break;
                }
                aux = nextList(lista);
              }
            printf("Acción deshecha\n");
            break;
            
          }
        //deshacer_uAccion(nombreJ);*/
        BARRA;
        break;
      
      case 8:
        printf("Exportar datos de jugadores a archivo de texto\n");
        exportar_datos(lista);
        break;
      
      case 9:
        printf("Cargar datos de jugadores desde un archivo de texto\n");
        lista = cargar_datos(); 
        break;
      case 0:
        if (lista != NULL) cleanList(lista);
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