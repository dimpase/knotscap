#include <stdio.h>
#include <stdlib.h>

FILE *fp, *gp;



main(int argc, char *argv[])
{
   int a[100], a0[100], i, j, k, nc, s, t, ck, offset, firstknot, lastknot;
   unsigned char ch, ch1, ch2, seq[100];
   
   fp = fopen(argv[1], "r");
   gp = fopen(argv[2], "w");

   firstknot = strtol(argv[3], NULL, 0);
   lastknot = strtol(argv[4], NULL, 0);
   
   nc = ((int) getc(fp))/17 + 1;
   ck = 0;
   
   fclose(fp);
   fp = fopen(argv[1], "r");
      
   for (i=0; i<nc; ++i) a0[i] = 0;
   
   
   while ((ch=getc(fp)) != 255 || feof(fp) == 0)
   {
      ++ck;
   
      offset = (nc-1) - (ch)/17;
      
      for (i=0; i<offset; ++i) a[i] = a0[i];
      
      if (ck >= firstknot && ck <= lastknot)
      {
         for (i=offset; i<nc-1; i+=2)
         {
            ch = getc(fp);
 
            a[i] = (int)(ch/16 + 1);
            a[i+1] = (int)(ch - 16*(a[i]-1) + 1);
         }
 
         t=0;
         for (i=0; i<nc-1; ++i) t+=a[i];
         a[nc-1] = (nc*(nc+1))/2 - t;
 
         fprintf(gp, "%2d  %7d   ", nc, ck);
         for (i=0; i<nc; ++i) fprintf(gp, "%4d", 2*a[i]);
         fprintf(gp, "\n");
 
         for (i=0; i<nc; ++i) a0[i] = a[i];
      }
      
      else
      {
         for (i=offset; i<nc-1; i+=2)
         {
            ch = getc(fp);
            
            a[i] = (int)(ch/16 + 1);
            a[i+1] = (int)(ch - 16*(a[i]-1) + 1);
         }
         
         for (i=0; i<nc; ++i) a0[i] = a[i];

      }     
   }
   
   return 0;
}
