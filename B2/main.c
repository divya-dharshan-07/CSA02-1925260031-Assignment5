#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
int main(int argc,char **argv){char name[16];if(argc!=2){fprintf(stderr,"Usage: %s donor-name\n",argv[0]);return 2;}if(strlen(argv[1])>=sizeof name){fprintf(stderr,"Name too long: maximum %zu characters\n",sizeof name-1);return 1;}strcpy(name,argv[1]);printf("Validated donor name: %s\n",name);FILE *fp=fopen("missing_donor_data.txt","r");if(!fp)perror("missing_donor_data.txt");else fclose(fp);char line[16];puts("Enter a donor name (fgets safely limits input):");if(fgets(line,sizeof line,stdin)){line[strcspn(line,"\n")]='\0';printf("Read: %s\n",line);if(strchr(line,'\0')&&strlen(line)==sizeof line-1)puts("Input may have been truncated; discard remainder in production code.");}return 0;}
