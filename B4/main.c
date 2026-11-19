#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct { int id; char group[4]; int days_since_donation; } Donor;
typedef struct { Donor *data; size_t size, capacity; } DonorList;

static int add_donor(DonorList *list,int id,const char *group,int days){
    if(list->size==list->capacity){
        size_t cap=list->capacity==0?2:list->capacity*2;
        if(cap>SIZE_MAX/sizeof *list->data)return 0;
        Donor *temporary=realloc(list->data,cap*sizeof *list->data);
        if(temporary==NULL)return 0;
        list->data=temporary;list->capacity=cap;
    }
    list->data[list->size].id=id;
    strncpy(list->data[list->size].group,group,3);
    list->data[list->size].group[3]='\0';
    list->data[list->size].days_since_donation=days;
    ++list->size;return 1;
}
static int load_donors(const char *path,DonorList *list){
    FILE *f=fopen(path,"r");if(!f){perror(path);return 0;}
    int id,days;char group[4];
    while(fscanf(f,"%d %3s %d",&id,group,&days)==3)
        if(!add_donor(list,id,group,days)){fclose(f);return 0;}
    int ok=feof(f);fclose(f);return ok;
}
int main(void){
    DonorList list={NULL,0,0};
    const char *groups[]={"O+","A+","B-","AB+","O-"};
    for(int i=0;i<5;i++){
        if(!add_donor(&list,7001+i,groups[i],120+i*10)){
            perror("realloc");free(list.data);return 1;
        }
        printf("added %d (%s): size = %zu, capacity = %zu\n",
               list.data[i].id,list.data[i].group,list.size,list.capacity);
    }
    printf("allocated bytes=%zu\n",list.capacity*sizeof *list.data);
    free(list.data);list=(DonorList){NULL,0,0};

    if(!load_donors("donors_input.txt",&list)){
        puts("Could not load donor input file.");free(list.data);return 1;
    }
    clock_t start=clock();size_t eligible=0;
    puts("Nightly eligibility report (minimum 90 days):");
    for(size_t i=0;i<list.size;i++)if(list.data[i].days_since_donation>=90){
        ++eligible;printf("donor %d (%s): eligible\n",list.data[i].id,list.data[i].group);
    }
    clock_t end=clock();
    printf("loaded=%zu eligible=%zu memory=%zu bytes report_cpu=%.6fs\n",
           list.size,eligible,list.capacity*sizeof *list.data,
           (double)(end-start)/CLOCKS_PER_SEC);
    free(list.data);list.data=NULL;list.size=list.capacity=0;
    return 0;
}
