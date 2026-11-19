#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct { int id; char name[32]; char group[4]; int age; } Donor;
typedef struct { int units; char group[4]; } Stock;

static double elapsed(clock_t a, clock_t b) {
    return (double)(b - a) / CLOCKS_PER_SEC;
}

static int same_donor(const Donor *a, const Donor *b) {
    return a->id == b->id && strcmp(a->name,b->name)==0 &&
           strcmp(a->group,b->group)==0 && a->age == b->age;
}

static int write_text(const char *path, const Donor *d, size_t n) {
    FILE *f=fopen(path,"w"); if(!f){perror(path);return 0;}
    for(size_t i=0;i<n;i++)
        if(fprintf(f,"%d|%s|%s|%d\n",d[i].id,d[i].name,d[i].group,d[i].age)<0){perror("fprintf");fclose(f);return 0;}
    return fclose(f)==0;
}

static int read_text(const char *path, Donor *out, size_t n) {
    FILE *f=fopen(path,"r"); if(!f){perror(path);return 0;}
    for(size_t i=0;i<n;i++)
        if(fscanf(f,"%d|%31[^|]|%3[^|]|%d",&out[i].id,out[i].name,out[i].group,&out[i].age)!=4){fclose(f);return 0;}
    fclose(f); return 1;
}

static int update_binary(const char *path, long index, int units) {
    FILE *f=fopen(path,"r+b"); if(!f){perror(path);return 0;}
    if(index<0 || fseek(f,index*(long)sizeof(Stock),SEEK_SET)!=0){fclose(f);return 0;}
    Stock s;
    if(fread(&s,sizeof s,1,f)!=1){fclose(f);return 0;}
    s.units=units;
    if(fseek(f,-(long)sizeof s,SEEK_CUR)!=0 || fwrite(&s,sizeof s,1,f)!=1){fclose(f);return 0;}
    return fclose(f)==0;
}

int main(void) {
    /* Required update demonstration: valid record 3, invalid record 9. */
    Stock stock[5]={{40,"O+"},{25,"A+"},{12,"B-"},{20,"AB+"},{30,"O-"}};
    FILE *sf=fopen("stock.bin","wb");
    if(!sf){perror("stock.bin");return 1;}
    if(fwrite(stock,sizeof stock[0],5,sf)!=5){perror("fwrite");fclose(sf);return 1;}
    fclose(sf);
    printf("update record 3: %s\n",update_binary("stock.bin",2,12)?"done":"failed");
    printf("update record 9: %s\n",update_binary("stock.bin",8,5)?"done":"failed");
    sf=fopen("stock.bin","rb");
    if(!sf){perror("stock.bin");return 1;}
    if(fread(stock,sizeof stock[0],5,sf)!=5){fclose(sf);return 1;}
    fclose(sf);
    printf("1:%d 2:%d 3:%d 4:%d 5:%d\n",stock[0].units,stock[1].units,stock[2].units,stock[3].units,stock[4].units);

    const size_t sizes[]={10,1000,10000,100000};
    for(size_t z=0;z<sizeof sizes/sizeof sizes[0];z++) {
        size_t n=sizes[z];
        Donor *data=malloc(n*sizeof *data), *copy=malloc(n*sizeof *copy);
        if(!data||!copy){perror("malloc");free(data);free(copy);return 1;}
        for(size_t i=0;i<n;i++){data[i].id=(int)i+1;snprintf(data[i].name,sizeof data[i].name,"Donor%zu",i+1);strcpy(data[i].group,"O+");data[i].age=20+(int)(i%40);}
        clock_t a=clock();
        if(!write_text("donors.txt",data,n)){free(data);free(copy);return 1;}
        clock_t b=clock();
        FILE *bf=fopen("donors.bin","wb");if(!bf){perror("donors.bin");free(data);free(copy);return 1;}
        clock_t c=clock();size_t wrote=fwrite(data,sizeof *data,n,bf);clock_t d=clock();fclose(bf);
        if(wrote!=n){puts("binary write failed");free(data);free(copy);return 1;}
        clock_t e=clock();
        if(!read_text("donors.txt",copy,n)){puts("text read failed");free(data);free(copy);return 1;}
        clock_t g=clock();
        bf=fopen("donors.bin","rb");if(!bf){perror("donors.bin");free(data);free(copy);return 1;}
        size_t got=fread(copy,sizeof *copy,n,bf);clock_t h=clock();fclose(bf);
        if(got!=n){puts("binary read failed");free(data);free(copy);return 1;}
        int identical=1;for(size_t i=0;i<n;i++)if(!same_donor(&data[i],&copy[i])){identical=0;break;}
        printf("n=%zu text_write=%.6fs binary_write=%.6fs text_read=%.6fs binary_read=%.6fs binary_bytes=%zu check=%s\n",
               n,elapsed(a,b),elapsed(c,d),elapsed(b,g),elapsed(g,h),n*sizeof *data,identical?"identical":"mismatch");
        (void)e;
        free(data);free(copy);
    }
    return 0;
}
