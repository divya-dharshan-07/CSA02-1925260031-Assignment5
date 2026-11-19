#include <stdio.h>
#include <limits.h>
long long calls=0;
static long long power_linear(long long b,int e){++calls;if(e==0)return 1;return b*power_linear(b,e-1);}
static long long power_fast(long long b,int e){++calls;if(e==0)return 1;long long half=power_fast(b,e/2);return (e%2)?b*half*half:half*half;}
static long long power_iter(long long b,unsigned e){long long result=1;while(e){if(e&1u)result*=b;b*=b;e>>=1;}return result;}
static int digital_root(int n){if(n<0)n=-n;if(n<10)return n;int sum=0;while(n){sum+=n%10;n/=10;}return digital_root(sum);}
int main(void){printf("digital root 9875 = %d; root 0 = %d; root 7 = %d\n",digital_root(9875),digital_root(0),digital_root(7));int exps[]={10,100,1000,10000};for(int i=0;i<4;++i){calls=0;(void)power_linear(1,exps[i]);long long lc=calls;calls=0;(void)power_fast(1,exps[i]);printf("n=%d linear_calls=%lld fast_calls=%lld\n",exps[i],lc,calls);}calls=0;printf("linear 2^30=%lld calls=%lld\n",power_linear(2,30),calls);calls=0;long long f=power_fast(2,30);printf("fast 2^30=%lld calls=%lld\n",f,calls);printf("iterative 2^30=%lld\n",power_iter(2,30));return 0;}
