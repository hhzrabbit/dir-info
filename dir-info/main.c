#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <math.h>


//=================== PRINTING CONTENTS OF DIR ==========================

int isDir(struct dirent * entry){
  // printf("isDir\n");
  return (entry->d_type != 8);
}

int isRegFile(struct dirent * entry){
  return ! isDir(entry);
}

//helper func for totalSizeR
int notMeOrParent(struct dirent * dir){
  int len = strlen(dir->d_name);
  return ! ((len == 1 && dir->d_name[0] == '.') ||
	    (len == 2 && dir->d_name[0] == '.' && dir->d_name[1] == '.') );
}

//prints everything, similar to ls
void printAll(DIR * theDir){
  printf("All files:\n");
  struct dirent * entry;
  while (entry = readdir(theDir))
      printf("\t%s\n", entry -> d_name);
  rewinddir(theDir);
}

void printAllRhelper(DIR * theDir, char * pathToHere, int amtTabs){
  char tabs[amtTabs + 1];
  tabs[amtTabs] = 0;
  int i;
  for (i = amtTabs; i > 0; i--) {
    tabs[i-1] = '\t';
  }
  struct dirent * entry;
  while (entry = readdir(theDir)) {
    printf("%s%s\n", tabs, entry -> d_name);
    if (isDir(entry) && notMeOrParent(entry) ) { //don't count . or ..
      char * path = (char *)(calloc(256, 1));
      strcpy(path, pathToHere);
      strcat(path, "/");
      strcat(path, entry->d_name);
      printAllRhelper(opendir(path), path, (amtTabs + 1));
    }
  }
}

void printAllR(DIR * theDir, char * pathToHere){
  printf("All files, including in subdirs:\n");
  printAllRhelper(theDir, pathToHere, 1);
  rewinddir(theDir);
}

//prints only the directories
void printDirs(DIR * theDir){
  printf("Directories:\n");
  struct dirent * entry;
  while (entry = readdir(theDir)){ //still more entries
    if (isDir(entry)){
      printf("\t%s\n",entry->d_name);
    }
  }
  rewinddir(theDir);
}


//prints only the non-directories
void printRegFiles(DIR * theDir){
  printf("Regular Files:\n");
  struct dirent * entry;
  while (entry = readdir(theDir)){
    if (isRegFile(entry)){
      printf("\t%s\n",entry->d_name);
    }
  }
  rewinddir(theDir);
}

//===================================================================

//=================== PRINTING SIZE OF DIR ==========================

//given amount of bytes, prints in gb, mb, kb, b
//from file stat hw
void printSizeHuman(int size){
  int save = size; //for printing in bytes at the end
  int vals[4]; // [ gb, mb, kb, b ]

  //fill vals array
  int pos = 0;
  int i;
  for (i = 3; i > 0; i--){
    int divisor = pow(1000, i);
    vals[pos] = size / divisor;
    size = size % divisor;
    pos++;
  }
  vals[3] = size;

  //fill labels array
  char * labels[4];
  labels[0] = "GB";
  labels[1] = "MB";
  labels[2] = "KB";
  labels[3] = "B";

  //print size
  for (pos = 0; pos < 4; pos++){
    if (vals[pos])
      printf("%d %s ", vals[pos], labels[pos]);
  }
  printf("(%d bytes)\n", save); 
}

//only looks at current dir not subdirs
int totalSize(DIR * theDir){
  int total = 0;
  struct stat buff;
  struct dirent * entry;
  while (entry = readdir(theDir)){
    stat(entry->d_name, &buff);
    total += buff.st_size;
  }
  rewinddir(theDir);
  return total;
}

//finds total size, including subdirs, recursively
int totalSizeR(DIR * theDir, char * pathToHere){
  int total = 0;
  struct stat buff;
  struct dirent * entry;
  while (entry = readdir(theDir))
    if (isDir(entry) && notMeOrParent(entry) ){ //don't count . or ..
      char * path = (char *)(calloc(256, 1));
      strcpy(path, pathToHere);
      strcat(path, "/");
      strcat(path, entry->d_name);
      total += totalSizeR(opendir(path), path);    
    }
    else {
      stat(entry->d_name, &buff);
      total += buff.st_size;
    }
  return total;
}

void printTotalSize(DIR * theDir){
  printf("Total size not including subdirs: ");
  printSizeHuman(totalSize(theDir));
}

void printTotalSizeR(DIR * theDir, char * pathToHere){
  printf("Total size including subdirs: ");
  printSizeHuman(totalSizeR(theDir, pathToHere));
}

//===================================================================

int main(){
  DIR * theDir = opendir(".");

  printf("\n");
  printAll(theDir);
  printf("\n");
  printAllR(theDir, ".");
  printf("\n");
  printDirs(theDir);
  printf("\n");
  printRegFiles(theDir);
  printf("\n");
  printTotalSize(theDir);
  printf("\n");
  printTotalSizeR(theDir, ".");
  printf("\n");

  closedir(theDir);
  return 0;
}
