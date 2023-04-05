#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <draw.h>


int a0[200], b1[200][3], c2[200][200], c3[200][200], c4[200][200], 
    cm[200][200], cop[200][200], dop[200][200], er[200][200], f[200], g0[200], 
    kf[200][200], md[500], opp[200][200],
    rm[200][200],  rot1[200][200], rot2[200][200], xeb[200][3], xbe[200][3],
    twoedge[200], nb[1000][200], vertex[200], newreg[200],
    nodedone[200], ivertex[200];

int *a, *g, *q, *eb[200], *be[200];

int dd, dd1, r, n, knit, knotid, dir, i, j, k, l, m, t, np,
    cedge, nedge, reg, fedge, node, edge1, edge2,
    reg1, reg2, numnodes, numnodes1, valence, cblob, prevedge,
    prevdir, nbleft, nextindex, previndex;



triang(infile, outfile)
char *infile, *outfile;

{
  FILE *fp, *gp;

  fp=fopen(infile, "r"); 
  gp=fopen(outfile, "w");
  
  fscanf(fp, "%3d %8d", &nc, &knotid);
  
  np=2*nc;
  n=np-1;
  nr=nc+2;
  a=a0;
  g=g0;
  
  for (i=1; i<=5*nc; ++i) for (j=0; j<=100; ++j) nb[i][j] = 0;
  for (i=1; i<=5*nc; ++i) nodedone[i] = 0;
           
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
  
  for (i=1; i<=n; i+=2) fscanf(fp, "%4d", a+i);
  
  fclose(fp);

  for (i=1; i<=n; i+=2) a[i]=abs(a[i]);
  
  for (i=1; i<=n; i+=2)
  {
    a[a[i]]=i; g[i]=1; g[a[i]]=-1;
  }
  
  for (i=1; i<=n; i+=2) vertex[i] = vertex[a[i]] = (i+1)/2;
    
  incidence();
  
  /* for each region, get list of incident edges in counter-clockwise order */
  /* last edge of list = first edge of list */
  
  for (reg=1; reg<=nr; ++reg)
  {
    cedge=nedge=0;
    while(c2[reg][++cedge] == 0);
    ccr[reg][++nedge]=fedge=cedge;
    dir = g[cedge]*kf[reg][cedge];
    do {
       node = eb[cedge][dir];
       if (c2[reg][a[node]] == 1)
         cedge = a[node];
       else
         cedge = q[a[node] - 1];
       ccr[reg][++nedge] = cedge;
       dir = g[cedge]*kf[reg][cedge];
    } while (cedge != fedge);
    ccr[reg][0] = nedge-1;    
  }
  
  /* rot1[i][j]=k means that edge k follows edge j clockwise on region i */
  /* rot2[i][j]=k means that edge j follows edge k clockwise on region i */
  
  for (reg=1; reg<=nr; ++reg)
  {
    for (i=1; i<=ccr[reg][0]; ++i)
    {
      edge1 = ccr[reg][i];
      edge2 = ccr[reg][i+1];
      rot1[reg][edge1] = edge2;
      rot2[reg][edge2] = edge1;
    }
  }

  /* keep tally of which nodes have been done */
  
  for (i=1; i<=2*nc; ++i) nodedone[i]=0;
  
  /* choose infinite region, if not already chosen */
  
  if (infinitereg == 0)
  {  
    nedge = 2;

    for (reg=1; reg<=nr; ++reg) if (ccr[reg][0] > nedge)
    {
      infinitereg = reg;
      nedge = ccr[reg][0];
    }
  }

  /* renumber regions, for purpose of numbering barycenters */
  
  t=0;
  for (reg=1; reg<=nr; ++reg) if (reg != infinitereg && ccr[reg][0] != 2)
    newreg[reg] = ++t;
    
  /*   record total number of nodes, including extra layer on outside */
  
  numnodes = 3*nc + t + ccr[infinitereg][0];
  numnodes1 = 3*nc + t;
  
  /*   identify edges adjacent to 2-sided regions */
  
  for (reg=1; reg<=nr; ++reg) if (ccr[reg][0] == 2)
    for (i=1; i<=2; ++i) twoedge[ccr[reg][i]] = reg;
  
  /*   assemble lists of neighbors */
  
  for (i=1; i<=nc; ++i) nb[i][0] = 0;
  
  /*
   *   deal first with infinite region; extra nodes are numbered starting
   *   at numnodes1 + 1
   */
  
  /*   nodes of type 1 are crossings */
  
  nedge = ccr[infinitereg][0];
    
  for (i=1; i<=nedge; ++i)
  {
    cedge = ccr[infinitereg][i];
    dir = g[cedge]*kf[infinitereg][cedge];
    prevedge = rot2[infinitereg][cedge];
    prevdir = g[prevedge]*kf[infinitereg][prevedge];
    cblob = eb[cedge][-dir]; /* crossing at cc end of oriented edge */
    ivertex[i] = vertex[cblob];
    node = vertex[cblob];
    nodedone[node] = 1;
    
    nb[node][0] = 5;
    nb[node][1] = nc + cedge;
    nb[node][2] = nc + q[prevedge + prevdir];
    nb[node][3] = nc + be[cblob][-dir];
    nb[node][4] = nc + prevedge; 
    nb[node][5] = numnodes1 + i;
  }
  
  /* type 2 nodes adjacent to infinite region */
  
  for (i=1; i<=nedge; ++i)
  {    
    cedge = ccr[infinitereg][i];
    node = nc + cedge;
    nodedone[node] = 1;
    dir = g[cedge]*kf[infinitereg][cedge];
    reg2 = opp[infinitereg][cedge];
    if (i == nedge) 
    {
      nbleft = numnodes1 + 1; 
    }
    else
    {
      nbleft = numnodes1 + i + 1;
    }
    
    if (ccr[reg2][0] == 2)         /* case where reg2 has 2 sides */
    {      
      nb[node][0] = 5;
      nb[node][1] = nbleft;  
      nb[node][3-dir] = vertex[q[cedge+1]];
      nb[node][3+dir] = vertex[cedge];
      nb[node][3] = nc + rot1[reg2][cedge];
      nb[node][5] = numnodes1 + i;
    }

    else
    {     
      nb[node][0] = 7;
      nb[node][1] = nbleft;
      nb[node][4-2*dir] = vertex[q[cedge+1]];
      nb[node][4+2*dir] = vertex[cedge];
      nb[node][3] = nc + rot2[reg2][cedge];
      nb[node][4] = 3*nc + newreg[reg2];      /* barycenter */
      nb[node][5] = nc + rot1[reg2][cedge];
      nb[node][7] = numnodes1 + i;
    }
  }

  /* outermost nodes (isolating knot from boundary) */
  
  for (i=1; i<=nedge; ++i)
  {
    node = numnodes1 + i;
    
    if (i == 1) previndex = nedge; else previndex = i-1;
    if (i == nedge) nextindex = 1; else nextindex = i+1;
    
    nb[node][0] = 4;
    nb[node][1] = numnodes1 + nextindex;
    nb[node][2] = nc + ccr[infinitereg][i];
    nb[node][3] = ivertex[i];
    nb[node][4] = nc + ccr[infinitereg][previndex];
    nb[node][5] = numnodes1 + previndex;    
  }

  /* nodes of type 1 are crossings */
  
  for (i=1; i<=n; i+=2) if (nodedone[vertex[i]] == 0)
  {
    node = vertex[i];
    nb[node][0] = 4;
    nb[node][1] = nc + i;
    nb[node][3] = nc + q[i-1];
    nb[node][3+f[i]] = nc + a[i];
    nb[node][3-f[i]] = nc + q[a[i]-1];      
  }
  
  /* now do nodes of type 2, which are midpoints of edges */
  
  /* now do interior type 2 nodes */
  
  for (cedge=1; cedge<=np; ++cedge) if (nodedone[nc+cedge] == 0)
  {
    node = nc + cedge;
    nb[node][0] = 8;
    reg1 = 0;
    while (c2[++reg1][cedge] == 0);
    reg2 = opp[reg1][cedge];
    
    /* if one of these regions is 2-sided, make sure it's reg2 */
    
    if (ccr[reg1][0] == 2)
    {
      k=reg1; reg1=reg2; reg2=k;
    }
    
    dir = g[cedge]*kf[reg1][cedge]; 
       
    /* deal with 2-sided case */

    if (ccr[reg2][0] == 2)
    {
      nb[node][0] = 6;
      nb[node][1] = nc + rot1[reg1][cedge];
      nb[node][3-dir] = vertex[q[cedge+1]];
      nb[node][3+dir] = vertex[cedge];
      nb[node][3] = nc + rot2[reg2][cedge];
      nb[node][5] = nc + rot2[reg1][cedge];
      nb[node][6] = 3*nc + newreg[reg1];    /* barycenter of reg1 */
    }
    
    else
    {
      nb[node][0] = 8;
      nb[node][1] = nc + rot1[reg1][cedge];
      nb[node][4-2*dir] = vertex[q[cedge+1]];
      nb[node][4+2*dir] = vertex[cedge];
      nb[node][3] = nc + rot2[reg2][cedge];
      nb[node][4] = 3*nc + newreg[reg2];      /* barycenter of reg2 */
      nb[node][5] = nc + rot1[reg2][cedge];
      nb[node][7] = nc + rot2[reg1][cedge];
      nb[node][8] = 3*nc + newreg[reg1];
    }
  }
  
  /* finally, do barycenters */
  
  for (reg=1; reg<=nr; ++reg) if (reg != infinitereg && ccr[reg][0] != 2)
  {
    node = 3*nc + newreg[reg];
    nedge = ccr[reg][0];
    nb[node][0] = nedge;
    for (i=1; i<=nedge; ++i)
      nb[node][nedge+1-i] = nc + ccr[reg][i];
  } 
  
  /* put extra entry for interior nodes */
  
  for (i=1; i<=numnodes; ++i)
  {
    valence = nb[i][0];
    if (nb[i][valence+1] == 0) nb[i][valence+1] = nb[i][1];
  }

  /* output results */
  
  fprintf(gp, "NODECOUNT:%4d\n", numnodes);
  fprintf(gp, "ALPHA/BETA/GAMMA: 1 %3d %3d\n", numnodes1+1, vertex[a[2]]);
  fprintf(gp, "GEOMETRY: hyperbolic\n");
  fprintf(gp, "FLOWERS:\n\n");
  for (i=1; i<=numnodes; ++i)
  {
    fprintf(gp, "%3d%5d   ", i, nb[i][0]);
    for (j=1; j<=nb[i][0]+1; ++j) fprintf(gp, "%5d", nb[i][j]);
    fprintf(gp, "\n");
  }
  fprintf(gp, "END\n");
  
  fclose(gp);
  
  return 0;
}

mod0(x)
int x;
{
  int i;
  i=x%(n+1);
  if (i<=0) i=i+n+1;
  return i;
}    

orient()
{
  int e[200], g[200], h[200], i, s, t;
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
            knit=1;
            return;
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
}


incidence()
{
  register int i,j;
  int reg1, reg2, reg3, reg4;

  orient();
  nr=(n+5)/2;

  for (i=1; i<=nr; i++) for (j=1; j<=nr; j++)
  { 
    c3[i][j]=0;
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

/*printf("orientations\n");
for (i=1; i<=np; i++) printf("%2d", f[i]*g[i]);
printf("\n");


printf("f\n");
for (i=1; i<=np; i++) printf("%2d", f[i]);
printf("\n");


printf("g\n");
for (i=1; i<=np; i++) printf("%2d", g[i]);
printf("\n");

printf("c2\n");
for (i=1; i<=nr; i++)
{
  for (j=1; j<=np; j++) printf("%2d", c2[i][j]);
  printf("\n");
}

printf("c3\n");
for (i=1; i<=np; i++)
{
  for (j=1; j<=4; j++) printf("%2d", c3[i][j]);
  printf("\n");
}
printf("\n");

printf("kf\n");
for (i=1; i<=nr; i++)
{
  for (j=1; j<=np; j++) printf("%2d", kf[i][j]);
  printf("\n");
}
printf("\n");

printf("opp\n");
for (i=1; i<=nr; i++)
{
  for (j=1; j<=np; j++) printf("%2d", opp[i][j]);
  printf("\n");
}
printf("\n");*/

}




      
    
  
  
  

  
  
  

