/**********************************************************
 * This code was written by Richard Zanibbi in May 1998
 * based on code by Donald Quon and Hai Pham,
 * and Dr. R. T. Whitaker of the University of Tennesee
 * for use in Queen's University's CISC 859 course

 * Modified to make use of dynamic memory and independent
 * reading and writing functions by
 * R. Hasimoto-CIMAT, Sept. 2022.
 * Filters created by A. Antonio Mendez H.
************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAXLENGTH 100
#define MAXVALUE  255
unsigned char **pgmRead(char *fileName,int *rows,int *cols)
{
      FILE *filePointer;
      char line[MAXLENGTH]; /* for character input from file */
      int maximumValue = 0; /* max value from header */
      int binary;           /* flag to indicate if file is binary (P5) or ascii (P2)*/
      long numberRead = 0;  /* counter for number of pixels read */
      long i,j;             /* (i,j) for loops */
      int test,temp;        /* for detecting EOF(test) and temp storage */
      unsigned char *tempPtr, **image;

      /* Open the file, return an error if necessary. */
      if ((filePointer = fopen(fileName,"r")) == NULL) {
	   printf ("ERROR: cannot open file\n\n");
	   fclose (filePointer);
	   return (0);
      }

      *cols = *rows =0;

      /* Check the file signature ("Magic Numbers" P2 and P5); skip comments
         and blank lines (CR with no spaces before it).*/
      fgets (line,MAXLENGTH,filePointer);
      while (line[0]=='#' || line[0]=='\n')
           fgets (line,MAXLENGTH,filePointer);

      if (line[0]=='P' && (line[1]=='2')) {
	   binary = 0;
	 /*   printf ("\nFile Format: P2\n"); */
      }
      else if (line[0]=='P' && (line[1]=='5')) {
	   binary = 1;
	  /*  printf ("\nFORMAT: P5\n"); */
      }
      else {
	   printf ("ERROR: incorrect file format\n\n");
	   fclose (filePointer);
	   return (0);
      }

      /* Input the width, height and maximum value, skip comments and blank
       * lines. */
      fgets (line,MAXLENGTH,filePointer);
      while (line[0]=='#' || line[0]=='\n')
         fgets (line,MAXLENGTH,filePointer);
      sscanf (line,"%d %d",cols,rows);

      fgets (line,MAXLENGTH,filePointer);
      while (line[0]=='#' || line[0]=='\n')
         fgets(line,MAXLENGTH,filePointer);
      sscanf (line,"%d",&maximumValue);

      if ((*cols)<1 ||(*rows)<1 || maximumValue<0 || maximumValue>MAXVALUE){
	   printf ("ERROR: invalid file specifications (cols/rows/max value)\n\n");
	   fclose (filePointer);
	   return (0);
      }

      // creating memory for the input image
      image = (unsigned char **)malloc((*rows) * sizeof(unsigned char *));
      if (image == NULL)
      {
         puts("\nFailure to allocate room for pointers");
         return(NULL);
      }

      tempPtr = (unsigned char *) malloc((*rows)*(*cols)*sizeof(unsigned char));
      if (tempPtr == NULL)
      {
         puts("\nFailure to allocate room for the array");
         return(NULL);
      }
      for (i = 0; i < *rows; i++)
      {
        image[i] = tempPtr + ( i * (*cols) );
      }


      /* Read in the data for binary (P5) or ascii (P2) PGM formats   */
      if (binary) {
	   for (i = 0; i < (*rows); i++) {
	        numberRead += fread((void *)&(image[i][0]),
		  sizeof(unsigned char),(*cols),filePointer);
		if (feof(filePointer)) break;
	   }
      }
      else {
	   for (i= 0; i < (*rows); i++) {
	        for (j =0; j < (*cols); j++) {
	             test = fscanf (filePointer,"%d",&temp);
		     if (test == EOF) break;
		     image[i][j] = (unsigned char)temp;

		     numberRead++;
		}
		if (test == EOF) break;
	   }
      }

      /* close the file and return 1 indicating success */
      fclose (filePointer);
      return (image);
}



/* INPUT: a filename (char*), the dimensions of the pixmap (rows,cols of
 *   type long), and a pointer to a 2D array (MAXROWS x MAXCOLS) in row
 *   major order.
 * OUTPUT: an integer is returned indicating if the desired file was written
 *   (in P5 PGM format (binary)).  A 1 is returned if the write was completed
 *   and 0 if it was not.  An error message is returned if the file is not
 *   properly opened.
 */
int pgmWrite(char* filename, int rows,int cols,unsigned char **image,char* comment_string)
{
      FILE* file;        /* pointer to the file buffer */
      int maxval;        /* maximum value in the image array */
      long nwritten = 0; /* counter for the number of pixels written */
      long i,j;          /* for loop counters */

      /* open the file; write header and comments specified by the user. */
      if ((file = fopen(filename, "w")) == NULL)	{
           printf("ERROR: file open failed\n");
	   return(0);
      }
      fprintf(file,"P5\n");

      if (comment_string != NULL) fprintf(file,"# %s \n", comment_string);

      /* write the dimensions of the image */
      fprintf(file,"%d %d \n", cols, rows);

      /* NOTE: MAXIMUM VALUE IS WHITE; COLOURS ARE SCALED FROM 0 - */
      /* MAXVALUE IN A .PGM FILE. */

      /* WRITE MAXIMUM VALUE TO FILE */
      fprintf(file, "%d\n", (int)255);

      /* Write data */
      for (i = 0;i < rows;i++) {
          nwritten += fwrite((void*)image[i],sizeof(unsigned char),cols,file);
      }
      printf("nwritten = %ld,",nwritten);

      fclose(file);
      return(1);
}

void promedio(unsigned char **imagen1, unsigned char **imagen2, int filas, int columnas){
    int dimv, lim1, lim2, i, j, k1, k2, suma, centro;
    printf("Ingrese el tamano de la ventana: ");
    scanf("%d", &dimv);
    centro=dimv/2;
    lim1=filas-centro;
    lim2=columnas-centro;
    //(i,j) son las coordenadas del centro
    for(i=centro; i<lim1; i++){
        for(j=centro; j<lim2; j++){
            //sumamos en los intervalos del centro hacia arriba y hacia abajo
            suma=0;
            for(k1=i-centro; k1<=i+centro; k1++){
                for(k2=j-centro; k2<=j+centro; k2++){
                    suma+=*((imagen1[k1])+k2);
                }
            }
            //guardamos
            imagen2[i][j] = suma / (dimv * dimv);
        }
    }
}

void gadelante(unsigned char **imagen1, unsigned char **imagen2, int filas, int columnas){
    //fijamos los limites para no salirnos de la imagen
    int lim1=filas-1, lim2=columnas-1, gx, gy, i, j;
    unsigned char *x, *x2, *y2;
    for(i=0; i<lim1; i++){
        for(j=0; j<lim2; j++){
            //ubicamos las posiciones deseadas
            x=imagen1[i]+j;
            x2=imagen1[i]+j+1;
            y2=imagen1[i+1]+j;
            gx=abs(*x2-*x);
            gy=abs(*y2-*x);
            //guardamos
            *(imagen2[i]+j)=gx+gy;
        }
    }
}

void gcentral(unsigned char **imagen1, unsigned char **imagen2, int filas, int columnas){
    int lim1=filas-1, lim2=columnas-1, gx, gy, i, j;
    unsigned char *pos, *x1, *x2, *y1, *y2;
    for(i=1;i<lim1;i++){
        for(j=1;j<lim2;j++){
            x1=imagen1[i]+j-1;
            x2=imagen1[i]+j+1;
            y1=imagen1[i-1]+j;
            y2=imagen1[i+1]+j;
            gx=abs(*x2-*x1)/2;
            gy=abs(*y2-*y1)/2;
            *(imagen2[i]+j)=gx+gy;
        }
    }
}

void gatras(unsigned char **imagen1, unsigned char **imagen2, int filas, int columnas){
    int gx, gy, i, j;
    unsigned char *x, *x2, *y2;
    for(i=1; i<filas; i++){
        for(j=1; j<columnas; j++){
            x=imagen1[i]+j;
            x2=imagen1[i]+j-1;
            y2=imagen1[i-1]+j;
            gx=abs(*x-*x2);
            gy=abs(*x-*y2);
            *(imagen2[i]+j)=gx+gy;
        }
    }
}

int main() {
   int i, j, max, ok, rows, cols, opcion;
   int count = 0;
   unsigned char **image, **image2;
   char file_in[80], file_out[80];

   // Archivo de entrada
   printf("Archivo pgm de entrada: ");
   gets(file_in);
   image = pgmRead(file_in,&rows,&cols);
   image2=pgmRead(file_in,&rows,&cols);
   if (image == NULL || image2==NULL) {
      printf("Error pgmRead\nO no hay espacio\n");
      exit(1);
   }
   printf("Ingrese la opcion que desee:\n1. Promedio\n2. Gradiente adelante\n3. Gradiente centro\n4. Gradiente atras\n");
   scanf("%d", &opcion);
   getchar();
   switch(opcion){
    case 1:
        promedio(image, image2, rows, cols);
        break;
    case 2:
        gadelante(image, image2, rows, cols);
        break;
    case 3:
        gcentral(image, image2, rows, cols);
        break;
    case 4:
        gatras(image, image2, rows, cols);
        break;
   }

   // limpiamos bufer
   fflush(stdin);
   int c;
   while ((c = getchar()) != '\n' && c != EOF)
      ;
   printf("Archivo pgm de salida: ");
   gets(file_out);
   ok = pgmWrite(file_out,rows,cols,image2,NULL);

   //liberar memoria
   for(int k=0; k<rows; k++){
        free(image[k]);
        free(image2[k]);
   }
   free(image);
   free(image2);
}
