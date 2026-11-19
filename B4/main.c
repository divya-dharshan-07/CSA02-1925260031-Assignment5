#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct{int id;char group[4];}Donor;
typedef struct{Donor *data;int size,capacity;}DonorList;
static int add_donor(DonorList *list,int id,const char *group){if(list->size==list->capacity){int cap=list->capacity==0?2:list->capacity*2;Donor *p=realloc(list->data,(size_t)cap*sizeof *p);if(!p)return 0;list->data=p;list->capacity=cap;}list->data[list->size].id=id;strncpy(list->data[list->size].group,group,3);list->data[list->size].group[3]='\0';++list->size;return 1;}
int main(void){DonorList list={NULL,0,0};const char *groups[]={"O+","A+","B-","AB+","O-"};for(int i=0;i<5;++i){if(!add_donor(&list,7001+i,groups[i])){perror("realloc");free(list.data);return 1;}printf("added %d (%s): size = %d, capacity = %d\n",list.data[i].id,list.data[i].group,list.size,list.capacity);}printf("allocated bytes=%zu\n",(size_t)list.capacity*sizeof *list.data);free(list.data);list.data=NULL;list.size=list.capacity=0;return 0;}
