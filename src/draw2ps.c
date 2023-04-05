#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  double coords1[100][6], coords2[100][10], cross[100][8], mid[200][8];
  int i, j, nc, id, a[100], g[100];

  FILE *fp1, *fp2, *gp;

  fp1 = fopen(argv[1],"r");
  fp2 = fopen(argv[2],"r");
  gp = fopen(argv[3],"w");
  
  fscanf(fp1, "%d%d", &nc, &id);
  for (i=1; i<=2*nc-1; i+=2) fscanf(fp1, "%d", a+i);
  
  for (i=1; i<=2*nc-1; i+=2) g[i] = a[i] / abs(a[i]);
  for (i=1; i<=2*nc-1; i+=2) a[i] = abs(a[i]);
  for (i=1; i<=2*nc-1; i+=2) {a[a[i]] = i; g[a[i]] = -g[i];}  
  
  
  for (i=0; i<2*nc; ++i)
    for (j=0; j<6; ++j) fscanf(fp2, "%lf", coords1[i]+j);

  for (i=0; i<2*nc; ++i)for (i=0; i<2*nc; ++i)
    for (j=0; j<10; ++j) fscanf(fp2, "%lf", coords2[i]+j);
    
  
  for (i=0; i<2*nc; ++i)
  {
    cross[i][0] = coords1[i][0];
    cross[i][1] = coords1[i][1];
    
    cross[i][2] = .3000*coords1[i][0] + .7000*coords1[i][2];
    cross[i][3] = .3000*coords1[i][1] + .7000*coords1[i][3];
    
    cross[i][4] = .7000*coords1[i][2] + .3000*coords1[i][4];
    cross[i][5] = .7000*coords1[i][3] + .3000*coords1[i][5];
    
    cross[i][6] = coords1[i][4];
    cross[i][7] = coords1[i][5];
  }
  
  for (i=0; i<2*nc; ++i)
  {
    mid[i][0] = coords2[i][0];
    mid[i][1] = coords2[i][1];
    
    mid[i][2] = .3000*coords2[i][0] + .7000*coords2[i][2];
    mid[i][3] = .3000*coords2[i][1] + .7000*coords2[i][3];
    
    mid[i][4] = .7000*coords2[i][2] + .3000*coords1[i][0];
    mid[i][5] = .7000*coords2[i][3] + .3000*coords1[i][1];
    
    mid[i][6] = coords1[i][0];
    mid[i][7] = coords1[i][1];
  }
  
 
  fprintf(gp, "%%!PS-Adobe-3.0 EPSF-3.0\n");
  fprintf(gp, "%%%%BoundingBox: 0 0 400 400\n");
  fprintf(gp, "%%%%Pages: 1\n");
  fprintf(gp, "%%%%DocumentData: Clean7Bit\n");
  fprintf(gp, "%%%%Orientation: Portrait\n");
  fprintf(gp, "%%%%EndComments\n");
  fprintf(gp, "%%%%Page: 1 1\n\n");
/*  fprintf(gp, "%%<</PageSize [360 360]>> setpagedevice\n");*/
  
  /*
   *  First draw underpasses
   */
   
  fprintf(gp, "gsave\n");
  fprintf(gp, "7 8 translate\n");
  fprintf(gp, ".6 .6 scale\n");
  fprintf(gp, "5 setlinewidth 0 0 1 setrgbcolor\n");
 
  for (i=0; i<2*nc; ++i) if (g[i+1] == -1)
  {
     fprintf(gp, "%9.3lf  %9.3lf  moveto\n", cross[i][0], cross[i][1]);
     for (j=2; j<8; ++j) fprintf(gp, "%9.3lf  ", cross[i][j]);
     fprintf(gp, "curveto\n");
  }
  
  fprintf(gp, "stroke\n");
  
  
  /*
   *  Next make gaps by drawing overpasses thickly in white
   */
 
  fprintf(gp, "35 setlinewidth 1 setgray\n");

  for (i=0; i<2*nc; ++i) if (g[i+1] == 1)
  {
     fprintf(gp, "%9.3lf  %9.3lf  moveto\n", cross[i][0], cross[i][1]);
     for (j=2; j<8; ++j) fprintf(gp, "%9.3lf  ", cross[i][j]);
     fprintf(gp, "curveto\n");
  }
  
  fprintf(gp, "stroke\n");
  
  /*
   *  Now draw overpasses normally, followed by mid-sections
   */
   
  fprintf(gp, "5 setlinewidth 0 0 1 setrgbcolor\n");
 
  for (i=0; i<2*nc; ++i) if (g[i+1] == 1)
  {
     fprintf(gp, "%9.3lf  %9.3lf  moveto\n", cross[i][0], cross[i][1]);
     for (j=2; j<8; ++j) fprintf(gp, "%9.3lf  ", cross[i][j]);
     fprintf(gp, "curveto\n");
  }
  
  for (i=0; i<2*nc; ++i)
  {
     fprintf(gp, "%9.3lf  %9.3lf  moveto\n", mid[i][0], mid[i][1]);
     for (j=2; j<8; ++j) fprintf(gp, "%9.3lf  ", mid[i][j]);
     fprintf(gp, "curveto\n");
  }
 
  fprintf(gp, "stroke\n");
  fprintf(gp, "grestore\n");
/*  
  fprintf(gp, "0 setgray\n");
  
  fprintf(gp, "/Times-Bold findfont 20 scalefont setfont\n");
  fprintf(gp, "10 10 moveto\n");
  fprintf(gp, "(%d . ) show  (%d) show\n", nc, id);

  for (i=1; i<=0; i+=2) fprintf(gp, "(%4d) show  10 0 rmoveto\n", a[i]*g[i]);
  fprintf(gp, "25 0 rmoveto\n");
  fprintf(gp, "(R: 3_1) show\n");
  fprintf(gp, "25 0 rmoveto\n");
  fprintf(gp, "(D1) show\n");
  fprintf(gp, "25 0 rmoveto\n");
  fprintf(gp, "(Trefoil) show\n");
*/  
  fprintf(gp, "showpage\n");
  fprintf(gp, "%%%%EOF\n");
  
  return 0;
 
} 
    
    
    
    
    
    
    
    
    
    
    
    
    
