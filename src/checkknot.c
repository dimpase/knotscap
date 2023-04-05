#include <stdio.h>
#include <string.h>
#include <stdlib.h>


FILE *fp, *gp, *tp;
 
int  a[100], a0[100], f[100], nc, n, np, knotid, i, j, k, comp, len, knit,
     line_number;
char *str, c;


int main(argc, argv)
int argc;
char *argv[];

{

  fp = fopen(argv[1], "r");
  gp = fopen(argv[2], "w");

  str = (char *) malloc(500*sizeof(char));

  if (fgetc(fp) == EOF)   /* no data */
  {
    fprintf(gp, "No input data.\n");
    exit(0);
  }
  
  if (fgetc(fp) == EOF)   /* no data */
  {
    fprintf(gp, "No input data.\n");
    exit(0);
  }

  fclose(fp);

  fp = fopen(argv[1], "r");

  line_number = 0;

  while (fgets(str, 500, fp) != NULL)
  {

    ++line_number;
 
    for (i=0; i<=strlen(str)-1; ++i)	   /* check for illegal characters */
    {
      c=str[i];
      if ((c < '0' || c > '9') && c != '-' && c != ' ' && c != '\n' && c != '\t')
      {
  	fprintf(gp, "Data on line %2d contains extraneous character.\n", line_number);
  	exit(0);
      }
    }
 
    for (i=1; i<=strlen(str)-1; ++i) if (str[i] == '-') /* check for non-parsability */
    {
      if ((str[i-1] != ' ' && str[i-1] != '\t') || str[i+1] < '0' || str[i+1] > '9')
      {
  	fprintf(gp, "Data on line %2d contains spurious instance of \"-\".\n", line_number);
  	exit(0);
      }
    }
 
    len=0;
    for (i=0; i<=strlen(str)-1; ++i)	 /* check for integers which are too large */
    {
      c=str[i];
      if (c >= '0' && c <= '9') ++len;
      else len=0;
 
      if (len >= 10)
      {
  	fprintf(gp, "Input data on line %2d contains integer out of range.\n", line_number);
  	exit(0);
      }
    }
 
    len=0;
    
    tp = fopen(argv[3], "w");
    
    fprintf(tp, "%s", str);
    
    fclose(tp);
    
    tp = fopen(argv[3], "r");    
 
    while(fscanf(tp, "%d", a0 + (++len)) != EOF);
    
    fclose(tp);     
    
    --len;
 
    if (len == 0) continue;    /*  we allow empty lines, as long as some line is non-empty */

    if (a0[1] != len-2)   /* check for consistent crossing-number */
    {
      fprintf(gp, "Line %2d: stated crossing-number is not consistent with data.\n", line_number);
      exit(0);
    }

    if (a0[1] <= 0 || a0[2] <= 0)  /* check for positive crossing-number, knot ID */
    {
      fprintf(gp, "Line %2d: crossing-number and knot ID must both be positive integers.\n", line_number);
      exit(0);
    }

    for (i=1; i<=len; ++i) if (a0[i] == 0)   /* check for zero entry */
    {
      fprintf(gp, "Line %2d: data contains zero entry.\n", line_number);
      exit(0);
    }

    if (a0[1] > 49)    /* crossing-number too large */
    {
      fprintf(gp, "Line %2d: crossing-number is too large; limit is 49.\n", line_number);
      exit(0);
    }

    for (i=1; i<=len; ++i) a0[i] = abs(a0[i]); /* no longer care about signs */

    for (i=3; i<=len; ++i) if (a0[i] > 2*a0[1])   /* Dowker code inconsistent */
    {
      fprintf(gp, "Line %2d: Dowker code contains entry larger than twice crossing-number.\n", line_number);
      exit(0);
    }

    for (i=3; i<=len; ++i) if (a0[i]%2 == 1)   /* Dowker code inconsistent */
    {
      fprintf(gp, "Line %2d: proposed Dowker code contains odd integer.\n", line_number);
      exit(0);
    }

    nc = a0[1];
    n = 2*nc - 1;
    np = 2*nc;

    for (i=1; i<=np; ++i) a[i] = 0;

    for (i=1; i<=nc; ++i) a[2*i-1] = a0[i+2];
    for (i=1; i<=n; i+=2) a[a[i]] = i;

    for (i=1; i<=np; ++i) if (a[i] == 0)   /* inconsistent Dowker code */
    {
      fprintf(gp, "Line %2d: proposed Dowker code has repeated even integer, up to sign.\n", line_number);
      exit(0);
    }

    for(i=1; i<=n-1; i++) for (j=i+1; j<=n; j++) /* check for primality */
    {
      comp=1;
      for (k=i; k<=j; k++) if ((a[k]<i) || (a[k]>j)) comp=0;
      if (comp==1)
      {
  	fprintf(gp, "Line %2d: proposed Dowker code does not represent a prime diagram.\n", line_number);
  	exit(0);
      }
    }

    orient();	  /* check realizability */

    if (knit != 0)
    {
      fprintf(gp, "Line %2d: proposed Dowker code is not realizable as a knot diagram.\n", line_number);
      exit(0);
    }
  }

  fprintf(gp, "OK\n");
  exit(0);

}


void orient()
{
  int e[100], g[100], h[100], k1, k2;
  register int i, s, t;
  knit=0;
  for (i=1; i<=np; i+=1)
  { 
    e[i]=0;f[i]=0;g[i]=0;h[i]=0;
  }
  f[1]=1; f[a[1]]=-1;
  h[1]=1; h[a[1]]=1;
  e[1]=1;
  t=1;
  while (t!=0)
  {
    e[t]=1;
    for (i=t+1; i<=np; i+=1)
      if ((a[i]>=t) && (a[i]<=a[t])) 
        e[i]=-e[i-1];
      else 
        e[i]=e[i-1];
    for (i=t-1; i>=1; i-=1)
      if ((a[i+1]>=t) && (a[i+1]<=a[t]))
        e[i]=-e[i+1];
      else
        e[i]=e[i+1];
    for (i=1; i<=t-1; i+=1) g[i]=1;
    for (i=a[t]+1; i<=np; i+=1) g[i]=1;
    s=0;i=1;
    while ((s==0) && (i<=np))
    {
      if (g[i]==1) s=i;
      i=i+1;
    }
    while (s!=0)
    {
      if ((a[s]<t) || (a[s]>a[t]))
      {
        if (e[s]*e[a[s]]==1)
        {
          g[s]=0;
          g[a[s]]=0;
        }
        if (e[s]*e[a[s]]==-1)
        {
          knit = 1;
          return;
        }
      }
      else
      {
        if (f[s]!=0)
        {
          if (e[s]*e[a[s]]*f[t]==f[s])
          {
            g[s]=0;
            g[a[s]]=0;
          }
          else
          {
            knit = 1;
            return;
          }
        }
        else
        {
          f[s]=e[s]*e[a[s]]*f[t];
          f[a[s]]=-f[s];
          if (((s==1) && (abs(a[np]-a[1])==1)) ||
            ((s!=1) && 
              ((abs(a[s-1]-a[s])==1) || (abs(a[s-1]-a[s])==n))))
          {
            g[s]=0;
            g[a[s]]=0;
          }
          else
          {
            g[s]=0;
            g[a[s]]=0;
            h[s]=1;
            h[a[s]]=1;
          }
        }
      }
      s=0;
      i=1;
      while ((s==0) && (i<=np))
      {
        if (g[i]==1) s=i;
        i=i+1;
      }
    }
    h[t]=0;
    h[a[t]]=0;
    t=0;
    i=1;
    while ((t==0) && (i<=np))
    {
      if (h[i]==1) t=i;
      i=i+1;
    }
  }
}

