#include <stdio.h>
#include <math.h>
#include <draw.h>

nodeseq(file1, file3, file4, file5)
char *file1, *file3, *file4, *file5;

{

FILE *fp1, *fp2, *gp1, *gp2;

float coords[500][4], coords1[500][3];

int knotseq[500], wv[500], a[100], vertex[100], n, knotid;

int node, i, j, nv;
float minx, maxx, miny, maxy, scx, scy, trx, try, xt, yt;


fp1 = fopen(file1, "r");
fp2 = fopen(file3, "r");
gp1 = fopen(file4, "w");
gp2 = fopen(file5, "w");

node=1;

fscanf(fp1, "%3d%8d", &nc, &knotid);

n=2*nc-1; nv = 4*nc;

for (i=1; i<=n; i+=2) fscanf(fp1, "%4d", a+i);
for (i=1; i<=n; i+=2) a[i] = abs(a[i]);

for (i=1; i<=n; i+=2) a[a[i]]=i;


for (i=1; i<=n; i+=2) vertex[i]=vertex[a[i]]=(i+1)/2;

for (i=1; i<=2*nc; ++i)
{
  knotseq[2*i-1] = vertex[i];
  knotseq[2*i] = nc + i;
}

for (i=1; i<=nv; ++i) wv[i]=0;
for (i=1; i<=nv; ++i) wv[knotseq[i]] = 1;

minx=maxx=miny=maxy=0;

while (fscanf(fp2, "%f%f", coords[node]+1, coords[node]+2)
    != EOF)
{
  if (coords[node][1] < minx && wv[node] == 1) minx = coords[node][1];
  if (coords[node][1] > maxx && wv[node] == 1) maxx = coords[node][1];  
  if (coords[node][2] < miny && wv[node] == 1) miny = coords[node][2];  
  if (coords[node][2] > maxy && wv[node] == 1) maxy = coords[node][2];  
  ++node;
}

fclose(fp1);
fclose(fp2);

--node;

/* scale so that picture fits nicely in canvas */

scx = 600/(maxx - minx);
trx = -300 - scx*minx;
scy = 600/(maxy - miny);
try = -300 - scy*miny;

knotseq[nv+1] = knotseq[1];
knotseq[nv+2] = knotseq[2];
knotseq[0] = knotseq[nv];

/* scale */

if (scx < scy)
  for (i=1; i<=node; ++i)
  {
    xt = coords[i][1]*scx + trx + 320;
    yt = coords[i][2]*scx + (scx/scy)*try + 320;
    coords[i][1] = xt;
    coords[i][2] = yt;
  }

else
  for (i=1; i<=node; ++i)
  {
    xt = coords[i][1]*scy + (scy/scx)*trx + 320;
    yt = coords[i][2]*scy + try + 320;
    coords[i][1] = xt;
    coords[i][2] = yt;
  }

/* get coordinates of midpoints */

for (i=1; i<=nv+1; ++i) for (j=1; j<=2; ++j)
  coords1[i][j] = 0.5*(coords[knotseq[i-1]][j] + coords[knotseq[i]][j]);
  
for (j=1; j<=2; ++j)
  coords1[0][j] = 0.5*(coords[knotseq[nv-1]][j] + coords[knotseq[0]][j]);
  
/* store polygon coords in order in drawcoords */

for (i=1; i<=nv; ++i) for (j=1; j<=2; ++j) drawcoords[i][j]=coords[knotseq[i]][j];
for (j=1; j<=2; ++j) drawcoords[0][j]=drawcoords[nv][j];
  
/* output centers, for drawing polygon */
  
for (i=1; i<=nv; ++i)
{
  for (j=1; j<=2; ++j) fprintf(gp1, "%8.3f  ", drawcoords[i][j]);
  fprintf(gp1, "\n");
}

/* output coordinates for segments */

for (i=1; i<=nv-1; i+=2)
{
  for (j=1; j<=2; ++j) fprintf(gp2, "%8.3f  ", coords1[i][j]);  
  for (j=1; j<=2; ++j) fprintf(gp2, "%8.3f  ", coords[knotseq[i]][j]);
  for (j=1; j<=2; ++j) fprintf(gp2, "%8.3f  ", coords1[i+1][j]);
  fprintf(gp2, "\n");
}

for (i=1; i<=nv-1; i+=2)
{
  for (j=1; j<=2; ++j) fprintf(gp2, "%8.3f  ", coords1[i-1][j]);  
  for (j=1; j<=2; ++j) fprintf(gp2, "%8.3f  ", coords[knotseq[i-1]][j]);
  for (j=1; j<=2; ++j) fprintf(gp2, "%8.3f  ", coords[knotseq[i]][j]); 
  for (j=1; j<=2; ++j) fprintf(gp2, "%8.3f  ", coords[knotseq[i+1]][j]);    
  for (j=1; j<=2; ++j) fprintf(gp2, "%8.3f  ", coords1[i+2][j]);
  fprintf(gp2, "\n");
}

fclose(gp1);
fclose(gp2);

return 0;
}
