#include <stdio.h>
#include <stdlib.h>

FILE *fp1, *fp2, *gp;


main(int argc, char *argv[])
{
   int a[100], g[100], a0[100], g0[100], a1[100], g1[100], 
       i, j, k, nc, s, t, pr, ck, offset, firstknot, lastknot;
   unsigned char ch, ch1, ch2, seq[100];
   
   
   fp1 = fopen(argv[1], "r");
   fp2 = fopen(argv[2], "r");
   gp = fopen(argv[3], "w");


   nc = ((int) getc(fp1))/17 + 1;
   ck=0;
   
   fclose(fp1);
   fp1 = fopen(argv[1], "r");
   
   fscanf(fp2, "%d", &nc);
      
   for (i=0; i<nc; ++i) fscanf(fp2, "%d", a1+i);
   for (i=0; i<nc; ++i) a1[i]/=2;
   
   for (i=0; i<nc; ++i) g1[i] = a1[i]/abs(a1[i]);
   for (i=0; i<nc; ++i) a1[i] = abs(a1[i]);

   for (i=0; i<nc; ++i) a0[i] = g0[i] = 0;
   
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

      pr=0;
      
      for (j=1; j<=1; ++j)
      {
         pr=0;
         
         for (i=0; i<nc-1; ++i) if (a[i] != a1[i])
         {
            pr=1;
            continue;
         }
         
         if (pr==1) continue;
         
         
         ch = getc(fp1);
 
         for (i=0; i<8; ++i)
         {
            ch1 = ch << i;
            ch2 = ch1 >> 7;
            g[i] = (int)ch2;
         }
 
         ch = getc(fp1);
 
         for (i=0; i<8; ++i)
         {
            ch1 = ch << i;
            ch2 = ch1 >> 7;
            g[i+8] = (int)ch2;
         }
 
         for (i=0; i<nc; ++i) g[i] = 2*g[i] - 1;
         
         
         for (i=0; i<nc; ++i) if (g[i] != g1[i]) pr=2;
         
         if (pr==2) continue;
         
         
         
         t=0;
         
         for (i=0; i<nc-1; ++i) t+=a[i];
         a[nc-1] = (nc*(nc+1))/2 - t;

         fprintf(gp, "%8d    ", ck);
         for (i=0; i<nc; ++i) fprintf(gp, "%4d", 2*a[i]*g[i]);
         fprintf(gp, "\n");
      }
      
      if (pr==1)
      {
         ch = getc(fp1);
         ch = getc(fp1);
      }
    
      
      for (i=0; i<nc; ++i) a0[i] = a[i];      
   }
return 0;
}
