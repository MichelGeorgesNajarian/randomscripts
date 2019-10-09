/*

VALGRIND OUTPUT:

nmigeo@nmigeo-VirtualBox:~/github_clone$ valgrind --leak-check=full ./recursive_rename.out .
==14231== Memcheck, a memory error detector
==14231== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==14231== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
==14231== Command: ./recursive_rename.out .
==14231==
==14234==
==14234== HEAP SUMMARY:
==14234==     in use at exit: 0 bytes in 0 blocks
==14234==   total heap usage: 46 allocs, 46 frees, 66,047 bytes allocated
==14234==
==14234== All heap blocks were freed -- no leaks are possible
==14234==
==14234== For counts of detected and suppressed errors, rerun with: -v
==14234== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
==14231==
==14231== HEAP SUMMARY:
==14231==     in use at exit: 0 bytes in 0 blocks
==14231==   total heap usage: 87 allocs, 87 frees, 33,636 bytes allocated
==14231==
==14231== All heap blocks were freed -- no leaks are possible
==14231==
==14231== For counts of detected and suppressed errors, rerun with: -v
==14231== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)

*/
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

char *concatenation(char *, char *);
char *fileType(char *);
void recursiveWalk(char *);
void renameFile(char *);
char *removeWhiteSpaces(char *);
char *clearWhiteSpace(char *, int, char *);

int main(int argc, char **argv)
{

  if (argc == 0)
  {
    printf("Usage: %s <root directory>\n", argv[0]);
    exit(1);
  }
  if (argv[1][strlen(argv[1]) - 1] == '/')
  {
    argv[1][strlen(argv[1]) - 1] = '\0';
  }
  recursiveWalk(argv[1]);
  return 0;
}

char *concatenation(char *str1, char *str2)
{
  int len1 = strlen(str1);
  int len2 = strlen(str2);

  char *ret = calloc(1, (len1 + len2 + 1) * sizeof(char)); /* +1 comes for: '\0' */
  memcpy(ret, str1, len1);
  memcpy(ret + len1, str2, len2);
  *(ret + len1 + len2) = '\0';
  return ret;
}

char *fileType(char *filename)
{
  struct stat buf;
  if (lstat(filename, &buf) < 0)
  {
    perror("lstat error");
  }
  char *ptr = NULL;
  if (S_ISREG(buf.st_mode))
    ptr = "regular";
  else if (S_ISDIR(buf.st_mode))
    ptr = "directory";
  else if (S_ISCHR(buf.st_mode))
    ptr = "character special";
  else if (S_ISBLK(buf.st_mode))
    ptr = "block special";
  else if (S_ISFIFO(buf.st_mode))
    ptr = "fifo";
#ifdef S_ISLNK
  else if (S_ISLNK(buf.st_mode))
    ptr = "symbolic link";
#endif
#ifdef S_ISSOCK
  else if (S_ISSOCK(buf.st_mode))
    ptr = "socket";
#endif
  else
    ptr = "**unknown mode**";

  return ptr;
}

void recursiveWalk(char *root_str)
{
  DIR *root_dir = NULL;
  struct dirent *root_dirent = NULL;
  if ((root_dir = opendir(root_str)) == NULL)
  {
    printf("ERROR on opendir(%s): %s\n", root_str, strerror(errno));
    closedir(root_dir);
  }
  else
  {
    while ((root_dirent = readdir(root_dir)) != NULL)
    {
      if (!strcmp(root_dirent->d_name, ".") | !strcmp(root_dirent->d_name, ".."))
      {
        //printf("directory to skip is: %s\n", root_dirent->d_name);
      }
      else
      {
        char *tempPtr = concatenation(root_str, "/");
        char *cat = concatenation(tempPtr, root_dirent->d_name);
        free(tempPtr);
        if (!memcmp(fileType(cat), "directory", 9 * sizeof(char)))
        {
          if (fork() == 0)
          {
            recursiveWalk(cat);
            closedir(root_dir);
            free(cat);
            return;
          }
        }
        else if (!memcmp(fileType(cat), "regular", 7 * sizeof(char)))
        {
          renameFile(cat);
        }
        free(cat);
      }
    }
    closedir(root_dir);
    while (wait(NULL) != -1)
      ;
  }
}

void renameFile(char *filename)
{
  char *filteredName = NULL;
  int i = 0;
  int isValid = 1;
  int startingInd = -1;
  int endingInd = -1;
  int startFileNameIndex = 0;
  for (i = 0; i < strlen(filename); i++)
  {
    if (filename[i] == '/' && i + 1 != strlen(filename))
    {
      startFileNameIndex = i;
    }
    else if (filename[i] == '_')
    {
      filename[i] = ' ';
    }
    else if (filename[i] == ']' || filename[i] == ')')
    {
      isValid = 1;
      startingInd = -1;
    }
    else if (filename[i] == '[' || filename[i] == '(')
    {
      isValid = 0;
      endingInd = i;
      char *temp = malloc(sizeof(char) * (endingInd - startingInd) + 1);
      memset(temp, 0, sizeof(char) * (endingInd - startingInd) + 1);
      memcpy(temp, filename + startingInd * sizeof(char), (endingInd - startingInd) * sizeof(char));
      if (filteredName != NULL)
      {
        filteredName = concatenation(filteredName, temp);
        free(temp);
      }
      else
      {
        filteredName = temp;
      }
      startingInd = -1;
      endingInd = -1;
    }
    else if (isValid && filename[i] != ' ')
    {
      if (startingInd == -1)
      {
        startingInd = i;
      }
    }
  }
  if (isValid)
  {
    endingInd = strlen(filename);
    char *temp = malloc(sizeof(char) * (endingInd - startingInd) + 1);
    memset(temp, 0, sizeof(char) * (endingInd - startingInd) + 1);
    memcpy(temp, filename + startingInd * sizeof(char), (endingInd - startingInd) * sizeof(char));
    if (filteredName != NULL)
    {
      filteredName = concatenation(filteredName, temp);
      free(temp);
    }
    else
    {
      filteredName = temp;
    }
    startingInd = -1;
    endingInd = -1;
  }
  char *path = malloc(sizeof(char) * (startFileNameIndex) + 2);
  memset(path, 0, sizeof(char) * startFileNameIndex + 2);
  memcpy(path, filteredName, startFileNameIndex * sizeof(char));
  *(path + startFileNameIndex + 1) = '\0';
  char *tempPath = concatenation(path, "/");
  char *rwsRet = removeWhiteSpaces(filteredName + startFileNameIndex);
  char *tempPtr = concatenation(tempPath, rwsRet);
  free(filteredName);
  free(tempPath);
  free(path);
  filteredName = tempPtr;
  rename(filename, filteredName);
  free(filteredName);
  //printf("value of ret is: %d for filename: %s\n", ret, filename);
}

char *removeWhiteSpaces(char *filename)
{
  int i = 0;
  int isStarted = 0;
  char *ret;
  int spaceCount = 0;
  int indexStart = -1;
  int beginindex = 0;
  for (i = 0; i < strlen(filename); i++)
  {
    if (!isStarted && filename[i] != ' ')
    {
      //removes any leading spaces
      isStarted = 1;
      beginindex = i;
      ret = filename + i;
    }
    else if (isStarted)
    {
      if (filename[i] == ' ')
      {
        spaceCount++;
        if (indexStart == -1)
        {
          indexStart = i;
        }
      }
      else if (filename[i] == '.')
      {
        if (spaceCount)
        {
          ret = clearWhiteSpace(ret, indexStart - beginindex, filename + i * sizeof(char));
          spaceCount = 0;
        }
      }
      else
      {
        if (spaceCount > 1)
        {
          // copy everything from ret from begining up to indexStart to temp var and concat temp var and filename starting at current index
          ret = clearWhiteSpace(ret, indexStart - beginindex + 1, filename + i * sizeof(char));
          beginindex += spaceCount - 1;
        }
        spaceCount = 0;
        indexStart = -1;
      }
    }
    if (filename[i] == '/')
    {
      isStarted = 0;
      beginindex = 0;
      ret = NULL;
    }
  }
  if (spaceCount)
  {
    char *temp = malloc((strlen(ret) - spaceCount) * sizeof(char) + 1); // +1 is for \0
    memset(temp, 0, (strlen(ret) - spaceCount) * sizeof(char) + 1);
    memcpy(temp, ret, (strlen(ret) - spaceCount) * sizeof(char));
    free(ret);
    ret = temp;
  }
  return ret;
}
char *clearWhiteSpace(char *ret, int length, char *restOfFile)
{
  // copy everything from ret from begining up to indexStart to temp var and concat temp var and filename starting at current index
  char *temp = malloc(length * sizeof(char) + 1);
  memset(temp, 0, 1 + length * sizeof(char));
  memcpy(temp, ret, length * sizeof(char));
  *(temp + length) = '\0';
  /* the above will copy everything from the begining of ret which is the filename with the leading spaces removed, up to the first space encountered */
  char *res = concatenation(temp, restOfFile);
  free(temp);
  return res;
}