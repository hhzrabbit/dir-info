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
  }
  closedir(theDir);
}

void printArray(char * arr){
  while (arr)
    printf("\t%s\n", arr);
}

void printOrganized(DIR * theDir){
  char dirs[256];
  char regFiles[256];

  struct dirent * entry;
  while (entry = readdir(theDir)){
    if (entry->d_type == 8){
      regFiles = * entry->d_name;
      regFiles++;
    }
    else {
      dirs = * entry->d_name;
      dirs++;
    }
  }
  printf("Directories:\n");
  printArray(dirs);
  printf("Regular files:\n");
  printArray(regFiles);
  
  
  closedir(theDir);
}

int main(){
  DIR * theDir = opendir(".");
  printAll(theDir);
  return 0;
}
