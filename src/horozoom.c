/*
 *   horozoom.c
 *   computes circles for display in horoball zoom window
 */

#include <stdio.h>

FILE *fp1, *fp2, *gp;

main(argc, argv)
int argc;
char *argv[];

{
  double u1, v1, u2, v2, mouse_x, mouse_y, coords[100001][5],
    x0, y0, x1, y1, x2, y2, x3, y3, radius, c1, c2;
  int numHoroballs, i, j, k, n,
    int_c1, int_c2, int_u1, int_u2, int_v1, int_v2, int_radius;
  
  fp1 = fopen(argv[1], "r");
  fp2 = fopen(argv[2], "r");
  gp = fopen(argv[3], "w");
  
  fscanf(fp1, "%10lf%10lf", &mouse_x, &mouse_y);
    
  fscanf(fp2, "%4d%10d", &i, &j);
  
  fscanf(fp2, "%10lf%10lf", &x0, &y0);
  fscanf(fp2, "%10lf%10lf", &x1, &y1);
  fscanf(fp2, "%10lf%10lf", &x2, &y2);
  fscanf(fp2, "%10lf%10lf", &x3, &y3);
  
  n = 1;
  
  while (fscanf(fp2, "%10lf", coords[n]+1) != EOF)
  {
    fscanf(fp2, "%10lf%10lf%10lf", coords[n]+2,  coords[n]+3, coords[n]+4);
    ++n;
  }
  
  numHoroballs = n-1;
  
  x0 = 5*(x0 - mouse_x) + 400;      /* vertices of fundamental region */
  y0 = 5*(y0 - mouse_y) + 400;
  x1 = 5*(x1 - mouse_x) + 400;
  y1 = 5*(y1 - mouse_y) + 400;
  x2 = 5*(x2 - mouse_x) + 400;
  y2 = 5*(y2 - mouse_y) + 400;
  x3 = 5*(x3 - mouse_x) + 400;
  y3 = 5*(y3 - mouse_y) + 400;

  fprintf(gp, "% 9.4f % 9.4f\n", x0, y0);  /* output vertices */
  fprintf(gp, "% 9.4f % 9.4f\n", x1, y1);
  fprintf(gp, "% 9.4f % 9.4f\n", x2, y2);
  fprintf(gp, "% 9.4f % 9.4f\n", x3, y3);
  
  for (i=1; i<=numHoroballs; ++i)  /* output circles in range */
  {
    u1 = 5*(coords[i][1] - mouse_x) + 400;
    v1 = 5*(coords[i][2] - mouse_y) + 400;
    u2 = 5*(coords[i][3] - mouse_x) + 400;
    v2 = 5*(coords[i][4] - mouse_y) + 400;
    
    c1 = (u1 + u2)/2;   /* coordinates of center */
    c2 = (v1 + v2)/2;
    
    radius = (u2 - u1)/2;
    
    u1 = c1 - radius;
    v1 = c2 - radius;
    u2 = c1 + radius;
    v2 = c2 + radius;
    
    
    if (c1 > -radius && c1 < 800+radius &&
        c2 > -radius && c2 < 800+radius)
    {
      fprintf(gp, "% 9.4f   ", u1);
      fprintf(gp, "% 9.4f   ", v1);
      fprintf(gp, "% 9.4f   ", u2);
      fprintf(gp, "% 9.4f   ", v2);
      fprintf(gp, "\n");
    }
    
  }
  
  return 0;
}
    
    
    
    
    
    
    
    
    
    
    
