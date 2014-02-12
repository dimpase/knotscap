#include <stdio.h>
#include <stdlib.h>

FILE *fp, *gp; 
  
int i, j, k, n, nc, knit, a[100], f[100];

main()
{
  char ch;

  fp=fopen("./tmp/mbt","r");
  gp=fopen("./tmp/orientresult","w");
  
  n=-1;
  
  if ((ch=getc(fp)) != '\n') ungetc(ch, fp);
  
  while ((ch=getc(fp)) != '\n')
  {
    n+=2;
    ungetc(ch, fp);
    fscanf(fp, "%3d", a+n);
  }
    
  nc=(n+1)/2;

  for (i=1; i<=n; i+=2)
  {
    a[i] = abs(a[i]);
  } 

  for (i=1; i<=n; i+=2)
  {
    a[a[i]] = i;
  }
  
  orient();
  
  fprintf(gp, "%1d", knit);
  exit(0);
}
  
orient()
{
  int e[100], g[100], h[100], i, s, t;
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
  return 0;
}

    
  
  
  

  
  
  

