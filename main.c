#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <dirent.h>

void printAll(DIR * theDir){
  printf("All files:\n");
  struct dirent * entry;
  while (entry = readdir(theDir)){
      printf("\t%s\n", entry -> d_name);
      printf("\t%d\n", entry -> d_type);
  }
  closedir(theDir);
}

void printDirs(DIR * theDir){
  printf("Directories:\n");
  struct dirent * entry;
  while (entry = readdir(theDir)){
    if (entry->d_type != 8){
      printf("\t%s\n",entry->d_name);
    }
  }
}

void printRegFiles(DIR * theDir){
  printf("Regular Files:\n");
  struct dirent * entry;
  while (entry = readdir(theDir)){
    if (entry->d_type == 8){
      printf("\t%s\n",entry->d_name);
    }
  }
}

/* void printOrganized(DIR * theDir){ */
/*   char * dirs[256]; */
/*   char * dirPoint = dirs[0]; */
  
/*   char * regFiles[256]; */
/*   char * regFilePoint = regFiles[0]; */
  
/*   struct dirent * entry; */
/*   while (entry = readdir(theDir)){ */
/*     if (entry->d_type == 8){ */
/*       printf("%s\n", entry->d_name); */
/*       regFilePoint = & (entry->d_name); */
/*       regFilePoint++; */
/*     } */
/*     else { */
/*       dirPoint = & (entry->d_name); */

/*       dirPoint++; */
/*     } */
/*   } */
/*   printf("Directories:\n"); */
/*   //printArray(dirs); */
/*   printf("Regular files:\n"); */
/*   //printArray(regFiles); */
  
  
/*   closedir(theDir); */
/* } */

int main(){
  DIR * theDir = opendir(".");
  // printAll(theDir);

  printDirs(theDir);
  rewinddir(theDir);
  
  printRegFiles(theDir);
  return 0;
}
