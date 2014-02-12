#include <stdio.h>
#include <stdlib.h>

int main(argc, argv)
int argc;
char *argv[];

{
  FILE *fp, *gp1, *gp2;
  int a[200], d[200], e[200], f[200], g[200], h[200];
  int b[200], b1[6], w[60][6];
  int n, nc, id, i, j, k, l, s, t, w1, w2, w3, knit, batch;
  fp=fopen(argv[1],"r");
  gp1=fopen(argv[2],"w");
  gp2=fopen(argv[3],"w");

  while (fscanf(fp, "%2d", &nc)!=EOF)
  {
    if (nc < 3 || nc > 50)
    {
      fprintf(gp2, "1");
      exit(0);
    }     
  
    fscanf(fp, "%7d", &id);
    
    n=2*nc-1;
    b[0]=n+1;
    b[n+2]=1;
    for (i=1; i<=n+1; i+=1) b[i]=i;
    b1[0]=4;
    b1[5]=1;
    for (i=1; i<=4; i+=1) b1[i]=i;
    
    for (i=1; i<=2*nc; ++i) a[i]=0;
    
    for (i=1; i<=n; i+=2)
    {
      fscanf(fp, "%3d", a+i);
      d[i]=a[i]/abs(a[i]);
      a[i]=abs(a[i]);
    }
    for(i=1; i<=n; i+=2)
    {
      a[a[i]]=i;
      d[a[i]]=-d[i];
    }
    
    for (i=1; i<=2*nc; ++i) if (a[i] == 0)
    {
      fprintf(gp2, "1");
      exit(0);
    }    
       
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
  
    for(i=1; i<=n; i+=2)
    {
      j=(i+1)/2;
      if (d[i]==1)
      { 
        w[j][1]=a[i+1]; 
        w[j][3]=a[b[i-1]];
        if (f[i]==-1)
        {
          w[j][2]=b[a[i]+1];
          w[j][4]=(a[i]-1);
        }
        else
        {
          w[j][2]=(a[i]-1);
          w[j][4]=b[a[i]+1];
        }
      }
      if (d[i]==-1)
      {
        w[j][1]=b[a[i]+1];
        w[j][3]=(a[i]-1);
        if (f[i]==-1)
        {
          w[j][2]=a[b[i-1]];
          w[j][4]=a[i+1];
        }
        else
        {
          w[j][2]=a[i+1];
          w[j][4]=a[b[i-1]];
        }
      }
    }
    for (i=1; i<=nc; i+=1) 
      for (j=1; j<=4; j+=1)
        w[i][j]=(w[i][j]+1)/2;
    fprintf(gp1, "%2d.%8d:\n", nc, id);
    
    for (i=1; i<=nc; i+=1)
    {
      fprintf(gp1, "%1d", i);
      if (d[2*i-1]*f[2*i-1]==1)
        fprintf(gp1, "+");
      else
        fprintf(gp1, "-");
      for(j=1; j<=4; j+=1)
      {
        fprintf(gp1, "%1d", w[i][j]);
        for (k=1; k<=4; k+=1)
        {
          w1=w[i][j];
          w2=w[i][b1[j+1]];
          w3=w[i][b1[j-1]];
          if ((w[w1][k]==i) &&
            ((w1!=w2) || (w[w1][k]!=w[w1][b1[k+1]])) &&
            ((w1!=w3) || (w[w1][k]!=w[w1][b1[k-1]])))
          {
            if (k==1) fprintf(gp1, "a");
            if (k==2) fprintf(gp1, "b");
            if (k==3) fprintf(gp1, "c");
            if (k==4) fprintf(gp1, "d");
          }
        }
      }
      fprintf(gp1, "\n");
    }
    fprintf(gp1, "\n");
  }
  
  fprintf(gp2, "0");
  
  exit(0);
}

   

      
            


