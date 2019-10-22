#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
  int file;
  if(argc == 3){
    printf("Digging info from %s\n",argv[1]);
    file = open(argv[1], O_RDONLY);
    if(file < 0){
      perror("open");
      printf("Error opening file %s!\n",argv[1]);
      return(-1);
    }
    int nb_readings = 10;
    if ((nb_readings = atoi(argv[2])) == 0) {
      nb_readings = 10;
      printf("OK, nerd, making %d readings\n", nb_readings);
    }


    char distance[5];
    while (1) {
      int size = read(file, distance, sizeof(distance));
      printf("Distance = %s\n", distance);
      sleep(nb_readings);
    }
    close(file);
  }
  else
    printf("usage: %s <filename> <interval to read>\n", argv[0]);
  return 0;
}
