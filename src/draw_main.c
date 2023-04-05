#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <draw.h>
#include <math.h>

int nc, nr, infinitereg, ccr[100][50];
float drawcoords[500][4];
double badness();

main(argc, argv)
int argc;
char *argv[];

{

int reg, bestreg;
double bd, bestbd;

bestreg = 0;

triang(argv[1], argv[2]);
ken(argv[2], argv[3]);
nodeseq(argv[1], argv[3], argv[4], argv[5]);

/* check first attempt for badness */

bd = (double) badness();

if (bd < 1000)   /* search for best infinite region, if cramped */
{
  int count;
  
  bestbd = 0;
  count=1;
  for (reg=1; reg<=nr; ++reg) if (ccr[reg][0] >= 5)
  //for (reg=1; reg<=nr; ++reg) if (ccr[reg][0] == 3 && count++ == 4)
  {
    infinitereg = reg;
    
    triang(argv[1], argv[2]);
    ken(argv[2], argv[3]);
    nodeseq(argv[1], argv[3], argv[4], argv[5]);
    
    bd = (double) badness();
    if (bd > bestbd)
    {
      bestbd = bd;
      bestreg = infinitereg;
    }
  }
  
  infinitereg = bestreg;
  triang(argv[1], argv[2]);
  ken(argv[2], argv[3]);
  nodeseq(argv[1], argv[3], argv[4], argv[5]);
}

exit(0);
}

double badness()
{
double x1, x2, y1, y2, z, w;
double worst;
int i;

worst = 100;

for (i=1; i<=4*nc; ++i)
{
  x1 = (double) drawcoords[i-1][1];
  x2 = (double) drawcoords[i][1];
  y1 = (double) drawcoords[i-1][2];
  y2 = (double) drawcoords[i][2];
  w = (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);
  z = sqrt(w);
  if (z < worst) worst = z;
}

return worst;
}















  



