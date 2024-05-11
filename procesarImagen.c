#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define MAX_SIZE 262144
#define threads 8
unsigned char archivo[MAX_SIZE]; 
int histograma[256];

int obtenerDatosImagen(char* imagen){
    char *file_name = imagen;
    FILE *file = fopen(file_name, "rb");
    if (file == NULL) {
        perror("Error al abrir el archivo");
        return 1;
    }

    int byte;
    size_t index = 0;
    while ((byte = fgetc(file)) != EOF && index < MAX_SIZE) {
        if(index == MAX_SIZE)
            printf("Advertencia: Se alcanzó el tamaño máximo del arreglo. Algunos bytes pueden no haber sido leídos.\n");
        archivo[index++] = (unsigned char)byte; 
    }
    
    fclose(file);

    return index;
}

void imprimirDatosImagen(unsigned char* data){
    printf("Bytes leídos:\n");
    for (size_t i = 0; i < MAX_SIZE; i++) {
        printf("%u ", data[i]); 
    }
    printf("\n");    
}

void imprimirHistograma(int* data){
    for(int i=0;i<256;i++){
        printf("%3d : %4d \n",i,data[i]);
    }
}

void normalizarHistograma(int* histogramaOriginal,int* histogramaNormalizado){
    int sum = 0;
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < 256; ++i) {
        sum += histogramaOriginal[i];
    }






}

int main() {
    printf("Obteniendo datos de la imagen..\n");
    int size = obtenerDatosImagen("lena_gray.raw");
   
    printf("Procesando...\n");
    for(int i=0;i<256;i++){
        histograma[i] = 0;
    }

    #pragma omp parallel shared(histograma) num_threads(threads)
    {
        #pragma omp for //schedule(static)
        for (int i = 0; i < size; i++) {
			histograma[archivo[i]] ++;
        }
    }

    imprimirHistograma(histograma);
    

    return 0;
}

