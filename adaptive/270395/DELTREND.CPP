#define Uses_delTrend_270395

#include <adaptive.hpp>

#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

delTrend_270395::delTrend_270395(unsigned int q,float vol,float ms,Defence_1x1 *dfn)
{
 int i;

 Vol=vol;
 MaxSpeed=ms;
 Dfn=dfn;
 Q=max(min(q,65000L/sizeof(float)),1);
 buf=new float[Q];
 if(buf) for(i=0;i<Q;i++) buf[i]=0;
 cnt=0; cur=0;
 return;
}

delTrend_270395::~delTrend_270395(void)
{
 if(buf) delete buf;
 return;
}

float delTrend_270395::Verify(float x,float y,float yt)
{
 cur++; buf[cur%=Q]=x;

 if(cnt<Q) cnt++;
 else      x=calc(x)+Vol;

 if(Dfn) x=Dfn->Verify(x,y,yt);

 buf[cur]=x;

 return x;
}

