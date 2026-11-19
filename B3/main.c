#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
typedef struct{int id;char name[32];char group[4];int age;}Donor;
static int equal(const Donor*a,const Donor*b){return a->id==b->id&&strcmp(a->name,b->name)==0&&strcmp(a->group,b->group)==0&&a->age==b->age;}
static double seconds(clock_t a,clock_t b){return (double)(b-a)/CLOCKS_PER_SEC;}
int main(void){const int sizes[]={10,1000,10000,100000};for(int z=0;z<4;++z){int n=sizes[z];Donor *d=malloc((size_t)n*sizeof *d);if(!d){perror("malloc");return 1;}for(int i=0;i<n;++i){d[i].id=i+1;snprintf(d[i].name,sizeof d[i].name,"Donor%d",i+1);strcpy(d[i].group,"O+");d[i].age=20+i%40;}
FILE *t=fopen("donors.txt","w");if(!t){perror("donors.txt");free(d);return 1;}clock_t a=clock();for(int i=0;i<n;++i)fprintf(t,"%d %s %s %d\n",d[i].id,d[i].name,d[i].group,d[i].age);fclose(t);clock_t b=clock();
FILE *bin=fopen("donors.bin","wb");if(!bin){perror("donors.bin");free(d);return 1;}clock_t c=clock();fwrite(d,sizeof *d,(size_t)n,bin);fclose(bin);clock_t e=clock();
printf("n=%d text_write=%.6fs binary_write=%.6fs binary_bytes=%zu\n",n,seconds(a,b),seconds(c,e),(size_t)n*sizeof *d);
FILE *readb=fopen("donors.bin","rb");Donor *back=malloc((size_t)n*sizeof *back);if(!readb||!back){perror("read allocation/open");if(readb)fclose(readb);free(back);free(d);return 1;}fread(back,sizeof *back,(size_t)n,readb);fclose(readb);int ok=1;for(int i=0;i<n;++i)if(!equal(&d[i],&back[i])){ok=0;break;}printf("binary round-trip: %s\n",ok?"identical":"MISMATCH");
free(back);free(d);remove("donors.txt");remove("donors.bin");}return 0;}
