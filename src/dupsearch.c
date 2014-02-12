#include <stdio.h>
#include <stdlib.h>

FILE *fp1, *fp2, *fp3, *gp;


main(int argc, char *argv[])
{
  int a[100], a1[100], nc, nc0, nc1, id0,
      i, j, k, p, q, s, t;
  unsigned char ch[100], r[3];
  
  
  fp1 = fopen(argv[1], "r");
  fp2 = fopen(argv[2], "r");
  gp = fopen(argv[3], "w");
  
  
  fscanf(fp1, "%d", &nc1);
  for (i=0; i<nc1; ++i) fscanf(fp1, "%d", a1+i);

  while (fgets(ch, 30, fp2) != NULL)
  {
    nc = ch[0] - 48 + 13;
  
    if (nc1 != nc) continue;
    
    for (i=0; i<nc; ++i)
    {
      if (ch[i+1] < 96) a[i] = -2*(ch[i+1] - 64);
      else a[i] = 2*(ch[i+1] - 96);
    }
    
    t=0;
    for (i=0; i<nc; ++i) if (a[i] != a1[i]) t=1;
    if (t == 1) continue;

    nc0 = ch[nc+1] - 48 + 13;
    id0 = 65536*ch[nc+2] + 256*ch[nc+3] + ch[nc+4];

    decode_n(nc0, id0);
    exit(0);
  }
  
return 0;
}


decode_n(int nc, int id)
{
   int a[100], g[100], a0[100], g0[100], i, j, k, s, t, ck, offset,
       firstknot, lastknot;
   unsigned char ch, ch1, ch2, seq[100];
   
   if (nc == 13) fp3 = fopen("./knotTable/13n.pak", "r");
   if (nc == 14) fp3 = fopen("./knotTable/14n.pak", "r");
   if (nc == 15) fp3 = fopen("./knotTable/15n.pak", "r");
   if (nc == 16) fp3 = fopen("./knotTable/16n.pak", "r");

   for (i=0; i<nc; ++i) a0[i] = g0[i] = 0;
   
   ck = offset = 0;

   while ((ch=getc(fp3)) != 255 || feof(fp3) == 0)
   {
      ++ck;
     
      if (ck > id) return 0;
   
      offset = (nc-1) - (ch)/17;
      
      for (i=0; i<offset; ++i) a[i] = a0[i];
   
      if (ck == id)
      {
         for (i=offset; i<nc-1; i+=2)
         {
            ch = getc(fp3);
 
            a[i] = (int)(ch/16 + 1);
            a[i+1] = (int)(ch - 16*(a[i]-1) + 1);
         }

         t=0;
         for (i=0; i<nc-1; ++i) t+=a[i];
         a[nc-1] = (nc*(nc+1))/2 - t;

         ch = getc(fp3);
 
         for (i=0; i<8; ++i)
         {
            ch1 = ch << i;
            ch2 = ch1 >> 7;
            g[i] = (int)ch2;
         }
 
         ch = getc(fp3);
 
         for (i=0; i<8; ++i)
         {
            ch1 = ch << i;
            ch2 = ch1 >> 7;
            g[i+8] = (int)ch2;
         }

         for (i=0; i<nc; ++i) g[i] = 2*g[i] - 1;

         fprintf(gp, "%2d  %7d   ", nc, ck);
         for (i=0; i<nc; ++i) fprintf(gp, "%4d", 2*a[i]*g[i]);
         fprintf(gp, "\n");
 
         for (i=0; i<nc; ++i) a0[i] = a[i];
      }
      
      else
      {
         for (i=offset; i<nc-1; i+=2)
         {
            ch = getc(fp3);
 
            a[i] = (int)(ch/16 + 1);
            a[i+1] = (int)(ch - 16*(a[i]-1) + 1);
         }
 
         ch = getc(fp3); 
         ch = getc(fp3);
 
         for (i=0; i<nc; ++i) a0[i] = a[i];
      }     
   }
   
   return 0;
}

    

    
    
    
    
    
