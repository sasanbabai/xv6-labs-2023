#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int
match(char *ptrn, char *str)
{
  int ptrn_len = strlen(ptrn), str_len = strlen(str);

  if (ptrn_len > str_len)
    return 0;

  if (ptrn_len == 0 || str_len == 0)
    return 0;
  
  for (int i = 0; i < str_len - ptrn_len + 1; i++) {
    int match = 1;	  
    for (int j = 0; j < ptrn_len; j++) {
      if (ptrn[j] == str[i + j])
        continue;

      match = 0;
      break;
    }
    if (match)
      return 1;
  }
  return 0;
}

void
find(char *path, char *file)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    exit(1);
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    exit(1);
  }

  if(st.type != T_DIR){
    fprintf(2, "find: %s is not a directory\n", path);
    close(fd);
    exit(1);
  }

  read(fd, &de, sizeof(de));
  read(fd, &de, sizeof(de));
  while(read(fd, &de, sizeof(de)) == sizeof(de)){
    if(de.inum == 0 || de.inum == 1)
      continue;

    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    memmove(p, de.name, strlen(de.name));
    p += strlen(de.name);
    *p = 0;

    if(stat(buf, &st) < 0){
      fprintf(2, "find: cannot stat %s\n", buf);
      continue;
    }

    switch(st.type){
    case T_FILE:
      if (match(file, de.name))
        printf("%s\n", buf);
      break;

    case T_DIR:
      find(buf, file);
      break;
    }
  }
  
  close(fd);
}

int
main(int argc, char *argv[])
{
  if(argc < 1){
    printf("usage: find [path] file [file]...");
    exit(1);
  }

  if(argc == 2){
    find(".", argv[1]);
    exit(0);
  }
  
  for(int i = 2; i < argc; i++)
    find(argv[1], argv[i]);
  
  exit(0);
}

