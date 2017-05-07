#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <semaphore.h>


void adult_maker(unsigned adult_count);
void child_maker(unsigned children_count);

void child(unsigned intern_id);
void adult(unsigned intern_id);

#define FILE_WRITE(semaphore,file,text,...) sem_wait(semaphore); states->actionNumber++; fprintf(file, text,__VA_ARGS__); fflush(file); sem_post(semaphore); 

enum shared_semaphores { SEM_PROCESS_PRIVATE,SEM_PROCESS_SHARED };
