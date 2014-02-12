
#include <stdio.h>
#include <stdlib.h>

FILE *fp, *gp;

signed char a0[100], a1[100], a2[100], a3[100],  b[100], b1[100][3], 
    bridge[100],
    c[100][100], c2[100][100], c3[100][100], c4[100][100], 
    circ[100], circ0[100], circ1[100], circ2[100], 
    class[100][100], cm[100][100],
    cop[100][100], direc1[100], direc[100], 
    dist[20][1500][20], dop[100][100], ecirc[100], er[100][100], 
    f[100], g0[100], g1[100], g2[100], g3[100], gcirc[100], 
    gxcirc[100], h[100], ins[100][100][3],
    kf[100][100], label[100][100], md[500],  
    merge[100], mid1[100][100], mid[100][100], nactive[100][100],
    ngap[100][100], ntangle[100], nplait[100], 
    opp[100][100], ord[100][100], perm[100], plait[100][100], 
    pole1[100][100], pole[100][100], pt[100], redcirc[100],
    regvalence[100],
    rins[100][100][3], rm[100][100], shrink[100], ssgn[100][100],
    stretch[100], tang[100][100], tang1[100][100], tangmin[100], 
    tb[100][100], te[100][100], totactive[100], 
    u[100][100], vec[20000][20], w[150], xbe[100][3], xeb[100][3],
    xcirc[100], xcirc1[100], xcirc2[100];

int a00[100], g00[100];
    
int max_class_size[50], *xrinv, ndist[1500];

signed char (*xa)[100], (*xg)[100], (*ya)[100], (*yg)[100],
      (*xar)[100], (*xgr)[100], (*xa0)[100], (*xg0)[100],
      *a, *g;

signed char *q, *eb[100], *be[100];

int alg, dd, dd1, r, nr, cc1, cc2, cc3, k1, row, gencount,
    ppn, i1, i2, cr1, cr2,
    x1, x2, x3, x4, e1, e2, e3, e4, r1, r2, r3, r4,  
    n, ncirc, ncirc0, j0, j1, s1, ej1, ej2,
    knit, knotid, j2, dir, la1t, p1, p2, q1, q2, s, bg, bg1,
    outoforder, nblobs, la1, cstretch, gap1, gpnumber,
    nplaces, nobjects, ccirc, pos, nvec, cshrink, ndel,
    i, j, k, l, m, p, t, nc, parity,
    try, count, curra1, nogood, comp, np, nxcirc, nxa0, nxar,
    firstknot, nonnumber, batchnumber,
    cknot, lknot, lknottemp, npp, necirc, lkt, conwaysign,
    reducenumber, batchsize, wa, lbound, rbound, 
    nppmax, searchtype, searchchange, mode, memsize;

main(int argc, char *argv[])
{
  fp=fopen(argv[1],"r");
  gp=fopen(argv[2],"w");

  searchchange=30;
  
  for (i=1; i<=10; ++i) max_class_size[i] = 50;
  max_class_size[11] = 120;
  max_class_size[12] = 300;
  max_class_size[13] = 800;
  max_class_size[14] = 2000;
  max_class_size[15] = 6000;
  max_class_size[16] = 10000;
  for (i=17; i<=49; ++i) max_class_size[i] = 500;
  
  fscanf(fp, "%d%d", &nc, &knotid);
  
  if (nc <= 16)
    memsize = max_class_size[nc];
  else
    memsize = max_class_size[16];
  
  xrinv = malloc(memsize * sizeof(int));
  xa = malloc(memsize * sizeof(char) * 100);
  xg = malloc(memsize * sizeof(char) * 100);
  ya = malloc(memsize * sizeof(char) * 100);
  yg = malloc(memsize * sizeof(char) * 100);
  xar = malloc(memsize * sizeof(char) * 100);
  xgr = malloc(memsize * sizeof(char) * 100);
  xa0 = malloc(memsize * sizeof(char) * 100);
  xg0 = malloc(memsize * sizeof(char) * 100);
  
  setmod();

  for (i=1; i<=n; i+=2) fscanf(fp, "%d", a00+i);

  for (i=1; i<=n; i+=2) g00[i]=a00[i]/abs(a00[i]);
  for (i=1; i<=n; i+=2) a00[i]=abs(a00[i]);
  for (i=1; i<=n; i+=2) a00[a00[i]]=i;
  for (i=1; i<=n; i+=2) g00[a00[i]]=-g00[i];

  for (i=1; i<=np; ++i)
  {
    a0[i]=a00[i];
    g0[i]=g00[i];
  }

  rectify(a0, g0, a1, g1);
  for (i=1; i<=np; ++i)
  {
    a0[i]=a1[i];
    g0[i]=g1[i];
  }

  while (reduce(a0, g0) == 1);
    
  while (filter() == 1)
  {
   while (reduce(a0, g0) == 1);
  }

  fprintf(gp, "{%2d  ", nc); 
  for (i=1; i<=np; i+=2) fprintf(gp, "%4d", a0[i]*g0[i]);
  fprintf(gp, "}");
  fclose(fp);
  fclose(gp);
  return 0;     
}
  

filter()
{
  int i, j;


  for (i=1; i<=np; i++)
  {
    xa[1][i]=a0[i];
    xg[1][i]=g0[i];
  }
  
  cknot=1; lknot=1; xrinv[1]=1; searchtype=1;

  while (cknot <= lknot  &&  lknot <= max_class_size[nc])
  {
    a = *(xa + cknot);
    g = *(xg + cknot);
    rectify(a, g, a1, g1);
   
    for (i=1; i<=np; ++i)
    {
      a[i]=a1[i];
      g[i]=g1[i];
    }

    incidence();

    if (alg == 0) alg=algtest();
    npp=1;

    for (i=1; i<=np; i++) ya[1][i]=a[i];
    for (i=1; i<=np; i++) yg[1][i]=g[i];

    if (pass() == 1)
    {      
      return 1;
    }

    if (alg != 150)
    {
      /*if (cknot <= lkt || cknot == 1)*/ if (conway() == 1) return 1;
      if (t3s() == 1) return 1;
      if (pass3() == 1) return 1;
      if (gf1()==1 || gf2()==1 || gf3()==1) return 1;
      if (gp1()==1 || gp2()==1 || gp3()==1 || 
          gp4()==1 || gp5()==1 || gp6()==1)
        return 1;
      if (perko() == 1) return 1;
    }

    for (i=1; i<=npp; i++)
    {
      a = *(ya + i);
      g = *(yg + i);

      if (xredundancy(a, g) == 0  ||  lknot == 1)
      {
        j = flype();
        if (j == 1) return 1;
        if (j == 2) return 0;
      }
      if (cknot == 1  &&  i == 1) lkt=lknot;

      if ((searchtype == 1) && (lknot > searchchange))
      {
        searchtype=2;
        for (j=1; j<=lknot; j++) xrinv[j]=j;
        heapsort(xrinv);
      }
    }

    cknot++;
  }
  return 0;
}


qxredundancy(signed char *a, signed char *g)
{
  int i, c, t, lb, rb;
 

  if (searchtype == 1)
  {
    for (i=1; i<=lknot; i++)
      if (fastcomp(*(xa+i), *(xg+i), a, g) == 0) return 1;
    return 0;
  }

  else


  {
    if (compare(a, g, *(xa+xrinv[1]), *(xg+xrinv[1])) == 1) return 0;

    if (compare(a, g, *(xa+xrinv[lknot]), *(xg+xrinv[lknot])) == -1) return 0;

    lb=1;
    rb=lknot;
    while (rb - lb > 1)
    {
      c=(lb + rb)/2;

      t=compare(a, g, *(xa+xrinv[c]), *(xg+xrinv[c]));

      if (t == 0) return 1;
      if (t == 1) rb=c;
      if (t == -1)  lb=c;
    }

    if ((compare(a, g, *(xa+xrinv[lb]), *(xg+xrinv[lb])) == 0) ||
       (compare(a, g, *(xa+xrinv[rb]), *(xg+xrinv[rb])) == 0)) return 1; else return 0;
  }
}



xredundancy(signed char *a, signed char *g)
{
  int i, c, t;

  if (searchtype == 1)
  {
    for (i=1; i<=lknot; i++)
      if (fastcomp(*(xa+i), *(xg+i), a, g) == 0) return 1;
    return 0;
  }

  else

  {
    if (compare(a, g, *(xa+xrinv[1]), *(xg+xrinv[1])) == 1)
    {
      lbound=0;
      return 0;
    }


    if (compare(a, g, *(xa+xrinv[lknot]), *(xg+xrinv[lknot])) == -1)
    {
      lbound=lknot;
      return 0;
    }


    lbound=1;
    rbound=lknot;
    while (rbound - lbound > 1)
    {
      c=(lbound + rbound)/2;

      t=compare(a, g, *(xa+xrinv[c]), *(xg+xrinv[c]));

      if (t == 0) return 1;
      if (t == 1) rbound=c;
      if (t == -1)  lbound=c;
    }

    if ((compare(a, g, *(xa+xrinv[lbound]), *(xg+xrinv[lbound])) == 0) ||
         (compare(a, g, *(xa+xrinv[rbound]), *(xg+xrinv[rbound])) == 0)) return 1; else return 0;
  }
}


xinsert(signed char *a, signed char *g)
{
  int i;

  lknot++;

  for (i=1; i<=np; i++) xa[lknot][i]=a[i];
  for (i=1; i<=np; i++) xg[lknot][i]=g[i];

  if (searchtype == 2)
  {
    for (i=lknot; i>=lbound+2; i--) xrinv[i]=xrinv[i-1];

    xrinv[lbound+1]=lknot;
  }

  return 0;
}


heapsort(int *x)
{
  int i, j, k, r, s;

  for (i=2; i<=lknot; i++) if (hcompare(x[i], x[i/2]) == -1)
  {

      k=x[i];
      r=i;
      s=r/2;
      while (r>1 && hcompare(k,x[s]) == -1)
      {
         x[r]=x[s];
         r=s;
         s=r/2;
      }
      x[r]=k;
   }

   for (i=lknot; i>=2; i--)
   {

      j=x[1];       /* swap x[1] with x[i] */
      x[1]=x[i];    /* least is put at the bottom of heap */
      x[i]=j;

      r=1;
      k=x[r];
      if (hcompare(x[2],x[3]) == -1  ||  i<4) s=2; else s=3;
      while (s < i  &&  hcompare(k,x[s]) == 1)
      {
         x[r]=x[s];
         r=s;
         if ( 2*r+1 >= i  ||  hcompare(x[2*r],x[2*r+1]) == -1 )
            s=2*r; else s=2*r+1;
      }
      x[r]=k;
   }
   return 0;
}


hcompare(int u, int v)
{
  int i;
  signed char *w, *x, *y, *z;

  w=*(xa+u); x=*(xg+u); y=*(xa+v); z=*(xg+v);

  for (i=1; i<=n-2; i+=2) if (*(w+i)!=*(y+i))
    {
      if (*(w+i)<*(y+i)) return 1; else return -1;
    }
  for (i=3; i<=n; i+=2)
    if ((*(x+i))*(*(x+1)) != (*(z+i))*(*(z+1)))
    {
      if ((*(x+i))*(*(x+1)) < (*(z+i))*(*(z+1))) return 1;
      else return -1;
    }
  return 0;
}


yredundancy()
{
  int i;
  for (i=1; i<=npp; i++)
    if (fastcomp(*(ya+i), *(yg+i), a2, g2) == 0) return 1;
  return 0;
}


fastcomp(signed char *w, signed char *x, signed char *y, signed char *z)
{
  int i;
  for (i=1; i<=n; i+=2)
    if ((*(w+i) != *(y+i)) || (*(x+i) != *(z+i))) return 1;
  return 0;
}


primetest(signed char *x, signed char *y)
{
  int l1, l2, k, comp, np1, np2, nc1, nc2;
  signed char u1[100], u2[100], v1[100], v2[100];
  
  for(l1=1; l1<=np-5; l1++) for (l2=l1+5; l2<=imin(np,np+l1-7); l2++)
  {
    comp=1;
    for (k=l1; k<=l2; k++) if ((x[k]<l1) || (x[k]>l2)) comp=0;
    if (comp==1)
    {
      for (i=l1; i<=l2; ++i)
      {
        u1[i-l1+1]=x[i]-l1+1;
        v1[i-l1+1]=y[i];
      }
      for (i=l2+1; i<=np+l1-1; ++i)
      {
        u2[q[i-l2]]=q[x[q[i]]-l2];
        v2[q[i-l2]]=y[q[i]];
      }
      np1=l2-l1+1;
      np2=np-np1;
      nc1=np1/2;
      nc2=np2/2;
      
      fprintf(gp, "{%2d  1", nc1);
      for (i=1; i<np1; i+=2) fprintf(gp, "%4d", u1[i]*v1[i]*v1[1]);
      fprintf(gp, "} {%2d  1", nc2);      
      for (i=1; i<np2; i+=2) fprintf(gp, "%4d", u2[i]*v2[i]*v2[1]);
      fprintf(gp, "}");
      exit(0);
    }    
  }
  return 0;
}  


nugtest(signed char *x, int cr)

{
  int i, l1, l2, t;
  
  l1=cr;
  l2=x[cr];
  if (l1 > l2) { i=l1; l1=l2; l2=i; }
  
  t=1;
  
  for (i=l1+1; i<=l2-1; ++i) if (x[i] > l2 || x[i] < l1) t=0;
  
  return t;
}


reduce(signed char *a, signed char *g)
{
  int i;

  signed char a1[150], a2[150], f[150], g1[150], g2[150], stretch[150], del[150], 
      rm[100][100], er[100][100], regdone[10];
      
  int dir, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, ej1, ej2,
      r1, r2, r3, r5, r6, r7, r8, r9,
      x1, x2, x3, cstretch, ret, ccr;

  ret=0; /* returned value of this procedure */
  
  if (nc < 3)
  {
    fprintf(gp, "unknot\n");
    exit(0);
  }

  for (i=1; i<=np; i++) if (nugtest(a, i) == 1)        /* nugatory crossing */
  {
    x1=i;                       /* x1 is first blob of nugatory crossing */

    cstretch=0;
    for (i=1; i<=np; i++) stretch[i]=0;
    for (i=1; i<=np; i++)
    {
      if (i==x1 || i==a[x1]) cstretch--;
      else
      {
        stretch[i]=cstretch;
      }
    }

    nc--;
    setmod();

    for (i=1; i<=np+2; i++) del[i]=0;
    del[a[x1]]=del[x1]=1;

    for (i=1; i<=np+2; i++) if (del[i] == 0  &&  del[a[i]] == 0)
    {
      a1[i + stretch[i]] = a[i] + stretch[a[i]];
      g1[i + stretch[i]] = g[i];
    }

    bugtest(a1, g1, 1);
    rectify(a1, g1, a2, g2);
      
    for (i=1; i<=np; ++i)
    {
      a[i]=a2[i];
      g[i]=g2[i];
    }
    
    return 1;
  }
  
  
  for (ccr=1; ccr<=np; ccr++)
    if ((g[ccr] == g[q[ccr+1]]) &&
       ((abs(a[q[ccr+1]]-a[ccr]) == 1) || (abs(a[q[ccr+1]]-a[ccr]) == n))) /* Reid II */
  {
    x1=ccr; x2=q[ccr+1];

    cstretch=0;
    for (i=1; i<=np; i++) stretch[i]=0;
    for (i=1; i<=np; i++)
    {
      if (i==a[x1] || i==a[x2] || i==x2 || i==x1) cstretch--;
      else
      {
        stretch[i]=cstretch;
      }
    }

    nc-=2;
    setmod();

    for (i=1; i<=np+4; i++) del[i]=0;
    del[a[x1]]=del[a[x2]]=del[x2]=del[x1]=1;

    for (i=1; i<=np+4; i++) if (del[i] == 0  &&  del[a[i]] == 0)
    {
      a1[i + stretch[i]] = a[i] + stretch[a[i]];
      g1[i + stretch[i]] = g[i];
    }    
    
    bugtest(a1, g1, 2);
    rectify(a1, g1, a2, g2);  
          
    for (i=1; i<=np; ++i)
    {
      a[i]=a2[i];
      g[i]=g2[i];
    }
    
    return 1;     
  }
  
  primetest(a, g);

  /* now compute matrices er, rm, so as to be able to identify degenerate 2-pass */

  {
    int i, j, k, reg1, nr, dd, dd1, r;
    signed char b1[150][5], c2[150][150], c3[150][10];

    orient(a, f);
    nr=(n+5)/2;

    for (i=1; i<=nr; i++) for (j=1; j<=nr; j++) c3[i][j]=rm[i][j]=0;
    for (i=1; i<=np; i++) for (j=1; j<=np; j++) c2[i][j]=er[i][j]=0;
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

  /* compute matrix 
  , telling which edge is shared by
     adjacent regions. */

    for (i=1; i<=n; i+=2)
    {
      rm[c3[i][1]][c3[i][2]]=i;
      rm[c3[i][2]][c3[i][1]]=i;
      rm[c3[i][2]][c3[i][3]]=a[i];
      rm[c3[i][3]][c3[i][2]]=a[i];
    }
  }

  /* now seek degenerate pass (i.e. (2,1)-pass) */

  for (ej1=1; ej1<=np; ej1++) if (g[ej1] == g[q[ej1+1]])
    for (dir=-1; dir<=1; dir+=2)
  {
    x1=eb[ej1][-1]; x2=eb[ej1][1];
    e3=be[x1][-1]; e4=be[x2][1];
    e7=be[a[x1]][dir]; e8=be[a[x2]][dir*f[x1]*f[x2]];
    e5=be[a[x1]][-dir]; e6=be[a[x2]][-dir*f[x1]*f[x2]];

    r1=er[ej1][e6]; r2=er[e4][e6]; r3=er[e4][e8];
    r5=er[e3][e7]; r6=er[e3][e5]; r7=er[ej1][e7];

    if ((rm[r2][r5] == 0) && (rm[r3][r6] == 0)) continue; /* filter out non-degeneracy */

    if (rm[r2][r5] != 0) e1=rm[r2][r5]; else e1=rm[r3][r6]; 
                                           /* e1 is location of new crossing */

    cstretch=0;
    for (i=1; i<=np; i++) stretch[i]=0;
    for (i=1; i<=np; i++)
    {
      if (i==a[x1] || i==a[x2] || i==x2) cstretch--;
      else
      {
        stretch[i]=cstretch;
        if (i==e1) cstretch++;
      }
    }

    nc--;
    setmod();

    a1[x1 + stretch[x1]] = q[e1 + stretch[e1] + 1];
    a1[q[e1 + stretch[e1] + 1]] = x1 + stretch[x1];
    g1[x1 + stretch[x1]] = g[x1];
    g1[q[e1 + stretch[e1] + 1]] = -g[x1];

    for (i=1; i<=np+2; i++) del[i]=0;
    del[a[x1]]=del[a[x2]]=del[x2]=1;

    for (i=1; i<=np+2; i++) if (del[i] == 0  &&  del[a[i]] == 0)
    {
      a1[i + stretch[i]] = a[i] + stretch[a[i]];
      g1[i + stretch[i]] = g[i];
    }

    bugtest(a1, g1, 3);
    rectify(a1, g1, a2, g2);
          
    for (i=1; i<=np; ++i)
    {
      a[i]=a2[i];
      g[i]=g2[i];
    }
    
    return 1;     
  }


  /*  seek (3,2)-pass  */
  for (ej1=1; ej1<=np; ej1++) 
    if (g[ej1] == g[q[ej1+1]]  &&  g[ej1] == g[q[ej1+2]])
    for (dir=-1; dir<=1; dir+=2)
  {
    x1=eb[ej1][-1]; x2=eb[ej1][1]; ej2=be[x2][1]; x3=eb[ej2][1];
    e7=be[x1][-1]; e8=be[x3][1];
    e1=be[a[x1]][dir];
    e2=be[a[x2]][dir*f[x1]*f[x2]];    
    e3=be[a[x3]][dir*f[x1]*f[x3]];    
    e9=be[a[x1]][-dir];
    e10=be[a[x2]][-dir*f[x1]*f[x2]];    
    e11=be[a[x3]][-dir*f[x1]*f[x3]];

    r1=er[e1][e7]; r2=er[e1][e2]; r3=er[e2][e3]; r4=er[e3][e8];
    r5=er[e7][e9]; r6=er[e9][e10]; r7=er[e10][e11]; r8=er[e8][e11];

    if (r1 == r8  ||  r4 == r5) continue;   /* avoid redundancy */

    for (i=1; i<=nr; i++) if (rm[i][r1] != 0 && rm[i][r8] != 0)
      continue;   /* degenerate configuration */

    for (r9=1; r9<=nr; r9++)
      if (rm[r5][r9]!=0  &&  rm[r4][r9]!=0)  /* indicates the (3,2)-pass */
    {
      for (i=1; i<=nr; i++) regdone[i]=0;
      regdone[r1]=1; regdone[r2]=1; regdone[r3]=1; regdone[r4]=1; 
      regdone[r5]=1; regdone[r6]=1; regdone[r7]=1; regdone[r8]=1; 
      regdone[r9]=1;

      j=0;
      for (i=1; i<=nr; i++) if (regdone[i] == 1) j++;
      if (j != 9) continue;         /* regions not all distinct */

      e4=rm[r5][r9]; e5=rm[r4][r9];  /* new crossings on these edges */

      cstretch=0;
      for (i=1; i<=np; i++) stretch[i]=0;
      for (i=1; i<=np; i++)         /* remove nodes ax1, ax2, ax3, x3 */
      {
        if (i==a[x1] || i==a[x2] || i==a[x3] || i==x3) cstretch--;
        else
        {
          stretch[i]=cstretch;
          if (i==e4 || i==e5) cstretch++;
        }
      }

      nc--;          /*  reduce crossing-number by 1  */
      setmod();

      a1[x1 + stretch[x1]] = q[e4 + stretch[e4] + 1];  /* x1 matched with new node */
      a1[q[e4 + stretch[e4] + 1]] = x1 + stretch[x1];  
      g1[x1 + stretch[x1]] = g[x1];
      g1[q[e4 + stretch[e4] + 1]] = -g[x1];
      
      a1[x2 + stretch[x2]] = q[e5 + stretch[e5] + 1];  /* x2 matched with new node */
      a1[q[e5 + stretch[e5] + 1]] = x2 + stretch[x2];  
      g1[x2 + stretch[x2]] = g[x2];
      g1[q[e5 + stretch[e5] + 1]] = -g[x2];

      for (i=1; i<=np+2; i++) del[i]=0;
      del[a[x1]]=del[a[x2]]=del[a[x3]]=del[x3]=1;

      for (i=1; i<=np+2; i++) if (del[i] == 0  &&  del[a[i]] == 0)
      {
        a1[i + stretch[i]] = a[i] + stretch[a[i]];
        g1[i + stretch[i]] = g[i];
      }

      bugtest(a1, g1, 4);
      rectify(a1, g1, a2, g2);
          
      for (i=1; i<=np; ++i)
      {
        a[i]=a2[i];
        g[i]=g2[i];
      }
    
      return 1;     
    }      
  }
  return 0;
}

generate()
{
  int i, m;

  m=ncirc0;
  while ((h[m] == 1) && (m>=1)) m--;
  if (m == 1) return 1;
  h[m]++;
  for (i=m+1; i<=ncirc0; i++) h[i]=0;
  return 0;
}


xarredundancy(signed char *a, signed char *g)
{
  int i;
  for (i=1; i<=nxar; i++)
    if (fastcomp(*(xar+i), *(xgr+i), a, g) == 0) return 1;
  return 0;
}


t3s()
{
  int i, j;
  int r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12,
      u1, u2, u3, u4, u5, u6, u7, u8, v1, 
      v2, v3, v4, v5, v6, v7, v8, x1, x2, x3, x4,
      e1, e2, e3, e4, cstretch;

  signed char stretch[150], del[150];

  if (algtest() != 10) return 0;


  t=0;
  for (i=1; i<=nr-1; i++) for (j=i+1; j<=nr; j++)
    if (regvalence[i] == 4  &&  regvalence[j] == 4  
    &&  rm[i][j] != 0)  t++;
  if (t != 4) return 0;

  r5=1; while (regvalence[r5] != 4) r5++;
  r6=r5+1; while (regvalence[r6] != 4  ||  rm[r5][r6] == 0) r6++;
  r7=r5+1; while (regvalence[r7] != 4  ||  rm[r6][r7] == 0) r7++;
  r8=r5+1; while (regvalence[r8] != 4  ||  rm[r5][r8] == 0
                  ||  r8 == r6) r8++;
  r1=1; while (regvalence[r1] != 3  ||  rm[r1][r5] == 0) r1++;
  r2=1; while (regvalence[r2] != 3  ||  rm[r2][r6] == 0
                  ||  rm[r1][r2] == 0) r2++;
  r3=1; while (regvalence[r3] != 3  ||  rm[r3][r7] == 0
                  ||  rm[r3][r2] == 0) r3++;
  r4=1; while (regvalence[r4] != 3  ||  rm[r4][r8] == 0
                  ||  rm[r4][r3] == 0) r4++;
  r9=1; while (regvalence[r9] != 3  ||  rm[r9][r5] == 0
                  ||  r9 == r1) r9++;
  r10=1; while (regvalence[r10] != 3  ||  rm[r10][r6] == 0
                  ||  r10 == r2) r10++;
  r11=1; while (regvalence[r11] != 3  ||  rm[r11][r7] == 0
                  ||  r11 == r3) r11++;
  r12=1; while (regvalence[r12] != 3  ||  rm[r12][r8] == 0
                  ||  r12 == r4) r12++;

  u1=rm[r1][r5]; u2=rm[r2][r6]; u3=rm[r3][r7]; u4=rm[r4][r8];
  u5=rm[r9][r5]; u6=rm[r10][r6]; u7=rm[r11][r7]; u8=rm[r12][r8];

  v1=cm[u1][u2]; v2=cm[u2][u3]; v3=cm[u3][u4]; v4=cm[u4][u1];
  v5=cm[u5][u6]; v6=cm[u6][u7]; v7=cm[u7][u8]; v8=cm[u8][u5];

  x1=x2=x3=x4=e1=e2=e3=e4=0;

  if (v1 != 0  &&  v3 != 0  &&  v6 != 0  &&  v8 != 0)
    if ( kf[r6][v1] != kf[r6][v6]  &&  kf[r6][v1] == kf[r8][v3]  &&
         kf[r8][v3] != kf[r8][v8])
    {
      x1=v1; x2=v3; x3=v6; x4=v8;
      e1=rm[r9][r10]; e2=rm[r11][r12]; e3=rm[r2][r3]; e4=rm[r4][r1];
    }
  if (v2 != 0  &&  v4 != 0  &&  v5 != 0  &&  v7 != 0)
    if ( kf[r6][v2] != kf[r6][v5]  &&  kf[r6][v2] == kf[r8][v4]  &&
         kf[r8][v4] != kf[r8][v7])
    {
      x1=v2; x2=v4; x3=v5; x4=v7;
      e1=rm[r10][r11]; e2=rm[r9][r12]; e3=rm[r1][r2]; e4=rm[r3][r4];
    }

  if (x1 == 0) return 0;


  cstretch=0;
  for (i=1; i<=np; i++) stretch[i]=0;
  for (i=1; i<=np; i++)
  {
    if (i==a[x1] || i==a[x2] || i==a[x3] || i==a[x4]) cstretch--;
    else
    {
      stretch[i]=cstretch;
      if (i==e1 || i==e2 || i==e3 || i==e4) cstretch++;
    }
  }
    
  a1[x1 + stretch[x1]] = q[e1 + stretch[e1] + 1];
  a1[x2 + stretch[x2]] = q[e2 + stretch[e2] + 1];
  a1[x3 + stretch[x3]] = q[e3 + stretch[e3] + 1];
  a1[x4 + stretch[x4]] = q[e4 + stretch[e4] + 1];

  a1[q[e1 + stretch[e1] + 1]] = x1 + stretch[x1];
  a1[q[e2 + stretch[e2] + 1]] = x2 + stretch[x2];
  a1[q[e3 + stretch[e3] + 1]] = x3 + stretch[x3];
  a1[q[e4 + stretch[e4] + 1]] = x4 + stretch[x4];

  g1[x1 + stretch[x1]] = g[x1];
  g1[x2 + stretch[x2]] = g[x2];
  g1[x3 + stretch[x3]] = g[x3];
  g1[x4 + stretch[x4]] = g[x4];

  g1[q[e1 + stretch[e1] + 1]] = -g[x1];
  g1[q[e2 + stretch[e2] + 1]] = -g[x2];
  g1[q[e3 + stretch[e3] + 1]] = -g[x3];
  g1[q[e4 + stretch[e4] + 1]] = -g[x4];

  for (i=1; i<=np; i++) del[i]=0;
  del[a[x1]]=1; del[a[x2]]=1; del[a[x3]]=1; del[a[x4]]=1;

  for (i=1; i<=np; i++) if (del[i] == 0  &&  del[a[i]] == 0)
  {
    a1[i + stretch[i]] = a[i] + stretch[a[i]];
    g1[i + stretch[i]] = g[i];
  }

  bugtest(a1, g1, 5);
  rectify(a1, g1, a2, g2);
  reducenumber=reduce(a2, g2);

  if (reducenumber == 1  ||  compare(a2, g2, a0, g0) == 1)
  {
    for (i=1; i<=np; ++i)
    {
      a0[i]=a2[i];
      g0[i]=g2[i];
    }
      
    return 1;
  }
    
  else if (reducenumber == 0  &&  yredundancy() == 0  &&  
           xredundancy(a2,g2) == 0)
  {
    npp++;
    for (i=1; i<=np; i++)
    {
      ya[npp][i] = a2[i];
      yg[npp][i] = g2[i];
    }
  }
  return 0;
}
  

pass3()
{
  int i, j;
  int ej1, ej2, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11,
      x1, x2, x3, h,
      r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, dir, cstretch;
  signed char regdone[100], stretch[150], del[150];

  for (ej1=1; ej1<=np; ej1++) 
    if (g[ej1] == g[q[ej1+1]]  &&  g[ej1] == g[q[ej1+2]])
    for (dir=-1; dir<=1; dir+=2)
  {
    x1=eb[ej1][-1]; x2=eb[ej1][1]; ej2=be[x2][1]; x3=eb[ej2][1];
    e7=be[x1][-1]; e8=be[x3][1];
    e1=be[a[x1]][dir];
    e2=be[a[x2]][dir*f[x1]*f[x2]];    
    e3=be[a[x3]][dir*f[x1]*f[x3]];    
    e9=be[a[x1]][-dir];
    e10=be[a[x2]][-dir*f[x1]*f[x2]];    
    e11=be[a[x3]][-dir*f[x1]*f[x3]];

    r1=er[e1][e7]; r2=er[e1][e2]; r3=er[e2][e3]; r4=er[e3][e8];
    r5=er[e7][e9]; r6=er[e9][e10]; r7=er[e10][e11]; r8=er[e8][e11];

    if (r1 == r8  ||  r4 == r5) return 0;

    for (i=1; i<=nr; i++) if ((rm[i][r1] != 0 && rm[i][r8] != 0)  ||
      (rm[i][r4] != 0 && rm[i][r5] != 0))
        return 0;

    for (r9=1; r9<=nr; r9++) for (r10=1; r10<=nr; r10++)
      if (rm[r1][r9]!=0  &&  rm[r4][r10]!=0  &&  rm[r9][r10]!=0)
    {
      for (i=1; i<=nr; i++) regdone[i]=0;
      regdone[r1]=1; regdone[r2]=1; regdone[r3]=1; regdone[r4]=1; 
      regdone[r5]=1; regdone[r6]=1; regdone[r7]=1; regdone[r8]=1; 
      regdone[r9]=1; regdone[r10]=1;

      j=0;
      for (i=1; i<=nr; i++) if (regdone[i] == 1) j++;
      if (j != 10) continue;

      e6=rm[r1][r9]; e5=rm[r9][r10]; e4=rm[r10][r4];
   
      cstretch=0;
      for (i=1; i<=np; i++) stretch[i]=0;
      for (i=1; i<=np; i++)
      {
        if (i==a[x1] || i==a[x2] || i==a[x3]) cstretch--;
        else
        {
          stretch[i]=cstretch;
          if (i==e4 || i==e5 || i==e6) cstretch++;
        }
      }

      h=g[x1];

      a1[x1+stretch[x1]] = q[e6 + stretch[e6] + 1];
      a1[x2+stretch[x2]] = q[e5 + stretch[e5] + 1];
      a1[x3+stretch[x3]] = q[e4 + stretch[e4] + 1];
      a1[q[e6 + stretch[e6] + 1]] = x1+stretch[x1];
      a1[q[e5 + stretch[e5] + 1]] = x2+stretch[x2];
      a1[q[e4 + stretch[e4] + 1]] = x3+stretch[x3];

      g1[x1+stretch[x1]] = h;
      g1[x2+stretch[x2]] = h;
      g1[x3+stretch[x3]] = h;
      g1[q[e6 + stretch[e6] + 1]] = -h;
      g1[q[e5 + stretch[e5] + 1]] = -h;
      g1[q[e4 + stretch[e4] + 1]] = -h;

      for (i=1; i<=np; i++) del[i]=0;
      del[x1]=1; del[x2]=1; del[x3]=1;
      for (i=1; i<=np; i++) 
        if ((del[i] == 0) && (del[a[i]] == 0))
      {
        a1[i+stretch[i]] = a[i]+stretch[a[i]];
        g1[i+stretch[i]] = g[i];
      }


      bugtest(a1, g1, 6);

      rectify(a1, g1, a2, g2);

      reducenumber=reduce(a2, g2);

      if (reducenumber == 1  ||  compare(a2, g2, a0, g0) == 1)
      {
        for (i=1; i<=np; ++i)
        {
          a0[i]=a2[i];
          g0[i]=g2[i];
        }
      
        return 1;
      }

      else if (reducenumber == 0  &&  yredundancy() == 0  &&
               xredundancy(a2,g2) == 0)
      {
        npp++;
        for (i=1; i<=np; i++)
        {
          ya[npp][i] = a2[i];
          yg[npp][i] = g2[i];
        }
      }
    }
  }
  return 0;
}


gf1()
{
  int i;
  int ej1, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11,
      r1, r2, r3, r4, r5, r6, r7, r8, r9, x1, x2, x3, cstretch,
      dir1, dir2, dir3, dir4, dir5, dir6, cups, t1, t2, i0, j, x3f, x3b, ax3, ax3f, ax3b;
  signed char regdone[100], stretch[150], del[150], ups[150];

  for (ej1=1; ej1<=np; ej1++) if (g[ej1] == g[q[ej1+1]])
    for (dir1=-1; dir1<=1; dir1+=2) for (dir2=-1; dir2<=1; dir2+=2)
  {
    x1=eb[ej1][-dir1];
    x2=eb[ej1][dir1];
    e7=be[x2][dir1];
    e1=be[x1][-dir1];
    e2=be[a[x1]][-dir2];
    e10=be[a[x1]][dir2];
    dir3=dir2*f[x1]*f[x2];
    e3=be[a[x2]][-dir3];
    e11=be[a[x2]][dir3];
    r1=er[e10][e1];
    r2=er[e1][e2];
    r3=er[e2][e3];
    r4=er[e3][e7];
    r7=er[e10][e11];
    r8=er[e7][e11];

    if (r1 == r8  ||  r2 == r4) continue;

    for (x3=1; x3<=np; x3++)
      if (kf[r1][x3] == -kf[r1][x1]  &&  g[x3] == g[x1]) 
    {
      r5=cop[r1][x3];
      e4=rm[r5][r4];
      if (e4 == 0  ||  r5 == r3) continue;
      
      x3f=be[x3][1];
      x3b=be[x3][-1];

      if (c2[r1][x3f] == 1) 
      {
        e6=x3f;
        e8=x3b;
        dir6=-1;
      }
      else
      {
        e6=x3b;
        e8=x3f;
        dir6=1;
      }

      ax3=a[x3];
      ax3f=be[ax3][1];
      ax3b=be[ax3][-1];

      if (c2[r5][ax3f] == 1) 
      {
        e5=ax3f;
        e9=ax3b;
        dir5=-1;
      }
      else
      {
        e5=ax3b;
        e9=ax3f;
        dir5=1;
      }

      r6=er[e5][e6];
      r9=er[e8][e9];

      dir4=-(dir1+dir2+dir3+dir5+dir6);


      for (i=1; i<=nr; i++) regdone[i]=0;
      regdone[r1]=1; regdone[r2]=1; regdone[r3]=1; regdone[r4]=1; 
      regdone[r5]=1; regdone[r6]=1; regdone[r7]=1; regdone[r8]=1; 
      regdone[r9]=1;

      j=0;
      for (i=1; i<=nr; i++) if (regdone[i] == 1) j++;
      if (j != 9) continue;

      cstretch=0;
      for (i=1; i<=np; i++) stretch[i]=0;
      for (i=1; i<=np; i++)
      {
        if (i==x1 || i==x2) cstretch--;
        else
        {
          stretch[i]=cstretch;
          if (i==e4) cstretch+=2;
        }
      }


      if (dir4 == 1) 
      {
        t1=1; 
        t2=2;
      }
      else 
      {
        t1=2;
        t2=1;
      }

      a1[a[x1] + stretch[a[x1]]] = q[a[x2] + stretch[a[x2]]];
      a1[a[x2] + stretch[a[x2]]] = q[a[x1] + stretch[a[x1]]];

      a1[x3 + stretch[x3]] = q[e4 + stretch[e4] + t2];
      a1[e4 + stretch[e4] + t2] = q[x3 + stretch[x3]];

      a1[a[x3] + stretch[a[x3]]] = q[e4 + stretch[e4] + t1];
      a1[e4 + stretch[e4] + t1] = q[a[x3] + stretch[a[x3]]];


      g1[a[x1] + stretch[a[x1]]] = -g[x1];
      g1[a[x2] + stretch[a[x2]]] = g[x1];

      g1[x3 + stretch[x3]] = -g[x1];
      g1[q[e4 + stretch[e4] + t2]] = g[x1];

      g1[a[x3] + stretch[a[x3]]] = -g[x1];
      g1[q[e4 + stretch[e4] + t1]] = g[x1];


      for (i=1; i<=np; i++) del[i]=0;
      del[x1]=1; del[x2]=1; del[x3]=1;
      del[a[x1]]=1; del[a[x2]]=1; del[a[x3]]=1;

      cups=1;
      for (i=ej1; i<=ej1+np-1; i++)
      {
        i0=q[i];
        ups[i0]=cups;     
        if(i0==e7||i0==e8||i0==e9 || i0==e4 || i0==e10 || i0==e11)
          cups*=-1;
      }

      for (i=1; i<=np; i++) if (del[i] == 0  &&  del[a[i]] == 0)
      {
        a1[i + stretch[i]] = a[i] + stretch[a[i]];
        g1[i + stretch[i]] = g[i]*ups[i];
      }   

      bugtest(a1, g1, 7);
      rectify(a1, g1, a2, g2);

      reducenumber=reduce(a2, g2);

      if (reducenumber == 1  ||  compare(a2, g2, a0, g0) == 1)
      {
        for (i=1; i<=np; ++i)
        {
          a0[i]=a2[i];
          g0[i]=g2[i];
        }
      
        return 1;
      }
 
      else if (reducenumber == 0  &&  yredundancy() == 0  && 
               xredundancy(a2,g2) == 0)
      {
        npp++;
        for (i=1; i<=np; i++)
        {
          ya[npp][i] = a2[i];
          yg[npp][i] = g2[i];
        }
      }
    }
  }
  return 0;
} 

     
gf2()
{
  int i;
  int ej1, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11,
      r1, r2, r3, r4, r5, r6, r7, r8, r9, x1, x2, x3,
      dir1, dir2, dir3, dir4, dir5, dir6, cstretch,
      cups, t1, t2, i0, j, x3f, x3b, ax3, ax3f, ax3b;
  signed char regdone[100], stretch[150], del[150], ups[150];

  for (ej1=1; ej1<=np; ej1++) if (g[ej1] == g[q[ej1+1]])
    for (dir3=-1; dir3<=1; dir3+=2) for (dir1=-1; dir1<=1; dir1+=2)
  {
    x1=eb[ej1][dir3];
    x2=eb[ej1][-dir3];
    e7=be[x1][dir3];
    e3=be[x2][-dir3];
    e8=be[a[x1]][dir1];
    e1=be[a[x1]][-dir1];
    dir2=dir1*f[x1]*f[x2];
    e9=be[a[x2]][dir2];
    e2=be[a[x2]][-dir2];
    r8=er[e8][e9];
    r1=er[e1][e7];
    r2=er[e1][e2];
    r3=er[e2][e3];
    r4=er[e3][e9];
    r7=er[e7][e8];
    for (x3=1; x3<=np; x3++)
      if (kf[r1][x3] == -kf[r1][x1]  &&  g[x3] == g[x1]) 
    {
      r5=cop[r1][x3];
      e4=rm[r5][r4];
      if (e4 == 0  ||  r5 == r3) continue;

      x3f=be[x3][1];
      x3b=be[x3][-1];

      if (c2[r1][x3f] == 1) 
      {
        e11=x3f;
        e5=x3b;
        dir5=1;
      }
      else
      {
        e11=x3b;
        e5=x3f;
        dir5=-1;
      }

      ax3=a[x3];
      ax3f=be[ax3][1];
      ax3b=be[ax3][-1];

      if (c2[r5][ax3f] == 1) 
      {
        e10=ax3f;
        e6=ax3b;
        dir6=1;
      }
      else
      {
        e10=ax3b;
        e6=ax3f;
        dir6=-1;
      }

      r6=er[e5][e6];
      r9=er[e10][e11];

      dir4=-(dir1+dir2+dir3+dir5+dir6);


      for (i=1; i<=nr; i++) regdone[i]=0;
      regdone[r1]=1; regdone[r2]=1; regdone[r3]=1; regdone[r4]=1; 
      regdone[r5]=1; regdone[r6]=1; regdone[r7]=1; regdone[r8]=1; 
      regdone[r9]=1;

      j=0;
      for (i=1; i<=nr; i++) if (regdone[i] == 1) j++;
      if (j != 9) continue;

      cstretch=0;
      for (i=1; i<=np; i++) stretch[i]=0;
      for (i=1; i<=np; i++)
      {
        if (i==x1 || i==x2) cstretch--;
        else
        {
          stretch[i]=cstretch;
          if (i==e4) cstretch+=2;
        }
      }

      if (dir4 == 1) 
      {
        t1=1; 
        t2=2;
      }
      else 
      {
        t1=2;
        t2=1;
      }

      a1[a[x1] + stretch[a[x1]]] = q[a[x2] + stretch[a[x2]]];
      a1[a[x2] + stretch[a[x2]]] = q[a[x1] + stretch[a[x1]]];

      a1[x3 + stretch[x3]] = q[e4 + stretch[e4] + t1];
      a1[e4 + stretch[e4] + t1] = q[x3 + stretch[x3]];

      a1[a[x3] + stretch[a[x3]]] = q[e4 + stretch[e4] + t2];
      a1[e4 + stretch[e4] + t2] = q[a[x3] + stretch[a[x3]]];


      g1[a[x1] + stretch[a[x1]]] = g[x2];
      g1[a[x2] + stretch[a[x2]]] = -g[x2];

      g1[x3 + stretch[x3]] = g[x3];
      g1[q[e4 + stretch[e4] + t2]] = -g[x3];

      g1[a[x3] + stretch[a[x3]]] = g[x3];
      g1[q[e4 + stretch[e4] + t1]] = -g[x3];


      for (i=1; i<=np; i++) del[i]=0;
      del[x1]=1; del[x2]=1; del[x3]=1;
      del[a[x1]]=1; del[a[x2]]=1; del[a[x3]]=1;

      cups=1;
      for (i=ej1; i<=ej1+np-1; i++)
      {
        i0=q[i];
        ups[i0]=cups;     
        if(i0==e7||i0==e8||i0==e9 || i0==e4 || i0==e10 || i0==e11)
          cups*=-1;
      }

      for (i=1; i<=np; i++) if (del[i] == 0  &&  del[a[i]] == 0)
      {
        a1[i + stretch[i]] = a[i] + stretch[a[i]];
        g1[i + stretch[i]] = g[i]*ups[i];
      }   

      bugtest(a1, g1, 8);
      rectify(a1, g1, a2, g2);

      reducenumber=reduce(a2, g2);
      
      if (reducenumber == 1  ||  compare(a2, g2, a0, g0) == 1)
      {
        for (i=1; i<=np; ++i)
        {
          a0[i]=a2[i];
          g0[i]=g2[i];
        }
      
        return 1;
      }

      else if (reducenumber == 0  &&  yredundancy() == 0  && 
               xredundancy(a2,g2) == 0)
      {
        npp++;
        for (i=1; i<=np; i++)
        {
          ya[npp][i] = a2[i];
          yg[npp][i] = g2[i];
        }
      }
    }
  }
  return 0;
} 
     

gf3()
{
  int i;
  int ej1, ej2, e1, e2, e3, e4, e5, e6, e7, e8, e9, e11, e12,
      r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, x1, x2, x3, x4,
      dir1, dir2, dir3, dir4, dir5, dir6, cups, t1, t2, i0, j, cstretch;
  signed char bridge[100], regdone[100], stretch[150], del[150], ups[150];


  for (i=1; i<=np; i++) 
    if (g[i] == g[q[i+1]]) bridge[i]=g[i];
    else bridge[i]=0;  
  for (ej1=1; ej1<=np-3; ej1++) for (ej2=ej1+3; ej2<=np; ej2++)
    if ((bridge[ej1] != 0) && (bridge[ej1] == -bridge[ej2]) 
         && (c4[ej1][ej2] != 0))
  {
    r7=er[ej1][ej2];
    r2=opp[r7][ej1];
    r6=opp[r7][ej2];

    if (a[ej1] == ej2  ||  a[ej1] == q[ej2+1]) dir1=1; else dir1=-1;
    if (a[ej2] == ej1  ||  a[ej2] == q[ej1+1]) dir2=1; else dir2=-1;

    x1=eb[ej1][-dir1];
    x2=eb[ej1][dir1];
    x4=a[eb[ej2][dir2]];
    dir3=dir2*f[x1]*f[x2];
    dir5=dir1*f[x1]*f[x4];
    x3=a[q[x4-dir5]];
    dir6=-dir2*f[x4]*f[x3];
    dir4=-(dir1+dir2+dir3+dir5+dir6);

    if (g[x4]!=g[x1] || g[x3]!= g[x1]) continue;

    r1=cop[r7][x1];
    r9=cop[r7][x4];
    r10=cop[r6][x4];
    r3=cop[r7][x2];
    r8=cop[r2][x2];
    r4=cop[r6][x3];
    r5=cop[r9][x3];

    if (rm[r3][r4] == 0) continue;

    for (i=1; i<=nr; i++) regdone[i]=0;
    regdone[r1]=1; regdone[r2]=1; regdone[r3]=1; regdone[r4]=1; 
    regdone[r5]=1; regdone[r6]=1; regdone[r7]=1; regdone[r8]=1; 
    regdone[r9]=1; regdone[r10]=1;

    j=0;
    for (i=1; i<=nr; i++) if (regdone[i] == 1) j++;
    if (j != 10) continue;

    e1=rm[r6][r1]; e2=rm[r1][r2]; e3=rm[r2][r3]; e4=rm[r3][r4];
    e5=rm[r4][r5]; e6=rm[r5][r6]; e7=rm[r7][r10]; e8=rm[r7][r8];
    e9=rm[r3][r8]; e11=rm[r4][r9]; e12=rm[r9][r10];

    cstretch=0;
    for (i=1; i<=np; i++) stretch[i]=0;
    for (i=1; i<=np; i++)
    {
      if (i==x1 || i==x2) cstretch--;
      else
      {
        stretch[i]=cstretch;
        if (i==e4) cstretch+=2;
      }
    }

    if (dir4 == 1) 
    {
      t1=1; 
      t2=2;
    }
    else 
    {
      t1=2;
      t2=1;
    }

    a1[a[x1] + stretch[a[x1]]] = q[a[x2] + stretch[a[x2]]];
    a1[a[x2] + stretch[a[x2]]] = q[a[x1] + stretch[a[x1]]];

    a1[a[x3] + stretch[a[x3]]] = q[e4 + stretch[e4] + t1];
    a1[e4 + stretch[e4] + t1] = q[a[x3] + stretch[a[x3]]];

    a1[x3 + stretch[x3]] = q[e4 + stretch[e4] + t2];
    a1[e4 + stretch[e4] + t2] = q[x3 + stretch[x3]];

    a1[x4 + stretch[x4]] = a[x4] + stretch[a[x4]];
    a1[a[x4] + stretch[a[x4]]] = x4 + stretch[x4];

  
    g1[a[x1] + stretch[a[x1]]] = -g[x2];
    g1[a[x2] + stretch[a[x2]]] = g[x2];

    g1[x4 + stretch[x4]] = -g[x4];
    g1[a[x4] + stretch[a[x4]]] = g[x4];

    g1[x3 + stretch[x3]] = -g[x3];
    g1[q[e4 + stretch[e4] + t2]] = g[x3];

    g1[a[x3] + stretch[a[x3]]] = -g[x3];
    g1[q[e4 + stretch[e4] + t1]] = g[x3];


    for (i=1; i<=np; i++) del[i]=0;
    del[x1]=1; del[x2]=1; del[x3]=1; del[x4]=1;
    del[a[x1]]=1; del[a[x2]]=1; del[a[x3]]=1; del[a[x4]]=1;

    cups=1;
    for (i=ej1; i<=ej1+np-1; i++)
    {
      i0=q[i];
      ups[i0]=cups;     
      if(i0==e7 || i0==e8 || i0==e9 || i0==e4 || i0==e11 || i0==e12)
        cups*=-1;
    }

    for (i=1; i<=np; i++) if (del[i] == 0  &&  del[a[i]] == 0)
    {
      a1[i + stretch[i]] = a[i] + stretch[a[i]];
      g1[i + stretch[i]] = g[i]*ups[i];
    }   


    bugtest(a1, g1, 9);
    rectify(a1, g1, a2, g2);

    reducenumber=reduce(a2, g2);
    
    if (reducenumber == 1  ||  compare(a2, g2, a0, g0) == 1)
    {
      for (i=1; i<=np; ++i)
      {
        a0[i]=a2[i];
        g0[i]=g2[i];
      }
      
      return 1;
    }


    else if (reducenumber == 0  &&  yredundancy() == 0  &&
             xredundancy(a2,g2) == 0)
    {
      npp++;
      for (i=1; i<=np; i++)
      {
        ya[npp][i] = a2[i];
        yg[npp][i] = g2[i];
      }
    }
  }
  return 0;
} 


gp1()
{
  int i, i0;
  int ej1, ej2, e5, e6, e7, e8, e9, e10,
      e11, e12, e13, r1, r2, s1, s2, t1, t2, t3, t4,
      u1, u2, u3, u4, dir1, dir2, j;
  signed char regdone[100];

  for (i0=1; i0<=np; i0++) if (g[i0] != g[q[i0+1]])
  {
    for (j0=-1; j0<=1; j0+=2)
    {
      x1=a[i0]; x2=a[q[i0+1]]; 
      dir1=f[x1]*j0; dir2=-f[x2]*j0;
      x3=q[x2+dir2]; x4=q[x1+dir1];

      if (g[x1] != g[x4]  ||  g[x2] != g[x3]) continue;

      e8=i0; e7=q[e8-1]; e13=q[e8+1];
      ej1=be[x2][dir2]; ej2=be[x1][dir1];
      e9=be[x3][dir2]; e12=be[x2][-dir2];
      e10=be[x4][dir1]; e6=be[x1][-dir1];

      r1=1; while(c2[r1][e8] == 0  ||  c2[r1][ej1] == 0) r1++;
      r2=opp[r1][e8]; s1=opp[r1][ej1]; s2=opp[r2][ej2];
      if (r1 == s2  ||  r2 == s1) continue;

      e5=be[a[x3]] [-f[x2]*f[x3]];
      e11=be[a[x4]] [f[x1]*f[x4]];

      t1=opp[r1][e5]; u1=opp[t1][e9]; t4=opp[r2][e11];
      u3=opp[t4][e10]; t2=opp[r1][e6]; t3=opp[r2][e12];

      for (u2=1; u2<=nr; u2++)
        if (rm[u2][u1] != 0  &&  rm[u2][s2] != 0)
      {
        for (u4=1; u4<=nr; u4++)
          if (rm[u4][u3] != 0  &&  rm[u4][s1] != 0)
        {
          for (i=1; i<=nr; i++) regdone[i]=0;
          regdone[r1]=1; regdone[r2]=1; 
          regdone[s1]=1; regdone[s2]=1; 
          regdone[t1]=1; regdone[t2]=1; 
          regdone[t3]=1; regdone[t4]=1; 
          regdone[u1]=1; regdone[u2]=1;
          regdone[u3]=1; regdone[u4]=1;

          j=0; for (i=1; i<=nr; i++) if (regdone[i] == 1) j++;
          if (j == 12) 
          {
            e1=rm[s1][u4];
            e2=rm[s2][u2];
            e3=rm[u1][u2];
            e4=rm[u3][u4];

            gpnumber=1;
            if (newtransform() == 1) return 1;
          }
        }
      }
    }
  }
  return 0;
}



gp2()
{
  int i;
  int e6, e10,
      r1, r2, s1, s2, t1, t2, u1, u2, v1, v2,
      w1, w2, dir1, dir2, j, sgn;
  signed char regdone[100];

  for (e2=1; e2<=np; e2++) for (sgn=1; sgn<=2; sgn++)
  {

    r1=c3[e2][sgn]; r2=c3[e2][3-sgn];

    for (x2=1; x2<=np; x2++) if 
     (kf[r1][x2] != 0  &&  x2 != e2  &&  x2 != q[e2+1]  &&
      a[x2] != e2  &&  a[x2] != q[e2+1])
    {

      if (c2[r1][x2] == 1) dir1=1; else dir1=-1;
      if (g[x2] != g[q[x2-dir1]]) continue;


      for (x4=1; x4<=np; x4++) if 
        ((x2+x4)%2 == 1  &&  a[x4] != x2  &&  x4 != e2  &&
          a[x4] != e2  &&  kf[r2][x4] == -kf[r1][x2]  &&  
         x4 != q[e2+1]  &&  a[x4] != q[e2+1])
      {


        if (c2[r1][x4] == 1) dir2=1; else dir2=-1;
        if (g[x4] != g[q[x4-dir2]]) continue;
        x1=q[x2-dir1]; x3=q[x4-dir2]; 
        ej1=be[x2][-dir1]; ej2=be[x4][-dir2];
        e10=be[x2][dir1]; e6=be[x4][dir2];
        s1=opp[r1][e10]; s2=opp[r2][e6];
        v1=cop[r1][x2]; u1=opp[v1][ej1];
        t1=cop[v1][x1]; w1=cop[u1][x1];
        v2=cop[r2][x4]; u2=opp[v2][ej2];
        t2=cop[v2][x3]; w2=cop[u2][x3]; 

        if (rm[s2][w1] == 0  ||  rm[s1][w2] == 0) continue;
        
        for (i=1; i<=nr; i++) regdone[i]=0;
        regdone[r1]=1; regdone[r2]=1; regdone[s1]=1;
        regdone[s2]=1; regdone[t1]=1; regdone[t2]=1;
        regdone[u1]=1; regdone[u2]=1; regdone[v1]=1;
        regdone[v2]=1; regdone[w1]=1; regdone[w2]=1;

        j=0; for (i=1; i<=nr; i++) if (regdone[i] == 1) j++;
        if (j == 12) 
        {
          gpnumber=2;
          e1=rm[s2][w1];
          e2=rm[r1][r2];
          e3=rm[s1][w2];
          e4=e2;

          if (newtransform() == 1) return 1;
        }
      }
    }
  }
  return 0;
}


gp3()
{
  int i;
  int e5, e6, e9, e10,
      e11, e12, e13, e14, r1, r2, s1, s2, t1, t2, 
      t3, t4, u1, u2, dir1, dir2, j, sgn;
  signed char regdone[100];


  for (ej1=1; ej1<=np; ej1++) for (ej2=1; ej2<=np; ej2++)
    if (g[ej1] == g[q[ej1+1]]  &&  g[ej2] == g[q[ej2+1]]  &&
        g[ej1] != g[ej2])
  {
    for (dir1=-1; dir1<=1; dir1+=2) for (dir2=-1; dir2<=1; dir2+=2)
    {
      x1=eb[ej1][-dir1]; x2=eb[ej1][dir1]; 
      x3=eb[ej2][-dir2]; x4=eb[ej2][dir2];
      e12=q[ej1-dir1]; e5=q[ej1+dir1]; 
      e6=q[ej2+dir2]; e13=q[ej2-dir2];

      for (sgn=-1; sgn<=1; sgn+=2)
      {
        e9=be[a[x4]][f[x4]*dir2*sgn];
        e14=be[a[x3]][f[x3]*dir2*sgn];
        e10=be[a[x2]][f[x2]*dir1*sgn];
        e11=be[a[x1]][f[x1]*dir1*sgn];

        r1=1; while(c2[r1][ej1] == 0  ||  c2[r1][e11] == 0) r1++;
        r3=opp[r1][e11]; 
        r2=1; while(c2[r2][ej2] == 0  ||  c2[r2][e9] == 0) r2++;
        if (rm[r2][r3] == 0) continue;
        r4=opp[r2][e14];
        if (rm[r4][r1] == 0) continue;
        s1=opp[r1][ej1]; s2=opp[r2][ej2]; t1=opp[r2][e9];
        u1=opp[t1][e6]; t2=opp[r3][e12]; t3=opp[r1][e10];
        u2=opp[t3][e5]; t4=opp[r4][e13];

        if (rm[s1][u1] == 0  ||  rm[s2][u2] == 0) continue;

        for (i=1; i<=nr; i++) regdone[i]=0;
        regdone[r1]=1; regdone[r2]=1; 
        regdone[r3]=1; regdone[r4]=1; 
        regdone[s1]=1; regdone[s2]=1; 
        regdone[t1]=1; regdone[t2]=1;
        regdone[t3]=1; regdone[t4]=1;
        regdone[u1]=1; regdone[u2]=1;

        j=0; for (i=1; i<=nr; i++) if (regdone[i] == 1) j++;
        if (j == 12) 
        {
          gpnumber=3;
          e1=rm[r2][r3];
          e2=rm[s2][u2];
          e3=rm[r1][r4];
          e4=rm[s1][u1];

          if (newtransform() == 1) return 1;
        }
      }
    }
  }
  return 0;
}


gp4()
{
  int i, j;
  int e5, e6, e7, e8, e9, e10, e11, e12, e13,
      e14, e15, e16, r1, r2, r3, r4, r5, r6, r7, r8, r9,
      r10, r11, r12, dir1, dir2, dir3, dir4, dir5, dir6, 
      ej1, ej2;
  signed char regdone[100];

  for (i=1; i<=np; i++) if (g[i] == g[q[i+1]]) bridge[i]=g[i];
    else bridge[i]=0;

  for (ej1=1; ej1<=np; ej1++) for (ej2=1; ej2<=np; ej2++)
    if (bridge[ej1] != 0  &&  bridge[ej1] == -bridge[ej2]
                          &&  er[ej1][ej2] == 0)
    for (dir1=-1; dir1<=1; dir1+=2) for (dir2=-1; dir2<=1; dir2+=2)
    for (dir3=-1; dir3<=1; dir3+=2) for (dir4=-1; dir4<=1; dir4+=2)
  {
    x1=eb[ej1][dir1]; x2=eb[ej1][-dir1];
    e6=be[x1][dir1]; e15=be[x2][-dir1];
    e5=be[a[x1]][dir2]; e16=be[a[x1]][-dir2];
    dir5=dir2*f[x1]*f[x2];
    e12=be[a[x2]][dir5]; e14=be[a[x2]][-dir5];

    r3=er[ej1][e5]; r4=er[e5][e6]; r5=er[e6][e16];
    r11=er[ej1][e16]; r12=er[e14][e15]; r8=er[e12][e15];

    if (r5 == r12  ||  r4 == r8) continue;

    
    x3=eb[ej2][dir3]; x4=eb[ej2][-dir3];
    e9=be[x3][dir3]; e13=be[x4][-dir3];
    e10=be[a[x3]][dir4]; e8=be[a[x3]][-dir4];
    dir6=dir4*f[x3]*f[x4];
    e11=be[a[x4]][dir6]; e7=be[a[x4]][-dir6];

    r1=er[ej2][e8]; r10=er[e10][e11]; r9=er[e9][e10];
    r2=er[e8][e9]; r7=er[e11][e13]; r6=er[e7][e13];

    if (rm[r5][r6] == 0  ||  rm[r7][r8] == 0  ||
        rm[r2][r3] == 0  ||  rm[r7][r11] == 0) continue;

    for (i=1; i<=nr; i++) regdone[i]=0;
    regdone[r1]=1; regdone[r2]=1; regdone[r3]=1; regdone[r4]=1; 
    regdone[r5]=1; regdone[r6]=1; regdone[r7]=1; regdone[r8]=1; 
    regdone[r9]=1; regdone[r10]=1; regdone[r11]=1; regdone[r12]=1; 

    j=0; for (i=1; i<=nr; i++) if (regdone[i] == 1) j++;
    if (j != 12) continue;

    gpnumber=4;
    e1=rm[r5][r6];
    e2=rm[r7][r8];
    e3=rm[r2][r3];
    e4=rm[r7][r11];

    if (newtransform() == 1) return 1;
  }
  return 0;
}


gp5()
{
  int i, j;
  int e5, e6, e7, e8, e9, e10, e11, e12, e13,
      e14, e15, e16, r1, r2, r3, r4, r5, r6, r7, r8, r9,
      r10, r11, r12, dir1, dir2, dir3, dir4, dir5, dir6, 
      ej1, ej2;
  signed char regdone[100];

  for (i=1; i<=np; i++) if (g[i] == g[q[i+1]]) bridge[i]=g[i];
    else bridge[i]=0;

  for (ej1=1; ej1<=np; ej1++) for (ej2=1; ej2<=np; ej2++)
    if (bridge[ej1] != 0  &&  bridge[ej1] == -bridge[ej2]
                          &&  er[ej1][ej2] == 0)
    for (dir1=-1; dir1<=1; dir1+=2) for (dir2=-1; dir2<=1; dir2+=2)
    for (dir3=-1; dir3<=1; dir3+=2) for (dir4=-1; dir4<=1; dir4+=2)
  {
    x1=eb[ej1][dir1]; x2=eb[ej1][-dir1];
    e6=be[x1][dir1]; e16=be[x2][-dir1];
    e7=be[a[x1]][dir2]; e10=be[a[x1]][-dir2];
    dir5=dir2*f[x1]*f[x2];
    e9=be[a[x2]][dir5]; e15=be[a[x2]][-dir5];

    r4=er[e6][e10]; r5=er[e6][e7]; r6=er[ej1][e7];
    r10=er[ej1][e10]; r9=er[e9][e16]; r12=er[e15][e16];

    if (r4 == r12  ||  r5 == r9) continue;

    x3=eb[ej2][dir3]; x4=eb[ej2][-dir3];
    e8=be[x3][dir3]; e13=be[x4][-dir3];
    e11=be[a[x3]][dir4]; e5=be[a[x3]][-dir4];
    dir6=dir4*f[x3]*f[x4];
    e14=be[a[x4]][dir6]; e12=be[a[x4]][-dir6];

    r2=er[e5][e8]; r3=er[ej2][e5]; r7=er[e8][e11];
    r8=er[ej2][e11]; r11=er[e12][e13];

    if (rm[r3][r4] == 0  ||  rm[r8][r9] == 0  ||  
        rm[r8][r10] == 0  ||  rm[r6][r9] == 0  ||
        rm[r10][r11] == 0)
      continue;

    if (kf[r10][x4] == 0) continue;

    for (r1=1; r1<=nr; r1++)
      if (rm[r1][r2] != 0  &&  rm[r1][r6] != 0  &&  r1 != r3  &&
          r1 != r5  &&  r1 != r7  && r1 != r9  &&  r1 != r10)
    {
      for (i=1; i<=nr; i++) regdone[i]=0;
      regdone[r1]=1; regdone[r2]=1; regdone[r3]=1; regdone[r4]=1; 
      regdone[r5]=1; regdone[r6]=1; regdone[r7]=1; regdone[r8]=1; 
      regdone[r9]=1; regdone[r10]=1; regdone[r11]=1;regdone[r12]=1; 

      j=0; for (i=1; i<=nr; i++) if (regdone[i] == 1) j++;
      if (j != 12) continue;

      gpnumber=5;
      e1=rm[r3][r4];
      e2=rm[r8][r9];
      e3=rm[r1][r2];
      e4=rm[r1][r6];

      if (newtransform() == 1) return 1;
    }
  }
  return 0;
}


gp6()
{
  int i, j;
  int e5, e6, e7, e8, e9, e10, e11, e12, e13,
      e14, e15, e16, r1, r2, r3, r4, r5, r6, r7, r8, r9,
      r10, r11, r12, dir1, dir2, dir3, dir4, dir5, dir6, 
      ej1, ej2, regnumber;
  signed char regdone[100];

  for (i=1; i<=np; i++) if (g[i] == g[q[i+1]]) bridge[i]=g[i];
    else bridge[i]=0;

  for (ej1=1; ej1<=np; ej1++) for (ej2=1; ej2<=np; ej2++)
    if (bridge[ej1] != 0  &&  bridge[ej1] == -bridge[ej2])
    for (dir1=-1; dir1<=1; dir1+=2) for (dir2=-1; dir2<=1; dir2+=2)
    for (dir3=-1; dir3<=1; dir3+=2) for (dir4=-1; dir4<=1; dir4+=2)
  {
    r5=r10=0;
    x1=eb[ej1][dir1]; x2=eb[ej1][-dir1];
    e6=be[x1][dir1]; e16=be[x2][-dir1];
    e7=be[a[x1]][dir2]; e10=be[a[x1]][-dir2];
    dir5=dir2*f[x1]*f[x2];
    e9=be[a[x2]][dir5]; e15=be[a[x2]][-dir5];

    r3=er[e6][e10]; r2=er[e6][e7]; r1=er[ej1][e7];
    r11=er[ej1][e10]; r6=er[e9][e16]; r9=er[e15][e16];

    if (r2 == r6  ||  r3 == r9) continue;

    x3=eb[ej2][dir3]; x4=eb[ej2][-dir3];
    e8=be[x3][dir3]; e13=be[x4][-dir3];
    e11=be[a[x3]][dir4]; e5=be[a[x3]][-dir4];
    dir6=dir4*f[x3]*f[x4];
    e14=be[a[x4]][dir6]; e12=be[a[x4]][-dir6];

    r4=er[e13][e14]; r7=er[ej2][e14]; r8=er[e8][e11];
    r12=er[ej2][e12];

    if (r4 == r8) continue;
    if (rm[r4][r1] != 0  ||  rm[r6][r7] != 0) continue;
    if (rm[r3][r6] != 0  ||  rm[r4][r9] != 0) continue;
    if (kf[r9][x3] == 0  ||  kf[r3][x4] == 0  ||
        rm[r9][r12] != e5) continue;

    for (i=1; i<=nr; i++) regdone[i]=0;
    regdone[r1]=regdone[r2]=regdone[r3]=regdone[r4]=1; 
    regdone[r6]=regdone[r7]=regdone[r8]=1; 
    regdone[r9]=regdone[r11]=regdone[r12]=1;

    for (r5=1; r5<=nr; r5++) 
      if (regdone[r5] == 0  &&  rm[r5][r4] != 0 && 
          rm[r5][r6] != 0 /* &&  rm[r5][r8] == 0*/)
    for (r10=1; r10<=nr; r10++)
      if (regdone[r10] == 0  &&  rm[r10][r4] != 0 && 
          rm[r10][r6] != 0  /*&&  rm[r10][r2] == 0*/)
    {

      if (r5 != r10) /* check they are round the right way
                        by checking cyclic order of incidences
                        of r4 with r3, r7, r10, r5. */
      {
        signed char u[150], v[150];
        int p1, p2, p3, p4, t0, t, cv, cnr, dir;
       
        for (i=1; i<=np; i++) u[i]=v[i]=0;
        p1=rm[r4][r3]; p2=rm[r4][r5]; p3=rm[r4][r10]; p4=rm[r4][r7];
        u[p1]=u[p2]=u[p3]=u[p4]=1;
        t0=t=p1;
        dir=1;
        cv=0;

        do{
          cnr=eb[t][dir];
          t=be[a[cnr]][1];
          dir=1;
          if (c2[r4][t] == 0)
          {
            t=be[a[cnr]][-1];
            dir=-1;
          }
          if (u[t] == 1) v[t]=++cv;
        } while (t != t0);

        if (v[p2] == 2) continue;
      }

      regdone[r5]=regdone[r10]=1;
        
      regnumber=12;
      if (r11 == r12  ||  r5 == r10) regnumber=11;
      if (r11 == r12  &&  r5 == r10) regnumber=10;

      j=0; for (i=1; i<=nr; i++) if (regdone[i] == 1) j++;
      regdone[r5]=regdone[r10]=0;
      if (j != regnumber) continue;

      gpnumber=6;
      e1=rm[r4][r10];
      e2=rm[r6][r10];
      e3=rm[r5][r6];
      e4=rm[r4][r5];

      if (newtransform() == 1) return 1;
    }
  }
  return 0;
}


newtransform()
{
  int i, t1, t2, t3, t4, cstretch;
  signed char stretch[150], del[150];

  cstretch=0;
  for (i=1; i<=np; i++) stretch[i]=0;
  for (i=1; i<=np; i++)
  {
    if (i==a[x1] || i==a[x2] || i==a[x3] || i==a[x4]) cstretch--;
    else
    {
      stretch[i]=cstretch;
      if (i==e1 || i==e2 || i==e3 || i==e4) cstretch++;
      if (e2==e4 && i==e2) cstretch++;
      if (e1==e4 && i==e1) cstretch++;
      if (e2==e3 && i==e2) cstretch++;
    }
  }

  t1=t2=t3=t4=1;

  if (gpnumber == 2)
  {
    if ((x2 + stretch[x2] + e2 + stretch[e2]) % 2 == 0)
    {
      t2=1;
      t4=2;
    }
    else
    {
      t2=2;
      t4=1;
    }
  }

  if (gpnumber == 6  &&  e1 == e4)
  {
    if ((x1 + stretch[x1] + e1 + stretch[e1]) % 2 == 0)
    {
      t1=1;
      t4=2;
    }
    else
    {
      t1=2;
      t4=1;
    }
  }

  if (gpnumber == 6  &&  e2 == e3)
  {
    if ((x2 + stretch[x2] + e2 + stretch[e2]) % 2 == 0)
    {
      t2=1;
      t3=2;
    }
    else
    {
      t2=2;
      t3=1;
    }
  }
    
  a1[x1 + stretch[x1]] = q[e1 + stretch[e1] + t1];
  a1[x2 + stretch[x2]] = q[e2 + stretch[e2] + t2];
  a1[x3 + stretch[x3]] = q[e3 + stretch[e3] + t3];
  a1[x4 + stretch[x4]] = q[e4 + stretch[e4] + t4];

  a1[q[e1 + stretch[e1] + t1]] = x1 + stretch[x1];
  a1[q[e2 + stretch[e2] + t2]] = x2 + stretch[x2];
  a1[q[e3 + stretch[e3] + t3]] = x3 + stretch[x3];
  a1[q[e4 + stretch[e4] + t4]] = x4 + stretch[x4];

  g1[x1 + stretch[x1]] = g[x1];
  g1[x2 + stretch[x2]] = g[x2];
  g1[x3 + stretch[x3]] = g[x3];
  g1[x4 + stretch[x4]] = g[x4];

  g1[q[e1 + stretch[e1] + t1]] = -g[x1];
  g1[q[e2 + stretch[e2] + t2]] = -g[x2];
  g1[q[e3 + stretch[e3] + t3]] = -g[x3];
  g1[q[e4 + stretch[e4] + t4]] = -g[x4];

  for (i=1; i<=np; i++) del[i]=0;
  del[a[x1]]=1; del[a[x2]]=1; del[a[x3]]=1; del[a[x4]]=1;

  for (i=1; i<=np; i++) if (del[i] == 0  &&  del[a[i]] == 0)
  {
    a1[i + stretch[i]] = a[i] + stretch[a[i]];
    g1[i + stretch[i]] = g[i];
  }

  bugtest(a1, g1, 10);
  rectify(a1, g1, a2, g2);

  reducenumber=reduce(a2, g2);
  
  if (reducenumber == 1  ||  compare(a2, g2, a0, g0) == 1)
  {
    for (i=1; i<=np; ++i)
    {
      a0[i]=a2[i];
      g0[i]=g2[i];
    }
      
    return 1;
  }

  else if (reducenumber == 0  &&  yredundancy() == 0  &&
           xredundancy(a2,g2) == 0)
  {
    npp++;
    for (i=1; i<=np; i++)
    {
      ya[npp][i] = a2[i];
      yg[npp][i] = g2[i];
    }
  }
  return 0;
}


conway()
{
  incidence();
  
  if (necirc < 4) return 0;

  for (cr1=1; cr1<=n; cr1+=2) for (cr2=1; cr2<=n; cr2+=2)
    if (g[cr1] == g[cr2]  &&  ecirc[cr1] != ecirc[cr2]
        &&  cr1 != cr2)

    if (conwayprocess() == 1) return 1;
    
  return 0;
}


conwayprocess()
{
  int i, j, i0;
  signed char bp[5], bq[5], mod4[9], regdone[100], spin[100],
      dir[100][100], impb[100], tang[100],
      cvec[100], cvecinv[100], ctb[100], cte[100], nbp[5], nbq[5],
      ejump[10][100], tjump[10][100], dihed[17][5], equat[100];

  int cr3, cr4, t1, t2, ej, bb,
      sgn1, bp1, bp2, bp3, bp4, bq1, bq2, bq3, bq4,
      e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
      e14, e15, e16, e17, e18, ctang, ej1, ej2, direc, newtang;


  /* new incidence arrays:
     cop[i,j]=k means that regions i,k are opposite wrt crossing j;
     dop[i,j]=1 means that regions i,j share a crossing.
   */

  for (i=1; i<=np; i++) spin[i]=0;
  for (i=1; i<=4; i++)  /* search for basic polyhedron */
  {
    t=0;
    for (j=1; j<=4; j++)
    {
      if (rm[c3[cr1][i]] [c3[cr2][j]] != 0) t++;
      if (c3[cr1][i] == c3[cr2][j]) return 0;
    }
    if (t != 1) return 0;
  }
  for (i=1; i<=4; i++)
  {
    bp[i]=c3[cr1][i];   /* basic polyhedral regions */
    for (j=1; j<=4; j++) if (rm[bp[i]] [c3[cr2][j]] != 0)
      bq[i]=c3[cr2][j];
  }

  for (i=1; i<=4; i++) mod4[i]=i;
  for (i=5; i<=8; i++) mod4[i]=i-4;

  for (i=1; i<=4; i++) for (j=1; j<=4; j++) dihed[i][j]=mod4[i+j-1];
  for (i=5; i<=8; i++) for (j=1; j<=4; j++) dihed[i][j]=mod4[i-j];

  for (i0=1; i0<=8; i0++)
  {
    for (i=1; i<=nr; i++) regdone[i]=0;
    for (i=1; i<=np; i++) if (equat[i] == 0) spin[i]=0;
    for (i=1; i<=4; i++) regdone[bp[i]]=1;
    for (i=1; i<=4; i++) regdone[bq[i]]=1;

    for (i=1; i<=4; i++) nbp[i]=bp[dihed[i0][i]];
    for (i=1; i<=4; i++) nbq[i]=bq[dihed[i0][i]];

    for (s1=1; s1<=nr; s1++) 
      if (regdone[s1] == 0  && 
         rm[nbp[1]] [s1] != 0  &&  rm[nbq[2]] [s1] != 0  &&
         dop[nbq[1]] [s1] != 0  &&  dop[nbp[2]] [s1] != 0)
    {

    for (i=1; i<=np; i++) ctb[i]=0;
    for (i=1; i<=np; i++) cte[i]=0;
    for (i=1; i<=np; i++) tang[i]=0;


  /* get directions of equatorial edges */

      ej=rm[nbp[1]] [s1];
      if (nbp[1] == c3[ej][1]) sgn1=-1; else sgn1=1;
      if (i0 <= 4)
        spin[ej] = f[cr1]*f[ej]*sgn1;
      else
        spin[ej] = -f[cr1]*f[ej]*sgn1;


      for (i=1; i<=4; i++)
      {
        ej=rm[bp[i]] [bq[i]];
        if (bp[i] == c3[ej][1]) sgn1=-1; else sgn1=1;
      if (i0 <= 4)
        spin[ej] = f[cr1]*f[ej]*sgn1;
      else
        spin[ej] = -f[cr1]*f[ej]*sgn1;
      }

      ej=rm[s1][nbq[2]];
      if (ej%2 == 0) 
      {
        cr3=q[ej+1];
        cr4=a[ej];
      }
      else
      {
        cr3=a[q[ej+1]];
        cr4=ej;
      }
      if (c2[nbq[1]] [q[ej+1]] == 0)
      { t=cr3; cr3=cr4; cr4=t; }     /* swap cr3, cr4 */
    
      if (g[cr3] != g[cr4]  ||  g[cr3] == g[cr2]) continue;
      if (cop[nbq[2]][cr3] == nbp[1]  ||  
          cop[nbq[2]][cr4] == nbp[1])
        continue;
      if (cop[s1][cr3] != nbq[1] || cop[s1][cr4] != nbp[2])
        continue;
      t1 = cop[nbq[2]][cr3];
      t2 = cop[nbq[2]][cr4];

      if (t1==t2) continue;
      if (kf[nbq[2]][cr3] == kf[nbq[2]][cr2]  || 
          kf[nbq[2]][cr3] != kf[nbq[2]][cr4])
        continue;

      /* test complete; may now effect transformation;
         note that cr1, cr2, cr3, cr4 are all odd. */

      bp1=nbp[4]; bp2=nbp[1];
      bp3=nbp[2]; bp4=nbp[3];
      bq1=nbq[4]; bq2=nbq[1];
      bq3=nbq[2]; bq4=nbq[3];

      for (i=1; i<=np; i++) impb[i]=1;
      impb[cr1]=0; impb[a[cr1]]=0;
      impb[cr2]=0; impb[a[cr2]]=0;
      impb[cr3]=0; impb[a[cr3]]=0;
      impb[cr4]=0; impb[a[cr4]]=0;

      /* name important edges */

      e1=rm[bp1][bp2]; e2=rm[bp2][bp3]; e3=rm[bp3][bp4];
      e4=rm[bp4][bp1]; e5=rm[bp1][bq1]; e6=rm[bp2][bq2];
      e7=rm[bp3][bq3]; e8=rm[bp4][bq4]; e9=rm[bq1][bq2];
      e10=rm[bq2][bq3]; e11=rm[bq3][bq4]; e12=rm[bq4][bq1];
      e13=rm[bq2][t1]; e14=rm[t1][s1]; e15=rm[s1][t2];
      e16=rm[t2][bp3]; e17=rm[bp2][s1]; e18=rm[bq3][s1];

      /* get directions and tangle ends */

      for (i=1; i<=5; i++) for (j=1; j<=4; j++) dir[i][j]=0;

      if (impb[e4] == 0)
      {dir[1][e4]=-1; ctb[e4]=1;}
      else
      {dir[1][e4]=1; cte[e4]=1;}
      if (spin[e5] == -1) 
      {dir[1][e5]=-1; ctb[e5]=1;} 
      else 
      {dir[1][e5]=1; cte[e5]=1;}
      if (impb[e12] == 0) 
      {dir[1][e12]=-1; ctb[e12]=1;}
      else 
      {dir[1][e12]=1; cte[e12]=1;}
      if (spin[e8] == 1) 
      {dir[1][e8]=-1; ctb[e8]=1;} 
      else 
      {dir[1][e8]=1; cte[e8]=1;}
      if (impb[e1] == 0) 
      {dir[2][e1]=-1; ctb[e1]=2;} 
      else 
      {dir[2][e1]=1; cte[e1]=2;}
      if (spin[e6] == -1) 
      {dir[2][e6]=-1; ctb[e6]=2;} 
      else 
      {dir[2][e6]=1; cte[e6]=2;}
      if (impb[e9] == 0) 
      {dir[2][e9]=-1; ctb[e9]=2;} 
      else 
      {dir[2][e9]=1; cte[e9]=2;}
      if (spin[e5] == 1) 
      {dir[2][e5]=-1; ctb[e5]=2;} 
      else 
      {dir[2][e5]=1; cte[e5]=2;}
      if (impb[e3] == 0) 
      {dir[5][e3]=-1; ctb[e3]=5;} 
      else 
      {dir[5][e3]=1; cte[e3]=5;}
      if (spin[e8] == -1) 
      {dir[5][e8]=-1; ctb[e8]=5;} 
      else 
      {dir[5][e8]=1; cte[e8]=5;}
      if (impb[e11] == 0)
      {dir[5][e11]=-1; ctb[e11]=5;} 
      else 
      {dir[5][e11]=1; cte[e11]=5;}
      if (spin[e7] == 1)
      {dir[5][e7]=-1; ctb[e7]=5;} 
      else 
      {dir[5][e7]=1; cte[e7]=5;}
      if (impb[e2] == 0) 
      {dir[4][e2]=-1; ctb[e2]=4;}
      else 
      {dir[4][e2]=1; cte[e2]=4;}
      if (impb[e16] == 0) 
      {dir[4][e16]=-1; ctb[e16]=4;} 
      else 
      {dir[4][e16]=1; cte[e16]=4;}
      if (impb[e15] == 0) 
      {dir[4][e15]=-1; ctb[e15]=4;}
      else 
      {dir[4][e15]=1; cte[e15]=4;}
      if (spin[e17] == 1) 
      {dir[4][e17]=-1; ctb[e17]=4;} 
      else 
      {dir[4][e17]=1; cte[e17]=4;}
      if (impb[e14] == 0) 
      {dir[3][e14]=-1; ctb[e14]=3;} 
      else 
      {dir[3][e14]=1; cte[e14]=3;}
      if (impb[e13] == 0) 
      {dir[3][e13]=-1; ctb[e13]=3;} 
      else 
      {dir[3][e13]=1; cte[e13]=3;}
      if (spin[e6] == 1) 
      {dir[3][e6]=-1; ctb[e6]=3;} 
      else 
      {dir[3][e6]=1; cte[e6]=3;}
      if (spin[e17] == -1) 
      {dir[3][e17]=-1; ctb[e17]=3;}
      else 
      {dir[3][e17]=1; cte[e17]=3;}

      for (i=0; i<=5; i++) for (j=1; j<=np; j++) ejump[i][j]=j;
      for (i=0; i<=5; i++) for (j=1; j<=5; j++) tjump[i][j]=0;

      ejump[1][e4]=e2;  tjump[1][e4]=4;
      ejump[1][e5]=e12;  tjump[1][e5]=0;
      ejump[1][e12]=e14; tjump[1][e12]=3;
      ejump[1][e8]=e4;  tjump[1][e8]=0;
      ejump[2][e1]=e13;  tjump[2][e1]=0;
      ejump[2][e6]=e6;  tjump[2][e6]=3;
      ejump[2][e9]=e11;  tjump[2][e9]=5;
      ejump[2][e5]=e14;  tjump[2][e5]=0;
      ejump[3][e17]=e9; tjump[3][e17]=0;
      ejump[3][e14]=e12; tjump[3][e14]=1;
      ejump[3][e13]=e1; tjump[3][e13]=0;
      ejump[3][e6]=e6;  tjump[3][e6]=2;
      ejump[4][e2]=e4;  tjump[4][e2]=1;
      ejump[4][e16]=e3; tjump[4][e16]=0;
      ejump[4][e15]=e7; tjump[4][e15]=0;
      ejump[4][e17]=e11; tjump[4][e17]=0;
      ejump[5][e3]=e16;  tjump[5][e3]=0;
      ejump[5][e8]=e2;  tjump[5][e8]=0;
      ejump[5][e11]=e9; tjump[5][e11]=2;
      ejump[5][e7]=e15;  tjump[5][e7]=0;
      ejump[0][e1]=e13;  tjump[0][e1]=3;
      ejump[0][e2]=e8;  tjump[0][e2]=5;
      ejump[0][e3]=e16;  tjump[0][e3]=4;
      ejump[0][e4]=e8;  tjump[0][e4]=1;
      ejump[0][e9]=e17;  tjump[0][e9]=3;
      ejump[0][e11]=e17;  tjump[0][e11]=4;
      ejump[0][e12]=e5;  tjump[0][e12]=1;
      ejump[0][e13]=e1;  tjump[0][e13]=2;
      ejump[0][e14]=e5;  tjump[0][e14]=2;
      ejump[0][e15]=e7;  tjump[0][e15]=5;
      ejump[0][e16]=e3;  tjump[0][e16]=5;
      ejump[0][e7]=e15;  tjump[0][e7]=4;

      /* find tangle to which each crossing belongs */

      ctang=0;
      for (i=cr1; i<=cr1+np; i++)
      {
        tang[q[i]]=ctang;
        if (ctb[q[i]] != 0) ctang=ctb[q[i]];
        else if (cte[q[i]] != 0) ctang=0;
      }

      /* get relabelling vector cvec */

      bb=cr1; direc=1;
      for (t=1; t<=np; t++)
      {
        cvec[t]=bb;
        ej1=be[bb][direc];
        ej2=ejump[tang[bb]][ej1];
        newtang=tjump[tang[bb]][ej1];
        if (newtang == 0)
        {
          if (cte[ej2] != 0) direc=1;
          if (ctb[ej2] != 0) direc=-1;
        }
        else
        {
          if (cte[ej2] == newtang) direc=-1;
          if (ctb[ej2] == newtang) direc=1;
        }
        bb=eb[ej2][direc];
        newtang=0;
      }

      for (i=1; i<=np; i++) cvecinv[cvec[i]]=i;

      /* get new sequence */

      for (i=1; i<=np; i++) a1[i] = cvecinv[a[cvec[i]]];

      /* switch crossings in tangles 1, 4, 5 */

      for (i=1; i<=np; i++)
      if (tang[cvec[i]]==1 || tang[cvec[i]]==4 || tang[cvec[i]]==5)
        g1[i]=-g[cvec[i]];
      else
        g1[i]=g[cvec[i]];

      bugtest(a1, g1, 11);

      rectify(a1, g1, a2, g2);

      reducenumber=reduce(a2, g2);

      if (reducenumber == 1  ||  compare(a2, g2, a0, g0) == 1)
      {
        for (i=1; i<=np; ++i)
        {
          a0[i]=a2[i];
          g0[i]=g2[i];
        }
      
        return 1;
      }

      else if (reducenumber == 0  &&  yredundancy() == 0  &&
               xredundancy(a2,g2) == 0)
      {

        npp++;
        for (i=1; i<=np; i++)
        {
          ya[npp][i] = a2[i];
          yg[npp][i] = g2[i];
        }
      }
    }
  }
  return 0;
}
    

plaitcheck() /*NOTE. This requires that "orient" has been done.*/
{
  int i;

  for (i=1; i<=np; i++) if (g[i] == g[q[i+1]])
  {
    i1=q[i];
    i2=q[i+1];
    p1 = c3[i1][3];
    p2 = c3[i1][4];
    if (f[i1] == f[i2])
    {
      q1 = c3[i2][2];
      q2 = c3[i2][1];
    }
    else
    {
      q1 = c3[i2][1];
      q2 = c3[i2][2];
    }
    if (rm[p1][q2] != 0  ||  rm[p2][q1] != 0) return 1;
  }
  return 0;
}


perko()
{
  if (necirc <= 5) return 0;
  if (plaitcheck() == 1) return 0;

  for (i=1; i<=np; i++) 
    if (g[i] == g[q[i+1]]) bridge[i]=g[i];
    else bridge[i]=0;  
  for (ej1=1; ej1<=np-3; ej1++) for (ej2=ej1+3; ej2<=np; ej2++)
    if ((bridge[ej1] != 0) && (bridge[ej1] == -bridge[ej2]) 
         && (c4[ej1][ej2] != 0))
    if (transform() == 1) return 0;
  return 0;
}



transform()
{
  int r1, r2, r3, r4, r5, r6, p1, p2, s1, s2, x1, x2, y1, crg;
  int dir1, dir2, cr1, cr2, cr3, cr4, gc, cups;
  int ins1, ins2, ins3, ins4, ins5, ins6, ins7;
  int rins1, rins2, rins3, rins4, rins5, rins6, rins7, rins8;
  signed char ins[150], del[150], ups[150], ends[150];

  if ((a[ej1] == ej2) || (a[ej1] == q[ej2+1])) dir1=1;
  else dir1=-1;
  if ((a[ej2] == ej1) || (a[ej2] == q[ej1+1])) dir2=1;
  else dir2=-1;
  r1=1;
  while ((c2[r1][ej1] == 0) || (c2[r1][ej2] == 0)) r1++;
  r5=1;
  while ((c2[r5][q[ej1+dir1]] == 0) || (rm[r5][r1] == 0)) 
    r5++;
  r6=1;
  while ((c2[r6][q[ej2+dir2]] == 0) || (rm[r6][r1] == 0)) 
    r6++;
  r2=opp[r1][ej1];
  r4=opp[r1][ej2];
  r3=opp[r4][q[ej1-dir1]];
  p1=opp[r5][q[ej1+dir1]];
  p2=opp[r6][q[ej2+dir2]];
  if ((p1 == p2) || (r5 == r6)) return 0;

  if (dir1 == 1)
  {
    cr1=q[ej1+1];
    cr2=ej1;
  }
  else
  {
    cr1=ej1;
    cr2=q[ej1+1];
  }
  if (dir2 == 1)
    cr3=q[ej2+1];
  else
    cr3=ej2;
  for (crg=1; crg<=n; crg+=2) if (kf[p1][crg] == -kf[p1][cr1])
  {
    if (g[crg] == g[cr1]) cr4=crg;
    else cr4=a[crg];
    if (c2[p1][cr4] == 1) 
    {
      x1=cr4;
      y1=q[cr4-1];
    }
    else 
    {
      x1=q[cr4-1];
      y1=cr4;
    }
    if (c2[p1][a[cr4]] == 1) x2=a[cr4];
    else x2=q[a[cr4]-1];
    s1=opp[p1][x1];
    s2=opp[p1][x2];
    if ((rm[s1][p2] != 0) && (rm[s2][p2] != 0) 
     && (opp[s2][y1] != p2) && (s1 != r5) && (s1 != r6)
     && (s2 != r2) && (s2 != r4))
    {
      for (i=1; i<=np; i++) ins[i]=0;
      ins1=rm[r5][r1]; ins[ins1]=1;
      ins2=rm[r6][r1]; ins[ins2]=1;
      ins3=rm[p1][r2]; ins[ins3]=1;
      ins4=rm[r2][r3]; ins[ins4]=1;
      ins5=rm[r3][r4]; ins[ins5]=1;
      ins6=rm[r4][p2]; ins[ins6]=1;
      ins7=rm[s2][p2]; ins[ins7]=1;
      for (i=1; i<=np; i++) del[i]=0;
      del[cr1]=1; del[a[cr1]]=1;
      del[cr2]=1; del[a[cr2]]=1;
      del[cr3]=1; del[a[cr3]]=1;
      del[a[cr4]]=1;
      cstretch=0;    
      for (i=1; i<=np; i++)
      {
        if (del[i] == 0)
          stretch[i]=cstretch;
        else
          stretch[i]=cstretch-1;
        if (del[i] == 1) cstretch--;
        if (ins[i] == 1) cstretch++;
      }

      for (i=1; i<=np; i++) ends[i]=0;
      ends[rm[r5][p1]]=1;
      ends[ins1]=1;
      ends[ins2]=1;
      ends[rm[r6][p2]]=1;
      ends[x1]=1;
      ends[rm[s1][p2]]=1;

      cups=0;
      for (i=ej1; i<=ej1+np; i++)
      {
        if (ends[q[i]] == 0) ups[q[i]]=cups;
        else
        {
          if (cups == 0) ups[q[i]]=cups++;
          else ups[q[i]]=cups--;
        }
      }

      for (i=1; i<=np; i++) 
        if ((del[i] == 0) && (del[a[i]] == 0))
      {
        a1[i+stretch[i]] = a[i]+stretch[a[i]];
        g1[i+stretch[i]] = g[i];
        if (ups[i] == 1) g1[i+stretch[i]]*=-1;
      } 

      gc=g[cr1];

      rins1=q[ins1+stretch[ins1]+1];
      rins2=q[ins2+stretch[ins2]+1];
      rins3=q[ins3+stretch[ins3]+1];
      rins4=q[ins4+stretch[ins4]+1];
      rins5=q[ins5+stretch[ins5]+1];
      rins6=q[ins6+stretch[ins6]+1];
      rins7=q[ins7+stretch[ins7]+1];
      rins8=q[cr4+stretch[cr4]];

      a1[rins1] = rins2;
      a1[rins2] = rins1;
      a1[rins3] = rins4;
      a1[rins4] = rins3;
      a1[rins5] = rins6;
      a1[rins6] = rins5;
      a1[rins7] = rins8;
      a1[rins8] = rins7;

      g1[rins1] = gc;
      g1[rins2] = -gc;
      g1[rins3] = gc;
      g1[rins4] = -gc;
      g1[rins5] = gc;
      g1[rins6] = -gc;
      g1[rins7] = -gc;
      g1[rins8] = gc;

      bugtest(a1, g1, 12);

      rectify(a1, g1, a2, g2);

      reducenumber=reduce(a2, g2);

      if (reducenumber == 1  ||  compare(a2, g2, a0, g0) == 1)
      {
        for (i=1; i<=np; ++i)
        {
          a0[i]=a2[i];
          g0[i]=g2[i];
        }
      
        return 1;
      }

      else if (reducenumber == 0  &&  yredundancy() == 0  &&
               xredundancy(a2,g2) == 0)
      {
        npp++;
        for (i=1; i<=np; i++)
        {
          ya[npp][i] = a2[i];
          yg[npp][i] = g2[i];
        }
      }
    }
  }
  return 0;
}


pass()
{
  int i, ej1, e1, e2, e3, e4, e5, e6, e7, e8,
           r1, r2, r3, r4, r5, r6, r7, dir, cstretch;
  signed char stretch[150], del[150];

  for (ej1=1; ej1<=np; ej1++) if (g[ej1] == g[q[ej1+1]])
    for (dir=-1; dir<=1; dir+=2)
  {
    x1=eb[ej1][-1]; x2=eb[ej1][1];
    e3=be[x1][-1]; e4=be[x2][1];
    e7=be[a[x1]][dir]; e8=be[a[x2]][dir*f[x1]*f[x2]];
    e5=be[a[x1]][-dir]; e6=be[a[x2]][-dir*f[x1]*f[x2]];

    r1=er[ej1][e6]; r2=er[e4][e6]; r3=er[e4][e8];
    r5=er[e3][e7]; r6=er[e3][e5]; r7=er[ej1][e7];

    if (r1 == r3  ||  r1 == r5) continue;

    for (r4=1; r4<=nr; r4++)
      if (r4 != r2  &&  r4 != r6  &&  r4 != r7  &&
         rm[r4][r3] != 0  &&  rm[r4][r5] != 0)
    {
      e1=rm[r4][r5]; e2=rm[r4][r3];

      cstretch=0;
      for (i=1; i<=np; i++) stretch[i]=0;
      for (i=1; i<=np; i++)
      {
        if (i==a[x1] || i==a[x2]) cstretch--;
        else
        {
          stretch[i]=cstretch;
          if (i==e1 || i==e2) cstretch++;
        }
      }
    
      a1[x1 + stretch[x1]] = q[e1 + stretch[e1] + 1];
      a1[x2 + stretch[x2]] = q[e2 + stretch[e2] + 1];

      a1[q[e1 + stretch[e1] + 1]] = x1 + stretch[x1];
      a1[q[e2 + stretch[e2] + 1]] = x2 + stretch[x2];

      g1[x1 + stretch[x1]] = g[x1];
      g1[x2 + stretch[x2]] = g[x2];

      g1[q[e1 + stretch[e1] + 1]] = -g[x1];
      g1[q[e2 + stretch[e2] + 1]] = -g[x2];

      for (i=1; i<=np; i++) del[i]=0;
      del[a[x1]]=1; del[a[x2]]=1;

      for (i=1; i<=np; i++) if (del[i] == 0  &&  del[a[i]] == 0)
      {
        a1[i + stretch[i]] = a[i] + stretch[a[i]];
        g1[i + stretch[i]] = g[i];
      }

      bugtest(a1, g1, 13);

      rectify(a1, g1, a2, g2);

      reducenumber=reduce(a2, g2);

      if (reducenumber == 1  ||  compare(a2, g2, a0, g0) == 1)
      {
        for (i=1; i<=np; ++i)
        {
          a0[i]=a2[i];
          g0[i]=g2[i];
        }
      
        return 1;
      }

      else if (reducenumber == 0  &&  yredundancy() == 0  &&
               xredundancy(a2,g2) == 0)
      {
        npp++;
        for (i=1; i<=np; i++)
        {
          ya[npp][i] = a2[i];
          yg[npp][i] = g2[i];
        }
      }
    }
  }
  return 0;
}


incidence()
{
  int i,j;
  signed char reg1, reg2, reg3, reg4;

  orient(a, f);
  nr=(n+5)/2;
  for (i=1; i<=n+1; i++)
  {
    circ[i]=0;
  }
  for (i=1; i<=nr; i++) for (j=1; j<=nr; j++)
  { 
    c3[i][j]=0;
    label[i][j]=0;
    rm[i][j]=0;
    dop[i][j]=0;
  }
  for (i=1; i<=np; i++) for (j=1; j<=np; j++)
  { 
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

  /* start to get flyping circuits.  For each odd i,
     fc indicates two flyping regions (if they exist),
     starting with region incident to edge i.
     direc1[i] indicates whether flyping circuit of
     ith crossing is parallel or antiparallel.
     mid1[i,j]=i (pole1[i,j]=1) says that jth region is a "mid" 
     (pole) region of flyping circuit of ith crossing. */

  ncirc=0;
  for (i=1; i<=n; i+=2)
  {
  r1=c3[i][1];
  r2=c3[i][2];
  r3=c3[i][3];
  r4=c3[i][4];
    for (j=1; j<=nr; j++) 
    {
      if (rm[j][r1]!=0 && rm[j][r3]!=0 &&
          r2!=j && r4!=j)
      {
        
        direc1[i]=1;
        if (label[r1][r3]==0)
        {
          ncirc++;
          label[r1][r3]=ncirc;
          label[r3][r1]=ncirc;
        }
      }
        if (rm[j][r2]!=0 && rm[j][r4]!=0 &&
            r1!=j && r3!=j)
      {
      
        direc1[i]=-1;
        if (label[r2][r4]==0)
        {
          ncirc++;
          label[r2][r4]=ncirc;
          label[r4][r2]=ncirc;
        }
      }
    }
  }   


  /* circ[i] = number of flyping circuit to which i belongs;
     ncirc = number of flyping circuits */

  for (i=1; i<=n; i+=2)
  {
    r1=c3[i][1];
    r2=c3[i][2];
    r3=c3[i][3];
    r4=c3[i][4];
    if (label[r1][r3]!=0) circ[i]=label[r1][r3];
    if (label[r2][r4]!=0) circ[i]=label[r2][r4];
  }
  for (i=1; i<=n; i+=2) circ[a[i]]=circ[i];
  necirc=ncirc;
  for (i=1; i<=n; i+=2) ecirc[i]=circ[i];

  for (i=1; i<=n; i+=2) if (ecirc[i] == 0) ecirc[i]=++necirc;
  for (i=1; i<=n; i+=2) ecirc[a[i]]=ecirc[i];
  
  return 0;
}


flype()
{
  int i, j, k;
  signed char del[150];

  incidence();

  for (i=1; i<=np; i++)
  {
    circ[i]=0;
    circ1[i]=0;
    circ2[i]=0;
    gcirc[i]=0;
    xcirc[i]=0;
    xcirc1[i]=0;
    xcirc2[i]=0;
    gxcirc[i]=0;
    nplait[i]=0;
    ntangle[i]=0;
    totactive[i]=0;
    merge[i]=0;
    stretch[i]=0;
  }
  for (i=1; i<=nr; i++) for (j=1; j<=nr; j++) plait[i][j]=0;
  for (i=1; i<=nr; i++) for (j=1; j<=nr; j++) ord[i][j]=0;
  for (i=1; i<=nr; i++) for (j=1; j<=nr; j++) for (k=1; k<=3; k++)
  {
    ins[i][j][k]=0;
    rins[i][j][k]=0;
  }
  for (i=1; i<=nr; i++) for (j=1; j<=nr; j++)
  { 
    pole[i][j]=0;
    mid[i][j]=0;
    label[i][j]=0;
    nactive[i][j]=0;
  }
  for (i=1; i<=n+1; i++) for (j=1; j<=n+1; j++)
  { 
    pole1[i][j]=0;
    mid1[i][j]=0;
    u[i][j]=0;
    tang[i][j]=0;
    tang1[i][j]=0;
    ngap[i][j]=0;
    tb[i][j]=0;
    te[i][j]=0;
  }

  /* start to get flyping circuits.
     direc1[i] indicates whether flyping circuit of
     ith crossing is parallel or antiparallel.
     mid1[i,j]=i (pole1[i,j]=1) says that jth region is a "mid" 
     (pole) region of flyping circuit of ith crossing. */

  ncirc=0;
  for (i=1; i<=n; i+=2)
  {
  r1=c3[i][1];
  r2=c3[i][2];
  r3=c3[i][3];
  r4=c3[i][4];
    for (j=1; j<=nr; j++) 
    {
      if (rm[j][r1]!=0 && rm[j][r3]!=0 &&
          r2!=j && r4!=j)
      {
        mid1[i][j]=1;
        mid1[i][r2]=1;
        mid1[i][r4]=1;
        pole1[i][r1]=r3;
        pole1[i][r3]=r1;
        direc1[i]=1;
        if (label[r1][r3]==0)
        {
          ncirc++;
          label[r1][r3]=ncirc;
          label[r3][r1]=ncirc;
        }
      }
        if (rm[j][r2]!=0 && rm[j][r4]!=0 &&
            r1!=j && r3!=j)
      {
        mid1[i][j]=1;
        mid1[i][r1]=1;
        mid1[i][r3]=1;
        pole1[i][r2]=r4;
        pole1[i][r4]=r2;
        direc1[i]=-1;
        if (label[r2][r4]==0)
        {
          ncirc++;
          label[r2][r4]=ncirc;
          label[r4][r2]=ncirc;
        }
      }
    }
  }   

  /* circ[i] = number of flyping circuit to which i belongs;
     ncirc = number of flyping circuits */

  for (i=1; i<=n; i+=2)
  {
    r1=c3[i][1];
    r2=c3[i][2];
    r3=c3[i][3];
    r4=c3[i][4];
    if (label[r1][r3]!=0) circ[i]=label[r1][r3];
    if (label[r2][r4]!=0) circ[i]=label[r2][r4];
  }
  for (i=1; i<=n; i+=2) circ[a[i]]=circ[i];
  for (i=1; i<=n; i+=2) if (circ[i] != 0) 
  {
    j=g[i];
    if (gcirc[circ[i]] == 0) gcirc[circ[i]]=g[i];
  }

  nxcirc=0;
  for (i=1; i<=n; i+=2) if (circ[i] == 0) xcirc[i]=++nxcirc;
  for (i=1; i<=n; i+=2) if (circ[i] == 0) xcirc[a[i]]=xcirc[i];
  for (i=1; i<=n; i+=2) if (circ[i] == 0) gxcirc[xcirc[i]]=g[i];

  /* pole[i,j]=1 iff jth region is a pole of ith circuit
     mid[i,j] = 1 iff jth region is a mid of ith circuit
     direc[i] = 1 iff ith circuit is parallel */

  for (i=1; i<=n; i+=2) for (j=1; j<=nr; j++)
    pole[circ[i]][j] = pole1[i][j];
  for (i=1; i<=n; i+=2) for (j=1; j<=nr; j++)
    if (mid1[i][j]==1) mid[circ[i]][j] = 1;
  for (i=1; i<=n; i+=2) if (direc1[i]!=0) 
    direc[circ[i]]=direc1[i];


  /* u[i,j] = k iff jth edge is a linking edge of the ith
     circuit and its opposite edge is k */

  for (i=1; i<=ncirc; i++) for (j=1; j<=nr; j++) if(pole[i][j]!=0)
  {
    for (k=1; k<=nr; k++) if (mid[i][k]==1)
      u[i][rm[j][k]]=rm[pole[i][j]][k];
  }

  /* find tangles (entries and exits) for each circuit.
     tb[i,j]=k means that the jth edge is an entry to a tangle
     of ith circuit, and that kth edge is corresponding.
     te[i,j]=k means ditto for exit to tangle. */

  for (i=1; i<=ncirc; i++) for (j=1; j<=n+1; j++) if (u[i][j]!=0)
  {
    if (circ[j]!=i) te[i][j]=u[i][j];
    if (circ[q[j+1]]!=i) tb[i][j]=u[i][j];
  }

  /* ntangle[i] = number of tangles in ith circuit */

  for (i=1; i<=ncirc; i++) for (j=1; j<=n+1; j++) if (tb[i][j]!=0)
    (ntangle[i])++;
  for (i=1; i<=ncirc; i++) (ntangle[i])/=2;
  
  if (ncirc == 1  &&  ntangle[1] == 0) return 0;

  /* ngap[i,j] = k means that jth edge is in 
     kth gap of ith circuit;
     nactive[i,j] is the number of active crossings in
     jth gap of ith circuit */

  for (i=1; i<=ncirc; i++)
  {
    j=0;
    k=0;
    if ((te[i][1]==0) && (u[i][1]!=0))
    {
      j=n+2;
      while (u[i][--j]!=0)
      {
        ngap[i][j]=1;
        ngap[i][u[i][j]]=1;
        (nactive[i][1])++;
      }
      j=0;
      while (u[i][++j]!=0)
      {
        ngap[i][j]=1;
        ngap[i][u[i][j]]=1;
        (nactive[i][1])++;
      }
      k=1;
    }
    while (++j<=n+1)
    {
      if ((te[i][j]!=0) && (te[i][j]>j) && (ngap[i][j]==0)) k++;
      if ((u[i][j]!=0) && (u[i][j]>j) && (ngap[i][j]==0))
      {
        ngap[i][j]=k;
        (nactive[i][k])++;
      }
      if ((u[i][j]!=0) && (u[i][j]<j) && (ngap[i][j]==0))
      {
        ngap[i][j]=ngap[i][u[i][j]];
      }
    }
  }


  /* number of crossings in each gap is one less than the
     number of edges.*/

  for (i=1; i<=ncirc; i++) for (j=1; j<=ntangle[i]; j++)
    (nactive[i][j])--;
    
  /* totactive[i] = number of active crossings in ith circuit */

  for (i=1; i<=ncirc; i++)
  {
    totactive[i]=0;
    for (j=1; j<=ntangle[i]; j++) totactive[i] += nactive[i][j];
  }

  /* obtain places where extra crossings are to be inserted,
     and where surplus crossings are to be subsequently
     deleted.  ins[i,j,1] is first edge of jth gap
     of ith circuit.  ins[i.j.2] is corresponding edge.
     Also, find edges which are simultaneously designated edges
     for gaps of different circuits.  nplait[i] is the number
     of circuits for which ith edge is designated; plait[i,j]
     (j=0, 1, 2 or 3) are the circuits associated to ith edge.*/

  for (i=1; i<=ncirc; i++) for (j=1; j<=ntangle[i]; j++)
  {
    k=1;
    while ((te[i][k]==0) || (ngap[i][k]!=j)) k++;
    k1=te[i][k];
    ins[i][j][1]=k;
    ins[i][j][2]=k1;
    (nplait[k])++;
    (nplait[k1])++;
    plait[k][nplait[k]]=i;
    plait[k1][nplait[k1]]=i;
  }

  /* tang[i,j]=k means that the jth edge is in the kth
     tangle of the ith circuit. */ 

  for (i=1; i<=ncirc; i++)
  {
    j=1;
    while (tb[i][j]==0) j++;
    j0=j;
    k=1;
    dir=1; 
    tangmin[i]=1;
    for (j=j0; j<=n+j0; j++)
    {
      j1=q[j];
      if (u[i][j1]==0) tang[i][j1]=k;
      if (te[i][j1]!=0)
      {
        j2=j1-1;
        while (tb[i][q[j2]]==0) j2--;
        if (tb[i][q[j2]] == j1) dir*=-1;
        if (dir==-1) tangmin[i]=--k; else k++;
      }
    }
    for (j=1; j<=n+1; j++) if (u[i][j]==0)
    {
      tang[i][j]-=tangmin[i];

      while (tang[i][j] >= ntangle[i]) tang[i][j]-=ntangle[i];

      tang[i][j]+=1;
    }
  }

  /* find order of circuits along edges with multiple
     designations. */

  for (i=1; i<=n+1; i++) if (nplait[i] > 1)
  {
    for (j=1; j<=nplait[i]; j++) for (k=1; k<=nplait[i]; k++)
      ord[j][k]=0;
    j=1;
    while ((circ[i]!=plait[i][j]) && (j<=nplait[i])) j++;
    if (j <= nplait[i])
    {
      for (k=1; k<=nplait[i]; k++) if (k!=j)
      {
        ord[j][k]=1;
        ord[k][j]=-1;
      }
    }
    j=1;
    while ((circ[q[i+1]]!=plait[i][j]) && (j<=nplait[i])) j++;
    if (j <= nplait[i])
    {
      for (k=1; k<=nplait[i]; k++) if (k!=j)
      {
        ord[j][k]=-1;
        ord[k][j]=1;
      }
    }
    for (j=1; j<=nplait[i]-1; j++) for (k=j+1; k<=nplait[i]; k++)
      if (ord[j][k]==0)
    {
      cc1=plait[i][j];
      cc2=plait[i][k];
      e1=q[u[cc1][i] - direc[cc1]];
      e2=q[i-1];
      if (tang[cc2][e1] != tang[cc2][e2])
      {
        ord[k][j]=1;
        ord[j][k]=-1;
      }
      else
      {
        ord[k][j]=-1;
        ord[j][k]=1;
      }
    }

    for (j=1; j<=nplait[i]; j++)  perm[j]=j;
    outoforder=1;
    while (outoforder == 1)
    {
      outoforder=0;
      for (j=1; j<=nplait[i]-1; j++)
        if (ord[perm[j]][perm[j+1]] == -1)
      {
      outoforder=1;
        j1=perm[j];
        perm[j]=perm[j+1];
        perm[j+1]=j1;
      }
    }
    for (j=1; j<=nplait[i]; j++) pt[j]=plait[i][j];
    for (j=1; j<=nplait[i]; j++) plait[i][j]=pt[perm[j]];
  }

  /* New blobs.  stretch[i] = amount by which ith edge is
     advanced.  rins[i, j, 1 or 2] : number of first (second)
     insert of jth gap of ith circuit. */


  cstretch=0;
  for (i=1; i<=n+1; i++)
  {
    stretch[i]=cstretch;
    if (nplait[i] != 0)
    {
      for (j=1; j<=nplait[i]; j++)
      {
        cc1=plait[i][j];
        gap1=ngap[cc1][i];
        nblobs = totactive[cc1] - nactive[cc1][gap1];
        if (ins[cc1][gap1][1] == i) 
          rins[cc1][gap1][1]=i+cstretch;
        else
          rins[cc1][gap1][2]=i+cstretch;
        cstretch+=nblobs;
      }
    }
  }

  la1t=cstretch;

  /* pair off blobs in array a1 */

  for (i=1; i<=ncirc; i++) for (j=1; j<=ntangle[i]; j++)
  {
    r1=rins[i][j][1];
    r2=rins[i][j][2];
    nblobs=totactive[i] - nactive[i][j];
    for (k=1; k<=nblobs; k++) if (direc[i] == 1)
    {
      a1[r1+k]=r2+k;
      a1[r2+k]=r1+k;
      circ1[r1+k]=i;
      circ1[r2+k]=i;
    }
    else
    {
      a1[r1+k]=r2+nblobs-k+1;
      a1[r2+nblobs-k+1]=r1+k;
      circ1[r1+k]=i;
      circ1[r2+nblobs-k+1]=i;
    }
  }
  la1=(n+1) + la1t;

  /* complete rest of master sequence a1. 
     la1= length of sequence a1.*/

  for (i=1; i<=n+1; i++) 
  {
    a1[i+stretch[i]] = a[i]+stretch[a[i]];
    circ1[i+stretch[i]] = circ[i];
    xcirc1[i+stretch[i]] = xcirc[i];
  }

  for (ccirc=1; ccirc<=ncirc; ccirc++)
  {
    row=1;
    nplaces=ntangle[ccirc];
    nobjects=totactive[ccirc];
    distribute();
    ndist[ccirc]=row-1;
  }

  row=1;
  pos=ncirc;
  vecgen();
  nvec=row-1;
  lknottemp=lknot+1;
  
  if (nvec <=1)
  {   
    rectify(a, g, a3, g3);
    bugtest(a3, g3, 17);
    if (xredundancy(a3, g3) == 0)
    {
      if (lknot < max_class_size[nc]) xinsert(a3, g3);
      else return 2;
    }
  }
  
  for (i=1; i<=nvec; i++) if (nvec > 1)
  {
    for (j=1; j<=la1; j++) del[j]=0;
    for (j=1; j<=la1; j++) shrink[j]=0;
    cshrink=0;
    for (j=1; j<=ncirc; j++) for (k=1; k<=ntangle[j]; k++)
    {
      ndel = totactive[j] - dist[j][vec[i][j]][k];
      r1=rins[j][k][1];
      for (l=1; l<=ndel; l++)
      {
        del[r1+l]=1;
        del[a1[r1+l]]=1;
      }
    }
    for (j=1; j<=la1; j++)
      if (del[j] == 1) cshrink++;
      else shrink[j]=cshrink;
    for (j=1; j<=la1; j++) if (del[j] == 0)
    {
      a2[j-shrink[j]] = a1[j] - shrink[a1[j]];
      circ2[j-shrink[j]] = circ1[j];
      xcirc2[j-shrink[j]] = xcirc1[j];
    }
    for (j=1; j<=n; j+=2) if (circ2[j] != 0) 
      g2[j]=gcirc[circ2[j]];
    for (j=1; j<=n; j+=2) if (circ2[j] == 0) 
      g2[j]=gxcirc[xcirc2[j]];
    for (j=1; j<=n; j+=2) g2[a2[j]]=-g2[j];

    rectify(a2, g2, a3, g3);
    reducenumber=reduce(a3, g3);
    
    bugtest(a3, g3, 15);
       
    if (reducenumber == 1  ||  compare(a3, g3, a0, g0) == 1)
    {

      for (i=1; i<=np; ++i)
      {
        a0[i]=a3[i];
        g0[i]=g3[i];
      }
      
      return 1;
    }

    else if (reducenumber == 0  &&  xredundancy(a3,g3) == 0)
    {

      if (lknot < max_class_size[nc]) xinsert(a3, g3);
      else return 2;

    }
  }

  return 0;
}





mod0(int x)
{
  int i;
  i=x%(n+1);
  if (i<=0) i=i+n+1;
  return i;
}    

imax(int x, int y)
{
  if (x<y) return y; else return x;
}

imin(int x, int y)
{
  if (x<y) return x; else return y;
}

    
orient(signed char *a, signed char *f)
{
  signed char e[100], g[100], h[100];
  int i, s, t;
  
  knit=0;
  for (i=1; i<=n+1; i+=1)
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
    for (i=t+1; i<=n+1; i+=1)
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
    for (i=a[t]+1; i<=n+1; i+=1) g[i]=1;
    s=0;i=1;
    while ((s==0) && (i<=n+1))
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
          knit=1;
          return 0;
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
            knit=1;
            return 0;
          }
        }
        else
        {
          f[s]=e[s]*e[a[s]]*f[t];
          f[a[s]]=-f[s];
          if (((s==1) && (abs(a[n+1]-a[1])==1)) ||
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
      while ((s==0) && (i<=n+1))
      {
        if (g[i]==1) s=i;
        i=i+1;
      }
    }
    h[t]=0;
    h[a[t]]=0;
    t=0;
    i=1;
    while ((t==0) && (i<=n+1))
    {
      if (h[i]==1) t=i;
      i=i+1;
    }
  }
  
  return 0;
}


primeness()
{
  int i, j, k;
  for(i=1; i<=n-1; i++) for (j=i+1; j<=n; j++)
  {
    comp=1;
    for (k=i; k<=j; k++) if ((a[k]<i) || (a[k]>j)) comp=0;
    if (comp==1) return;
  }
  
  return 0;
}   
shift(signed char *w, signed char *x, signed char *y, signed char *z, int m)
{
   int i;
   for (i=1; i<=np-m; i++) 
      if (*(w+i+m)>m) *(y+i)=*(w+i+m)-m; 
      else *(y+i)=*(w+i+m)-m+np;
   for (i=np+1-m; i<=np; i++)
      if (*(w+i+m-np)>m) *(y+i)=*(w+i+m-np)-m;
      else *(y+i)=*(w+i+m-np)-m+np;
   for (i=1; i<=np; i++) 
      if (i<np+1-m) *(z+i)=*(x+i+m); else *(z+i)=*(x+i+m-np);
   return 0;
}

antishift(signed char *w, signed char *x, signed char *y, signed char *z, int m)
{
   int i;
   for (i=1; i<=m-1; i++) 
      if (*(w+m-i)<m) *(y+i)=m-*(w+m-i); 
      else *(y+i)=np+m-*(w+m-i);
   for (i=m; i<=np; i++) 
      if (*(w+np+m-i)<m) *(y+i)=m-*(w+np+m-i); 
      else *(y+i)=np+m-*(w+np+m-i);
   for (i=1; i<=np; i++) 
      if (i<m) *(z+i)=*(x+m-i); else *(z+i)=*(x+m-i+np);
   return 0;
}

compare(signed char *w, signed char *x, signed char *y, signed char *z)
{
  int i;
  for (i=1; i<=n-2; i+=2) if (*(w+i)!=*(y+i))
    {
      if (*(w+i)<*(y+i)) return 1; else return -1;
    }
  for (i=3; i<=n; i+=2) 
    if ((*(x+i))*(*(x+1)) != (*(z+i))*(*(z+1)))
    {
      if ((*(x+i))*(*(x+1)) < (*(z+i))*(*(z+1))) return 1;
      else return -1;
    }       
  return 0;
}


rectify(signed char *w, signed char *x, signed char *y, signed char *z)
{
  int d, d1, d2, i, j;
  signed char u[100], v[100], u1[100], v1[100];

  for (i=1; i<=np; i++)
  {
    u1[i]=w[i];
    v1[i]=x[i];
  }
  
  d1=*(w+1)-1;
  d2=np-d1;
  d=imin(d1, d2);
  
  for (i=1; i<=np; i++)
  {
    d1=q[*(w+i)-i];
    d2=np-d1;
    if (d1 <= d)
    {
      shift(w, x, u, v, i-1);
      if (compare(u1, v1, u, v) == -1) for (j=1; j<=np; j++)
      {
        u1[j]=u[j];
        v1[j]=v[j];
      }
    }
    if (d2 <= d)
    {
      antishift(w, x, u, v, i+1);
      if (compare(u1, v1, u, v) == -1) for (j=1; j<=np; j++)
      {
        u1[j]=u[j];
        v1[j]=v[j];
      }
    }
    if (d > imin(d1, d2)) d=imin(d1, d2);
  }
  if (*(v1+1) == -1)
    for (i=1; i<=np; i++) (*(v1+i))*=-1;
  for (i=1; i<=np; i++)
  {
    *(y+i)=*(u1+i);
    *(z+i)=*(v1+i);
  }
  bugtest(y, z, 16);
  return 0;
}


qrectify(signed char *w, signed char *x)
{
  int d, d1, d2, i;
  signed char u[100], v[100];
  
  d1=*(w+1)-1;
  d2=np-d1;
  d=imin(d1, d2);
  
  for (i=1; i<=np; i++)
  {
    d1=q[*(w+i)-i];
    d2=np-d1;
    if (d1 <= d)
    {
      shift(w, x, u, v, i-1);
      if (compare(w, x, u, v) == -1) return 1;
    }
    if (d2 <= d)
    {
      antishift(w, x, u, v, i+1);
      if (compare(w, x, u, v) == -1) return 1;
    }
  }
  return 0;
}


distribute()
{
  int i;
  
  if (nplaces==1)
  {
    dist[ccirc][row][1]=nobjects;
    for (i=1; i<=5; i++) dist[ccirc][row+1][i]=dist[ccirc][row][i];
    row++;
  }
  else 
  {
    for (i=0; i<=nobjects; i++)
    {
      dist[ccirc][row][nplaces]=i;
      (nplaces)--;
      nobjects-=i;
      distribute();
    }
  }
  (nplaces)++;
  nobjects+=dist[ccirc][row][nplaces];
  return 0;
}


vecgen()
{
  int i,j;

  for (i=1; i<=ndist[pos]; i++)
  {
    vec[row][pos]=i;
    if (pos==1)
    {
      row++;
      for (j=pos+1; j<=ncirc; j++) vec[row][j]=vec[row-1][j];
    }
    else
    {
      pos--;
      vecgen();
    }
  }
  pos++;
  return 0;
}


bugtest(signed char *x, signed char *y, int nproc)
{
  int i, t;
  signed char u[100], v[100], w[100];

  t=0;
  for (i=1; i<=np; i++) u[i]=v[i]=w[i]=0;
  for (i=1; i<=np; i++) u[i]=x[i];
  for (i=1; i<=np; i++) v[i]=y[i];
  for (i=1; i<=np; i++) if (u[i] == 0) t=1;
  for (i=1; i<=np; i++) if ((i+u[i])%2 != 1) t=1;
  for (i=1; i<=np; i++) w[u[i]]=1;
  for (i=1; i<=np; i++) if (w[i] == 0) t=1;
  for (i=1; i<=np; i++) if (u[i] > np) t=1;
  for (i=1; i<=np; i++) if (abs(v[i]) != 1) t=1;
  if (t == 1)
  {
    printf("bug found; procedure %3d, cknot=%5d, lknot=%5d\n", nproc, cknot, lknot);
    for (i=1; i<=np; i++) printf("%4d", u[i]);
    printf("\n");
    for (i=1; i<=np; i++) printf("%4d", v[i]);
    printf("\n");
    printf("ag\n");
    for (i=1; i<=n; i+=2) printf("%4d", a[i]*g[i]);
    printf("\n");
    exit(0);
  }
  return 0;
}


algtest()

{
  int i, j, bp;
  int t, ne, nrdone;
  signed char rdone[100], edone[100];

  t=1;
  nrdone=0;
  for (i=1; i<=nr; i++) rdone[i]=0;
  for (i=1; i<=np; i++) edone[i]=0;

  while (t==1)
  {
    t=0;
    for (i=1; i<=nr; i++) if (rdone[i] == 0)
    {
      ne=0;
      for (j=1; j<=np; j++) if (edone[j]==0 && c2[i][j]==1) ne++;
      if (ne <= 2)
      {
        t=1;
        nrdone++;
        rdone[i]=1;
        for (j=1; j<=np; j++) if (edone[j]==0 && c2[i][j]==1)
          edone[j]=1;
      }
    }
  }

  if (nrdone == nr) return 1; 
  else 
  {
    for (i=1; i<=nr; i++) regvalence[i]=0;
    for (i=1; i<=nr; i++) for (j=1; j<=np; j++)
      if (c2[i][j] == 1  &&  edone[j] == 0) (regvalence[i])++;
    bp=nr-nrdone-2;
    return bp;
  }
}


setmod()
{
  int i;

  n=2*nc-1;
  np=2*nc;
  nr=nc+2;
  
  /* incidence arrays:
     be[i,dir] gives edge next to ith blob, in direction dir;
     eb[i,dir] gives blob next to ith edge, in direction dir.
  */
       
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
  
  return 0;
}

  
          
      
    
  
  
  

  
  
  

