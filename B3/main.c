#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct { int id; char name[32]; char group[4]; int age; } Donor;
typedef struct { int id; int units; } Stock;

static double seconds(clock_t a, clock_t b) {
    return (double)(b-a)/(double)CLOCKS_PER_SEC;
}
static int same_donor(const Donor *a,const Donor *b) {
    return a->id==b->id && strcmp(a->name,b->name)==0 &&
           strcmp(a->group,b->group)==0 && a->age==b->age;
}
static int write_text(const char *path,const Donor *d,size_t n) {
    FILE *f=fopen(path,"w"); if(!f){perror(path);return 0;}
    for(size_t i=0;i<n;i++)
        if(fprintf(f,"%d|%s|%s|%d\n",d[i].id,d[i].name,d[i].group,d[i].age)<0){perror("fprintf");fclose(f);return 0;}
    return fclose(f)==0;
}
static int read_text(const char *path,Donor *out,size_t n) {
    FILE *f=fopen(path,"r"); if(!f){perror(path);return 0;}
    for(size_t i=0;i<n;i++)
        if(fscanf(f,"%d|%31[^|]|%3[^|]|%d",&out[i].id,out[i].name,out[i].group,&out[i].age)!=4){fclose(f);return 0;}
    fclose(f);return 1;
}
static long file_size(const char *path) {
    FILE *f=fopen(path,"rb"); if(!f){perror(path);return -1;}
    if(fseek(f,0,SEEK_END)!=0){fclose(f);return -1;}
    long n=ftell(f);fclose(f);return n;
}
static int update_binary_donor(const char *path,long index,int age) {
    FILE *f=fopen(path,"r+b");if(!f){perror(path);return 0;}
    Donor d;
    if(index<0 || fseek(f,index*(long)sizeof d,SEEK_SET)!=0 ||
       fread(&d,sizeof d,1,f)!=1){fclose(f);return 0;}
    d.age=age;
    int ok=fseek(f,-(long)sizeof d,SEEK_CUR)==0 && fwrite(&d,sizeof d,1,f)==1;
    return fclose(f)==0 && ok;
}
static int update_text_donor(const char *path,long target,int age) {
    FILE *in=fopen(path,"r");if(!in){perror(path);return 0;}
    FILE *out=fopen("donors.tmp","w");if(!out){perror("donors.tmp");fclose(in);return 0;}
    int id,old_age,updated=0;char name[32],group[4];long index=0;int ok=1;
    while(fscanf(in,"%d|%31[^|]|%3[^|]|%d",&id,name,group,&old_age)==4) {
        if(index==target){old_age=age;updated=1;}
        if(fprintf(out,"%d|%s|%s|%d\n",id,name,group,old_age)<0){ok=0;break;}
        ++index;
    }
    if(ferror(in))ok=0;
    if(fclose(in)!=0)ok=0;
    if(fclose(out)!=0)ok=0;
    if(!ok || !updated){remove("donors.tmp");return 0;}
    if(remove(path)!=0){perror("remove old text file");remove("donors.tmp");return 0;}
    if(rename("donors.tmp",path)!=0){perror("rename");return 0;}
    return 1;
}
static int update_stock(FILE *f,long index,int units) {
    Stock s;
    if(index<0 || fseek(f,index*(long)sizeof s,SEEK_SET)!=0 ||
       fread(&s,sizeof s,1,f)!=1)return 0;
    s.units=units;
    return fseek(f,-(long)sizeof s,SEEK_CUR)==0 && fwrite(&s,sizeof s,1,f)==1;
}
int main(void) {
    /* Ten-record verification requested in the brief. */
    Donor ten[10],text_back[10],binary_back[10];
    for(int i=0;i<10;i++){ten[i].id=i+1;snprintf(ten[i].name,sizeof ten[i].name,"Donor%d",i+1);strcpy(ten[i].group,"O+");ten[i].age=20+i;}
    if(!write_text("ten_donors.txt",ten,10) || !read_text("ten_donors.txt",text_back,10)){puts("text ten-record test failed");return 1;}
    FILE *f=fopen("ten_donors.bin","wb");if(!f){perror("ten_donors.bin");return 1;}
    if(fwrite(ten,sizeof ten[0],10,f)!=10){fclose(f);return 1;}fclose(f);
    f=fopen("ten_donors.bin","rb");if(!f){perror("ten_donors.bin");return 1;}
    size_t got=fread(binary_back,sizeof binary_back[0],10,f);fclose(f);
    int text_ok=1,binary_ok=got==10;
    for(int i=0;i<10;i++){if(!same_donor(&ten[i],&text_back[i]))text_ok=0;if(binary_ok && !same_donor(&ten[i],&binary_back[i]))binary_ok=0;}
    printf("10 donor verification: text=%s binary=%s\n",text_ok?"identical":"failed",binary_ok?"identical":"failed");

    /* Required valid/invalid stock update test: zero-based index 2 is record 3. */
    Stock stocks[5]={{1,40},{2,25},{3,30},{4,20},{5,30}};
    f=fopen("stock.bin","wb");if(!f){perror("stock.bin");return 1;}
    fwrite(stocks,sizeof stocks[0],5,f);fclose(f);
    f=fopen("stock.bin","r+b");if(!f){perror("stock.bin");return 1;}
    printf("update record 3: %s\n",update_stock(f,2,12)?"done":"failed");
    printf("update record 9: %s\n",update_stock(f,8,5)?"done":"failed");
    rewind(f);fread(stocks,sizeof stocks[0],5,f);fclose(f);
    printf("1:%d 2:%d 3:%d 4:%d 5:%d\n",stocks[0].units,stocks[1].units,stocks[2].units,stocks[3].units,stocks[4].units);

    /* Timed 1,000 / 10,000 / 100,000 donor investigations. */
    const size_t sizes[]={1000,10000,100000};
    for(size_t z=0;z<3;z++){
        size_t n=sizes[z];Donor *data=malloc(n*sizeof *data),*copy=malloc(n*sizeof *copy);
        if(!data||!copy){perror("malloc");free(data);free(copy);return 1;}
        for(size_t i=0;i<n;i++){data[i].id=(int)i+1;snprintf(data[i].name,sizeof data[i].name,"Donor%zu",i+1);strcpy(data[i].group,"O+");data[i].age=20+(int)(i%40);}
        clock_t a=clock();if(!write_text("donors.txt",data,n)){free(data);free(copy);return 1;}clock_t b=clock();
        f=fopen("donors.bin","wb");if(!f){perror("donors.bin");free(data);free(copy);return 1;}
        clock_t c=clock();size_t wrote=fwrite(data,sizeof *data,n,f);fclose(f);clock_t d=clock();
        if(wrote!=n){puts("binary write failed");free(data);free(copy);return 1;}
        clock_t e=clock();int tr=read_text("donors.txt",copy,n);clock_t g=clock();
        f=fopen("donors.bin","rb");if(!f){perror("donors.bin");free(data);free(copy);return 1;}
        size_t br=fread(copy,sizeof *copy,n,f);fclose(f);clock_t h=clock();
        int binary_same=br==n;for(size_t i=0;i<n&&binary_same;i++)if(!same_donor(&data[i],&copy[i]))binary_same=0;
        printf("n=%zu text_write=%.6fs binary_write=%.6fs text_read=%.6fs binary_read=%.6fs text_bytes=%ld binary_bytes=%ld binary_check=%s text_read=%s\n",
            n,seconds(a,b),seconds(c,d),seconds(e,g),seconds(g,h),file_size("donors.txt"),file_size("donors.bin"),binary_same?"identical":"failed",tr?"ok":"failed");
        if(n==100000){
            clock_t u=clock();int bu=update_binary_donor("donors.bin",49999,45);clock_t v=clock();
            clock_t x=clock();int tu=update_text_donor("donors.txt",49999,45);clock_t y=clock();
            printf("record 50000 update: binary=%s (%.6fs), text_rewrite=%s (%.6fs)\n",bu?"done":"failed",seconds(u,v),tu?"done":"failed",seconds(x,y));
        }
        free(data);free(copy);
    }
    return 0;
}
