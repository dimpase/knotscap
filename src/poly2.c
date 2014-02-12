/* there is a hard limit of 127 crossings in this program */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define XCNT   80    /* maximum "crossing count" (at most 127)  */
FILE *fp, *gp ;
long plybuf[XCNT*(XCNT+2)]; 
char *crsptr[XCNT], sign[128], xlist[128];
char buf[30000], cbuf[3074], clist[2][XCNT*2];
short overs, unders, xcrs, numlps, notbeg, binoms[78];

main(argc,argv)
int argc;
char *argv[];
{
  register short i, numcrs, j;
  register char **cross, *p;
  register long *lp1;
  int in, out, stats, len, i1, j1, nk;
  int w[100001], nc[100001], knotid[100001];
  long lngi, count[XCNT], *poly[XCNT], *lp2;
  short bstcnt, flag, d, e, f, g, h, l, k, m, n;
  short vnum, vbrnch, xpow, ypow, top, *sp, dir, lowx;
  char *ptrbuf[XCNT], crsbuf[XCNT*8], **inbuf, nbuf[82];
  char *c, *s, *st1, sta[500], tobust, bstlst[127], dspair[3], t[64];
  char line[200], subline[50], *lp3, *lp4;

  fp=fopen(argv[1], "r");
  nk=0;
  st1=sta;
  while( fgets(line, 50, fp) != NULL)
  {
    int coln;

    if (line[0] == '\n') continue;
    if (line[2] == '.')
    {  
      ++nk;

      subline[0]=line[0]; subline[1]=line[1]; subline[2]='\0';
      nc[nk]=atoi(subline);
      coln=0; while(line[coln] != ':') ++coln;
      for(i=3; i<=coln-1; ++i) subline[i-3]=line[i];
      subline[coln-3]='\0';
      knotid[nk]=atoi(subline);
    }
    else
       if ((line[1]=='+') || (line[2]=='+')) ++w[nk];  else --w[nk];
  }

  gp=fopen(argv[2],"w"); 
  nk=0;
    
  if (argc<2)
  {
    printf ("usage:  poly knotfile [knotfile ...]\n");
    exit (-1);
  }

  *count= i= 0;
  cross= crsptr;
  inbuf= ptrbuf;
  *crsptr= crsbuf;
  while (++i!=XCNT) crsptr[i]= crsptr[i-1]+8;
  i=0;
  lp1= plybuf;
  while (i<XCNT)
  {
    poly[i]= lp1;
    lp1+= 1+ 2* (XCNT-i++);
  }

  NEWFIL:

  if (--argc==1) exit(0); 
  in=open(*++argv,0);

  i= strlen(*argv);
  c= *argv+i-1;
  if (i>11) i=11;
  while (i--!=0 && *c!='/') --c;
  p=t;
  while (*++c!=0) *(p++)= *c;
  *(p++)= '.';
  *(p++)= 's';
  *p=0;

  s=cbuf+1;
  s[read(in,s,3072)]=0x80;

/**** computation of binomial coefficients (added by MBT) ****/

/* 
  {
    int i,j;
    for(i=0; i<=63; i+=1) bin[i]=binpol[i]+64;
    *bin[0]=1;
    for(i=1; i<=63; i+=1)
    {
      *(bin[i]-i)=1; *(bin[i]+i)=1;
      for(j=(-i+2); j<=(i-2); j+=2) 
         *(bin[i]+j)= *(bin[i-1]+j-1) + *(bin[i-1]+j+1);
    }

    for(i=1; i<=63; i+=2)
      for(j=(-i); j<=i; j+=2) 
         *(bin[i]+j) *= (-1);
  }
*/  
  
/******** end of computation of binomial coefficients ********/  
  

  NEWNOT:
  i= XCNT-1;
  while (i!=0 && count[i]==0) --i;
  if (*count!=0)
  { 
    k= XCNT;
    i=0;
    while (--k!=0 && i==0)
    {
      lp1= poly[k-1];
      i= 2+ 2*(XCNT-k);
      while (--i!=0 && *lp1==0) ++lp1;
    }

    if (k!=0)
    {
      len=m= -1;
      if (lowx>0)
      {
        lowx=1; 
      }
   
      {
        int i, j, k, wr, def, zmax, amin[101], amax[101];        
        long *ppol[100];

        ++nk;
  
        for(i=0;i<=60;++i) ppol[i]=(poly[i+1]+79-i-w[nk]);

        zmax=nc[nk]-1;
        def=1;

        while (def == 1)
        {
          for (i=zmax-nc[nk]; i<=nc[nk]-zmax; i++)	 
            if (ppol[zmax][i] != 0) def=0;
          if (def == 1) zmax--;
        }

        for(i=0;i<=zmax;i++)
        {
          j=i-nc[nk];
          while(ppol[i][j] == 0  &&  j < nc[nk]-i)  j++;
          amin[i]=j;

          j=nc[nk]-i;
          while(ppol[i][j] == 0  &&  j > i-nc[nk])  j--;
          amax[i]=j;

          if (amax[i] < amin[i]) amax[i]=amin[i]=0;
        }
        
        wr=w[nk];
        
        fprintf(gp, "%2d%8d\n", nc[nk], knotid[nk]);
        for (i=0; i<=zmax; i++)
        {
          fprintf(gp, "%4d%4d  ", amin[i]-wr, amax[i]-wr);
          for (j=amin[i]; j<=amax[i]; j+=2)
	  {
	    if (nc[nk] < 20) fprintf(gp, "%6d", ppol[i][j]);
            else fprintf(gp, "%8d", ppol[i][j]);
	  }
          fprintf(gp, "\n");
        }

        fprintf(gp, "\n");
      }
    }
  }

  while (*s=='\n') ++s;
  numcrs= 0;
  i= XCNT;
  while (i--!=0) count[i]=0;
  c=s;
  if (*s!='1' || ((*(s+1)&0x5f)!='V' && *(s+1)!='+' && *(s+1)!='-'))
  {
    p=s;
    i=0;
    while (*s!='\n' && *s>0)
    {
      ++i;
      ++s;
    }

    if (*s>0)
    {
      s= nbuf;
      while (i--!=0) *(s++)= *(p++);
      *s= 0;
      s=p;
    }
  }

  if (*s=='\n') ++s;
  while (*s!='\n' && *s>0)
  {
    if (numcrs==XCNT)
    {
      printf ("too many crossings in knot\n");
      goto NEWFIL;
    }

    while (*s>='0' && *s<='9') ++s;
    if (*s=='+') sign[numcrs]=6;      /* most of sign[] shows + or - crossings */
    else if (*s=='V') sign[numcrs]=7;   /* the 1 bit in sign[] shows whether */
    else if (*s=='-') sign[numcrs]=2;        /* the crossing is a vertex */
    else if (*s=='v') sign[numcrs]=3;
    else if (*s<0) --s;
    else 
    {
      printf ("the format of this knot is unreadable, skipping file\n");
      close (in);
      goto NEWFIL;
    }

    ++s;
    j=0;
    while (j!=8 && *s>0)
    {
      i=0;
      while (*s>='0' && *s<='9')
      {
        i*=10;
        i+= *(s++)-0x30;
      }

      if (i==0)
      {
        printf ("the format of this knot is unreadable, skipping file\n");
        close (in);
        goto NEWFIL;
      }

      cross[numcrs][j++]= i-1;
      if (*s>0)
      {
        if (*s<'a' || *s>'d')
        {
          printf ("the format of this knot is unreadable, skipping file\n");
          close (in);
          goto NEWFIL;
        }

        cross[numcrs][j++]= (*(s++)-'a')*2;
      }
    }

    ++numcrs;
    if (*s=='\n') ++s;
  }

  if (*s<0)
  {
    s=cbuf;
    len=3072;
    while (*c>0)
    {
     *(s++)= *(c++);
     --len;
    }

    if (in!=0 && (i=read(in,s,len))!=0)
    {
      s[i]=0x80;
      s=cbuf;
      while (*s=='\n') ++s;
      goto NEWNOT;
    }

    *s=0x80;
    if (in!=0)
    {
      close (in);
      in=0;
    }

    if (numcrs<2) goto NEWFIL;
  }

  lp1= *poly;
  j= XCNT* (XCNT+2);
  while (--j!=0) *(lp1++)= 0;
  notbeg=numlps= xpow= ypow= 0;
  if ((lowx=conchk(numcrs))>0) exit(0);
  *count=1;
  
  STEP1:

/* FIRST -- remove figure 8 loops, 2 cases  "#-#d#c#b#a"  "#+#b#a#d#c" */
/* SECOND -- remove monogons (must recheck previous crossings) */
/* 4 cases  "#+#b#a????"  "#+????#d#c"  "#-#d????#a"  "#-??#c#b??" */
/* monogon twister must be thorough to make the bigon yanker thorough */


  i= numcrs;
  while (i--!=0)
  {  
                 /* loop through all crossings */
    p= cross[i];
    if (*p==i || p[4]==i)
    {
      if (*p==p[4]) ++numlps;           /* figure 8 */
      else if (*p==i)
      {
        /* connect the previous and the following crossings (case 1 and 3) */

        j= sign[i] & 6;
        k= p[j];
        m= p[j|1];
        h= cross[k][m]= p[4];
        n= cross[k][m|1]= p[5];
        cross[h][n]= k;
        cross[h][n|1]= m;
      }

      else 
      {
        /* connect the previous and the following crossings (case 2 and 4) */

        j= (sign[i] & 6)^4;
        k= p[j];
        m= p[j|1];
        h= cross[k][m]= *p;
        n= cross[k][m|1]= p[1];
        cross[h][n]= k;
        cross[h][n|1]= m;
      }
        /* squish crossing out of the list */

      SQUISH:
      j= --numcrs-i;
      lp3 =  cross[i];
      lp4 =  lp3+8;
      p= sign+i;
      c=p+1;
      while (j--!=0)
      {
        for (i1=1; i1<=8; ++i1) *(lp3++)= *(lp4++);
        *(p++)= *(c++);
      }
      /* renumber branch pointers (subtract 1 if the crossing was after) */

      j=0;
      while (j!=numcrs)
      {
        if (*cross[j]>=i) --*cross[j];
        if (cross[j][2]>=i) --cross[j][2];
        if (cross[j][4]>=i) --cross[j][4];
        if (cross[j][6]>=i) --cross[j][6];
        ++j;
      }

      goto STEP1;
    }
  }


/* THIRD -- remove bigons -- make sure loops don't get forgotten. */
/* 2 cases "k?mcmb???? m???kbka??"  "k?mc????md  m?????kakd" */
/* bigon yanker MUST be completely thorough or loops can get lost later */

  i= numcrs;
  while (i--!=0)
  {
       /* this next test checks that ka points to some mc, */
       /* kd to md or kb to mb, and neither crossing is a vertex */ 

    p= cross[i];
    if (p[1]==4 && ((*p==p[6] && p[7]==6) || (*p==p[2] && p[3]==2)))
    {
      m= *p;
      if (p[4]== m) ++numlps;       /* loop disappearing off top */
      if (p[2]== p[6]) ++numlps;    /* loop disappearing underneath */
                                    /* relink */
      k= p[4];
      j= p[5];
      h= cross[k][j]= *cross[m];
      n= cross[k][j|1]= cross[m][1];
      cross[h][n]= k;
      cross[h][n|1]= j;
      g=2;
      if (p[2]==m) g=6;
      k= p[g];
      j= p[g|1];
      h=cross[k][j]= cross[m][g];
      n= cross[k][j|1]= cross[m][g|1];
      cross[h][n]= k;
      cross[h][n|1]= j;
                      /* squish crossings out of the list */
      numcrs-=2;
      if (m<i)
      {
        k=m;
        m=i;
        i=k;
      }

      lp3= cross[i];
      lp4=lp3+8;
      p=sign+i;
      c=p+1;
      j= m-i;
      while (--j!=0)
      {
        for (i1=1; i1<=8; ++i1) *(lp3++)= *(lp4++);
        *(p++)= *(c++);
      }

      lp4+=8;
      ++c;
      j= numcrs-m+1;
      while (j--!=0)
      {
        for (i1=1; i1<=8; ++i1) *(lp3++)= *(lp4++);
        *(p++)= *(c++);
      }

      /* renumber branch pointers (subtract 1 if the crossing was after) */

      while (++j!=numcrs)
      {
        if (*cross[j]>=m) --*cross[j];
        if (cross[j][2]>=m) --cross[j][2];
        if (cross[j][4]>=m) --cross[j][4];
        if (cross[j][6]>=m) --cross[j][6];
        if (*cross[j]>=i) --*cross[j];
        if (cross[j][2]>=i) --cross[j][2];
        if (cross[j][4]>=i) --cross[j][4];
        if (cross[j][6]>=i) --cross[j][6];
      }

      goto STEP1;
    }
  }

  while (++i!=numcrs) bstlst[i]= 0;
  if (numcrs<6) i=0;
               /* find very good and ok triples */
  while (i--!=0)
  {
    p= cross[i];
    g= *p;
    j= p[1]^2;
    h= cross[g][j];
    k= cross[g][j^4];
    g= p[4];
    j= p[5]^2;
    m= cross[g][j];
    n= cross[g][j^4];
    g= p[2];
    j= p[6];
    l= 0;
    if (h!=k && m!=n && g!=j && *p!=p[4])
    {
      if ((j==h && k==g) || (h==g && k==j)) l=1;
      if ((m==j && n==g) || (m==g && n==j)) l|=2;
      if ((j==h && n==j) || (k==j && m==j)) l|=4;
      if ((m==g && k==g) || (n==g && h==g)) l|=8;
    }

    if (l!=0)
    {
      if (((l&3)!=0 && (p[3]&2)+(p[7]&2)==4) || ((l&12)!=0 &&  
         (p[1]&2)+(p[5]&2)==0))
      {
        if ((l&3)!=0 && (p[3]&2)+(p[7]&2)==4)
        {
          if ((l&1)!=0) m=0;
          else m=4;
        }

        else 
        {
          if ((l&4)!=0) m=6;
          else m=2;
        }

        n= m^4;
        k= p[m];
        j= p[m|1];
        d= cross[k][j]= p[n];
        e= cross[k][j|1]= p[n|1];
        cross[d][e]= k;
        cross[d][e|1]= j;
        j^=4;
        d= p[m]= cross[k][j];
        e= p[m|1]= cross[k][j|1];
        cross[d][e]= i;
        cross[d][e|1]= m;
        p[n]= k;
        p[n|1]= j;
        cross[k][j]= i;
        cross[k][j|1]= n;
        j^=2;
        n= cross[k][j];
        h= cross[k][j|1]^4;
        d= cross[k][j]= cross[n][h];
        e= cross[k][j|1]= cross[n][h|1];
        cross[d][e]= k;
        cross[d][e|1]= j;
        j^=4;
        g= cross[k][j];
        h= cross[k][j|1]^4;
        d= cross[k][j]= cross[g][h];
        e= cross[k][j|1]= cross[g][h|1];
        cross[d][e]= k;
        cross[d][e|1]= j;
        j=m^2;
        n= p[j];
        h= p[j|1]^4;
        d= p[j]= cross[n][h];
        e= p[j|1]= cross[n][h|1];
        cross[d][e]= i;
        cross[d][e|1]= j;
        j^=4;
        g= p[j];
        h= p[j|1]^4;
        d= p[j]= cross[g][h];
        e= p[j|1]= cross[g][h|1];
        cross[d][e]= i;
        cross[d][e|1]= j;
                 /* squish crossings out of the list */
        numcrs-=2;
        if (n<g)
        {
          k=n;
          n=g;
          g=k;
        }

        lp3= cross[g];
        lp4=lp3+8;
        p=sign+g;
        c=p+1;
        j= n-g;
        while (--j!=0)
        {
          for (i1=1; i1<=8; ++i1) *(lp3++)= *(lp4++);
          *(p++)= *(c++);
        }

        lp4+=8;
        ++c;
        j= numcrs-n+1;
        while (j--!=0)
        {
          for (i1=1; i1<=8; ++i1) *(lp3++)= *(lp4++);
          *(p++)= *(c++);
        }

       /* renumber branch pointers (subtract 1 if the crossing was after) */

        while (++j!=numcrs)
        {
          if (*cross[j]>=n) --*cross[j];
          if (cross[j][2]>=n) --cross[j][2];
          if (cross[j][4]>=n) --cross[j][4];
          if (cross[j][6]>=n) --cross[j][6];
          if (*cross[j]>=g) --*cross[j];
          if (cross[j][2]>=g) --cross[j][2];
          if (cross[j][4]>=g) --cross[j][4];
          if (cross[j][6]>=g) --cross[j][6];
        }

        goto STEP1;
      }

      h=j;
      if ((l&3)!=0)
      {
        if ((p[3]&2) + (p[7]&2) == 2)
        {
          if ((p[((l&2)<<1)|1]&2) == 0)
          {
            bstlst[g]= -5;
            bstlst[j]= -5;
          }

          else if ((p[3]&2)!=0)
          {
            if (bstlst[j]==0) bstlst[j]=2;
          }

          else if (bstlst[g]==0) bstlst[g]=2;
        }

        else 
        {
          if (l>3 && l!=15 && (p[((l&3)*4+1)&5]&2)==0) bstlst[i]= -5;
          else bstlst[i]+= 2;
        }
      }

      if ((l&12)!=0)
      {
        if ((p[1]&2) + (p[5]&2) == 2)
        {
          if ((p[(l&4)|3]&2) != 0)
          {
            bstlst[(int)(*p)]= -5;
            bstlst[(int)(p[4])]= -5;
          }

          else if ((p[1]&2)!=0)
          {
            if (bstlst[(int)(*p)]==0) bstlst[(int)(*p)]=2;
          }

          else if (bstlst[(int)(p[4])]==0) bstlst[(int)(p[4])]=2;
        }

        else 
        {
          if ((l&3)!=0 && l!=15 && (p[(l&12)-1]&2)!=0) bstlst[i]= -5;
          else bstlst[i]+= 2;
        }
      }
    }
  }

                 /* circuit remover */
  vnum=n= 0;
  *dspair= numcrs;
  p= *clist;
  while (vnum!=numcrs)
  {
    overs=unders=xcrs=top= 0;
    m=j= -1;
    vbrnch=n;
    while (vnum!=j)
    {
      if (j<0) j=vnum;
      i= cross[j][vbrnch];
      vbrnch= cross[j][vbrnch|1]^4;
      j= i;
      i=flag= -1;
      while (++i<top && (*p&127) != j)
      {
        if (*(p++)==127) flag=0;
      }
  
      if (i!=top)
      {                  /* circuit crosses itself */
        if (flag==0) goto NXTCIR;
                         /* this circuit ties itself, illegal to remove */
        if (*p<0) --unders;
        else --overs;
        xlist[xcrs++]=(char)j;
        m=0;
        *p= (*clist)[top++]= 127;
      }

      else 
      {
        if (vbrnch!=0)
        {
          (*clist)[top++]= j|128;      /* flag "used b-d part of crossing" */
          ++unders;
        }

        else 
        {
          (*clist)[top++]= j;
          ++overs;
        }
      }

      p= *clist;
    }

    if (vbrnch != n)
    {
      if (n==0) --unders;
      else --overs;
      xlist[xcrs++]=(char) (j|128);
      xlist[xcrs]=(char) vbrnch;
      p[top-1]= 127;
    }

    else p[top]=127;
    if (unders<overs) bstcnt=unders;
    else bstcnt=overs;
    if (bstcnt==0)
    {
      rmcir(top,numcrs);
      numcrs-= overs+unders+xcrs;
      goto STEP1;
    }

    if (xcrs==0 || xlist[xcrs-1]>=0)
    {
      if (bstcnt==2 && overs+unders!=4)
      {                  /* try to shrink links to 4 crossings */
        i=0;
        if (overs==2)
        {
          while (p[i]<0 || p[i]==127) ++i;
          j= p[i];
          if (p[i+1]<0 || p[i+1]==127)
          {
            if (i==0 && p[top-1]>=0)
            {                   /* p[top-1] can't be 127 */
              while (p[++i]>=0);
              vnum= p[i]&127;
              n=0;
              goto NXTCIR;
            }

            ++i;
            while (p[i]<0 || p[i]==127) ++i;
            k= p[i];
            i= overs+unders+xcrs-2;
            if (bstlst[k]>=0)
            {
              if (bstlst[k]<i) bstlst[k]= i;
            }

            else if (bstlst[k]> -2-i) bstlst[k]= -2 -i;
            if (bstlst[j]>=0)
            {
              if (bstlst[j]<i) bstlst[j]= i;
            }

            else if (bstlst[j]> -2-i) bstlst[j]= -2 -i;
            goto NXTCIR;
          }

          k= p[++i];
          g=0;
          h= sign[k];
        }
   
        else 
        {
          while (p[i]>=0) ++i;
          j= p[i]&127;
          if (p[i+1]>=0)
          {
            if (i==0 && p[top-1]<0)
            {
              while (p[++i]==127);
              vnum= p[i]-1;
              n= 2;
              goto NXTCIR;
            }

            ++i;
            while (p[i]>=0) ++i;
            k= p[i]&127;
            i= overs+unders+xcrs-2;
            if (bstlst[k]>=0)
            {
              if (bstlst[k]<i) bstlst[k]= i;
            }

            else if (bstlst[k]> -2-i) bstlst[k]= -2 -i;
            if (bstlst[j]>=0)
            {
              if (bstlst[j]<i) bstlst[j]= i;
            }

            else if (bstlst[j]> -2-i) bstlst[j]= -2 -i;
            goto NXTCIR;
          }

          k= p[++i]&127;
          g= sign[k];
          h=0;
        }

        while (++i!=top) p[i-2]= p[i];
        p+=i-2;
        while (*--p==127);
        i= *p;
        *p= 127;
        if (i<0)
        {
          i&=127;
          f= *cross[i];
          l= cross[i][1];
          m= cross[i][4];
          n= cross[i][5];
          cross[f][l]= m;
          cross[f][l|1]= n;
          cross[m][n]= f;
          cross[m][n|1]= l;
        }

        else 
        {
          f= cross[i][2];
          l= cross[i][3];
          m= cross[i][6];
          n= cross[i][7];
          cross[f][l]= m;
          cross[f][l|1]= n;
          cross[m][n]= f;
          cross[m][n|1]= l;
        }

        while (*--p==127);
        l= *p;
        *p= 127;
        if (l<0)
        {
          l&=127;
          f= *cross[l];
          m= cross[l][4];
          n= cross[l][5];
          d= cross[l][1];
          cross[f][d]= m;
          cross[f][d|1]= n;
          cross[m][n]= f;
          cross[m][n|1]= cross[l][1];
        }

        else 
        {
          f= cross[l][2];
          m= cross[l][6];
          n= cross[l][7];
          d=cross[l][3];
          cross[f][d]= m;
          cross[f][d|1]= n;
          cross[m][n]= f;
          cross[m][n|1]= cross[l][3];
        }

        top-=4;
                        /* relink crossings */
        cross[i][g]= m= cross[k][h];
        cross[i][g|1]= n= cross[k][h|1];
        cross[m][n]= i;
        cross[m][n|1]= g;
        sign[i]= sign[k];
        cross[k][g]= cross[k][h]= i;
        m= h^4;
        cross[k][g|1]= m;
        cross[i][m|1]= g;
        g^=4;
        cross[i][m]= cross[i][g]= k;
        cross[k][h|1]= g;
        cross[i][g|1]= h;
        sign[l]= sign[j];
        n=g;
        d=h;
        if (sign[j]!=sign[k])
        {
          if (h!=0) m=h;
          else 
          {
            g^=4;
            n^=4;
            h=4;
          }
        }

        else g^=4;
        cross[i][d]= l;
        cross[i][d|1]= m;
        cross[l][m]= i;
        cross[l][m|1]= d;
        cross[l][g]= i= cross[j][h];
        cross[l][g|1]= k= cross[j][h|1];
        cross[i][k]= l;
        cross[i][k|1]= g;
        g^=4;
        m^=4;
        cross[j][n]= cross[j][h]= l;
        cross[j][n|1]= m;
        cross[j][h|1]= g;
        cross[l][m]= cross[l][g]= j;
        cross[l][m|1]= n;
        cross[l][g|1]= h;
        overs-=4;
        --numlps;
        rmcir(top,numcrs);
        numcrs-= overs+unders+xcrs;
        goto STEP1;
      }

      else if (bstcnt==3)
      {
        i=2;
        k= 0;
        m= -1;
        if (overs==3)
        {
          while (i--!=0)
          {
            while (p[k]<0 || p[k]==127) ++k;
            if ((p[k+1]<0 || p[k+1]==127) && (k!=0 || (k==0 && (p[top-1]<0 ||
              p[top-1]==127))))
            {
              if (m<128) m=0;
              m|= p[k];
            }

            else 
            {
              if (m<0) m=128;
              else if (m==128) m|= p[k];
              else m|=128;
              if (p[k+1]>=0 && p[k+1]!=127) ++k;
            }

            ++k;
          }
        }

        else 
        {
          while (--i>=0)
          {
            while (p[k]>=0) ++k;
            if (p[k+1]>=0 && (k!=0 || (k==0 && p[top-1]>=0)))
            {
              if (m<128) m=0;
              m|= p[k]&127;
            }

            else 
            {
              if (m<0) m=128;
              else if (m==128) m|= p[k]&127;
              else m|=128;
              if (p[k+1]<0) ++k;
            }

            ++k;
          }
        }

        if (m>127)
        {
          m&= 127;
          i= 2-overs-unders-xcrs;
          if (bstlst[m]> -2) bstlst[m]= -1;
          else if (bstlst[m]>i) bstlst[m]= i;
          goto NXTCIR;
        }
      }
    }

    else if (xcrs==1 && top==3 && (n-1)*(*p+1)<0)
    {
      i= *xlist&127;
      j= sign[i];          /* flip 3 crossing circuits if possible */
      if (n==0)
      {
        l= *p;
        n= p[1]&127;
        f= 4;
        k= j;
        m= j^4;
      }

      else 
      {
        l= p[1];
        n= *p&127;
        f=0;
        k= j^4;
        m= j;
      }

      if (sign[l]-f==2)
      {
        g= j^2;
        h= g^4;
        j= 6;
      }

      else 
      {
        h= j^2;
        g= h^4;
        j= 2;
      }

      d= cross[l][m];
      e= cross[l][m|1];
      cross[l][m]= cross[n][g];
      cross[l][m|1]= cross[n][g|1];
      if (d!=n)
      {
        cross[n][g]= d;
        cross[n][g|1]= e;
      }

      else 
      {
        cross[n][g]= l;
        cross[n][g|1]= k;
      }

      cross[cross[l][m]][cross[l][m|1]]= l;
      cross[cross[l][m]][cross[l][m|1]|1]= m;
      d= cross[n][h];
      e= cross[n][h|1];
      cross[n][h]= cross[l][k];
      cross[n][h|1]= m= cross[l][k|1];
      if (d!=l)
      {
        cross[l][k]= d;
        cross[l][k|1]= e;
      }

      else
      {
        cross[l][k]= n;
        cross[l][k|1]= g;
      }

      cross[cross[n][h]][m]= n;
      cross[cross[n][h]][m|1]= h;
      m= cross[n][g|1];
      cross[cross[n][g]][m]= n;
      cross[cross[n][g]][m|1]= g;
      d= cross[l][k];
      e= cross[l][k|1];
      cross[d][e]= l;
      cross[d][e|1]= k;
      cross[l][f]= g= cross[i][f];
      cross[l][f|1]= h= cross[i][f|1];
      cross[g][h]= l;
      cross[g][h|1]= f;
      cross[n][j]= g= cross[i][k];
      cross[n][j|1]= h= cross[i][k|1];
      cross[g][h]= n;
      cross[g][h|1]= j;
      goto SQUISH;
    }

    if (bstcnt==1)
    {
      if (overs==1)
      {
        while (*p<0 || *p==127) ++p;
      }
  
      else 
      {
        while (*p>=0) ++p;
      }

      k= *p&127;
      i=overs+unders+xcrs;
      if (bstlst[k]>=0)
      {
        if (bstlst[k]<i) bstlst[k]= i;
      }

      else if (bstlst[k]> -2-i) bstlst[k]= -2 -i;
      *dspair=1;
    }

    else if (*dspair>bstcnt || (*dspair==bstcnt && dspair[1]<overs+unders+xcrs))
    {
      i=0;
      if (overs<unders)
      {
        while (p[i]<0 || p[i]==127) ++i;
      }

      else 
      {
        while (p[i]>=0) ++i;
      }

      *dspair= bstcnt;
      dspair[1]= overs+unders+xcrs;
      dspair[2]= p[i]&127;
    }

    NXTCIR:
    if (n==0) n= sign[vnum]&6;
    else 
    {
      ++vnum;
      n=0;
    }
  }

/* if knot is tiny, store polynomial coefficients and restart a stored knot */

  if (numcrs<6)
  {
    if (numcrs==0) --numlps;
    if (numlps>12)
    {
      *count= 0;
      goto NEWNOT;
    }

    delpow(numlps);
    lngi= 1;
    if ((xpow&128)!=0)
    {
      lngi= -1;
      xpow&=127;
    }

    if (numcrs==4)
    {
      g=0;
      k=6;
      while (k!=0 && cross[j][k]==cross[j][k-2]) k-=2;
      l= cross[j][k];
      n= cross[j][k-2];
      if (k==6)
      {
        if (*cross[j]==cross[j][2] && (*cross[j]==n || *cross[j]==l)) g= -3;
      }

      else if (k==4)
      {
        if (*cross[j]==cross[j][6] && (*cross[j]==n || *cross[j]==l)) g= -3;
      }
 
      else if (k==2)
      {
        if (cross[j][4]==cross[j][6] && (cross[j][4]==n || cross[j][4]==l)) g= -3;
      }

      if (k==0) g= -4;
      else if (g==0)
      {
        j=4;
        m=0;
        while (j--!=0) m+=sign[j];
        if (m!=16) g=2;    /* flag for 2 links */
        else g=0;          /* flag for knot */
        j=1;
      }
    }

    else if (numcrs==5)
    {
      f=10;
      n= 0;
      vnum= g= -1;
      while ((vnum!=0 || n!=0) && f>0)
      {
        if (vnum<0) vnum=0;
        m= cross[vnum][n];
        n= cross[vnum][n|1]^4;
        vnum= m;
        --f;
      }

      if (vnum+n!=0) count[2]= -100000000;    /* knot has become inconsistent */
      if (f>0)
      {
        n=h= 0;   /* 5 crossing 2 link  or twisted 3 link  or tref and 2 links */
        j=5;
        g=m= 1;
        while (j--!=0) n+=sign[j];
        j=0;
        if (n>10 && n<30)
        {
          if (n>20)
          {
            while (sign[j]==6) ++j;
          }

          else 
          {
            while (sign[j]==2) ++j;
          }

          if (n==14 || n==26)
          {
            sign[j]= (n-8)/3;
            goto FOURX;
          }

          k=6;
          while (k!=0 && cross[j][k]==cross[j][k-2]) k-=2;
          if (k==0) g=3;                /* tref + link */
          else 
          {
            l= cross[j][k];
            n= cross[j][k-2];
            if (k==6)
            {
              if (*cross[j]!=cross[j][2] || (*cross[j]!=n && *cross[j]!=l)) g=2;
            }

            else if (k==4)
            {
              if (*cross[j]!=cross[j][6] || (*cross[j]!=n && *cross[j]!=l)) g=2;
            }

            else if (k==2)
            {
              if (cross[j][4]!=cross[j][6] || 
                (cross[j][4]!=n && cross[j][4]!=l)) g=2;
            }

            if (g!=2)
            {
              h=4;
              if (sign[j]==6) m= -1;
            }

            else 
            {
              n=0;
              if (sign[(int)(*cross[j])]!=sign[j]) n=4;
              k= cross[j][n];
              if (cross[k][n]!=j)
              {
                if (sign[j]==6) m= -1;
              }

              else if (sign[j]==2) m= -1;
            }
          }
        }

        if (g==1)
        {
          if (h==0 && sign[j]==2) m= -1;
          n=0;
          while (n!=5) xlist[n++]=0;
          while (n--!=0)
          {
            j= sign[n]; /* a tref + circle has fake bigons at all crossings */
            p= cross[n]; /* so find all fake bigons */
            if (p[1]==j^4 && (*p==p[2] || *p==p[6])) xlist[n]=xlist[(int)(*p)]= 1;
            if (p[5]==j && p[4]==p[j]) xlist[n]=xlist[(int)(p[4])]= 1;
          }

          n=0;
          while (n!=5 && xlist[n]!=0) ++n;
          if (n!=5)
          {          /* found twisted 3 link --  "squish" twist */
            while (++n!=5) sign[n-1]= sign[n]; /* before making polynomial */  
            FOURX:
            numcrs=4;
            g= -3;           /* flag for 4 crossing 3 links */
          }
 
          else 
          {
            p= *cross;
            if (f==4 || (f==8 && *p==p[2] && *p==p[4] && *p==p[6])) g=3;
          }
        }
      }
    } 

    i= (*sign)/2 -2;
    if (numcrs==5)
    {
      if (g<0)
      {                /* knots */
        p= *cross;
        if (i>0)
        {
          if (*p==p[6] && p[2]==p[4] && cross[1][2]==cross[1][4]) g= -2;
        }

        else if (*p==p[2] && p[4]==p[6] && *cross[1]==cross[1][2]) g= -2;
        if (g== -1)
        {                 /* knot five-two */
          addin(lngi,ypow-i*6,lowx,xpow,numlps,poly,0);
          addin(lngi,ypow-i*4,lowx,xpow,numlps,poly,1);
          addin(-lngi,ypow-i*2,lowx,xpow,numlps,poly,1);
          addin(-2*lngi,ypow-i*7,lowx,++xpow,numlps,poly,0);
          addin(-2*lngi,ypow-i*5,lowx,xpow,numlps,poly,1);
          addin(-2*lngi,ypow-i*6,lowx,++xpow,numlps,poly,0);
          addin(lngi,ypow-i*7,lowx,++xpow,numlps,poly,0);
          addin(lngi,ypow-i*3,lowx,xpow,numlps,poly,0);
          addin(lngi,ypow-i*6,lowx,++xpow,numlps,poly,0);
          addin(lngi,ypow-i*4,lowx,xpow,numlps,poly,0);
        }

        else 
        {                   /* knot five-one */
          addin(2*lngi,ypow-i*6,lowx,xpow,numlps,poly,0);
          addin(3*lngi,ypow-i*4,lowx,xpow,numlps,poly,0);
          addin(lngi,ypow-i*9,lowx,++xpow,numlps,poly,0);
          addin(-lngi,ypow-i*7,lowx,xpow,numlps,poly,1);
          addin(-2*lngi,ypow-i*5,lowx,xpow,numlps,poly,0);
          addin(lngi,ypow-i*8,lowx,++xpow,numlps,poly,0);
          addin(-3*lngi,ypow-i*6,lowx,xpow,numlps,poly,0);
          addin(-4*lngi,ypow-i*4,lowx,xpow,numlps,poly,0);
          addin(lngi,ypow-i*5,lowx,++xpow,numlps,poly,0);
          addin(lngi,ypow-i*6,lowx,++xpow,numlps,poly,0);
          addin(lngi,ypow-i*4,lowx,xpow,numlps,poly,0);
        }
      }

      else 
      {                     /* links */
        if (g==1)
        {                   /* trefoil + circle */
          addin(lngi,ypow-m*(7-h),lowx,--xpow,numlps,poly,1);
          addin(3*lngi,ypow-m*(5-h),lowx,xpow,numlps,poly,0);
          addin(2*lngi,ypow-m*(3-h),lowx,xpow,numlps,poly,0);
          addin(-lngi,ypow-m*(8-h),lowx,++xpow,numlps,poly,1);
          addin(-3*lngi,ypow-m*(6-h),lowx,xpow,numlps,poly,1);
          addin(-3*lngi,ypow-m*(4-h),lowx,xpow,numlps,poly,0);
          addin(-4*lngi,ypow-m*(5-h),lowx,++xpow,numlps,poly,0);
          addin(-3*lngi,ypow-m*(3-h),lowx,xpow,numlps,poly,0);
          addin(2*lngi,ypow-m*(4-h),lowx,++xpow,numlps,poly,0);
          addin(lngi,ypow-m*(7-h),lowx,++xpow,numlps,poly,0);
          addin(2*lngi,ypow-m*(5-h),lowx,xpow,numlps,poly,0);
          addin(lngi,ypow-m*(3-h),lowx,xpow,numlps,poly,0);
        }

        else if (g==2)
        {                      /* 2 component link five-one */
          addin(lngi,ypow+m,lowx,--xpow,numlps,poly,0);
          addin(lngi,ypow-m,lowx,xpow,numlps,poly,0);
          addin(-lngi,ypow,lowx,++xpow,numlps,poly,0);
          addin(-2*lngi,ypow+m*3,lowx,++xpow,numlps,poly,1);
          addin(-4*lngi,ypow+m,lowx,xpow,numlps,poly,0);
          addin(-2*lngi,ypow-m,lowx,xpow,numlps,poly,0);
          addin(lngi,ypow+m*4,lowx,++xpow,numlps,poly,0);
          addin(-lngi,ypow,lowx,xpow,numlps,poly,1);
          addin(3*lngi,ypow+m,lowx,++xpow,numlps,poly,0);
          addin(lngi,ypow-m,lowx,xpow,numlps,poly,0);
          addin(lngi,ypow+m*2,lowx,++xpow,numlps,poly,0);
        }

        else 
        {                      /* trefoil and circle + circle */
          i=0;
          m=1;
          j=5;
          while (j--!=0) i+= sign[j]/-2;
          if (i>-10)
          {
            m= -1;
            i= -20-i;
          }

          i+=15;
          xpow-=2;
          addin(lngi,ypow-m*(8-i),lowx,xpow,numlps,poly,0);
          addin(4*lngi,ypow-m*(6-i),lowx,xpow,numlps,poly,0);
          addin(5*lngi,ypow-m*(4-i),lowx,xpow,numlps,poly,0);
          addin(2*lngi,ypow-m*(2-i),lowx,xpow,numlps,poly,0);
          addin(-lngi,ypow-m*(9-i),lowx,++xpow,numlps,poly,1);
          addin(-5*lngi,ypow-m*(7-i),lowx,xpow,numlps,poly,1);
          addin(-9*lngi,ypow-m*(5-i),lowx,xpow,numlps,poly,1);
          addin(-5*lngi,ypow-m*(3-i),lowx,xpow,numlps,poly,1);
          addin(-2*lngi,ypow-m*(6-i),lowx,++xpow,numlps,poly,0);
          addin(-4*lngi,ypow-m*(4-i),lowx,xpow,numlps,poly,0);
          addin(-3*lngi,ypow-m*(2-i),lowx,xpow,numlps,poly,0);
          xpow+=2;
          addin(lngi,ypow-m*(4-i),lowx,xpow,numlps,poly,0);
          addin(lngi,ypow-m*(2-i),lowx,xpow,numlps,poly,0);
          addin(-lngi,ypow-m*(7-i),lowx,++xpow,numlps,poly,0);
          addin(-2*lngi,ypow-m*(5-i),lowx,xpow,numlps,poly,0);
          addin(-lngi,ypow-m*(3-i),lowx,xpow,numlps,poly,0);
        }
      }
    }

    else if (numcrs==4)
    {
      if (g==0)
      {                            /* knot 4-1 */
        addin(-lngi,ypow-2,lowx,xpow,numlps,poly,0);
        addin(-lngi,ypow,lowx,xpow,numlps,poly,0);
        addin(-lngi,ypow+2,lowx,xpow,numlps,poly,0);
        addin(-lngi,ypow-1,lowx,++xpow,numlps,poly,1);
        addin(-lngi,ypow+1,lowx,xpow,numlps,poly,1);
        addin(lngi,ypow-2,lowx,++xpow,numlps,poly,0);
        addin(2*lngi,ypow,lowx,xpow,numlps,poly,0);
        addin(lngi,ypow+2,lowx,xpow,numlps,poly,0);
      }

      else if (g>0)
      {            
        i=1;
        j=0;
        k= **cross;
        if (*cross[k]==0)
        {
          j=8;
          if (*sign==6) i= -1;        /* 2 link 4^2-1 */
        }

        else if (*sign==2) i= -1;
        addin(lngi,ypow-i*(5-j),lowx,--xpow,numlps,poly,0);
        addin(lngi,ypow-i*(3-j),lowx,xpow,numlps,poly,0);
        addin(-lngi,ypow-i*(4-j),lowx,++xpow,numlps,poly,1);
        addin(lngi,ypow-i*(7-j),lowx,++xpow,numlps,poly,0); 
        addin(-2*lngi,ypow-i*(5-j),lowx,xpow,numlps,poly,0);
        addin(-3*lngi,ypow-i*(3-j),lowx,xpow,numlps,poly,0);
        addin(lngi,ypow-i*(6-j),lowx,++xpow,numlps,poly,0);
        addin(lngi,ypow-i*(5-j),lowx,++xpow,numlps,poly,0);
        addin(lngi,ypow-i*(3-j),lowx,xpow,numlps,poly,0);
      }

      else if (g== -3)
      {                               /* 3 link */
        i=0;
        j=4;
        while (j--!=0) i+= sign[j]/-2;
        i+=8;
        xpow-=2;
        addin(lngi,ypow-2+i,lowx,xpow,numlps,poly,0);
        addin(2*lngi,ypow+i,lowx,xpow,numlps,poly,0);
        addin(lngi,ypow+2+i,lowx,xpow,numlps,poly,0);
        addin(-2*lngi,ypow-1+i,lowx,++xpow,numlps,poly,1);
        addin(-2*lngi,ypow+1+i,lowx,xpow,numlps,poly,1);
        addin(-2*lngi,ypow-2+i,lowx,++xpow,numlps,poly,0);
        addin(-3*lngi,ypow+i,lowx,xpow,numlps,poly,0);
        addin(-2*lngi,ypow+2+i,lowx,xpow,numlps,poly,0);
        xpow+=2;
        addin(lngi,ypow-2+i,lowx,xpow,numlps,poly,0);
        addin(2*lngi,ypow+i,lowx,xpow,numlps,poly,0);
        addin(lngi,ypow+2+i,lowx,xpow,numlps,poly,0);
      }

      else 
      {              /* circle + circle  and  circle + circle */
        i=0;
        j=4;
        while (j--!=0) i+= sign[j]/-2;         
        i+=8;
        xpow-=3; 
        addin(lngi,ypow-3+i,lowx,xpow,numlps,poly,0);
        addin(3*lngi,ypow-1+i,lowx,xpow,numlps,poly,1);
        addin(3*lngi,ypow+1+i,lowx,xpow,numlps,poly,1);
        addin(lngi,ypow+3+i,lowx,xpow,numlps,poly,0);
        addin(-3*lngi,ypow-2+i,lowx,++xpow,numlps,poly,0);
        addin(-6*lngi,ypow+i,lowx,xpow,numlps,poly,0);
        addin(-3*lngi,ypow+2+i,lowx,xpow,numlps,poly,0);
        addin(-2*lngi,ypow-3+i,lowx,++xpow,numlps,poly,0);
        addin(-2*lngi,ypow+3+i,lowx,xpow,numlps,poly,0);
        addin(4*lngi,ypow-2+i,lowx,++xpow,numlps,poly,0);
        addin(7*lngi,ypow+i,lowx,xpow,numlps,poly,0);
        addin(4*lngi,ypow+2+i,lowx,xpow,numlps,poly,0);
        addin(lngi,ypow-3+i,lowx,++xpow,numlps,poly,0);
        addin(lngi,ypow-1+i,lowx,xpow,numlps,poly,0);
        addin(lngi,ypow+1+i,lowx,xpow,numlps,poly,0);
        addin(lngi,ypow+3+i,lowx,xpow,numlps,poly,0);
        addin(-lngi,ypow-2+i,lowx,++xpow,numlps,poly,0);
        addin(-2*lngi,ypow+i,lowx,xpow,numlps,poly,0);
        addin(-lngi,ypow+2+i,lowx,xpow,numlps,poly,0);
      }
    }

    else if (numcrs==3)
    {
      addin(-2*lngi,ypow-i*2,lowx,xpow,numlps,poly,0);
      addin(-lngi,ypow-i*4,lowx,xpow,numlps,poly,0);
      addin(lngi,ypow-i*3,lowx,++xpow,numlps,poly,0);
      addin(lngi,ypow-i*5,lowx,xpow,numlps,poly,0);
      addin(lngi,ypow-i*2,lowx,++xpow,numlps,poly,0);
      addin(lngi,ypow-i*4,lowx,xpow,numlps,poly,0);
    }

    else if (numcrs!=0)
    {
      addin(lngi,ypow-i*2,lowx,xpow,numlps,poly,0);
      addin(-lngi,ypow-i*1,lowx,--xpow,numlps,poly,1);
      addin(-lngi,ypow-i*3,lowx,xpow,numlps,poly,1);
    }

    else addin(lngi,ypow,lowx,xpow,numlps,poly,0);
    if (notbeg!=0)
    {
      numcrs= buf[notbeg-1];
      numlps= buf[notbeg-4];
      xpow= ((short) buf[notbeg-3])&255;
      ypow= buf[notbeg-2];
      if ((numcrs&128)!=0)
      {
        numcrs&= 127;
        ypow= -ypow;
      }

      notbeg-= 4+ numcrs*8;
      i=0;
      m= notbeg;
      while (i!=numcrs)
      {
        j=0;
        while (j!=8)
        {
          cross[i][j++]= buf[m++];
          cross[i][j++]= buf[m++]&6;
        }

        sign[i++]= buf[m-7]>>4;
      }
    }

    else numcrs=0;
  }
 
  else 
  {
    i= -1;
    while (++i!=numcrs) clist[1][i]= (*clist)[i]= -1;
                    /* locate fake bigons for bstlst -- 2 cases */
    while (i--!=0)
    {               /* store the fake bigon endcrossings in clist */
      j= sign[i];
      p= cross[i];
      if (p[1]==j^4 && (*p==p[2] || *p==p[6])) (*clist)[i]= *p;
      if (p[5]==j && p[4]==p[j]) clist[1][i]= p[4];
    }

    i=0;                 /* don't need this if using vertices */
    while (i!=numcrs)
    {               /* remove duplicates to find fake bigon strings */
      while (i!=numcrs && clist[1][i]<0 && (*clist)[i]<0) ++i;
      if (i!=numcrs)
      {
        j= (*clist)[i];
        k= 2;
        c=xlist;
        n= 0;
        while (k--!=0)
        {
          l=1;
          if (k==0) i=j;
          while (i>=0)
          {
            *c=(char) i;
            i=clist[l][i];
            if (i<0 && l!=0 && n!=0 && *(c-1)!= (*clist)[(int)(*c)]) i= (*clist)[(int)(*c)];
            else l^=1;
            clist[1][(int)(*c)]= (*clist)[(int)(*c)]= -1;
            n++;
            c++;
          }
        }

        *c= -1;
        i=0;
        c= xlist;
        while (*c>=0)
        {                 /* store good bust value for this string in bstlst */
          if (bstlst[(int)(*c)]>0) bstlst[(int)(*c)]= 1-n - bstlst[(int)(*c)];
          else bstlst[(int)(*c)]-= 1+n;
          ++c;
        }
      }
    } 

    m=n= 0;
    while (i!=0)
    {
      if (bstlst[--i]<0)
      {
        if (bstlst[i]<n)
        {
          n=bstlst[i];
          j=i;
        }
      }

      else if (bstlst[i]!=0)
      {
        if (bstlst[i]>m)
        {
          m=bstlst[i];
          k=i;
        }
      }
    }

    tobust=j;
    if (n==0)
    {
      if (m==0) tobust= dspair[2];
      else tobust=k;
    }

    c= xlist;
    p= sign;
    *inbuf= buf+notbeg;
    lp3= *cross;
    lp4= *inbuf;
    *bstlst=i=0;
    while (i++!=numcrs)
    {
      bstlst[i]=0;
      *(c++)= *(p++);
      inbuf[i]= inbuf[i-1]+8;
      for (i1=1; i1<=8; ++i1) *(lp4++)= *(lp3++);
    }

    l= sign[(int)(tobust)]/2;
    dir= 2-l;
    m= cross[tobust][7-l];
    n= cross[tobust][8-l];
    j= cross[tobust][5-l];
    k= cross[tobust][6-l];
    inbuf[j][k]=(char) m;
    inbuf[j][k|1]=(char) n;
    inbuf[m][n]=(char) j;
    inbuf[m][n|1]=(char) k;
    k^=4;
    while (j!=tobust)
    {
      i=j;
      if ((sign[i]&1)==0) bstlst[i]^= 4-sign[i];
      if (k==4)
      {
        char ch;
        
        g=inbuf[i][3]|1;    /* g and h save values from being accidentally */
        h=inbuf[i][7]|1;     /* modified by the first two "^=4"s */
        inbuf[*inbuf[i]][inbuf[i][1]|1]^=4;
        inbuf[inbuf[i][4]][inbuf[i][5]|1]^=4;
        inbuf[inbuf[i][2]][g]^=4;
        inbuf[inbuf[i][6]][h]^=4;
        lp3= inbuf[i];
        lp4= lp3+4;
        for (i1=0; i1<4; ++i1)
        {
          ch= *(lp3+i1);
          *(lp3+i1)= *(lp4+i1);
          *(lp4+i1)= ch;
        }
        k=0;
      }

      xlist[i]^=4;
      h=k;
      j= inbuf[i][h];
      k= inbuf[i][h|1]^4;
    }

    if (k==4) g= sign[(int) tobust];
    else g=0;
    j= inbuf[i][h]= inbuf[(int) tobust][g];
    k= inbuf[i][h|1]= inbuf[(int) tobust][g|1];
    inbuf[j][k]=(char) i;
    inbuf[j][k|1]=(char) h;
                  /* squish crossing out of the list */
    i= numcrs-1-tobust;
    lp3= inbuf[tobust];
    lp4=lp3+8;
    p= xlist+tobust;
    c=p+1;
    while (i--!=0)
    {
      for (i1=1; i1<=8; ++i1) *(lp3++)= *(lp4++);
      *(p++)= *(c++);
    }
          /* renumber branch pointers (subtract 1 if the crossing was after) */
    i= numcrs-1;
    while (i!=0)
    {
      if (*inbuf[--i]>=tobust) --*inbuf[i];
      if (inbuf[i][2]>=tobust) --inbuf[i][2];
      if (inbuf[i][4]>=tobust) --inbuf[i][4];
      if (inbuf[i][6]>=tobust) --inbuf[i][6];
      inbuf[i][1]|= xlist[i]<<4;
    }

    j=numcrs;
    i= dir+ypow;
    while (j--!=0) i+= bstlst[j];
    if (i>239 || i<-238)
    {                /* 17 could be added or subtracted later, so protect */
      printf ("exceeded max. range of powers of y, ~ +/-255\n");
      *count= 0;
      goto NEWNOT;
    }

    notbeg+= numcrs*8 - 4;
    buf[notbeg-4]= (char)numlps;
    buf[notbeg-3]= (char)(xpow+1);
    if (i<0)
    {                    /* save a minus bit for ypow */
      buf[notbeg-1]= 128|(numcrs-1);
      buf[notbeg-2]= (char)(-i);
    }

    else 
    {
      buf[notbeg-2]= (char)i;
      buf[notbeg-1]= (char)(numcrs-1);
    }

    p=sign;
    c=xlist;
    if (tobust==0) ++p;
    lp3= *cross;
    *inbuf= buf+notbeg;
    lp4= *inbuf;
    i=0;
    while (i++!=numcrs)
    {
      *(c++)= *(p++);
      if (i==tobust) ++p;
      inbuf[i]= inbuf[i-1]+8;
      for (i1=1; i1<=8; ++i1) *(lp4++)= *(lp3++);
    }

    j= cross[tobust][3*l-3];    /* m, n, and l are from the same  */
    k= cross[tobust][3*l-2];    /* as for the infinity knot above */
    inbuf[j][k]= (char)m;
    inbuf[j][k|1]= (char)n;
    inbuf[m][n]= (char)j;
    inbuf[m][n|1]= (char)k;
    m= cross[tobust][5-l];
    n= cross[tobust][6-l];
    j= cross[tobust][3-l];
    k= cross[tobust][4-l];
    inbuf[j][k]= (char)m;
    inbuf[j][k|1]= (char)n;
    inbuf[m][n]= (char)j;
    inbuf[m][n|1]= (char)k;
          /* squish crossing out of the list (xlist has already been squished) */
    i= numcrs-1-tobust;
    lp3= inbuf[tobust];
    lp4=lp3+8;
    while (i--!=0)
    {
      for (i1=1; i1<=8; ++i1) *(lp3++)= *(lp4++);
    }

      /* renumber branch pointers (subtract 1 if the crossing was after) */
    i= numcrs-1;
    while (i!=0)
    {
      if (*inbuf[--i]>=tobust) --*inbuf[i];
      if (inbuf[i][2]>=tobust) --inbuf[i][2];
      if (inbuf[i][4]>=tobust) --inbuf[i][4];
      if (inbuf[i][6]>=tobust) --inbuf[i][6];
      inbuf[i][1]|= xlist[i]<<4;
    }
 
    notbeg+= numcrs*8 - 4;
    i= ypow+dir;
    if (i>239 || i<-238)
    {
      printf ("exceeded max. range of powers of y, ~ +/-255\n");
      *count= 0;
      goto NEWNOT;
    }

    buf[notbeg-4]= (char)numlps;
    buf[notbeg-3]= (char)(xpow+1);
    if (i<0)
    {                    /* save a minus bit for ypow */
      buf[notbeg-2]= (char)(-i);
      buf[notbeg-1]= (char)(128|(numcrs-1));
    }

    else 
    {
      buf[notbeg-2]= (char)i;
      buf[notbeg-1]= (char)(numcrs-1);
    }

    sp= (short *) cross[tobust];
    i= *sp;
    *sp= sp[l];
    sp[l]= sp[2];
    sp[2]= sp[l^2];
    sp[l^2]= i;
    ypow+= dir*2;
    xpow^=128;
    if (ypow>239 || ypow<-238)
    {
      printf ("exceeded max. range of powers of y, ~ +/-255\n");
      *count= 0;
      goto NEWNOT;
    }
 
    sign[(int) tobust]^=4;
    lp1= (long *) cross[(int) tobust];
    cross[*((char *) lp1)][((char *) lp1)[1]|1]=0;
    cross[((char *) lp1)[2]][((char *) lp1)[3]|1]=2;
    cross[((char *) lp1)[4]][((char *) lp1)[5]|1]=4;
    cross[((char *) lp1)[6]][((char *) lp1)[7]|1]=6;
    ++count[numcrs-1];
  }

  if (numcrs!=0) goto STEP1;
  goto NEWNOT;
}


rmcir(top,numcrs)
short top, numcrs;
{
  register short i;
  short g, h, j, k, m, n, i1;
  long *lp1, *lp2;
  char *lp3, *lp4;
  char *p, *c;
  m=xcrs;
  if (m!=0 && xlist[m-1]<0)
  {
    i= xlist[--m]&127;
    g= xlist[xcrs--];
    k= crsptr[i][g];
    j= crsptr[i][g|1];
    if (g!=0) g=4;
    else g= (sign[i]&6)^4;
    h= crsptr[k][j]= crsptr[i][g];
    n= crsptr[k][j|1]= crsptr[i][g|1];
    crsptr[h][n]= (char)k;
    crsptr[h][n|1]= (char)j;
    crsptr[i][4]= *crsptr[i]= (char)i;
    crsptr[i][1]= crsptr[i][5]= 4;
    --numlps;
  }

  else ++numlps;
  i=0;
  while (m!=0)
  {
    if ((*clist)[i]==127)
    {
      j= xlist[--m];
      (*clist)[i]=crsptr[j][6]=crsptr[j][2]=crsptr[j][4]= *crsptr[j]= (char)j;
    }

    ++i;
  }

  j=0;
  while (top>j+1)
  {
    i=k= j;
    while (++i!=top)
    {
      if (((*clist)[i]&127) < ((*clist)[k]&127)) k=i;
    }

    if (k!=j)
    {
      i= (*clist)[j];
      (*clist)[j]= (*clist)[k];
      (*clist)[k]=i;
    }

    ++j;
  }

                         /* relink */
  m= -1;
  while (++m!=top && (*clist)[m]!=127)
  {
    i= (*clist)[m];
    if (i>=0) g=2;
    else 
    {
      i&=127;
      (*clist)[m]=i;
      g=0;
    }

    k= crsptr[i][g];
    j= crsptr[i][g|1];
    h= crsptr[k][j]= crsptr[i][g|4];
    n= crsptr[k][j|1]= crsptr[i][g|5];
    crsptr[h][n]= (char)k;
    crsptr[h][n|1]= (char)j;
  }

                /* squish crossings out of the list */
  lp3= crsptr[(int)(**clist)];
  lp4=lp3+8;
  p=sign+ **clist;
  c=p+1;
  j= 0;
  k=1;
  i= **clist;
  while (++i!=numcrs)
  {
    if ((*clist)[k]== i)
    {
      lp4+=8;
      ++c;
      if (++k==top) k=0;
    }

    else 
    {
      for (i1=1; i1<=8; ++i1) *(lp3++)= *(lp4++);
      *(p++)= *(c++);
    }
  }

  numcrs-= overs+unders+xcrs;
      /* renumber branch pointers (subtract 1 if the crossing was after) */
  h= -1;
  while (++h!=numcrs)
  {
    j= *crsptr[h];
    k=crsptr[h][2];
    m=crsptr[h][4];
    n=crsptr[h][6];
    i= top;
    while (i--!=0)
    {
      if ((*clist)[i]<j) --j;
      if ((*clist)[i]<k) --k;
      if ((*clist)[i]<m) --m;
      if ((*clist)[i]<n) --n;
    }

    *crsptr[h]=(char)j;
    crsptr[h][2]=(char)k;
    crsptr[h][4]=(char)m;
    crsptr[h][6]=(char)n;
  }
}

ntc(i,buf)
long i;
char *buf;
{
  long j;
  int r;
  char *p;
  p=buf;
  r=0;
  if (i<0)
  {
    i= -i;
    r=1;
    *(p++)= '-';
  }

  if (i>999999999) j=1000000000;
  else if (i>99999999) j=100000000;
  else if (i>9999999) j=10000000;
  else if (i>999999) j=1000000;
  else if (i>99999) j=100000;
  else if (i>9999) j=10000;
  else if (i>999) j=1000;
  else if (i>99) j=100;
  else if (i>9) j=10;
  else j=1;
  while (j!=0)
  {
    ++r;
    *(p++)= (i/j)%10+0x30;
    j/=10;
  }

  *p='\n';
  return (r);
}

delpow(power)
short power;
{
  register short *p, j, k;
  short coeff[64], minus;
  p=binoms;
  if (power<4)
  {
    if (power==1)
    {
      *(p++)= -1;
      *(p++)= 1;
      *(p++)= 1;
    }

    else if (power==2)
    {
      *(p++)= 1;
      *(p++)= -2;
      *(p++)= -2;
      *(p++)= 1;
      *(p++)= 2;
    }

    else if (power!=0)
    {
      *(p++)= -1;
      *(p++)= 3;
      *(p++)= 3;
      *(p++)= -3;
      *(p++)= -6;
      *(p++)= -3;
      *(p++)= 1;
      *(p++)= 3;
      *(p++)= 3;
    }

    *p= 1;
    return;
  }

  *coeff= 1;
  coeff[1]= 4;
  coeff[2]= 6;
  coeff[3]= 4;
  coeff[4]= 1;
  *(p++)= minus= 1;
  *(p++)= 1;
  *(p++)= 1;
  *(p++)= 1;
  *(p++)= 2;
  *(p++)= 1;
  *(p++)= 1;
  *(p++)= 3;
  *(p++)= 3;
  *(p++)= 1;
  *(p++)= 1;
  *(p++)= 4;
  *(p++)= 6;
  *(p++)= 4;
  *(p++)= 1;
  k=4;
  while (k++!=power)
  {
    j=k;
    coeff[j]=1;
    while (--j!=0) coeff[j]+=coeff[j-1];
    j= k+1;
    while (j!=0) *(p++)= coeff[--j];
  }

  j= k--;
  while (k!=0)
  {
    while (j--!=0) *--p *= coeff[k] * minus;
    j= k--;
    minus= -minus;
  }

  return;
}


addin(kcoeff,ypow,lowx,xpow,power,plybuf,flag)
short ypow, xpow, power, lowx;
long kcoeff, *plybuf[];
int flag;
{
  short i, j, *p;
  long *lp1, *lp2, addon;
  i=0;
  p=binoms;
  ++power;
  while (i!=power)
  {
    j= xpow-lowx;
    lp1= plybuf[j]+XCNT-j+ypow-i;
    if (flag!=0) lp2= plybuf[j+2]+XCNT-(j+2)+ypow-i;
    j= ++i;
    while (j--!=0)
    {
      addon= kcoeff* *(p++);
      *lp1+= addon;
      lp1+=2;
      if (flag!=0)
      {
        *lp2-= addon;
        lp2+=2;
      }
    }

    --xpow;
  }
}

conchk(numcrs)
short numcrs;
{
  register short i, j;
  int vnum, vbrnch, top, numlnk;
  char str[20], cbuf[256];
  vnum=vbrnch=numlnk= 0;
  i=top= numcrs*2;
  while (i>=0) cbuf[i--]=0;
  i=0;
  while (i!=top)
  {
    --numlnk;
    
    while (cbuf[i]==0)
    {
      cbuf[i]=1;
      i=vnum;
      j=vbrnch;
      vnum= crsptr[i][j];
      vbrnch= crsptr[i][j|1];


      i= (vnum<<1) + ((vbrnch>>1)&1);
      vbrnch^=4;
    }

    i=0;
    while (cbuf[i]!=0) ++i;
    vnum= i/2;
    if ((i&1)!=0) vbrnch= sign[vnum] & 6;
    else vbrnch=0;
  }

  return (numlnk);
}

