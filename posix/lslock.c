#define _GNU_SOURCE
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <limits.h> /* PATH_MAX */



int main (int argc, char *argv[])
{
    struct dirent *pDirent;
    DIR *pDir;

    int fd;
    int res;
    struct flock lslock;

    if (argc < 2)
    {
        printf ("Usage: %s <dirname>\n", argv[0]);
        return 1;
    }
    pDir = opendir (argv[1]);
    if (pDir == NULL)
    {
        printf ("Cannot open directory '%s'\n", argv[1]);
        return 1;
    }

    while ((pDirent = readdir(pDir)) != NULL)
    {
      if (pDirent->d_type == DT_REG) // We don't need to read . or ..
      {
        char absolute_path[PATH_MAX+1];
        strncpy (absolute_path, argv[1], PATH_MAX); // Buffer overflow prevention
        strcat (absolute_path,"/");
        strcat (absolute_path, pDirent->d_name);

        fd = open(absolute_path, O_RDWR | O_CREAT, 0666);
        if (!fd) {
          fprintf(stderr, "Unable to open %s for read/write", absolute_path);
          return 1;
        }

        lslock.l_type = F_WRLCK;
        lslock.l_whence = SEEK_SET;
        lslock.l_pid = -1;

        // Let's use the syscall fcntl using the command F_GETLK and flock struct data
        // We don't need bypass the kernel reading the /proc/locks for locked pid or node
        //
        res = fcntl(fd, F_GETLK, &lslock);
        if (res == -1) {
          fprintf(stderr, "F_GETLK failed\n");
          return 1;
        }

        // Let's check if the l_pid is different
        printf("pid [%d]\n", lslock.l_pid);

        if (lslock.l_pid != -1) {
          printf("PID [%d] - [%s]\n", lslock.l_pid, absolute_path);
        }


      }
    }
    closedir (pDir);
    close(fd);
    return 0;
}
