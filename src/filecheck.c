
#include <malloc.h>
#include <stdio.h>
#include <string.h>

#ifdef __linux__
#include <fcntl.h>
#include <sys/syscall.h>
#include <unistd.h>

// Size of buffer used to store raw directory entries
#define BUF_SIZE 4096

// Structure returned by the getdents syscall (old Linux ABI)
struct linux_dirent {
  long d_ino;              // inode number
  long d_off;              // offset to next directory entry
  unsigned short d_reclen; // size of this directory entry
  char d_name[];           // filename (null-terminated)
};
#endif

void read_file_and_print(const char *filepath) {
  FILE *fp = fopen(filepath, "r");
  if (!fp) {
    perror("fopen");
    return;
  }

  // allocate buffer
  fseek(fp, 0, SEEK_END);
  long size = ftell(fp);
  rewind(fp);

  char *buffer = malloc(size + 1);
  if (!buffer) {
    fclose(fp);
    return;
  }

  fread(buffer, 1, size, fp);
  buffer[size] = '\0';

  printf("--- %s ---\n%s\n", filepath, buffer);

  free(buffer);
  fclose(fp);
}

// ------------------------------------------------------------
// Linux directory listing using raw getdents system call.
// NOTE: This is low-level and Linux-specific.
// ------------------------------------------------------------

void list_linux(const char *path) {
#ifdef __linux__
  int fd = open(path, O_RDONLY | O_DIRECTORY);
  if (fd == -1) {
    perror("open");
    return;
  }

  char buffer[BUF_SIZE];
  int nread;

  while ((nread = syscall(SYS_getdents, fd, buffer, BUF_SIZE)) > 0) {
    int offset = 0;

    while (offset < nread) {
      struct linux_dirent *d = (struct linux_dirent *)(buffer + offset);

      // skip . and ..
      if (strcmp(d->d_name, ".") != 0 && strcmp(d->d_name, "..") != 0) {
        printf("Found: %s\n", d->d_name);

        // build full path
        char fullpath[1024];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, d->d_name);

        // try reading file contents
        read_file_and_print(fullpath);
      }

      offset += d->d_reclen;
    }
  }

  close(fd);
#endif
}

// ------------------------------------------------------------
// Windows directory listing using the 'dir' command via popen().
// No <windows.h> required, avoids platform-dependent structs.
// ------------------------------------------------------------
void list_windows(const char *path) {
#ifndef __linux__
  char cmd[512];
  snprintf(cmd, sizeof(cmd), "dir /b \"%s\"", path);

  FILE *fp = popen(cmd, "r");
  if (!fp) {
    printf("Failed to run dir command.\n");
    return;
  }

  char file[512];
  char fullpath[1024];

  while (fgets(file, sizeof(file), fp)) {
    file[strcspn(file, "\r\n")] = 0;
    printf("Found: %s\n", file);

    snprintf(fullpath, sizeof(fullpath), "%s\\%s", path, file);

    // print file contents
    FILE *fp2 = fopen(fullpath, "r");
    if (fp2) {
      printf("--- %s ---\n", fullpath);

      int c;
      while ((c = fgetc(fp2)) != EOF)
        putchar(c);

      fclose(fp2);
    }
  }

  pclose(fp);
#endif
}
/*
d_inot main() {
  const char *path = "../folder/";

#ifdef __linux__
  list_linux(path); // Use Linux implementation
#else
  list_windows(path); // Use Windows fallback
#endif

  return 0;
}*/
