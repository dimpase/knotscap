#include <stdio.h>
#include <math.h>

FILE *fp, *gp;

int a[100], b[100], b1[100][3],
    c[100][100], c2[100][100], c3[100][100], c4[100][100], 
    cm[50][50], cop[50][50], dop[50][50], er[50][50], 
    f[100], g[100], kf[100][100], md[200], opp[100][100], ord[100][100],
    pt[100], rm[100][100], xbe[50][3], xeb[50][3],
    color[50], brn[50], vbr[50][3], mat[50][50], ksgn[50];
    
int nbr, l1, l2, nrow, ncol, r1, r2, determinant, signature;
    
int *q, *eb[50], *be[50];

int dd, dd1, n, r, nr, cc1, cc2, cc3, k1, row, ppn, knotid, dir,
    i, j, k, l, m, p, t, nc, np, knit;
    
double d[100];


int main(int argc, char **argv)
{

  fp=fopen(argv[1],"r");
  gp=fopen(argv[2],"w");

  
  while (fscanf(fp, "%d%d", &nc, &knotid) != EOF)
  {
    n = 2*nc - 1;
    np = 2*nc;
    nr = nc + 2;
    
    q=md+np;  /* q maps x to x mod np, with 0 replaced by np */
    for(i=-np; i<=3*np; i++) *(q+i)=mod0(i);
    
    for (i=1; i<=np; i++)
    {
      eb[i] = xeb[i]+1;
      be[i] = xbe[i]+1;
    }

    for (i=1; i<=np; i++)
    {
      be[i][1]=i;
      be[i][-1]=q[i-1];
      eb[i][1]=q[i+1];
      eb[i][-1]=i;
    }

    for (i=1; i<=np-1; i++) cm[i][i+1]=cm[i+1][i]=i+1;
    cm[np][1]=cm[1][np]=1;

    
    
    for (i=1; i<=n; i+=2) fscanf(fp, "%d", a+i);
    
    for (i=1; i<=n; i+=2)
    {
      g[i] = a[i]/abs(a[i]);
      a[i] = abs(a[i]);
    }
    for (i=1; i<=n; i+=2)
    {
      a[a[i]] = i;
      g[a[i]] = -g[i];
    }

/* assemble data for goeritz matrix */

    incidence();

    for (i=1; i<=n; i+=2)
    {
      int v, w1, w2;
 
      v = eb[i][1];
      w1 = be[a[v]][f[v]];
      w2 = be[a[v]][-f[v]];
      r1 = er[i][w1];
      r2 = er[i][w2];
      color[r1] = 1;
      color[r2] = -1;
    }
 
    for (i=2; i<=n+1; i+=2)
    {
      int v, w1, w2;
 
      v = eb[i][1];
      w1 = be[a[v]][f[v]];
      w2 = be[a[v]][-f[v]];
      r1 = er[i][w1];
      r2 = er[i][w2];
      color[r1] = -1;
      color[r2] = 1;
    }
    
/* number black regions */

    j=1;
    for (i=1; i<=nr; ++i) if (color[i]==1) brn[i]=j++; else brn[i]=0;
    nbr = j-1;
  
/* switch colors if necessary */

    if (nbr > nr/2)
    {
      for (i=1; i<=nr; ++i) color[i]*=-1;
      j=1;
      for (i=1; i<=nr; ++i) if (color[i]==1) brn[i]=j++; else brn[i]=0;
      nbr = j-1;
    }
  
/* find black regions incident to each crossing */

    for (i=1; i<=n; i+=2) if (color[c3[i][1]]==1)
    {
      vbr[i][1] = c3[i][1];
      vbr[i][2] = c3[i][3];
    }
    else
    {
      vbr[i][1] = c3[i][2];
      vbr[i][2] = c3[i][4];
    }
  
/* record Kauffman sign of each crossing */

    for (i=1; i<=n; i+=2) ksgn[i] = kf[vbr[i][1]][i];
 
    for (i=1; i<=nbr; ++i) for (j=1; j<=nbr; ++j) mat[i][j]=0;

 
    for (i=1; i<=0; ++i)
    {
      for (j=1; j<=nbr; ++j) printf("%3d", mat[i][j]);
      printf("\n");
    }
 
    for (i=1; i<=nbr; ++i) for(j=1; j<=nbr; ++j) mat[i][j]=0;
  
    for (i=1; i<=n; i+=2)
    {
      int r1, r2;
    
      r1 = brn[vbr[i][1]];
      r2 = brn[vbr[i][2]];
    
      mat[r1][r1] += ksgn[i];
      mat[r2][r2] += ksgn[i];
      mat[r1][r2] -= ksgn[i];
      mat[r2][r1] -= ksgn[i];
    } 
 
    for (i=1; i<=0; ++i)
    {
      for (j=1; j<=nbr; ++j) printf("%3d", mat[i][j]);
      printf("\n");
    }  

   
    jacobi();

    signature = 0;
    
    for (i=1; i<=nbr-1; ++i)
    {
      if (d[i] > 0) ++signature;
      else --signature;
    }
        
    for (i=1; i<=n; i+=2) if (f[i]*g[i] != ksgn[i]) signature -= ksgn[i];
    signature = abs(signature);
    
    determinant = det();

    fprintf(gp, "%3d %8d   ", nc, knotid);
    for (i=1; i<=n; i+=2) fprintf(gp, "%4d", a[i]*g[i]);
    fprintf(gp, "     %5d%5d\n", determinant, signature);
    
  }
  return 0;
}


jacobi()
{
  double sm, c, s, t, h, g, tau, theta, tresh, z[100], b[100], a[100][100], v[100][100];
  int p, q, i, j, n, rot;
  
  n = nbr-1;
  
  for (i=1; i<=nbr-1; ++i) for (j=1; j<=nbr-1; ++j) a[i][j] = (double) mat[i][j];
  
for (i=1; i<=0; ++i)
{
  for (j=1; j<=n; ++j) printf("% 10.5lf", a[i][j]);
  printf("\n");
}
  
  for (p=1; p<=n; ++p) for (q=1; q<=n; ++q)
  {
    if (p == q) v[p][q] = 1.0;
    else v[p][q] = 0.0;
  }
  
  for (p=1; p<=n; ++p)
  {
    b[p] = d[p] = a[p][p];
    z[p] = 0;
  }
  
  rot = 0;
  
  for (i=1; i<=50; ++i)
  {
    sm = 0.0;
    
    for (p=1; p<=n-1; ++p) for (q=p+1; q<=n; ++q)
      sm += fabs(a[p][q]);
    
    if (fabs(sm) == 0) return 0;
    
    if (i < 4) tresh = 0.2 * (sm / (n*n));
    else tresh = 0.0;
    
    for (p=1; p<=n-1; ++p) for (q=p+1; q<=n; ++q)
    {
      g = 100 * fabs(a[p][q]);
      
      if (i>4 && fabs(d[p])+g == fabs(d[p]) && fabs(d[q])+g == fabs(d[q]))
        a[p][q] = 0.0;
      else if (fabs(a[p][q]) > tresh)
      {
        h = d[q] - d[p];
	if (fabs(h) + g == fabs(h))
	  t = a[p][q] / h;
	else
	{
	  theta = 0.5 * h / a[p][q];
	  t = 1 / (fabs(theta) + sqrt(1 + theta*theta));
	  if (theta < 0) t *=-1;
	}
	
	c = 1 / sqrt(1 + t*t);
	s = t * c;
	tau = s / (1 + c);
	h = t * a[p][q];
	
	z[p] = z[p] - h;
	z[q] = z[q] + h;
	d[p] = d[p] - h;
	d[q] = d[q] + h;
	
	a[p][q] = 0;
	
	for (j=1; j<=p-1; ++j)
	{
	  g = a[j][p];
	  h = a[j][q];
	  a[j][p] = g - s * (h + g * tau);
	  a[j][q] = h + s * (g - h * tau);
	}
	
	for (j=p+1; j<=q-1; ++j)
	{
	  g = a[p][j];
	  h = a[j][q];
	  a[p][j] = g - s * (h + g * tau);
	  a[j][q] = h + s * (g - h * tau);
	}
        
	for (j=q+1; j<=n; ++j)
	{
	  g = a[p][j];
	  h = a[q][j];
	  a[p][j] = g - s * (h + g * tau);
	  a[q][j] = h + s * (g - h * tau);
	}
	
	for (j=1; j<=n; ++j)
	{
	  g = v[j][p];
	  h = v[j][q];
	  v[j][p] = g - s * (h + g * tau);
	  v[j][q] = h + s * (g - h * tau);
        }
	
	++rot;
      }
    }
    
    for (p=1; p<=n; ++p)
    {
      d[p] = b[p] = b[p] + z[p];
      z[p] = 0;
    }
  }
  
  return 0;
}



int det()
{

  int tor[50], ntor, i, j, k, t;

/* now reduce matrix to Smith normal form */
      
/* l1 = current row; l2 = current column */

  l1=1;
  l2=1;
  
  nrow=ncol=nbr-1;

/* tor[i] : invariant factors of group */

  for (i=1; i<=40; i++) tor[i]=-1;
  ntor=0;

  while (l1 <= nrow  &&  l2 <= ncol)
  {

    if (l1 == nrow  &&  zr(l1) == 1) 

/* all generators to right are free; reduction is terminated */

    {
      for (j=1; j<=ncol-l2+1; j++) tor[++ntor]=0;
      l1++;
      continue;
    }
    
    if (abs(mat[l1][l1]) != 1)
    {
      int t1, t2;
      
      t1=1000000000; t2=l1;
      for (j=l1; j<=nrow; ++j)
      {
        t=abs(mat[j][l1]);
        if (t < t1  &&  t > 0)
        {
          t1=abs(mat[j][l1]);
          t2=j;
        }
      }
      swapr(l1,t2);
    }
    
    if (mat[l1][l1] == 0)
    {
      int t2;
      
      t2=l1+1;
      while (mat[t2][l1] == 0) ++t2;
      swapr(l1,t2);
    }    


/* repeatedly use euclidean() to get all entries off diagonal
   in row l1 and column l2 zero */


   for (i=l1+1; i<=nrow; i++) if (mat[i][l2] != 0) euclidean(i, 1);
   

/* if diagonal entry has absolute value not equal to 1, save as
   invariant factor of group; if at last row, the zeros to right
   indicate free generators. */


    if (abs(mat[l1][l2]) != 1) tor[++ntor]=abs(mat[l1][l2]);

    if (l1 == nrow) for (i=l2+1; i<=ncol; i++) tor[++ntor]=0;

    l1++;
    l2++;
  }
  
  t=1;
  for (i=1; i<=ntor; ++i) t*=tor[i];
  return t;

}


gc(x, y, d, a, b)
register int x, y, *d, *a, *b;
{
  int c[100], i, p, q, r, a1, b1, t;

  p=abs(x);
  q=abs(y);
  t=0;
  while (q != 0)
  {
    c[++t]=p/q;
    r=p-c[t]*q;
    p=q;
    q=r;
  }

  *d=p;
  *a=1;
  *b=-c[t-1];

  for (i=t-1; i>=2; i--)
  {
    a1=*b;
    b1=(*a) - c[i-1]*(*b);
    *a=a1;
    *b=b1; 
  }

  (*a)*=sign(x);
  (*b)*=sign(y);
}


swapr(i,j)
register int i, j;
{
  int k, l;

  for (k=1; k<=ncol; k++)
  {
    l=mat[i][k];
    mat[i][k]=mat[j][k];
    mat[j][k]=l;
  }
}


swapc(i,j)
register int i, j;
{
  int k, l;

  for (k=1; k<=nrow; k++)
  {
    l=mat[k][i];
    mat[k][i]=mat[k][j];
    mat[k][j]=l;
  }
}



euclidean(pos,t)
register int pos, t;
{
  int a, b, d, k, p, q, r, s, u, v;

  r=mat[l1][l2];
  if (t == 1) s=mat[pos][l2]; else s=mat[l1][pos];

  if (abs(r) == 1)
  {
    a=r;
    b=0;
    p=-s*r;
    q=1;
  }
  else if (s % abs(r) == 0) 
  {
    a=1;
    b=0;
    p=-(s/r);
    q=1;
  }
  else if (r % abs(s) == 0)
  {
    a=0;
    b=1;
    p=1;
    q=-(r/s);
  }
  else
  {
    gc(r, s, &d, &a, &b);
    p=s/d;
    q=-r/d;

  }

  if (t == 1)
  {
    for (k=l2; k<=ncol; k++) 
      if (mat[l1][k] != 0  ||  mat[pos][k] != 0)
    {
      u=mat[l1][k];
      v=mat[pos][k];
      mat[l1][k]=a*u+b*v;
      mat[pos][k]=p*u+q*v;
    }
  }

  if (t == 2)
  {
    for (k=l1; k<=nrow; k++) 
      if (mat[k][l2] != 0  ||  mat[k][pos] != 0)
    {
      u=mat[k][l2];
      v=mat[k][pos];
      mat[k][l2]=a*u+b*v;
      mat[k][pos]=p*u+q*v;
    }
  }
}

sign(x)
register int x;
{
  if (x > 0) return 1;
  if (x < 0) return -1;
  return 0;
}

matprint()
{
  int i, j;

  printf("l1=%3d  l2=%3d\n", l1, l2);

  if (l1 < 5) for (i=l1; i<=nrow; i++)
  {
    for (j=l2; j<=ncol; j++) printf("%4d", mat[i][j]);
    printf("\n");
  }

  else for (i=l1; i<=nrow; i++)
  {
    for (j=l2; j<=ncol; j++) printf("%12d", mat[i][j]);
    printf("\n");
  }
}


zc(v)
register int v;

{
  int i;

  for (i=l1; i<=nrow; i++) if (mat[i][v] != 0) return 0;
  return 1;
}


zr(u)
register int u;

{
  int i;

  for (i=l2; i<=ncol; i++) if (mat[u][i] != 0) return 0;
  return 1;
}


eudone()

{
  register int i;

  for (i=l1+1; i<=nrow; i++) if (mat[i][l2] != 0) 
    return 0;
  for (i=l2+1; i<=ncol; i++) if (mat[l1][i] != 0) 
    return 0;
  return 1;
}




int incidence()
{
  register int i,j;
  int reg1, reg2, reg3, reg4;

  orient();
  nr=(n+5)/2;

  for (i=1; i<=np; i++) for (j=1; j<=np; j++)
  { 
    c3[i][j]=0;
    rm[i][j]=0;
    dop[i][j]=0;
    c2[i][j]=0;
    kf[i][j]=0;
    opp[i][j]=0;
    c4[i][j]=0;
    cop[i][j]=0;
    er[i][j]=0;
  }

  for (i=1; i<=np; i++) for (j=1; j<=2; j++) b1[i][j]=0;

/* next, compute incidence matrix c2 of regions to edges;
 c2[i,j] tells whether ith region is incident to jth edge.
 The matrix b1 is a tally of which edges have been
 accounted for. */

  r=0;
  for (i=1; i<=np; i++) if (b1[i][1]==0)
  {
    k=i;
    r=r+1;
    c2[r][i]=1;
    b1[i][1]=1;
    dd=1;
    while (a[q[k+((dd+1)/2)]]!=i)
    {
      if (dd==1) dd1=f[q[k+1]]; else dd1=-f[k];
      if (dd==1)
        k=q[a[q[k+1]]+((dd1-1)/2)];
      else
        k=q[a[k]+((dd1-1)/2)];
      dd=dd1;
      c2[r][k]=1;
      if (dd==1) b1[k][1]=1; else b1[k][2]=1;
    }
  }
  for (i=1; i<=np; i++) if (b1[i][2]==0)
  {
    k=i;
    dd=-1;
    r=r+1;
    c2[r][i]=1;
    b1[i][2]=1;
    while (a[q[k+((dd+1)/2)]]!=q[i+1])
    {
      if (dd==1) dd1=f[q[k+1]]; else dd1=-f[k];
      if (dd==1)
        k=q[a[q[k+1]]+((dd1-1)/2)];
      else 
        k=q[a[k]+((dd1-1)/2)];
      dd=dd1;
      c2[r][k]=1;
      if (dd==1) b1[k][1]=1; else b1[k][2]=1;
    }
  }

  /* compute matrix c3, giving the four regions around each
     crossing, clockwise */

  for (i=1; i<=nr; i++)
  {
    for (j=1; j<=np; j++) if (c2[i][j]==1)
    {
      if (c2[i][q[a[j]-1]]==1) c3[j][1]=i;
      if (c2[i][a[j]]==1)   c3[j][2]=i;
    }
    for (j=1; j<=np; j++) if (c2[i][q[j-1]]==1)
    {
      if (c2[i][a[j]]==1) c3[j][3]=i;
      if (c2[i][q[a[j]-1]]==1)   c3[j][4]=i;
    }
  }


  /* compute matrix er:
     er[i,j]=k says that edges i, j are incident to region k. */

  for(i=1; i<=np-1; i++) for(j=i+1; j<=np; j++) for(k=1; k<=2; k++)
  {
    reg1=c3[i][k];
    if (c2[reg1][j] == 1) 
    {
      er[i][j]=reg1;
      er[j][i]=reg1;
    }
  }

    
  /* compute matrix c4:
     c4[i,j]=1 if edges i, j meet at a crossing. */

  for (i=1; i<=np; i++)
  {
    c4[i][a[i]]=1;
    c4[i][q[a[i]-1]]=1;
    c4[i][a[q[i+1]]]=1;
    c4[i][q[a[q[i+1]]-1]]=1;
  }



  for (i=1; i<=n; i+=2)
  {
    reg1=c3[i][1];
    reg2=c3[i][2];
    reg3=c3[i][3];
    reg4=c3[i][4];

    dop[reg1][reg3] = 1; dop[reg3][reg1] = 1;
    dop[reg2][reg4] = 1; dop[reg4][reg2] = 1;
    cop[reg1][i] = reg3; cop[reg3][i] = reg1;
    cop[reg2][i] = reg4; cop[reg4][i] = reg2;
    cop[reg1][a[i]] = reg3; cop[reg3][a[i]] = reg1;
    cop[reg2][a[i]] = reg4; cop[reg4][a[i]] = reg2;
  }



  /* compute matrix rm, telling which edge is shared by
     adjacent regions. */

  for (i=1; i<=n; i+=2)
  {
    rm[c3[i][1]][c3[i][2]]=i;
    rm[c3[i][2]][c3[i][1]]=i;
    rm[c3[i][2]][c3[i][3]]=a[i];
    rm[c3[i][3]][c3[i][2]]=a[i];
  }

  /* matrix kf:
     kf[i,j]=1 says that as seen from ith region, crossing j is
     positive (in checkerboard sense).*/

  for (i=1; i<=np; i++)
  {
    kf[c3[i][1]][i] = -f[i]*g[i];
    kf[c3[i][2]][i] = f[i]*g[i];
    kf[c3[i][3]][i] = -f[i]*g[i];
    kf[c3[i][4]][i] = f[i]*g[i];
  }

  /* matrix opp:
     opp[i,j]=k says that wrt jth edge, i, k are opposite regions.
     */
  for (i=1; i<=nr-1; i++) for (j=i+1; j<=nr; j++) if (rm[i][j]!=0)
  {
    opp[i][rm[i][j]]=j;
    opp[j][rm[i][j]]=i;
  }


  return 0;
}


int mod0(x)
int x;
{
  int i;
  i=x%(n+1);
  if (i<=0) i=i+n+1;
  return i;
}    

int imax(x, y)
int x, y;
{
  if (x<y) return y; else return x;
}

int imin(x, y)
int x, y;
{
  if (x<y) return x; else return y;
}


    
int orient()
{
  int e[100], g[100], h[100], i, s, t;
  for (i=1; i<=n+1; ++i)
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
    for (i=t+1; i<=n+1; ++i)
      if ((a[i]>=t) && (a[i]<=a[t])) 
        e[i]=-e[i-1];
      else 
        e[i]=e[i-1];
    for (i=t-1; i>=1; --i)
      if ((a[i+1]>=t) && (a[i+1]<=a[t]))
        e[i]=-e[i+1];
      else
        e[i]=e[i+1];
    for (i=1; i<=t-1; ++i) g[i]=1;
    for (i=a[t]+1; i<=n+1; ++i) g[i]=1;
    s=0;i=1;
    while ((s==0) && (i<=n+1))
    {
      if (g[i]==1) s=i;
      ++i;
    }
    while (s!=0)
    {
      if ((a[s]<t) || (a[s]>a[t])) g[s]=g[a[s]]=0;
      else
      {
        if (f[s]!=0) g[s]=g[a[s]]=0;
        else
        {
          f[s]=e[s]*e[a[s]]*f[t];
          f[a[s]]=-f[s];
          g[s]=g[a[s]]=0;
          if (((s!=1) || (abs(a[n+1]-a[1])!=1)) &&
            ((s==1) || 
              ((abs(a[s-1]-a[s])!=1) && (abs(a[s-1]-a[s])!=n))))
          {
            h[s]=h[a[s]]=1;
          }
        }
      }
      s=0;
      i=1;
      while ((s==0) && (i<=n+1))
      {
        if (g[i]==1) s=i;
        ++i;
      }
    }
    h[t]=h[a[t]]=0;
    t=0;
    i=1;
    while ((t==0) && (i<=n+1))
    {
      if (h[i]==1) t=i;
      ++i;
    }
  }
  return 0;
}



    
  
  
  

  
  
  

