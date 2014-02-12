#include <stdio.h>
#include <stdlib.h>

FILE *fp1, *fp2, *gp;


main(int argc, char *argv[])
{
   int a[100], a0[100], a1[100], i, j, k, nc, s, t, u, ck, offset, firstknot, lastknot;
   unsigned char ch, ch1, ch2, seq[100];
   
   
   fp1 = fopen(argv[1], "r");
   fp2 = fopen(argv[2], "r");
   gp = fopen(argv[3], "w");
   
   
   nc = ((int) getc(fp1))/17 + 1;
   ck = 0;
   
   fclose(fp1);
   fp1 = fopen(argv[1], "r");
   
   fscanf(fp2, "%d", &nc);
   
   for (i=0; i<nc; ++i) fscanf(fp2, "%d", a1+i);
   for (i=0; i<nc; ++i) a1[i]/=2;
      
   for (i=0; i<nc; ++i) a0[i] = 0;
   
   
   while ((ch=getc(fp1)) != 255 || feof(fp1) == 0)
   {
      ++ck;
   
      offset = (nc-1) - (ch)/17;
      
      for (i=0; i<offset; ++i) a[i] = a0[i];
      
      for (i=offset; i<nc-1; i+=2)
      {
         ch = getc(fp1);
 
         a[i] = (int)(ch/16 + 1);
         a[i+1] = (int)(ch - 16*(a[i]-1) + 1);
      }
            
      for (j=1; j<=1; ++j) if (a[0]==a1[0] && a[1]==a1[1])
      {
         t=0;
         
         for (i=2; i<nc-1; ++i) if (a[i] != a1[i])
         {
            t=1;
            continue;
         }
         
         if (t == 0)
         {
            u=0;
            for (i=0; i<nc-1; ++i) u+=a[i];
            a[nc-1] = (nc*(nc+1))/2 - u;

            fprintf(gp, "%8d    ", ck);
            for (i=0; i<nc; ++i) fprintf(gp, "%4d", 2*a[i]);
            fprintf(gp, "\n");
         }
      }
 
      for (i=0; i<nc; ++i) a0[i] = a[i];
   }

return 0;
}
