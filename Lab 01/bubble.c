// Compilar:
// gcc -o bubble_seq bubble.c
// ./bubble_seq 2 10

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#define IMPRIME

void imprime(int **array, int num_arrays, int size){
  int i,j;
  for(i=0; i < num_arrays; i++){
	 for(j=0; j < size; j++){
		printf("%d ", array[i][j]);
	 }
	 printf("\n");
  }
}

int bubble(int *array,unsigned int size){
  int i,j;
  int temp;
  for(i = 0; i < size-1; i++){       
    for(j = 0; j < size-i-1; j++){          
      if(array[j] > array[j+1]){               
        temp = array[j];
        array[j] = array[j+1];
        array[j+1] = temp;
      }
    }
  }
}

int main(int argc, char **argv){
  int **elementos,n,tam,i,j;    
  struct timeval t1, t2;
  double t_total;

  if(argc!=3){
	 printf("Digite %s Num_arrays Num_elementos\n", argv[0]);
	 exit(0);
  }
  n = atoi(argv[1]);
  tam = atoi(argv[2]);	

  //srand(time(NULL));

  /*Aloca memória para os vetores*/
  elementos=(int**)malloc(n*sizeof(int*));
  for(i = 0; i < n; i++)
    elementos[i]=(int*)malloc(tam*sizeof(int));

  /*Popula os arrays com elementos aleatorios entre 0 e 1000*/
  for(i = 0; i < n; i++) 
    for (j = 0; j < tam; j++)
      elementos[i][j] = rand() % 1000;

  #ifdef IMPRIME
  printf("Antes da ordenacao!\n");
  imprime(elementos, n, tam);
  printf("\n");
  #endif

  gettimeofday(&t1, NULL);
  for (i = 0; i < n; i++)
    bubble(elementos[i],tam);
  gettimeofday(&t2, NULL);
  
  t_total = (t2.tv_sec - t1.tv_sec) + ((t2.tv_usec - t1.tv_usec)/1000000.0);
 
  #ifdef IMPRIME
  printf("apos a ordenacao!\n");
  imprime(elementos, n, tam);
  printf("\n");
  #endif

  /*Libera memoria alocada*/
  for(i=0;i<n;i++){
		free(elementos[i]);
	}
	free(elementos);

  printf("tempo total = %f\n", t_total);

  return 0;
}
