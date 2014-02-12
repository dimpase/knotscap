#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *fp1, *fp2, *fp3, *gp;

int   x[7][40][80],
      r[100][100], g[100], h[100], u[100], v[100],
      rt[100], lon[200], a[100], b1[100], c1[100], e1[100],
      f[100], hom[50], hom1[50], conj[50][50], inner[200],
      im[200], inv[200], m1[50][50], m2[50], m3[50], homvec[50],
      ninner[50], done[100], nvec[5], ovorder[50],
      a[100], over[100], span[100], phi[100], phivec[100],
      gs[100], md1[500], md2[500], gen[100], reldone[100], psi[100],
      w1[100][100], w2[100][100], nw1[50], nw2[50], s7[1000][12],
      s5[121][6], s6[721][7], nrc[50], ninn[50], tor[100],
      inn[20][200], rc[20][1000][50], deg[100], mult[150][150],
      conjsize[7]={0,10,20,30,24,15,20},
      conje[6] = {1, 11, 31, 61, 85, 100},
      cl[7] = {0, 11, 5, 3, 4, 7, 5},
      mat[450][250],
      classchoice[50], plongi[50], cperm[50], newperm[50],
      grouptype[200],
      
      b, c, d, e, i, j, k, l, l11, l21, longi, lonl, m, n,
      r1, r2, r3, r4, r5, s, sn, t, tt, mm, nk, ck, jk, jx, nc,
      a1, a2, a3, a4, a5, a6, a7, try, test, nrep, totrep,
      bridge, np, ns, nk, totrep, np, ns, cvr, nrel, cr, ctry,
      nknots, cknot, pos, nplaces, row, cspan, mnumber,
      cb, ce, class, scnd, crel, writhe, imsize,
      homtype, knotcount, ntor, nrow, ncol, l1, l2,
      cbs, nhom, wa,
      
      nclasschoice, wh1, danger, dir1, over1, quickdist,
      datacheck, count, result,

      *q1, *q2;
      

      
char  cycles[150][100], *groupdesc[8]={"", "S3", "D5", "A4", "A(1,5)", "S4", "A5", "S5"},
      (*vecm)[4], (*vec[4])[4];
   
extern char *malloc();


main(int argc, char *argv[])
{
  int i, j, k;
  int i1;
  char ch;

  fp1=fopen(argv[1], "r");
  fp2=fopen(argv[2], "r");
  fp3=fopen(argv[3], "r");
  gp =fopen(argv[4], "w");

  vecm = (char (*)[4]) malloc(512000 * sizeof(char));
  vec[0] = vecm;
  vec[1] = vecm + 2;
  vec[2] = vecm + 52;
  vec[3] = vecm + 2552;  

  totrep=knotcount=0;

  for (i=1; i<=120; i++) for (j=1; j<=5; j++)
    fscanf(fp1, "%d", s5[i]+j);

  for (i=1; i<=120; i++) for (j=1; j<=120; j++)
    fscanf(fp1, "%d", mult[i] + j);

  for (i=1; i<=120; i++) fscanf(fp1, "%d", inv+i);

  for (i=1; i<=6; i++) for (j=1; j<=conjsize[i]; j++)
    for (k=1; k<=2*conjsize[i]; k++)
    fscanf(fp1, "%d", x[i][j]+k);

  for (i=1; i<=6; i++)
  {
    fscanf(fp1, "%d", nrc+i);
    for (j=1; j<=conjsize[i]; j++) for (k=1; k<=nrc[i]; k++)
      fscanf(fp1, "%d", rc[i][j]+k);
    fscanf(fp1, "%d", ninn+i);
    for (j=1; j<=ninn[i]; j++) fscanf(fp1, "%d", inn[i]+j);
  }

  grouptype[6]=1; grouptype[10]=2; grouptype[12]=3; grouptype[20]=4;
  grouptype[24]=5; grouptype[60]=6; grouptype[120]=7;

  for (i=1; i<=10; ++i) classchoice[i]=0;

  fscanf(fp3, "%d", &nclasschoice);
  
  for(i=1; i<=nclasschoice; ++i) 
  {
    fscanf(fp3, "%d", &i1);
    classchoice[i1]=1;
  }

  fscanf(fp3, "%d", &wh1);
  
  nrep=0;
  nknots=1;

  for (i=1; i<=120; ++i) fgets(cycles[i], 30, fp2);
  
  for (i=1; i<=120; ++i) for (j=0; j<=20; ++j) 
  if (cycles[i][j]==10) cycles[i][j]=0;

  for (i=1; i<=6; i++) deg[i]=5;
  for (i=7; i<=10; i++) deg[i]=6;

  while (fscanf(fp3, "%d", &nc) != EOF)
  {

    if (nc < 3 || nc > 49)
    {
      fprintf(gp, "crossing-number out of range\n");
      exit(0);
    }
    process();
  }
  return 0;
}


process()
{
  register int i, j;
  int i1;

  np=2*nc;
  ns=np-1;
  
  for (nplaces=1; nplaces<=3; nplaces++)
  {
    pos=nplaces;
    row=1;
    vecgen();
    nvec[nplaces]=row-1;
  }
  nvec[0]=1;
  
  for (i=1; i<=49; i++) homvec[i]=0;
  knotcount++;
  
  fscanf(fp3, "%8d", &cknot);

 
  q1=md1+nc;  /* q1 maps x to x mod nc, with 0 replaced by nc */
  for(i=-nc; i<=3*nc; i++) 
  {
    j=i%nc;
    if (j <= 0) j+=nc;
    *(q1+i)=j;
  }
 
  q2=md2+np;  /* q2 maps x to x mod np, with 0 replaced by np */
  for(i=-np; i<=3*np; i++) 
  {
    j=i%np;
    if (j <= 0) j+=np;
    *(q2+i)=j;
  }
  
  for (i=1; i<=np; ++i) a[i]=0;

  for (i=1; i<=ns; i+=2) fscanf(fp3, "%4d", a+i);

  for (i=1; i<=ns; i+=2)
  {
    if (a[i] > 0) g[i]=1; else g[i]=-1;
    a[i] = abs(a[i]);
  } 

  for (i=1; i<=ns; i+=2)
  {
    a[a[i]] = i;
    g[a[i]] = -g[i];
  }

  for (i=1; i<=np; ++i) if (a[i] == 0)
  {
    fprintf(gp, "input became unreadable\n");
    exit(0);
  }

  orient();
  
  writhe=0;
  for (i=1; i<=np; i+=2) writhe+=f[i]*g[i];

  for (i=1; i<=nc; i++)  for (j=1; j<=3; j++)  
  {
    r[i][j]=0;
  }

  for (i=1; i<=np; i++) gs[i]=0;

  j=0;
  cr=1;
  while (g[cr] == 1) cr++; 

  for (i=cr; i<=cr+np-1; i++)
  {
    if (g[q2[i]] == -1) 
    {
      span[over[q2[i-1]]]=cspan;
      cspan=0;
      j++;
    }
    else cspan++;
    over[q2[i]]=j;
  }
  span[over[q2[cr+np-1]]]=cspan; 

/* r[i][1] is overpass across and just ahead of ith overpass;
   r[i][2] is orientation of the corresponding crossing. */

  for (i=1; i<=np; i++) if (g[q2[i+1]] == -1)
  {
    r[over[i]][1] = over[a[q2[i+1]]];
    r[over[i]][2] = f[q2[i+1]];
  }

  bridge=2;

  while (search() == 0)
  {
    if (bridge==5)
    {
      fprintf(gp, "Can't process this knot, as its bridge number is greater than 5.\n");
      exit(0);
    }
    else bridge++;
  }

/*  list remaining relators (i.e. "consistency" relators) */

  for (i=1; i<=nc; i++) reldone[i]=0;
  for (i=1; i<=nc; i++) 
  {
    if (gs[i] > 0) reldone[gs[i]]=1;
    if (gs[i] < 0) reldone[q1[-gs[i]-1]]=1;
  }

  crel=nc-bridge;
  for (i=1; i<=nc; i++) if (reldone[i] == 0)
    gs[++crel]=i;
    
/* search for homomorphisms */

  nhom=quickdist=0;

  for (class=1; class<=10; class++) if (classchoice[class] == 1)
  {
    phi[gen[1]] = 1;

    for (scnd=1; scnd<=ninn[class]; scnd++)
    {
      phi[gen[2]] = inn[class][scnd];

      for (i=1; i<=bridge-2; i++) phivec[i]=1;

      do{

        if (filter() == 1) continue;

        (nhom)++;

        (homvec[homtype])++;

        fprintf(gp, "%2d.%6d          Bridge number of diagram:%2d\n", nc, cknot, bridge);

        fprintf(gp, "Meridians:   ");
        for (i=1; i<=nc; i++) fprintf(gp, "%s ", cycles[conje[class-1]+phi[i]]);
        fprintf(gp, "\n");
        fprintf(gp, "Longitude:   %s\n", cycles[longi]);
        fprintf(gp, "Order of image group:   %4d    ", imsize);
        fprintf(gp, "Image group: %s\n", groupdesc[grouptype[imsize]]);
        if (wh1 == 1)
        {
          result=h1();
	  if (result==99)
	    fprintf(gp, "Could not compute homology group, as matrix entries became too large.\n");
	  else
	  {
            fprintf(gp, "Homology group:       ");
            for (i=1; i<=ntor; i++) fprintf(gp, "%6d", tor[i]);
            fprintf(gp, "\n");
	  }
        }
        fprintf(gp, "\n");
        fflush(gp);

      } while (update() == 0);
    }
  } 
  return 0;
}


imorder()
{
  int i, j, psi[100];
  int im[200], size1, size2, bigger, elt;

  for (i=1; i<=nc; i++) psi[i]=phi[i]+conje[class-1];
  for (i=1; i<=120; i++) im[i]=0;
  for (i=1; i<=nc; i++) im[psi[i]]=1;
  size1=0;
  size2=0;
  for (i=1; i<=120; i++) if (im[i] == 1) size2++;
  while (size2 > size1)
  {
    size1=size2;
    for (i=1; i<=nc; i++) for (j=1; j<=120; j++) if (im[j] == 1)
      im[mult[psi[i]] [j]] = 1;
    size2=0;
    for (i=1; i<=120; i++) if (im[i] == 1) size2++;
  }

  return size2;
}
  

filter()
{
  int i;
  int over1, over2, over3, elt, eltinv, elt1, elt2, 
    elt2inv, elt3, dir1, dir2, k;

  for (i=3; i<=bridge; i++) phi[gen[i]]=phivec[i-2];

  for (crel=1; crel<=nc; crel++)
  {
    over1=gs[crel];

    if (over1 > 0) dir1=1; else dir1=-1; 

    over1=abs(over1); 

    if (dir1 == 1)
    {
      over2=r[over1][1];
      dir2=r[over1][2];
      over3=q1[over1+1];
    }
    else
    {
      over2=r[q1[over1-1]][1];
      dir2=r[q1[over1-1]][2];
      over3=q1[over1-1];
    }

    elt1=phi[over1];
    elt2=phi[over2];

    if (dir1*dir2 == 1)
      elt3 = x[class][elt1][elt2];
    else
      elt3 = x[class][elt1][elt2+conjsize[class]];

    if (crel > nc-bridge)
    {
      if (phi[over3] != elt3) return 1;
    }
    else
    {
      phi[over3] = elt3;
    }
  }

  for (i=1; i<=nrc[class]; i++)
  {
    for (j=1; j<=nc; j++) psi[j]= rc[class][phi[j]][i];
    j=1; 
    while (psi[ovorder[j]] == phi[ovorder[j]]  &&  j <= nc) j++;

    if (j <= nc  &&  psi[ovorder[j]] < phi[ovorder[j]]) return 1;
  }

/* for s6, test for transitivity */
  
  if (class >= 7) if (transtest() == 1) return 1;

imorder();

  if (class <=6) imsize=imorder(); else imsize=0;

  homtype=val();
  if (homtype == 0) return 1;



/* compute longitude (s5 only) */

  if (class <= 6)
  {
    longi=1;
    for (i=1; i<=nc; i++) psi[i]=phi[i]+conje[class-1];
    for (i=gen[1]; i<=gen[1]+nc-1; i++)
    {
      crel=q1[i];
      if (r[crel][2] == 1) elt=inv[psi[r[crel][1]]];
      else elt=psi[r[crel][1]];

      longi = mult[elt][longi];
    }
    for (i=1; i<=abs(writhe); i++)
    {
      if (writhe > 0) elt=inv[psi[gen[1]]];
      else elt=psi[gen[1]];

      longi = mult[elt][longi];
    }
  }

  else

  {
    int tperm[20], cg;

    for (i=1; i<=6; ++i) plongi[i]=i;
    for (i=1; i<=nc; i++) psi[i]=phi[i]+conje[class-1];
    for (cg=gen[1]; cg<=gen[1]+nc-1; cg++)
    {
      crel=q1[cg];
      elt=psi[r[crel][1]];

      for (i=1; i<=6; ++i) cperm[i]=s6[elt][i];
 
      if (r[crel][2] != 1)   /* invert cperm */
      {
        for (i=1; i<=6; ++i) tperm[cperm[i]]=i;
        for (i=1; i<=6; ++i) cperm[i]=tperm[i];
      }

      for (i=1; i<=6; ++i) newperm[i]=plongi[cperm[i]];
      for (i=1; i<=6; ++i) plongi[i]=newperm[i];
    }
    for (cg=1; cg<=abs(writhe); cg++)
    {
      elt=psi[gen[1]];

      for (i=1; i<=6; ++i) cperm[i]=s6[elt][i];

      if (writhe < 0)   /* invert cperm */
      {
        for (i=1; i<=6; ++i) tperm[cperm[i]]=i;
        for (i=1; i<=6; ++i) cperm[i]=tperm[i];
      }

      for (i=1; i<=6; ++i) newperm[i]=plongi[cperm[i]];
      for (i=1; i<=6; ++i) plongi[i]=newperm[i];
    }
  }

  return 0;
}


update()
{
  register int i, j, pos;

  pos=bridge-2;
  while (phivec[pos] == conjsize[class] && pos > 0) pos--;
  if (pos == 0) return 1;
  (phivec[pos])++;
  for (i=pos+1; i<=bridge-2; i++) phivec[i]=1;
  return 0;
}


search()
{
  register int i, j, k;
  int cgen, found, gennumber, first, second, gd[100];

  for (first=1; first<=nc; first++) if (span[first] > 0)
  {
    gen[1]=first;            /*gs="generating sequence"*/
    gen[2]=r[first][1];      /*first:  first generator */
    second=gen[2];

    for (i=1; i<=2; i++) ovorder[i]=gen[i];
    
    for (ctry=1; ctry<=nvec[bridge-2]; ctry++)
    {
      for (i=1; i<=nc; i++) gd[i]=0; /* gd="already generated"*/
      gd[gen[1]]=1; gd[gen[2]]=1;

      t=0;
      for (i=1; i<=bridge-2; i++)
      {
        cgen=vec[bridge-2][ctry][i];
 
        if (span[cgen] == 0  ||  cgen == first  || cgen == second)
          t=1;
      }

      if (t == 1) continue;

      for (i=1; i<=bridge-2; i++)
      {
        cgen=vec[bridge-2][ctry][i];
        gen[i+2]=cgen;
        gd[cgen]=1;
        ovorder[i+2]=cgen;
      }

      mnumber=0;
      found=1;
      while (found == 1)
      {
        found=0;
        for (i=1; i<=nc; i++)
          if (gd[i] == 1  &&  gd[r[i][1]] == 1  &&  
              gd[q1[i+1]] == 0  &&  found == 0)
        {
          gd[q1[i+1]]=1;
          found=1;
          gs[++mnumber]=i;
          ovorder[bridge+mnumber]=q1[i+1];
        }

        if (found == 1) continue;

        for (i=1; i<=nc; i++)
          if (gd[i] == 1  &&  gd[r[q1[i-1]][1]] == 1  &&  
              gd[q1[i-1]] == 0  &&  found == 0)
        {
          gd[q1[i-1]]=1;
          found=1;
          gs[++mnumber]=-i;
          ovorder[bridge+mnumber]=q1[i-1];
        }
      }

     if (mnumber == nc-bridge) return 1;

    }
  }
  return 0;
}



orient()
{
  int e[100], g[100], h[100], knit; 
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
  return 0;
}


val()
{
  if ((imsize < 6  && imsize > 0) 
      ||  (imsize == 6  &&  class >= 5)) return 0;
  if (class == 1  &&  imsize == 6) return 1;
  if (class == 1  &&  imsize == 24) return 2;
  if (class == 1  &&  imsize == 120) return 3;
  if (class == 2  &&  imsize == 12) return 4;
  if (class == 2  &&  imsize == 60) return 5;
  if (class == 3  &&  imsize == 20) return 6;
  if (class == 3  &&  imsize == 24) return 7;
  if (class == 3  &&  imsize == 120) return 8;
  if (class == 4) return 9;
  if (class == 5  &&  imsize == 10) return 10;
  if (class == 5  &&  imsize == 60) return 11;
  if (class >= 6) return (class+6);
}


val2()
{
  if (imsize < 6  ||  (imsize == 6  &&  class >= 5)) return 0;

  if (class == 1  &&  imsize == 6) return 1;
  if (class == 1  &&  imsize == 24) return 1;
  if (class == 1  &&  imsize == 120) return 1;
  if (class == 2  &&  imsize == 12  &&  longi == 12) return 1;
  if (class == 2  &&  imsize == 12  &&  longi == 12) return 1;
  if (class == 2  &&  imsize == 12  &&  longi == 12) return 1;
  if (class == 2  &&  imsize == 12  &&  longi == 12) return 1;
  if (class == 2  &&  imsize == 12  &&  longi == 13) return 2;
  if (class == 2  &&  imsize == 60  &&  longi == 12) return 3;
  if (class == 2  &&  imsize == 60  &&  longi == 13) return 4;
  if (class == 4  &&  longi == 62) return 5;
  if (class == 4  &&  longi == 65) return 6;
  if (class == 4  &&  longi == 63) return 7;
  if (class == 4  &&  longi == 64) return 8;
  if (class == 6  &&  longi == 12) return 9;
  if (class == 6  &&  longi == 13) return 10;
}



vecgen()
{
  register int i,j;

  for (i=1; i<=nc; i++)
  {
    vec[nplaces][row][pos]=i;
    if (pos==1)
    {
      row++;
      for (j=pos+1; j<=nplaces; j++) 
        vec[nplaces][row][j]=vec[nplaces][row-1][j];
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


transtest()
{
  int i, j, perm, u[10], *v, found, t, degree;

  degree=6;
  t=0;
  for (i=1; i<=nc; i++) if (phi[i] != 1) t=1;
  if (t==0) return 1;

  u[1]=1;

  for (i=2; i<=degree; i++) u[i]=0;
  found=1;
  while (found == 1)
  {
    found=0;
    for (perm=1; perm<=nc; perm++)
    {
      if (degree == 6) v=s6[phi[perm] + conje[class-1]];
      else v=s7[phi[perm] + conje[class-1]];

      for (i=1; i<=degree; i++) if (u[i] == 1  &&  u[v[i]] == 0)
      {
        u[v[i]]=1;
        found=1;
      }

    }
  }
  for (i=1; i<=degree; i++) if (u[i] == 0) return 1;
  return 0;
}
    
h1()
{
  int i, j;

  int w[100][100], ws[100][100], wn[100], 
      rs[100][50][50], tr[100][50], col[250][10], 
      *gs[100], gv[100][50], r1[100][50], done[100],

      a, p, q, t, t0,

      deg, j0, k, l, h, hh, found;

 /* class=4;
  for (i=1; i<=nc; i++) phi[i]=1;*/
 

  for (i=1; i<=nc; i++) gs[i] = s5[phi[i]+conje[class-1]];

  if (imsize == 6) deg=3;
  else if (imsize == 12  ||  imsize == 24) deg=4;
  else deg=5;

  for (i=1; i<=nc; i++) for (j=1; j<=deg; j++) gv[i][gs[i][j]] = j;

  for (i=1; i<=nc*(deg+4); i++) for (j=1; j<=nc*deg; j++) 
    mat[i][j]=0;
  for (i=1; i<=nc; i++) for (j=1; j<=deg; j++) tr[i][j]=0;
  for (i=1; i<=6; i++) for (j=1; j<=3; j++) w[i][j]=ws[i][j]=0;
  for (i=1; i<=6; i++) wn[i]=1;

  for (i=1; i<=nc-1; i++)
  {
    r1[i][1]=r[i][1]*r[i][2];
    r1[i][2]=i;
    r1[i][3]=-r1[i][1];
    r1[i][4]=-(i+1);
  }

  /*  w = Schreier tree ; the product of w[i][j] (i fixed) maps
      i to i+1; wn[i] is the number of w[i][j] required. */

  for (i=1; i<=deg-1; i++)
  {
    j=0;
    found=0;
    for (j=1; j<=nc; j++) if (found == 0  &&  gs[j][i] == i+1)
    {
      found=1;
      wn[i]=1;
      w[i][1]=j;
    }
    if (found == 1) continue;

    for (j=1; j<=nc; j++) for (k=1; k<=nc; k++) 
      if (found == 0  &&  gs[k][gs[j][i]] == i+1)
    {
      found=1;
      wn[i]=2;
      w[i][1]=j;
      w[i][2]=k;
    }
    if (found == 1) continue;

    for (j=1; j<=nc; j++) for (k=1; k<=nc; k++) for (l=1; l<=nc; l++) 
      if (found == 0  &&  gs[l][gs[k][gs[j][i]]] == i+1)
    {
      found=1;
      wn[i]=3;
      w[i][1]=j;
      w[i][2]=k;
      w[i][3]=l;
    }
    if (found == 1) continue;

    for (j=1; j<=nc; j++) for (k=1; k<=nc; k++) 
      for (l=1; l<=nc; l++) for (h=1; h<=nc; h++) 
      if (found == 0  &&  gs[h][gs[l][gs[k][gs[j][i]]]] == i+1)
    {
      found=1;
      wn[i]=4;
      w[i][1]=j;
      w[i][2]=k;
      w[i][3]=l;
      w[i][4]=h;
    }
    if (found == 1) continue;

    for (j=1; j<=nc; j++) for (k=1; k<=nc; k++) 
      for (l=1; l<=nc; l++) for (h=1; h<=nc; h++)
        for (hh=1; hh<=nc; hh++)
      if (found == 0  &&  gs[hh][gs[h][gs[l][gs[k][gs[j][i]]]]] == i+1)
    {
      found=1;
      wn[i]=5;
      w[i][1]=j;
      w[i][2]=k;
      w[i][3]=l;
      w[i][4]=h;
      w[i][5]=hh;
    }
    if (w[i][1] == 0)
    {
      fprintf(gp, "Schreier tree failure in computation of H1\n");
      exit(0);
    }
  }
/* ws = history of symbol 1 under the permutations w[i][j] */

  t=1;
  for (i=1; i<=deg-1; i++) for (j=1; j<=wn[i]; j++)
  {
    ws[i][j]=t;
    t=gs[w[i][j]] [t];
  }

  for (i=1; i<=nc-1; i++) for (j=1; j<=deg; j++)
  {
    t=j;
    for (k=1; k<=4; k++)
    {
      if (r1[i][k] > 0)
      {
        rs[i][j][k]=t;
        t=gs[abs(r1[i][k])] [t]; 
      }
      else 
      {
        t=gv[abs(r1[i][k])] [t];
        rs[i][j][k]=t;
      }
    }
  }


/* assemble matrix mat[i][j] */

  for (i=1; i<=nc; i++) for (j=1; j<=deg; j++) 
    if (gs[i][j] == j) tr[i][j]=1;

  nrow=0;
  ncol=0;

  for (i=1; i<=nc; i++) for (j=1; j<=deg; j++) if (tr[i][j] == 0)
    col[i][j]=++ncol;

/* lifts of Wirtinger relators */

  for (i=1; i<=nc-1; i++) for (j=1; j<=deg; j++)
  {
    nrow++;
    for (k=1; k<=4; k++)
    {
      l1=abs(r1[i][k]);
      l2=rs[i][j][k];

      if (r1[i][k] > 0) t=1; else t=-1;
      if (tr[l1][l2] == 0) mat[nrow][col[l1][l2]] += t;
    }
  }
  
/* Schreier relators */

  for (i=1; i<=deg-1; i++)
  {
    nrow++;
    for (j=1; j<=i; j++) for (k=1; k<=wn[j]; k++)
    {
      l1=w[j][k];
      l2=ws[j][k];
      mat[nrow][col[l1][l2]] ++;
    }
  }

/* branch relators */

  for (i=1; i<=nc; i++)
  {
    for (j=1; j<=deg; j++) done[j]=0;
    for (j=1; j<=deg; j++) if (done[j] == 0  &&  gs[i][j] != j)
    {
      j0=j;
      nrow++;
      do {
        done[j]=1;
        mat[nrow][col[i][j]] = 1;
        j=gs[i][j];
      } while (j != j0);
    }
  }

/* now reduce matrix to Smith normal form */
      
/* l1 = current row; l2 = current column */

  l1=1;
  l2=1;



/* tor[i] : invariant factors of group */

  for (i=1; i<=100; i++) tor[i]=-1;
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

/* if a row is zero, replace by last row */

    for (i=nrow; i>=l1; i--) if (zr(i) == 1)
    {
      if (i == nrow) nrow--;
      else swapr(i, nrow--);
    }

/* apply heuristic reduction routine if less than 20 columns left */

    if (ncol-l2 < 60) for (i=1; i<=2; i++) if (hrr()==99) return 99;

 
/* search for columns of zeroes */

    for (i=l2; i<=ncol; i++) if (zc(i) == 1)
    {
      swapc(l2++, i);
      tor[++ntor]=0;
    }

    if (l2 > ncol) continue;

/* use hsr() to get optimal row and column in place */

    if (hsr()==99) return 99;

/* repeatedly use euclidean() to make all entries off diagonal
   in row l1 and column l2 zero */

    while (eudone() == 0)
    {
      for (i=l1+1; i<=nrow; i++) if (mat[i][l2] != 0)
        {if (euclidean(i,1) == 99) return 99;}


      for (i=l2+1; i<=ncol; i++) if (mat[l1][i] != 0)
        {if (euclidean(i,2) == 99) return 99;}

    }


/* if diagonal entry has absolute value not equal to 1, save as
   invariant factor of group; if at last row, the zeros to right
   indicate free generators. */


    if (abs(mat[l1][l2]) != 1) tor[++ntor]=abs(mat[l1][l2]);

    if (l1 == nrow) for (i=l2+1; i<=ncol; i++) tor[++ntor]=0;

    l1++;
    l2++;

  }

/* what to do if group is trivial */

  if (ntor == 0)
  {
    ntor=1;
    tor[1]=1;
  }

  else
  {
    l=1;
    while (tor[l] == 0  &&  l <= ntor) l++;

    if (l <= ntor)  /* group has torsion */
    {
      t=1;
      while (t == 1)
      {
        t=0;

/* arrange factors in increasing order of magnitude */
      
        for (i=1; i<=ntor; i++) for (j=1; j<=ntor-1; j++)
          if (tor[j] > tor[j+1])
        {
          k=tor[j];
          tor[j]=tor[j+1];
          tor[j+1]=k;
        }

        l=1;
        while (tor[l] == 0  &&  l <= ntor) l++;
        for (i=l; i<=ntor-1; i++) if (tor[i+1]%tor[i] != 0)
        {
          t=1;
          p=gcd(tor[i], tor[i+1]);
          q=(tor[i]*tor[i+1])/p;
          tor[i]=p;
          tor[i+1]=q;
        }
      }

/* get rid of initial 1's */

      while (tor[l] == 1)
      {
        for (i=l; i<=ntor-1; i++) tor[i]=tor[i+1];
        ntor--;
      }
    }
  }

  return 0;

}

gcd(int x, int y)
{
  int p, q, r, i;

  p=abs(x);
  q=abs(y);
  i=1;
  while (q != 0)
  {
    r=p%q;
    p=q;
    q=r;
    i++;
  }
  return p;
}

gc(int x, int y, int *d, int *a, int *b)

{
  int c[100], i, p, q, r, a1, b1, t, sx, sy;

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

  if (x > 0) sx=1; else sx=-1;
  if (y > 0) sy=1; else sy=-1;

  (*a)*=sx;
  (*b)*=sy;

  return 0;
}


swapr(int i, int j)
{
  int k, l;

  for (k=1; k<=ncol; k++)
  {
    l=mat[i][k];
    mat[i][k]=mat[j][k];
    mat[j][k]=l;
  }
  return 0;
}


swapc(int i, int j)
{
  int k, l;

  for (k=1; k<=nrow; k++)
  {
    l=mat[k][i];
    mat[k][i]=mat[k][j];
    mat[k][j]=l;
  }
  return 0;
}



euclidean(int pos, int t)
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

      if (a>100000000 || u>100000000 || p>100000000 || v>100000000) return 99;
      if ((a/10)*(u/10) + (b/10)*(v/10) > 1000000) return 99;
      if ((p/10)*(u/10) + (q/10)*(v/10) > 1000000) return 99;

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

      if (a>100000000 || u>100000000 || p>100000000 || v>100000000) return 99;
      if ((a/10)*(u/10) + (b/10)*(v/10) > 1000000) return 99;
      if ((p/10)*(u/10) + (q/10)*(v/10) > 1000000) return 99;

      mat[k][l2]=a*u+b*v;
      mat[k][pos]=p*u+q*v;
    }
  }
  return 0;
}


hrr()

{
  register int i, j;
  int p1, p2, q1, q2, t, tsum, u, v, *ru, *rv, rusum, rvsum, better,
      sgn, wsum, w[250];

  t=tsum=0;
  for (i=l1; i<=nrow; i++)
  {
    t=0;

    for (j=l2; j<=ncol; j++) 
    {
      t+=abs(mat[i][j]);
      tsum+=abs(mat[i][j]);
      if (t > 500000000) return 99;
    }
  }
 
  if (tsum < 10000)
  {
    if (2*tsum < 1*(ncol-l2)*(ncol-l2)) return 0;
  }
 
  do{
  better=-1;

  for (u=l1; u<=nrow-1; u++) for (v=u+1; v<=nrow; v++)
  {
    ru=mat[u];
    rv=mat[v];
    rusum=rvsum=0;
    for (i=l2; i<=ncol; i++) rusum+=abs(ru[i]);
    for (i=l2; i<=ncol; i++) rvsum+=abs(rv[i]);

    better=0;
    if (rusum >= rvsum  &&  rvsum != 0) 
      for (sgn=-1; sgn<=1; sgn+=2) if (better == 0)
    {
      m=(rusum/rvsum)*sgn;
      for (i=l2; i<=ncol; i++) w[i]=ru[i]+m*rv[i];
      wsum=0;
      for (i=l2; i<=ncol; i++) wsum+=abs(w[i]);
      if (wsum < rusum)
      {
        better=1;
        for (i=l2; i<=ncol; i++) mat[u][i]=w[i];
      }
    }

    if (rvsum > rusum  &&  rusum != 0) 
      for (sgn=-1; sgn<=1; sgn+=2) if (better == 0)
    {
      m=(rvsum/rusum)*sgn;
      for (i=l2; i<=ncol; i++) w[i]=rv[i]+m*ru[i];
      wsum=0;
      for (i=l2; i<=ncol; i++) wsum+=abs(w[i]);
      if (wsum < rvsum)
      {
        better=1;
        for (i=l2; i<=ncol; i++) mat[v][i]=w[i];
      }
    }
  }
  } while (better == 1);

  return 0;
}
            

hsr()
{
  register int i, j;

  int rowsum, colsum, nicerow, nicecol, u, v, weight, entrysize, t;

  nicerow=l1;
  nicecol=l2;

  weight=entrysize=1000000000;

  for (u=l1; u<=nrow; u++) for (v=l2; v<=ncol; v++)
    if (mat[u][v] != 0)
  {
    rowsum=colsum=0;
    for (i=l2; i<=ncol; i++) rowsum+=abs(mat[u][i]);
    for (i=l1; i<=nrow; i++) colsum+=abs(mat[i][v]);

    if (rowsum > 500000000  ||  colsum > 500000000) return 99;

    t=abs(mat[u][v]);

    if ((t < entrysize) || (t == entrysize && rowsum+colsum < weight)) 
   /* if (rowsum+colsum < weight)*/
    {
      entrysize=t;
      weight=rowsum+colsum;
      nicerow=u;
      nicecol=v;
    }
  }
  
  if (l1 != nicerow) swapr(l1, nicerow);
  if (l2 != nicecol) swapc(l2, nicecol);

  return 0;
}

matprint()
{
  int i, j;

  printf("l1=%3d  l2=%3d\n", l1, l2);

  if (l1 < 40) for (i=l1; i<=nrow; i++)
  {
    for (j=l2; j<=ncol; j++) printf("%3d", mat[i][j]);
    printf("\n");
  }

  else for (i=l1; i<=nrow; i++)
  {
    for (j=l2; j<=ncol; j++) printf("%6d", mat[i][j]);
    printf("\n");
  }
  return 0;
}


zc(int v)

{
  int i;

  for (i=l1; i<=nrow; i++) if (mat[i][v] != 0) return 0;
  return 1;
}


zr(int u)

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


