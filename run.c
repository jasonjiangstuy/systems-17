#include <stdio.h>
#include <stdlib.h>

#include <sys/ipc.h>
#include <sys/types.h>

#include <string.h>
#include <unistd.h>

#include <sys/sem.h>
#include <sys/shm.h>

#include <errno.h>

#include <fcntl.h>

#define KEY 24602
#define SHMKEY 24601
//
// struct sembuf {
//   short sem_op;
//   short sem_num;
//   short sem_flag;
// };

int main(int argc, char const *argv[]) {
  int semd = semget(KEY, 1, 0); //get access
  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_flg = SEM_UNDO;
  sb.sem_op = -1; //setting the operation to down

  semop(semd, &sb, 1); //preform operation

  // get the last line
    // get size
  int shmd = shmget(SHMKEY, 0, 0);
  int* size = shmat(shmd, 0, 0);
  // printf("%d\n", *size);
  int fd = open("data.txt", O_RDWR | O_APPEND);
  lseek(fd, -*size, SEEK_END);
  char last_message[*size];
  read(fd, last_message, *size);

  printf("LAST MESSAGE: %s\n", last_message);
  // NEXT MESSAGE
  // max size of message
  int maxsizeofmessage = 200;
  char message[maxsizeofmessage];
  fgets(message, maxsizeofmessage, stdin);
  lseek(fd, 0, SEEK_END);
  // save size
  *size = strlen(message);
  write(fd, message, *size);
  shmdt(size);

  //let someone else have it
  sb.sem_op = 1; //set the operation to up
  semop(semd, &sb, 1); //preform operation
  return 0;
}
