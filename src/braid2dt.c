/* braid2DT.c 
 * Jim Hoste 
 * revised 1/19/98
 */

#include <stdio.h>
#include <stdlib.h>

#define maxCrossings 500
#define maxGenerators 100
#define maxStrings 20

FILE *fp, *gp;

int main(int argc, char **argv);

int braid2DT(
   int braidword[ ],
   int DTcode[ ])
{
   /* This routine recieves braid input and produces
    * DT code output.
    *
    * INPUT:
    * The braid input is in the array braidword[ ] and
    * should be of the following format: the entries are 
    * integers and come in pairs. The first integer i in a
    * pair represents the braid generator sigma_i. The second
    * integer in each pair, j, is the exponent of the
    * generator sigma_i. Here we assume that sigma_i
    * means that the i-th string crosses over the (i-1)st
    * string, producing a right handed crossing.
    * After one pair of entries for each generator in 
    * the braid word, a sentinel of zero is expected in the
    * array braidword[ ]. The length of braidword[] is
    * determined by the golobal constant, maxGenerators,
    * and is set to 2*maxGenerators+1. 
    *
    * Example of input:
    * braidword[0]=1;
    * braidword[1]=3;
    * braidword[2]=3;
    * braidword[3]=-3;
    * braidword[4]=0;
    *
    * This input represents sigma_1^3 sigma_3^-3.
    * 
    * OUTPUT:
    * Some elementary error checking is done to the input
    * braid. If no errors are detected, the braid is converted 
    * even-integer DT code. If the diagram is indeed a knot, and
    * not a link, then a 0 is returned and the DT code is in the
    * array DTcode[] that was passed into the routine.
    * If the input data was bad or, the diagram is found to be of 
    * a link, then a 1 is returned. In this case DTcode will
    * contain all zeros.
    *
    * The length of the array DTcode[] is controlled by the
    * global constant, maxCrossings, and is set to 
    * maxCrossings +1. A sentinel of zero follows the DT sequence.
    *
    * ERROR CHECKING OF INPUT:
    * The input is checked for these errors:
    * 1) no generators, ie the empty word was input.
    * 2) bad generator, checks for negative generator index.
    * 3) too many generators, ie the number of generators
    * implies a number of strings exceeding the global
    * constant maxStrings
    * 4) missing generators, i.e. every generator between
    * sigma_1 and sigma_k, where k is the largest generator
    * appearing, must appear. If not, then the braid represents
    * a split link.
    *
    */
   
   int braid[maxCrossings][2];
   int diagram[maxCrossings][maxStrings];
   
   int generator,exponent; 			/*used to read in input braid*/
   int generators[maxStrings], maxGen;		/*used for error checking of input*/
   int row,string; 				/*used to chase through braid*/
   int label, evenlabel, oddlabel, DTsign; 	/*used to give DT labeling to diagram*/
   int N;   					/* N is the number of crossings*/
   int i,j;

   /* zero the DTcode array */
   for(i=0;i<maxCrossings;i++) DTcode[i]=0;
   
   /* zero generators[] array */
   for(i=0;i<maxStrings;i++) generators[i]=0;
   
   /* expand braidword[] into braid[] array, one row per crossing.
    * Determine number of crossings while we're at it and do 
    * some error checking of input.
    * Sentinel of zero at end of braidword[] is needed for this step.
    */
   
   N=0;
   j=0;
   maxGen=0;
   while((generator=braidword[j])!=0){
      exponent=braidword[j+1];

      if(generator<0 || generator>= maxStrings){
         fprintf(gp, "Generator outside range of 1 to %3d\n",maxStrings-1);
         return 1;
      }
      maxGen=(generator>maxGen)? generator:maxGen;
      generators[generator]=1;
      for(i=0;i<abs(exponent);i++){
         if(N+i>=maxCrossings){
            fprintf(gp, "Number of crossings exceeds constant
            maxCrossings=%4d\n",maxCrossings);
            return 1;
         }
      	 braid[N+i][0]=generator;
      	 braid[N+i][1]=abs(exponent)/exponent;
      }
      N=N+i;
      j+=2;
   }
   
   /*check that braid word is not empty*/
   if(N==0){
      fprintf(gp, "Braid is empty word\n");
      return 1;
   }
    
   /* check to see if the braidword contains all generators
    * between 1 and maxGen.
    */
    
    for(i=1;i<=maxGen;i++){
    	if(generators[i]==0){
    	   fprintf(gp, "Closure of braid is a split link\n");
    	   return 1;
    	}
    }
  
   /* zero the rows and columns of the diagram array that will be used*/
   for(i=0;i<N;i++)
      for(j=0;j<=maxGen;j++)
         diagram[i][j]=0;

   /* chase though the braid, DT labeling the diagram as you go
    * until you get back to the first row, first string
    */
    
   label=1; /* label starts at 1 and advances 1 per crossing */
   row=0;
   string=0;
   diagram[row][string]=label;
   do{
      /* advance along this string to its first crossing*/
      while(braid[row][0]!=string && braid[row][0]!=string+1 ){
         row=(row<N-1)? row+1 :0;
         diagram[row][string]=label;
      }
      /* if we're not back at the begining, or we're at the begining
       * and have never advanced, go through the crossing
       */
      if(row+string>0 || (row+string==0 && label==1)){
         string= (braid[row][0]==string) ? string-1:string+1;
         row=(row<N-1)?row+1:0;
         label++;
         diagram[row][string]=label;
      }
   } while (row+string>0);
   
   /* we have returned to the beginning. Reset label to 1*/
   diagram[0][0]=1;
  
   /* we have arrived back at the starting point.
    * we have a knot iff we have crossed all N
    * crossings. I.e., iff the label is now 2N+1
    */

   if(label!=2*N+1){
      fprintf(gp, "Closure of braid is a link of more than one component\n");
      return 1;
   } else {
   /* go down through the rows of the diagram and
    * enter the data for each crossing in the DTcode
    * array. If an odd crossing label is on top, then
    * the even label for that crossing receives a 
    * plus sign in the DT code, otherwise a minus sign.
    */
      for(row=0;row<N;row++){
         if(diagram[row][braid[row][0]]%2==1){
            oddlabel=diagram[row][braid[row][0]];
            evenlabel=diagram[row][braid[row][0]-1];
            DTsign=braid[row][1];
         } else{
            oddlabel=diagram[row][braid[row][0]-1];
            evenlabel=diagram[row][braid[row][0]];
            DTsign=-braid[row][1];
         }
         DTcode[(oddlabel-1)/2]=DTsign*evenlabel;
      }
      return 0;
   }
}

main(int argc, char **argv)
{
   int braidword[2*maxGenerators];
   int DTcode[maxCrossings], a[100];
   int is_composite, sgn, nc, ns;
   
   int i, j, k, length;
   
   fp = fopen(argv[1], "r");
   gp = fopen(argv[2], "w");
   
   length = i = 0;
   
   while ((ns=getc(fp)) != EOF)
   {
     if (ns != ' ' && ns != '\n')
     {
        if (ns >= 'A' && ns <= 'Z')
	{
	   braidword[i] = ns - 'A' + 1;
	   braidword[i+1] = -1;
	}
	else if (ns >= 'a' && ns <= 'z')
	{
	   braidword[i] = ns - 'a' + 1;
	   braidword[i+1] = 1;
	}
	else
	{
	   fprintf(gp, "Data is invalid.\n");
	   exit(0);
	}
	
	i+=2;
	length+=2; 
     }
   }

   braidword[length]=0;
   
   if (length > 98)
   {
      fprintf(gp, "Crossing-number is too large; limit is 49.\n");
      exit(0);
   }
         
   /* pass braid input and DT code array to 
    * braid2DT routine 
    */

   if (braid2DT(braidword, DTcode) == 0)
   {

      /* print out DTcode. Note that a sentinel of zero
       * marks end of DT code since braid2DT
       * procedure will first zero the array
       */
 
      nc = -1;
      while (DTcode[++nc] != 0);

      /* one final check, namely for primality */
 
      for (i=0; i<=nc-1; ++i) a[2*i+1] = abs(DTcode[i]);
      for (i=1; i<=2*nc-1; i+=2) a[a[i]] = i;

      for(i=1; i<=2*nc-2; i++) for (j=i+1; j<=2*nc-1; j++)
      {
        is_composite = 1;
        for (k=i; k<=j; k++) if ((a[k]<i) || (a[k]>j)) is_composite = 0;
        if (is_composite)
        {
          fprintf(gp, "Proposed Dowker code does not represent a prime diagram.\n");
          exit(0);
        }
      }
       /* primality check completed */

      fprintf(gp, "%2d        1   ", nc);
 
      sgn = DTcode[0] / abs(DTcode[0]);    /* normalize so that first term is positive */
      for (i=0; i<=nc-1; ++i) fprintf(gp, "%4d", DTcode[i] * sgn);
      fprintf(gp, "\n");
   }
 
   return 0;
}
