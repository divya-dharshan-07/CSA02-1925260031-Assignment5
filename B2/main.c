#include <stdio.h>
#include <string.h>

#define NAME_CAP 16

int main(int argc,char **argv){
    char name[NAME_CAP];
    if(argc!=2){fprintf(stderr,"Usage: %s donor-name\n",argv[0]);return 2;}
    if(strlen(argv[1])>=sizeof name){
        fprintf(stderr,"Name too long (maximum %zu characters)\n",sizeof name-1U);return 1;
    }
    strcpy(name,argv[1]);
    printf("Command-line donor: %s\n",name);

    FILE *missing=fopen("missing_donor_data.txt","r");
    if(missing==NULL)perror("missing_donor_data.txt");else fclose(missing);

    char token[NAME_CAP];
    printf("Enter one-word name (scanf with width limit): ");
    if(scanf("%15s",token)==1)printf("scanf read: %s\n",token);
    else puts("No token read.");

    int ch;
    while((ch=getchar())!='\n'&&ch!=EOF){}
    char line[32];
    printf("Enter full donor name (fgets): ");
    if(fgets(line,sizeof line,stdin)!=NULL){
        size_t length=strcspn(line,"\n");
        int complete=(line[length]=='\n');
        line[length]='\\0';
        printf("fgets read: %s\n",line);
        if(!complete&&!feof(stdin)){
            while((ch=getchar())!='\n'&&ch!=EOF){}
            puts("Input was longer than the buffer; remainder discarded.");
        }
    }
    return 0;
}
