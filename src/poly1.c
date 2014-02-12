/* A load balanced calculation of the oriented polynomial */
/* Bruce Ewing and Kenneth C. Millett  (author: Ewing) */
/* Department of Mathematics */
/* University of California */
/* Santa Barbara, CA 93106 */
/* there is a hard limit of 127 crossings in this program */

/* Front end for use in Knotscape by MBT (also some minor alterations) */
/* This version will only deal with knots, and the limit is 50 crossings */
/* Program will process a batch of up to 100000 knots */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define XCNT   120    /* maximum "crossing count" */

FILE *fp, *fp2, *gp ;
int polybuf[XCNT*(XCNT+2)], *poly[XCNT], b[78];
char *crsptr[XCNT], sign[128], skirc[XCNT*2];
char buf[30000], cbuf[3074], clist[XCNT*2];
short numcrs, numlps, poslnk, neglnk, lowx, notbeg;

main(argc,argv)
int argc;
char *argv[];

{
  register short i, j, k, h, m, n;
  register char *p, *c, *q, **cross;
  int in, len, pause(), i1, j1, nc[100001], nk,
      w[100001], knotid[100001];
  long long int binpol[130][260], *bin[130];
  int lngi, count[XCNT], *lp1, kstrt;
  short g, xpow, ypow;
  char crsbuf[XCNT*8], nbuf[82], *s, bstlst[128], dspair[3], t[64],
            line[100], subline[50];


  fp=fopen(argv[1], "r");
  nk=0;

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

  fp=fopen(argv[2],"w");
  fp2=fopen(argv[3],"w");
  gp=fopen(argv[4],"w"); 
  nk=0;

  if (argc<2)
  {
    printf ("usage:  poly knotfile [knotfile ...]\n");
    exit(-1);
  }
  
  *count= i= j= kstrt= 0;
  cross= crsptr;
  *crsptr= crsbuf;
  while (++i!=XCNT) crsptr[i]= crsptr[i-1]+8;
  lp1= polybuf;
  while (j<XCNT)
  {
    poly[j]= lp1;
    lp1+= 1+ 2* (XCNT-j++);
  }

NEWFIL:

  {
    i= strlen(*argv);
    c= *argv+i-1;
    if (i>11) i=11;
    while (i--!=0 && *c!='/') --c;
    p=t;
    while (*++c!=0) *(p++)= *c;
    *(p++)= '.';
    *(p++)= 'l';
    *p=0;
  }

  if (--argc==3) exit(0);

  in=open(*++argv,0);
  i= strlen(*argv);
  c= *argv+i-1;
  if (i>11) i=11;
  while (i--!=0 && *c!='/') --c;
  p=t;
  while (*++c!=0) *(p++)= *c;
  *(p++)= '.';
  *(p++)= 'l';
  *p=0;

  s=cbuf+1;
  s[read(in,s,3072)]=0x80;

/**** computation of binomial coefficients (added by MBT) ****/

  {
    int i,j;
    for(i=0; i<=63; ++i) bin[i]=binpol[i]+64;
    bin[0][0]=1;

    for(i=1; i<=63; ++i)
   
    {
      bin[i][-i]=1; bin[i][i]=1;
      for(j=(-i+2); j<=(i-2); j+=2) 
        bin[i][j]= bin[i-1][j-1] + bin[i-1][j+1];
    }

    for(i=0; i<=62; i+=2)
    for(j=(-i); j<=i; j+=2) 
      if (j%4 != 0) bin[i][j] *= (-1);
  }

    
/******** end of computation of binomial coefficients ********/


NEWNOT:
  i= XCNT-1;
  while (i!=0 && count[i]==0) --i;

  if (*count!= (numcrs=0))
  {
    k= XCNT;
    i=0;
    while (k!=0 && i==0)
    {
      i= 2+ 2*(XCNT-k);
      lp1= poly[--k];
      while (--i!=0 && *lp1==0) ++lp1;
    }
    
    
/*************************** front end starts here ************************/


    {
      int i, j, k, jmin[150], jmax[150], jminn, imax, zer[150], wr, sgn;
      long long int vaugstore[401], *vaug;
      int *ppol[150], *intvaug, intvaugstore[401];
      ++nk;

      for(i=0;i<=60;i++) ppol[i]=(poly[i+1]-121+i-w[nk]);
          
      for(i=0;i<=nc[nk]-1;i+=1)
      {
        j=-nc[nk];
        while((ppol[i][j] == 0) && (j < nc[nk]))  j++;
        jmin[i]=j;
        j=nc[nk];
        while((ppol[i][j] == 0) && (j > -nc[nk]))  j--;
        jmax[i]=j;
        
        if (jmax[i]<jmin[i]) 
          {jmax[i]=jmin[i]=0; zer[i]=1;}
        else zer[i]=0;
      }

      imax=nc[nk]-1;
      while(zer[imax] == 1) imax--;
    
      fprintf(gp, "%2d.%3d\n", nc[nk], knotid[nk]);
      jminn=0;
      for(i=0;i<=imax;i+=1) if (jmin[i]<jminn) jminn = jmin[i];

      wr=w[nk];
          
      for(i=0;i<=imax;i+=2)  /* print homfly polynomial using Morton normalization */

      {
        fprintf(gp, "       %4d%4d      ", -(jmax[i]-wr), -(jmin[i]-wr));
        for(j=jmax[i];j>=jmin[i];j-=2)
	{
          k = 1 - (abs(((i+j - wr)/2)%2))*2;
         /* printf("%4d%4d%4d\n", i, j, k);*/
	  if (nc[nk] < 20) fprintf(gp, "%6d", (*(ppol[i]+j))*k);
	  else fprintf(gp, "%8d", (*(ppol[i]+j))*k);
	}
        fprintf(gp, "\n");
      }

      fprintf(gp, "\n");
      fflush(gp);
 
      for(i=0; i<=400; ++i) vaugstore[i]=0;
      vaug=vaugstore+150;
      intvaug = intvaugstore+150;

      for(i=0; i<=nc[nk]; i+=2)
      for(j=(-nc[nk]+i); j<=(nc[nk]-i); j+=2)
       
      {
        k=((j-wr)/2)%2;
        if (k == 0) sgn=1; else sgn=-1;
          
        for(k=(-i); k<=i; ++k)          
        {
          vaug[k] += bin[i][2*k] * ppol[i][j] * sgn;
        }
      }
 
      
    /*  for (i=-10; i<=10; ++i) printf("%4d", vaug[i]); printf("\n");*/
      
      for (i=-2*nc[nk]; i<=2*nc[nk]; ++i) intvaug[i] = (int) vaug[i];

      i=(-2*nc[nk]); while(intvaug[i] == 0) ++i; i1=i;
      j=  2*nc[nk] ; while(intvaug[j] == 0) --j; j1=j; /* Alexander poly */
    
      fprintf(fp, "%2d %8d     ", nc[nk], knotid[nk]);
      for(k=i; k<=j; ++k) 
      {
        if (nc[nk] < 20) fprintf(fp, "%5d", intvaug[k]);
	else fprintf(fp, "%10d", intvaug[k]);
      }
      fprintf(fp, "\n");
      fflush(fp);

      for (i=-2*nc[nk]; i<=2*nc[nk]; ++i) vaug[i]=0;
      wr*=-1;
        
      for(i=0; i<=nc[nk]; i+=2)
        for(j=(-nc[nk]+i); j<=(nc[nk]-i); j+=2)        
      {
        k=((j+wr)/2)%2;
        if (k == 0) sgn=1; else sgn=-1;
            
        for(k=(-i); k<=i; ++k)
        {
          vaug[k+j] += bin[i][2*k] * ppol[i][j] * sgn;
        }
      }
      
      for (i=-2*nc[nk]; i<=2*nc[nk]; ++i) intvaug[i] = (int) vaug[i];

      i=(-2*nc[nk]); while(intvaug[i] == 0) ++i; i1=i+wr;
      j=  2*nc[nk] ; while(intvaug[j] == 0) --j; j1=j+wr;
    
      fprintf(fp2, "%2d %7d      ", nc[nk], knotid[nk]); /* Jones poly */
      fprintf(fp2, "%3d  %3d     ", -j1, -i1);
      for(k=j; k>=i; --k)
      {
        if (nc[nk] < 20) fprintf(fp2, "%5d", intvaug[k]);
	else fprintf(fp2, "%10d", intvaug[k]);
      }
      fprintf(fp2, "\n");
      fflush(fp2);   
    }
    
/*********************** front end finishes here ************************/
    
    
  }

  while (*s=='\n') ++s;
  i= XCNT;
  while (i!=0) count[--i]=0;
  c=s;
  if (*s!='1' || ((*(s+1)&0x5f)!='V' && *(s+1)!='+' && *(s+1)!='-'))
  {
    p=s;
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
  p= crsbuf; /* HI also use next reg pointer for sign */
  while (*s!='\n' && *s>0)
  {
    while (*s>='0' && *s<='9') ++s;
    if (*s=='+') sign[numcrs]=6;      /* most of sign[] shows + or - crossings */
    else if (*s=='V') sign[numcrs]=7;   /* the 1 bit in sign[] shows whether */
    else if (*s=='-') sign[numcrs]=2;        /* the crossing is a vertex */
    else if (*s=='v') sign[numcrs]=3;
    else if (*s<0) --s;
    ++s;
    j=4;
    while (j--!= (i=0) && *s>0)
    {
      while (*s>='0' && *s<='9')
      {
        i*=10;
        i+= *(s++)-0x30;
      }

      *(p++)= i-1;
      if (*s>0)
      {
        *(p++)= (*(s++)-'a')*2;
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
  notbeg=numlps=poslnk=neglnk= xpow= ypow= 0;
  if (conchk()!=0) exit(0);
  *count=1;
  skirc[numcrs*2]=0;
  
STEP1:

/* FIRST -- remove figure 8 loops, 2 cases  "#-#d#c#b#a"  "#+#b#a#d#c" */
/* SECOND -- remove monogons (must recheck previous crossings) */
/* 4 cases  "#+#b#a????"  "#+????#d#c"  "#-#d????#a"  "#-??#c#b??" */
/* THIRD -- remove bigons -- make sure loops don't get forgotten. */
/* 2 cases "k?mcmb???? m???kbka??"  "k?mc????md  m?????kakd" */
/* bigon yanker MUST be completely thorough or loops can get lost later */

  *dspair=g= i= numcrs;
  c=p= cross[i];
  while (i--!=0)
  {                             /* loop through all crossings */
    p-=8;
    if (*p==i || p[4]==i)
    {
      monos(i,p);
      squish (i);
      goto STEP1;
    }

                  /* this bigon test checks that ka points to some mc, */
                  /* kd to md or kb to mb, and neither crossing is a vertex */
    if (p[1]==4 && ((*p==p[6] && p[7]==6) || (*p==p[2] && p[3]==2)))
    {
      bigons(i);
      goto STEP1;
    }

    bstlst[i]= 0;
  }

  if (numcrs<6) g=0;
                  /* find very good and ok triples */
  while (g--!= (lngi=0))
  {
    c-=8;
    p= cross[*c]+ ((c[1]^2)&2);
    q= cross[c[4]]+ ((c[5]^2)&2);
    if ((i= c[2]) != (j= c[6]) && *c!=c[4])
    {
      if ((m= *q) == (n= q[4])) m=n= -1;
      if ((h= *p) == (k= p[4])) h=k= -1;
      if ((h==i && k==j) || (j==h && k==i)) lngi=1;
      if ((m==i && n==j) || (m==j && n==i)) lngi|=2;
      if ((n==j || m==j) && (k==j || h==j)) lngi|=4;
      if ((m==i || n==i) && (h==i || k==i)) lngi|=8;
    }

    else 
    {
      if (i!=j)
      {           /* there is a ring through this crossing */
        i= g;
        j= *c;
        h=1;
      }

      else 
      {
        if (*c==c[4]) ++numlps;            /* it's a distant union with a link */
        j= g;
        h=3;
      }

      if (sign[j]!=sign[i])
      {                                    /* loose ring */
        k=0;
        if ((cross[g][h]&2)!=0) k=128;     /* if other crossing from g is under */
        *clist= i|k;
        clist[1]= j|k;
      }

      else 
      {
        if (sign[j]==2) ++neglnk;
        else ++poslnk;
        *clist= i;
        clist[1]= j|128;
        --numlps;             /* rmcir will add an extra numlps, so get rid of it here */
      }

      h=2;
      k= -1;
      rmcir(h,j,k,0);
      goto STEP1;
    }

    if (lngi!=0)
    {
      k= lngi;
      *dspair=1;
      if ((k&3)!=0)
      {
        if ((c[3]&c[7]&2)!=0)
        {
          triple((short) ((k&2)<<1),g,c);
          goto STEP1;
        }

        if (((c[3]^c[7])&2)!=0)
        {
          if ((c[((k&2)<<1)|1]&2)==0)
          {
            if (sign[(int) c[(k&2)<<1]]!=sign[g]) bstlst[i]= bstlst[j]= -5;
            else bstlst[i]= bstlst[j]= 2;
          }

          else 
          {
            n=2;
            if (sign[(int) c[(k&2)<<1]]==sign[g]) n= -2;
            p= bstlst+i;
            if ((c[3]&2)!=0)
            {
              p= bstlst+j;
              if (*p * *p< n*n) *p= n;
            }

            else if (*p * *p< n*n) *p= n;
          }
        }

        else if (bstlst[g]==0) bstlst[g]= 2;
      }

      if (k>3)
      {
        if (((c[1]|c[5])&2)==0)
        {
          triple((short) ((k&4)|2),g,c);
          goto STEP1;
        }

        if (((c[1]^c[5])&2)!=0)
        {
          if ((c[(k&4)|3]&2)!=0)
          {
            if (sign[(int) c[(k&4)|2]]!=sign[g]) bstlst[(int) *c]= bstlst[(int) c[4]]= -5;
            else bstlst[(int) *c]= bstlst[(int) c[4]]= 2;
          }

          else 
          {
            n=2;
            if (sign[(int) c[(k&4)|2]]==sign[g]) n= -2;
            p= bstlst + c[4];
            if ((c[1]&2)!=0)
            {
              p= bstlst+ *c;
              if (*p * *p< n*n) *p= n;
            }

            else if (*p * *p< n*n) *p= n;
          }
        }

        else if (bstlst[g]==0) bstlst[g]= 2;
      }
    }
  }

                     /* circuit remover */
  j= numcrs-1;
  if (j<3) j= -1;
  else if (j<11)
  {                  /* <12 will blow up */
    p= crsbuf+1;
    i=numcrs;
    while (i--!=0 && *p!=4) p+=8; /* only non-alternating (a to c) knots */
    if (i< (n=0)) j= -1;
    else p= cross[j]+5;
    while (j>=0)
    {
      n^=4;
      k= *(p--)^4;
      if ((k&2) == 0)
      {
        c= cross[*p]+k;
        i= *c;
        k= c[1]^4;
        if ((k&2) != 0 && cross[i][k]==j)
        {
          untwst ((short) *p,i,j,n);
          squish (j);
          goto STEP1;
        }
      }

      if (n==0) --j;
      p-=3;
    }
  }

  g= 0;
  c= clist;
  while ((n=j) >= (*b=b[2]=h= 0))
  {
    k=g;
    do 
    {
      p= cross[j]+k;
      j= *p;
      k= p[1]^4;
      i= h++;
      p=c;
      while (i--!=0 && (*(p++)&127) != j);
      if (i>=0)
      {
        j= n;              /* circuit crosses itself */
        goto NXTCIR; /* skip circuit, wait for a sub-piece that doesn't */
      }

      else 
      {
        ++b[k&2];
        *p= j|(k<<6);
      }
    } while (n!=j);
    
    if (k != g) --b[(g&2)^2];
    else k= -1;
    if (b[2]<*b) n=b[2];
    else n= *b;
    if (n==0)
    {
      rmcir(h,j,k,0);
      goto STEP1;
    }

    if (tstcir(h,(short) *b-n,bstlst,dspair) !=0) goto STEP1;
    
NXTCIR:
    if (g==0) g= sign[j];
    else 
    {
      --j;
      g=0;
    }
  }

            /* if knot is tiny, store polynomial coefficients and restart a stored knot */
            
  if (numcrs<6)
  {
    lngi= 1;
    if (numcrs==0) --numlps;
    delpow();
    if ((xpow&128)!=0)
    {
      lngi= -1;
      xpow&=127;
    }

    if (numcrs==4)
    {
      c= crsbuf;
      p= c+4;
      if (*p==*c || c[2]==p[2])
      {
        g= -3;
        if (*p==p[2] && *c==c[2]) g= -4;
      }

      else 
      {
        i=g= 0;  /* 0 is flag for knot */
        j=4;
        while (j--!=0) i+=sign[j];
        if (i!=16) g=2;     /* flag for 2 links   */
      }
    }

    else if (numcrs==5)
    {
      n=10;
      i=j= 0; /* step through knot, if don't get to end, it's a link */
      do 
      {
        p= cross[i]+j;
        i= *p;
        j= p[1]^4;
        --n;
      } while (i!=0 || j!=0);
      
      if (n>0)
      {
        i=k=h= 0; /* 5 crossing 2 link  or twisted 3 link  or tref and 2 links */
        j=5;      /* or tref + link */
        g=m= 1;
        while (j--!=0) k+=sign[j];
        if (k>10 && k<30)
        {
          if (k>20)
          {
            while (sign[i]==6) ++i;
          }

          else 
          {
            while (sign[i]==2) ++i;
          }

          if (k==14 || k==26) goto FOURX;
          c= cross[i];
          p= c+4;
          if (*p==*c || p[2]==c[2])
          {
            if (*p==p[2] && *c==c[2]) g=3;
            else 
            {
              h=4;
              if (sign[i]==6) m= -1;
            }
          }

          else 
          {
            g=2;
            j=0;
            if (sign[(int) *c]!=sign[i]) j=4;
            k= c[j];
            if (cross[k][j]!=i)
            {
              if (sign[i]==6) m= -1;
            }

            else if (sign[i]==2) m= -1;
          }
        }

        if (g==1)
        {
          if (h==0 && sign[i]==2) m= -1;
          i=j=k= 5;
          c= bstlst;
          while (j!=0) c[--j]=0;
          while (k--!=0)
          {
            j= sign[k]; /* trefs + circles have fake bigons at all crossings */
            p= cross[k]; /* so find all fake bigons */
            if (p[1]==(j^4) && (*p==p[2] || *p==p[6])) c[k]=c[*p]= 1;
            if (p[5]==j && p[4]==p[j]) c[k]=c[p[4]]= 1;
          }

          while (--i>=0 && c[i]!=0);
          if (i>=0)
          {
          
FOURX:         /* found twisted 3 link -- "squish" twist before making polynomial */

            while (++i!=6) sign[i-1]= sign[i];
            numcrs=4;
            g= -3;                  /* flag for 4 crossing 3 links */
          }

          else 
          {
            p= crsbuf;
            if (n==4 || (n==8 && *p==p[4] && p[2]==p[6])) g=3;
          }
        }
      }

      else g= -1;                   /* it's a knot! */
    }

    i= (*sign)/2 -2;
    k= xpow;
    n=ypow;
    if (numcrs==5)
    {
      if (g<0)
      {                             /* knots */
        p= crsbuf;
        if (i>0)
        {
          if (*p==p[6] && p[2]==p[4] && p[10]==p[12]) g= -2;
        }

        else if (*p==p[2] && p[4]==p[6] && p[8]==p[10]) g= -2;
        if (g== -1)
        {                               /* knot five-two */
          addin(lngi,n-i*6,k,0,0);      /* the flags at the end of addin  */
          addin(lngi,n-i*4,k,-1,2);     /* let you do two addins at once, */
          addin(-lngi,n-i*2,k,-1,2);    /* for addins with equal ypows.   */
        }

        else 
        {                               /* knot five-one */
          k+=2;
          addin(-4*lngi,n-i*4,k,0,0);
          addin(-lngi,n-i*6,k,-2,-2);
          k+=2;
          addin(lngi,n-i*4,k,3,-4);
        }
      }

      else 
      {                                 /* links */
        if (g==1)
        {                               /* trefoil + circle */
          if (h!=0)
          {
            addin(lngi,n-m*3,++k,-1,-2);
            addin(3*lngi,n-m,k,-1,-2);
            addin(lngi,n+m,k,-2,-2);
            k+=2;
            addin(-lngi,n-m,k,0,0);
          }

          else 
          {
            addin(-lngi,n-m*7,--k,0,0);
            addin(-3*lngi,n-m*5,k,0,0);
            k+=2;
            addin(3*lngi,n-m*3,k,0,0);
            addin(2*lngi,n-m*5,k,0,0);
            k+=2;
            addin(-lngi,n-m*3,k,2,-4);
          }
        }

        else if (g==2)
        {                                /* 2 component link five-one */
          addin(-lngi,n+m,--k,-2,2);
          addin(-lngi,n-m,k,-1,2);
          k+=2;
          addin(lngi,n+m*3,k,0,0);
          k+=2;
          addin(-lngi,n+m,k,0,0);
        }

        else 
        {                                /* trefoil and circle + circle */
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
          k-=2;
          if (i!=0)
          {
            addin(lngi,n-m*4,k,-1,2);
            addin(4*lngi,n-m*2,k,-1,2);
            addin(5*lngi,n,k,0,0);
            k+=2;
            addin(-lngi,n+m*2,k,-2,-2);
            k+=2;
            addin(lngi,n,k,-4,-2);
            addin(lngi,n-m*2,k,0,0);
          }

          else 
          {
            addin(lngi,n-m*8,k,0,0);
            addin(5*lngi,n-m*4,k,0,0);
            addin(2*lngi,n-m*2,k,0,0);
            k+=2;
            addin(-2*lngi,n-m*6,k,-2,-2);
            k+=2;
            addin(lngi,n-m*4,k,-5,-2);
            addin(lngi,n-m*2,k,-3,-2);
          }
        }
      }
    }

    else if (numcrs==4)
    {
      if (g==0)
      {                                        /* knot 4-1 */
        addin(-lngi,n-2,k,0,0);
        addin(-lngi,n,k,-1,2);
        addin(-lngi,n+2,k,0,0);
      }

      else if (g>0)
      {
        i=1;
        if (*cross[*crsbuf]==0)
        {
          if (*sign==6) i= -1; /* 2 link 4^2-1 */
          addin(-lngi,n+i*3,--k,-1,2);
          addin(-lngi,n+i*5,k,0,0);
          k+=2;
          addin(-lngi,n+i,k,0,0);
        }

        else 
        {
          if (*sign==2) i= -1;
          addin(-lngi,n-i*5,--k,-1,2);
          addin(-lngi,n-i*3,k,-3,2);
          k+=4;
          addin(-lngi,n-i*3,k,0,0);
        }
      }

      else if (g== -3)
      {                                  /* 3 link */
        i=0;
        j=4;
        while (j--!=0) i+= sign[j]/-2;
        i+=8;
        if (i==0)
        {
          addin(-lngi,n-2,k,-1,-2);
          addin(-2*lngi,n,k,-1,-2);
          addin(-lngi,n+2,k,-1,-2);
          addin(lngi,n,k+2,0,0);
        }

        else 
        {
          j= k-2;
          addin(lngi,n-2+i,j,0,0);
          addin(2*lngi,n+i,j,-1,2);
          addin(lngi,n+2+i,j,0,0);
          addin(-2*lngi,n+i/2,k,0,0);
          addin(lngi,n+i/2,k+2,0,0);
        }
      }

      else 
      {                       /* circle + circle  and  circle + circle */
        i=0;
        j=4;
        while (j--!=0) i+= sign[j]/-2;
        i+=8;
        if (i==0)
        {
          addin(lngi,n-3+i,--k,-1,-2);
          addin(3*lngi,n-1+i,k,-1,-2);
          addin(3*lngi,n+1+i,k,-1,-2);
          addin(lngi,n+3+i,k,-1,-2);
          k+=2;
          addin(-lngi,n-1+i,k,0,0);
          addin(-lngi,n+1+i,k,0,0);
        }

        else 
        {
          k-=3;
          addin(-lngi,n-3+i,k,0,0);
          addin(-3*lngi,n-1+i,k,0,0);
          addin(-3*lngi,n+1+i,k,0,0);
          addin(-lngi,n+3+i,k,0,0);
          k+=2;
          addin(2*lngi,n-2+(i*3)/4,k,0,0);
          addin(4*lngi,n+(i*3)/4,k,0,0);
          addin(2*lngi,n+2+(i*3)/4,k,0,0);
          k+=2;
          addin(-lngi,n-1+i/2,k,0,0);
          addin(-lngi,n+1+i/2,k,0,0);
        }
      }
    }

    else if (numcrs==3)
    {
      addin(-lngi,n-i*4,k,0,0);
      k+=2;
      addin(lngi,n-i*2,k,-2,-2);
    }

    else if (numcrs!=0)
    {
      addin(lngi,n-i,--k,-1,2);
      addin(lngi,n-i*3,k,0,0);
    }

    else addin(lngi,n,k,0,0);
    if (notbeg!=0)
    {
      numcrs= buf[notbeg-1];
      numlps= buf[notbeg-4];
      poslnk= buf[notbeg-5];
      neglnk= buf[notbeg-6];
      xpow= ((short) buf[notbeg-3])&255;
      ypow= buf[notbeg-2];
      notbeg-= 6+ numcrs*8;
      i=0;
      p=crsbuf;
      c= buf+notbeg;
      while (i!=numcrs)
      {
        j=4;
        while (j--!=0)
        {
          *(p++)= *(c++);
          *(p++)= *(c++)&6;
        }

        sign[i++]= (*(c-7))>>4;
      }

      skirc[numcrs*2]=0;
    }

    else numcrs=0;
  }

  else 
  {
                          /* locate fake bigons for bstlst -- 2 cases */
    k= numcrs;
    p= cross[k];
    c= sign+k;
    while (k--!=0)
    {                     /* used to store the fake bigon endcrossings */
      p-=8;
      j= *--c;
      i= *p;
      m= p[4];
      if (bstlst[m]>-3 && p[5]==j && m==p[j])
      {
        if (bstlst[m]== -2) bstlst[m]= -3;
        else bstlst[m]= -2 - bstlst[m];
        if (bstlst[k]== -2) bstlst[k]= -3;
        else bstlst[k]= -2 - bstlst[k];
      }

      j^=4;
      if (bstlst[i]>-3 && p[1]==j)
      {
        if (p[j]==i)
        {
          if (bstlst[i]== -2) bstlst[i]= -3;
          else bstlst[i]= -2 - bstlst[i];
          if (bstlst[k]== -2) bstlst[k]= -3;
          else bstlst[k]= -2 - bstlst[k];
        }

        else if (p[j^4]==i)
        {
          if (bstlst[i]==0) bstlst[i]= 2;
          if (bstlst[k]==0) bstlst[k]= 2;
        }
      }
    }

    i=numcrs;
    k=n= 0;
    while (i!=0)
    {
      if (bstlst[--i]<n)
      {
        n=bstlst[i];
        j=i;
      }

      else if (bstlst[i]>k)
      {
        k=bstlst[i];
        m=i;
      }
    }

    if (n==0)
    {
      if (k==0) j= dspair[2];
      else j=m;
    }

    else if (n==-2 && k>3) j=m;              /* since exponential growth is 1.41^n */
    ypow += bust(j,xpow,ypow);
    xpow^=128;
    ++count[numcrs-1];
  }

  if (numcrs!=0) goto STEP1;
  goto NEWNOT;
  exit(0);
}


rmcir(top,vnum,vbrnch,flag)
short top, vnum, vbrnch;
int flag;

{
  register short i, j, k, h, m, n, i1;
  register char *s, *c, *p;
  register int *lp1, *lp2;
  char *lp3, *lp4;
  short g;
  m=vbrnch;
  if (m>=0)
  {
    i= vnum;
    s= crsptr[i];
    k= s[m];
    g= s[m|1];
    if (m!=0) c= s+4;
    else c= s+(sign[i]^4);
    p= crsptr[k]+g;
    h= *p= *c;
    n= p[1]= c[1];
    p= crsptr[h]+n;
    *p= k;
    p[1]= g;
    s[6]= s[4]= s[2]= *s= i;
  }

  else ++numlps;
  s= clist;
  j=0;
  while (top>j+1)
  {
    i=k= j;
    while (++i!=top)
    {
      if ((s[i]&127) < (s[k]&127)) k=i;
    }

    if (k!=j)
    {
      i= s[j];
      s[j]= s[k];
      s[k]=i;
    }

    ++j;
  }

                            /* relink */
  m= top;
  while (m--!=0)
  {
    i= s[m];
    if (i>=0) g=2;
    else 
    {
      i&=127;
      s[m]=i;
      g=0;
    }

    k= crsptr[i][g];
    j= crsptr[i][g|1];
    h= crsptr[k][j]= crsptr[i][g|4];
    n= crsptr[k][j|1]= crsptr[i][g|5];
    crsptr[h][n]= k;
    crsptr[h][n|1]= j;
  }

  if (flag!=0) return 0;
                            /* squish crossings out of the list */
  i= *s;
 
  lp3= crsptr[i];
  lp4=lp3+8;  
  
  p= sign+i;
  c=p+1;
  j= 0;
  k=1;
  while (++i!=numcrs)
  {
    if (s[k]== i)
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
  
                   /* renumber branch pointers (subtract 1 if the crossing was after) */
  numcrs-= top;
  k= numcrs;
  c= crsptr[k];
  while (k--!=0)
  {
    j=5;
    while (--j!=0)
    {
      c-=2;
      i=top;
      s= clist+i;
      while (i--!=0)
      {
        if (*c> *--s) --*c;
      }
    }
  }
  
  return 0;
}


ntc(i,buf)
int i;
char *buf;

{
  int j;
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


delpow()

{
  register int *lp1, *lp2, *lsp, *osp;
  register short i, j, k, m, t, mpos;
  short numlnk, mpow, u, v, x, y, z, order[2], prslen[2], pstlen[2];
  
  lp1=b;
  if (numlps<4)
  {
    if (numlps==1)
    {
      *(lp1++)= -1;
      *lp1= -1;
    }

    else if (numlps==3)
    {
      *(lp1++)= -1;
      *(lp1++)= -3;
      *(lp1++)= -3;
      *lp1= -1;
    }

    else 
    {
      *(lp1++)= 1;
      *(lp1++)= 2;
      *lp1= 1;
    }
  }

  else 
  {
    *lp1= 1;
    b[1]= 4;
    b[2]= 6;
    k= b[3]= 4;
    b[4]= 1;
    while (k++!=numlps)
    {
      j=k;
      b[j]=1;
      while (--j!=0) b[j]+= b[j-1];
    }

    if ((numlps&1)!=0)
    {
      k= numlps+1;
      while (--k>=0) b[k]= -b[k];
    }
  }

  *prslen= *pstlen= prslen[1]= pstlen[1]= mpos= mpow= 0;
  osp= b+ numlps+1;
  if (poslnk>neglnk)
  {
    order[1]= poslnk;
    *order= neglnk;
  }

  else 
  {
    order[1]= neglnk;
    *order= poslnk;
    mpos=2;
  }

  z=2;
  while (z--!=0)
  {
    numlnk=order[z];
    mpos^=2;
    while (numlnk--!=0)
    {
      m= ++mpow;
      t=j=k= numlps+1;
      u= *pstlen;
      v= pstlen[1];
      ++prslen[z];
      *pstlen= x= *prslen;
      pstlen[1]= y= prslen[1];
      lp2= osp;
      lp1= lp2+ j+ m;
      if (z==0) lp1+= order[1];
      osp= lp1;
      while (j--!=0) *--lp1=0;
      while (m--!=0)
      {
        i=j= t;
        lsp= lp1;
        if (mpos== (z|2) && y>0) *--lsp=0;
        if (mpos!=0) lp1+=k;
        else lp1+=i;
        while (j--!=0) *--lp1-= *--lp2;
        lp2+=i;
        lp1= lsp;
        *--lp1=0;
        while (i--!=0) *--lp1= *--lp2;
        lp1= lsp;
        lsp= lp2;
        lp2= lp1-1;
        j=t;
        while (j--!=0) *--lp1+= *--lp2;
        --lp1;
        if (x-->0) ++k;
        if (y-->0)
        {
          ++k;
          if (mpos==z) *--lp1=0;
        }

        if (u-->0) ++t;
        if (v-->0) ++t;
        lp2=lsp;
      }
    }
  }
  
  return 0;
}


addin(kcoeff,ypow,xpow,mult,reach)
short ypow, xpow;
int kcoeff;
int mult, reach;

{
  register int *lp1, *lp2, addon, *p;
  register short i, j, k, numlnk, loops;
  i= poslnk;
  k= neglnk;
  numlnk= i+k;
  ypow+= XCNT-xpow+lowx+ 2*(k-i);
  xpow-=lowx+numlps+numlnk;
  p=b;
  loops= numlps+ numlnk;
  while (numlnk-->=0)
  {
    if (reach!=0) lp2= poly[xpow+reach]-reach+ypow;
    lp1= poly[xpow]+ ypow;
    j= loops;
    while (j-->=0)
    {
      addon= kcoeff* *(p++);
      *lp1+= addon;
      lp1+=2;
      if (reach!=0)
      {
        *lp2+= mult*addon;
        lp2+=2;
      }
    }

    if (--i<0) --loops;
    if (--k<0) --loops;
    else ypow-=2;
    xpow+=2;
  }
  
  return 0;
}


untwst(l,n,i,ex)
short l, n, i, ex;

{
  register char *pl, *pn, **cross;
  register short g, h, m, k, d, e;
  short j;
  cross= crsptr;
  j= sign[i]; /* flip 3 crossing circuits */
  k= j^ex;
  ex^=4;
  m= j^ex;
  if (sign[l]-ex==2)
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

  pl= cross[l];
  pn= cross[n];
  d= pl[m];
  e= pl[m|1];
  pl[m]= pn[g];
  pl[m|1]= pn[g|1];
  if (d!=n)
  {
    pn[g]= d;
    pn[g|1]= e;
  }

  else 
  {
    pn[g]= l;
    pn[g|1]= k;
  }

  cross[pl[m]][pl[m|1]]= l;
  cross[pl[m]][pl[m|1]|1]= m;
  d= pn[h];
  e= pn[h|1];
  pn[h]= pl[k];
  pn[h|1]= m= pl[k|1];
  if (d!=l)
  {
    pl[k]= d;
    pl[k|1]= e;
  }

  else 
  {
    pl[k]= n;
    pl[k|1]= g;
  }

  cross[pn[h]][m]= n;
  cross[pn[h]][m|1]= h;
  h= pn[g|1];
  cross[pn[g]][h]= n;
  cross[pn[g]][h|1]= g;
  d= pl[k];
  e= pl[k|1];
  cross[d][e]= l;
  cross[d][e|1]= k;
  pl[ex]= g= cross[i][ex];
  pl[ex|1]= h= cross[i][ex|1];
  cross[g][h]= l;
  cross[g][h|1]= ex;
  pn[j]= g= cross[i][k];
  pn[j|1]= h= cross[i][k|1];
  cross[g][h]= n;
  cross[g][h|1]= j;
  return 0;
}


conchk()

{
  register short i, j;
  int vnum, vbrnch, top;
  char str[20], cbuf[256];
  vnum=vbrnch= 0;
  lowx= 1;
  i=top= numcrs*2;
  while (i>=0) cbuf[i--]=0;
  i=0;
  while (i!=top)
  {
    --lowx;
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

  return 0;
}


bust(tobust,xpow,ypow)
short tobust, xpow, ypow;

{
  register char *fastp, *c;
  int *lp1, *lp2;
  char *lp3, *lp4;
  register short i, j, m, k, l, n, *sp, i1;
  char *inbuf[XCNT], tsign[XCNT], *bstcrs;
  int dir;
  int lngi;
  bstcrs= crsptr[tobust];
  fastp=sign;
  c=tsign;
  
  lp4=  *crsptr;
  lp3=  (buf+notbeg);
  j=0;
  while (j!=numcrs)
  {
    if (j==tobust) ++fastp;
    *(c++)= *(fastp++);
    inbuf[j++]=  lp3;
    
    for (i1=1; i1<=8; ++i1)
    {   
      *lp3=*lp4;
      lp3++;
      lp4++;
    }
  }  

  fastp= bstcrs;
  l= sign[tobust]/2;
  dir= 2-l;
  m= fastp[7-l];
  n= fastp[8-l];
  j= fastp[3*l-3];
  k= fastp[3*l-2];
  inbuf[j][k]= m;
  inbuf[j][k|1]= n;
  inbuf[m][n]= j;
  inbuf[m][n|1]= k;
  m= fastp[5-l];
  n= fastp[6-l];
  j= fastp[3-l];
  k= fastp[4-l];
  inbuf[j][k]= m;
  inbuf[j][k|1]= n;
  inbuf[m][n]= j;
  inbuf[m][n|1]= k;
  k= numcrs-1;
          /* squish crossing out of the list (tsign has already been squished) */
  i= k-tobust;
  
  lp3= inbuf[tobust];
  lp4=lp3+8;
  while (i--!=0)
  {
    for (i1=1; i1<=8; ++i1)
    {
      *lp3=*lp4;
      lp3++;
      lp4++;
    }
  }  

          /* renumber branch pointers (subtract 1 if the crossing was after) */
  notbeg+= numcrs*8 - 2;
  fastp= *inbuf;
  while (++i!=k)
  {
    fastp[1]|= tsign[i]<<4;
    j=5;
    while (--j!=0)
    {
      if (*fastp>=tobust) --*fastp;
      fastp+=2;
    }
  }

  buf[notbeg-6]= neglnk;
  buf[notbeg-5]= poslnk;
  buf[notbeg-4]= numlps;
  buf[notbeg-3]= (xpow+1)^128;
  buf[notbeg-2]= ypow+dir;
  buf[notbeg-1]= i;
  fastp= bstcrs;
  sp= (short *) fastp;
  j= *sp;
  *sp= sp[l];
  sp[l]= sp[2];
  sp[2]= sp[l^2];
  sp[l^2]= j;
  sign[tobust]^=4;
  crsptr[(int) *fastp][(int) fastp[1]|1]=0;
  crsptr[(int) fastp[2]][(int) fastp[3]|1]=2;
  crsptr[(int) fastp[4]][(int) fastp[5]|1]=4;
  crsptr[(int) fastp[6]][(int) fastp[7]|1]=6;
  return (dir*2);
}


monos(crssng,p)
short crssng;
char *p;

{
  register short j, k, m, n;
  register char *c;
  if (*p==p[4])
  {
    ++numlps;                   /* figure 8 */
    return 0;
  }

  else if (*p==crssng)
  {
                     /* connect the previous and the following crossings (case 1 and 3) */
    c= p+sign[crssng];
    p+=4;
  }

  else c= p+ (sign[crssng]^4);    /* else connect for case 2 and 4 */
  k= *c;
  m= c[1];
  c= crsptr[k]+m;
  j= *c= *p;
  n= c[1]= p[1];
  c= crsptr[j]+n;
  *c= k;
  c[1]= m;
  return 0;
}


int squish(sqush)
short sqush;

{
  register short i, j, tosqsh, i1;
  register char *p, *c;
  register int *lp1, *lp2;
  char *lp3, *lp4;
                                  /* squish crossing out of the list */
  tosqsh= sqush;
  j= --numcrs-tosqsh;
  
  lp3= crsptr[tosqsh];
  lp4=lp3+8;
  
  p= sign+tosqsh;
  c=p+1;
  
  while (j--!=0)
  {
    for (i1=1; i1<=8; ++i1) *(lp3++)= *(lp4++);
    *(p++)= *(c++);  
  }

                  /* renumber branch pointers (subtract 1 if the crossing was after) */
  i= numcrs;
  p= crsptr[i];
  while (i--!=0)
  {
    j=5;
    while (--j!=0)
    {
      p-=2;
      if (*p>=tosqsh) --*p;
    }
  }
  
  return 0;
}


bigons(crssng)
short crssng;

{
  register short i, j, m, h, k, n;
  register char *p, *c, **cross;
  short g;
  cross= crsptr;
  i= crssng;
  p= cross[i];
  m= *p;
  c= cross[m];
  if (p[4]== m) ++numlps;               /* loop disappearing off top */
  if (p[2]== p[6]) ++numlps;            /* loop disappearing underneath */
                                        /* relink */
  k= p[4];
  j= p[5];
  h= cross[k][j]= *c;
  n= cross[k][j|1]= c[1];
  cross[h][n]= k;
  cross[h][n|1]= j;
  g=2;
  if (p[2]==m) g=6;
  k= p[g];
  j= p[g|1];
  h=cross[k][j]= c[g];
  n= cross[k][j|1]= c[g|1];
  cross[h][n]= k;
  cross[h][n|1]= j;
  sqush2(i,m);
  return 0;
}


sqush2(n,g)
short n, g;

{
  int *lp1, *lp2;
  char *lp3, *lp4;
  register char *p, *c;
  register short m, i, j, i1;
          /* squish crossings out of the list */
  i= n;
  m= g;
  if (m>i)
  {
    j=m;
    m=i;
    i=j;
  }

  lp3= crsptr[m];
  lp4=lp3+8;
  
  p=sign+m;
  c=p+1;
  j= i-m;
  
  while (--j!=0)
  {
    for (i1=1; i1<=8; ++i1) *(lp3++)= *(lp4++);
    *(p++)= *(c++);
  }

  lp4+=8;
  ++c;
  j= numcrs-i;
  numcrs-=2;
  
  while (--j!=0)
  {
    for (i1=1; i1<=8; ++i1) *(lp3++)= *(lp4++);
    *(p++)= *(c++);
  }  
  
        /* renumber branch pointers (subtract 1 if the crossing was after) */
  p= crsptr[numcrs];
  n= numcrs;
  while (n--!=0)
  {
    j=5;
    while (--j!=0)
    {
      p-=2;
      if (*p>=i) --*p;
      if (*p>=m) --*p;
    }
  }
  
  return 0;
}


triple(m,i,p)
short m, i;
char *p;

{
  register char *crsi, *crsk, **cross;
  register short j, d, e, n, h, k;
  crsi= p;
  cross= crsptr;
  n= m^4;
  k= crsi[m];
  crsk= cross[k];
  j= crsi[m|1];
  d= crsk[j]= crsi[n];
  e= crsk[j|1]= crsi[n|1];
  cross[d][e]= k;
  cross[d][e|1]= j;
  j^=4;
  d= crsi[m]= crsk[j];
  e= crsi[m|1]= crsk[j|1];
  cross[d][e]= i;
  cross[d][e|1]= m;
  crsi[n]= k;
  crsi[n|1]= j;
  crsk[j]= i;
  crsk[j|1]= n;
  j^=2;
  n= crsk[j];
  h= crsk[j|1]^4;
  d= crsk[j]= cross[n][h];
  e= crsk[j|1]= cross[n][h|1];
  cross[d][e]= k;
  cross[d][e|1]= j;
  j^=4;
  n= crsk[j];
  h= crsk[j|1]^4;
  d= crsk[j]= cross[n][h];
  e= crsk[j|1]= cross[n][h|1];
  cross[d][e]= k;
  cross[d][e|1]= j;
  j=m^2;
  n= crsi[j];
  h= crsi[j|1]^4;
  d= crsi[j]= cross[n][h];
  e= crsi[j|1]= cross[n][h|1];
  cross[d][e]= i;
  cross[d][e|1]= j;
  j^=4;
  m= crsi[j];
  h= crsi[j|1]^4;
  d= crsi[j]= cross[m][h];
  e= crsi[j|1]= cross[m][h|1];
  cross[d][e]= i;
  cross[d][e|1]= j;
  sqush2(n,m);
  return 0;
}


tstcir(h,n,bstlst,dspair)                /* n is actually *b-n from main */
short h, n;
char *bstlst, *dspair;

{
  register char **cross, *p, *q, *vp;
  register short d, g, i, j, m, negpos;
  char t[128], t2[128], vertex;
  short k, length, lngsum, badcrs;
  cross= crsptr;
  i= numcrs*2;
  if (skirc[i]>=0)
  {
    p= skirc;
    while (i--!=0) *(p++)= 0;
    *p= -1;
  }

  q=t2;
  vp=t;
  p=clist;
  if ((h&1)==0)
  {
    if (n== (i=0))
    {
      while (*p<0)
      {
        ++p;
        ++i;
      }
    }

    else 
    {
      while (*p>=0)
      {
        ++p;
        ++i;
      }
    }

    j=i;
    while (j++!=h) *(q++)= *(vp++)= *(p++);  /* bad crossing is #1 */
    p= clist;
    while (i--!=0) *(q++)= *(vp++)= *(p++);
    vertex= -1;
  }

  else 
  {
    j=h;
    while (--j!=0) *(q++)= *(vp++)= *(p++);
    *(q++)= vertex= *p;
  }

  *q= -1;
  if (n== (length=0) && (*b!= h/2 || ((*(vp-1)|*t)&128)!=0)) *vp= negpos= 1;
  else *vp= negpos= -1;
  m=i= h/2;
  p= t2;
  while (i-->length)
  {
    j=2;
    while (*p==127) ++p;
    g= *p;
    if (g<0)
    {
      g&=127;
      j+= sign[g];
    }

    d= cross[g][j&6];
    q= ++p;
    while (*q!= -1 && (*q&127)!=d) ++q;
    if (*q== -1) i= -100;
    else if (*q==vertex) length= -1;
    else *q= 127;
  }

  if (i== -101)
  {
    p= t2;
    q=t;
    j=m;
    while (j--!=0)
    {
      *(p++)= *(q++);
      *(p++)= *(q++);
    }

    p= t2;
    length=0;
    while (m-->length)
    {
      j=6;
      while (*p==127) ++p;
      g= *p;
      if (g<0)
      {
        g&=127;
        j+= sign[g];
      }

      d= cross[g][j&6];
      q= ++p;
      while (*q!= -1 && (*q&127)!=d) ++q;
      if (*q== -1) m= -100;
      else if (*q==vertex) length= -1;
      else *q= 127;
    }
  }

  i-=m;
  length= lngsum= d= m= 0;
  j= -1;
  g= (h&126)+1;
  q=t;
  while (++j!=g)
  {
    if ((*q+1)*negpos<0) ++m;
    else 
    {
      if (m>=length)
      {
        length=m;
        k=j-m;
      }

      if (m+d>=lngsum)
      {
        lngsum= m+d+1;
        if (m!=j) badcrs= *(q-m-1); 
        else badcrs= *q;
      }

      d=m;
      m=0;
    }

    ++q;
  }

  j= h/2;
  if ((h&1)!=0 && length>=j)
  {                              /* can this circuit untwist its vertex? */
    *(vp--)= m= vertex++;
    if (k==0 && vertex*(*clist+1) < 0) d= -1;
    else if (k+length+1==h && vertex*(*vp+1) >0)
    {
      d= -1;
      k= j;
    }

    else 
    {                   /* aviod infinite loop on inside out circuits (be sure that */
                        /* skinny will reduce crossing #) */
      p= skirc;
      if (m<0)
      {
        m&=127;
        ++p;
      }

      p+= m*2;
      if (*p!=0) --length;
      else *p=1;
    }
  }

  if (i==0)
  {                     /* its got stuff inside it */
    if (length>=j)
    {
      skinny(d,t,h,k);
      return(1);
    }

    if (lngsum<j)
    {
      *(vp+1)= negpos;
      k= h/4;
      if (*dspair<k) k= *dspair;
      m=1;
      while (m++<k)
      {
        p=q= t;
        i= 0;
        d= m+1;
        while (d--!=0)
        {
          while ((*q+1)*negpos<0)
          {
            ++q;
            ++i;
          }

          ++q;
          ++i;
        }

        while (i!=g)
        {
          if ((length= (i+d)*2 -h) >= 0)
          {
            if (m<*dspair || (m==*dspair && length>=dspair[1]))
            {
       *dspair=m;
       dspair[1]= length+1;
       dspair[2]= *(q-1)&127;
       k=0;
            }
          }

          while ((*q+1)*negpos<0)
          {
            ++q;
            ++i;
          }

          ++q;
          ++i;
          while ((*p+1)*negpos<0)
          {
            ++p;
            --d;
          }

          ++p;
          --d;
        }
      }
    }
  }

  else if (d<0 && length>=j)
  {
    skinny(d,t,h,k);
    return(1);
  }

  if (h==3)
  {
    bstlst[*t&127]= 3;
    bstlst[t[1]&127]= 3;
  }

  else if (lngsum>=j)
  {
    g= badcrs&127;
    *dspair=1;
    dspair[2]=g;
    if (numcrs==12)
    {
      p= cross[g];
      if (*p!=p[2] && *p!=p[6] && p[4]!=p[2] && p[4]!=p[6]) return(0);
    }

    i= (lngsum-j)*2 +1;
    if (bstlst[g]>=0 && bstlst[g]<i) bstlst[g]= i;
  }

  return 0;
}


skinny(d,t,h,lngpos)
short d, h, lngpos;
char *t;

{
  register short a, b, g, n, vnum, dir;
  register char **cross, *p, *c, *q;
  short i, j, k, m, last, f, v2;
  cross= crsptr;
  i=m= h/2;
  p= clist;
  c= t+lngpos;
  while (i--!=0) *(p++)= *(c++);
  dir=2;
  if ((h&1) != (f=0))
  {
    c= t+h-1;
    dir= sign[*c&127];
    if (*c<0) dir^=4;
    if (d<0) dir^=4;
  }

  last= -1;
  --numlps;
  rmcir (m,lngpos,last,1);
  j= h&126;
  if (t[lngpos]>=0) f=4;
  i= k= lngpos+m;
  while (m--!=0)
  {
    i%=j;
    b=g= dir;
    vnum= t[i++];
    if (vnum<0)
    {
      vnum&=127;
      if ((n=sign[vnum])!=dir) b^=4;
      g+= n;
    }

    else if (sign[vnum]==dir) b^=4;
    if (f!=4)
    {
      n= t[--k]&127;
      sign[n]= a= b^4;
      q= cross[n];
      if (last>=0)
      {
        *p= n;
        *(p+1)= a;
        p= q+a;
        *p= last;
        *(p+1)= f;
      }

      f= b;
      b^=dir;
    }

    else 
    {
      n= t[--k];
      sign[n]= b;
      q= cross[n];
      b= dir;
      if (last>=0)
      {
        *p= n;
        *(p+1)= 0;
        *q= last;
        *(q+1)= 4;
      }
    }

    g&=6;
    c= cross[vnum]+g;
    v2= *c;
    *(c++) = last= n;
    a= *c;
    *c= b;
    c= cross[n];
    p= c+b;
    *p= vnum;
    *(p+1)= g;
    b^=4;
    p= cross[v2]+a;
    *p= n;
    *(p+1)= b;
    p= c+b;
    *p= v2;
    *(p+1)= a;
    p= q+f;
  }

                         /* reconnect the 2 or 3 loose ends; */
  b= lngpos;
  if (b==0) b= h/2;      /* bigon first */
  a= b-1;
  vnum= t[a];
  if (vnum< (a=0))
  {
    vnum&= 127;
    a= sign[vnum];
  }

  n= t[b];
  b= 4;
  if (n<0)
  {
    n&= 127;
    b= sign[n]^4;
  }

  p= cross[vnum]+a;
  *p= n;
  *(p+1)= b;
  p= cross[n]+b;
  *p= vnum;
  *(p+1)= a;
  if (lngpos==0 || ((lngpos<<1)|1) ==h)
  {
    if (d<0)
    {
      p= t+h-1;
      vnum= *p&127;
      if (*p< (dir=0)) dir= sign[vnum];
      c= cross[vnum]+dir;
      n= *c;
      b= *(c+1);
      g= *(p-1);
      if (g<0)
      {
        g&=127;
        a= sign[g];
      }

      else a=0;
      p= cross[g]+a;
      *p= n;
      *(p+1)= b;
      p= cross[n]+b;
      *p=g;
      *(p+1)= a;    /* finished with t[h-2] */
      if (dir==0) dir= sign[vnum]^4;
      else dir=4;
      c= cross[vnum]+dir;
      n= *c;
      b= *(c+1);
      g= *t;
      if (g<0)
      {
        g&=127;
        a= sign[g]^4;
      }

      else a=4;
      p= cross[g]+a;
      *p= n;
      *(p+1)= b;
      p= cross[n]+b;
      *p=g;
      *(p+1)= a;    /* finished with *t */
      squish(vnum);
    }

    else if (f!= (b=4))
    {
                  /*  the circuit crossings should have maintained position and value,  */
                  /* unless the moving section was under;  then the vertex might change */
      a= h-2;
      if (lngpos== (dir=0))
      {
        b=a= 0;
        dir= 4;
      }

      n= t[a]&127;
      dir^= sign[n];
      vnum= t[h-1]&127;
      p= cross[vnum]+b;
      *p= n;
      *(p+1)= dir;
      p= cross[n]+dir;
      *p= vnum;
      *(p+1)= b;
    }
  }

  else 
  {
    b= lngpos+h/2;
    a= b-1;
    if (lngpos==h/2) b=0;
    vnum= t[a];
    if (vnum< (a=0))
    {
      vnum&= 127;
      a= sign[vnum];
    }

    n= t[b];
    b= 4;
    if (n<0)
    {
      n&= 127;
      b= sign[n]^4;
    }

    p= cross[vnum]+a;
    *p= n;
    *(p+1)= b;
    p= cross[n]+b;
    *p= vnum;
    *(p+1)= a;
  }
  
  return 0;
}
