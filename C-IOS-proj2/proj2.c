#include "proj2.h"

/* arguments
 * A - number of adults
 * C - number of children
 * AGT - max time for generate adult
 * CGT - max time for generate children
 * AWT - max time of adult work simulation
 * CWT - max time of child work simulation
 */

int A, C, AGT, CGT, AWT, CWT;

sem_t *child_free_place = NULL;
sem_t *state_mutex = NULL;
sem_t *file_mutex = NULL;
sem_t *wait_for_ending = NULL;

FILE *output = NULL;

int segment_id;
int memmory_id;

/**
  * Field which holds every needed values as number of generated adult/children and their actual number
  * [0] = generated adults
  * [1] = actual number of adults in center
  * [2] = generated children
  * [3] = actual number of children in center
  * [4] = number of adults that left the center
  * [5] = number of children that left the center
  * [6] = action number
  */
//int *state_field;

typedef struct States{
  int generatedAdults;
  int adultsInCenter;
  int generatedChildren;
  int childrenInCenter;
  int adultsLeftCenter;
  int childrenLeftCenter;
  int actionNumber;
}States;

States *states;

int main(int argc, char const *argv[]) {
  if (argc == 7) {
    A = atoi(argv[1]);
    C = atoi(argv[2]);
    AGT = atoi(argv[3]);
    CGT = atoi(argv[4]);
    AWT = atoi(argv[5]);
    CWT = atoi(argv[6]);
  } else {
    fprintf(stderr, "%s\n", "Neplatný počet argumentů");
    return -1;

  }

  output = fopen("proj2.out", "w");
  if (output == NULL) {
    fprintf(stderr, "%s\n", "Nepodařilo se otevřít soubor pro zápis");
    return -1;
  }

  //alocate shared memmory for state informations
  memmory_id = shmget(IPC_PRIVATE, sizeof(States), IPC_EXCL | S_IRUSR | S_IWUSR);
  states = (States*) shmat(memmory_id, NULL, 0);
  states->actionNumber = 0;
  states->adultsInCenter = 0;
  states->adultsLeftCenter = 0;
  states->childrenInCenter = 0;
  states->childrenLeftCenter = 0;
  states->generatedAdults = 0;
  states->generatedChildren = 0;

  segment_id = shmget(IPC_PRIVATE, sizeof(sem_t) , IPC_CREAT | S_IRUSR | S_IWUSR);
  child_free_place = (sem_t *) shmat(segment_id++, NULL, 0);
  sem_init(child_free_place, SEM_PROCESS_SHARED, 0);

  segment_id = shmget(IPC_PRIVATE, sizeof(sem_t) , IPC_CREAT | S_IRUSR | S_IWUSR);
  state_mutex = (sem_t *) shmat(segment_id++, NULL, 0);
  sem_init(state_mutex, SEM_PROCESS_SHARED, 1);

  segment_id = shmget(IPC_PRIVATE, sizeof(sem_t) , IPC_CREAT | S_IRUSR | S_IWUSR);
  file_mutex = (sem_t *) shmat(segment_id++, NULL, 0);
  sem_init(file_mutex, SEM_PROCESS_SHARED, 1);

  segment_id = shmget(IPC_PRIVATE, sizeof(sem_t) , IPC_CREAT | S_IRUSR | S_IWUSR);
  wait_for_ending = (sem_t *) shmat(segment_id++, NULL, 0);
  sem_init(wait_for_ending, SEM_PROCESS_SHARED, 0);

  pid_t pid_adult_maker =fork();
  if (pid_adult_maker == 0) {
    //children code of adult maker
    adult_maker(A);
  }
  else {
    // then create child_maker
    pid_t pid_child_maker = fork();
    if (pid_child_maker == 0) {
      //child code and make children
      child_maker(C);
    }else {
      wait(NULL);// wait for every child process
    }
  }

  fclose(output);

  sem_destroy(child_free_place);
  sem_destroy(state_mutex);
  sem_destroy(file_mutex);
  sem_destroy(wait_for_ending);
  return 0;
}

// process which creates "adults"
void adult_maker(unsigned adult_count){
  srand(2);
  pid_t adults_pid[adult_count];
  for (size_t i = 1; i <= adult_count; i++) {
    adults_pid[i] = fork(); // make adult
    if (adults_pid[i] == 0) {
      // the adult code
      adult(i);
    } else {
      sem_wait(state_mutex);
        states->generatedAdults++;
      sem_post(state_mutex);
      if(AGT > 0)
        usleep(rand() % AGT);
    }
  }
  wait(NULL);// wait for every child process
}

// process which creates "children"
void child_maker(unsigned children_count){
  srand(2);
  pid_t children_pid[children_count];
  for (size_t i = 1; i <= children_count; i++) {
    children_pid[i] = fork(); // make child
    if (children_pid[i] == 0) {
      // the adult code
      child(i);
    } else {
      sem_wait(state_mutex);
        states->generatedChildren++;
      sem_post(state_mutex);
      if (CGT > 0) {
        usleep(rand() % CGT);
      }
    }
  }
  wait(NULL); // wait for every child process
}

// child process
void child(unsigned intern_id){
  FILE_WRITE(file_mutex,output,"%u\t: C %u\t: started\n",states->actionNumber,intern_id);

  //-------------------------------enter sequence ---------------------------------------
  sem_wait(state_mutex);
  if (states->adultsLeftCenter != A) {// if there is no more adult available don't apply this condition
  // waiting for free place in the center
  int val;
  sem_getvalue(child_free_place, &val);
  if (val <= 0) {
    FILE_WRITE(file_mutex,output,"%u\t: C %u\t: waiting: %u: %u\n",states->actionNumber,intern_id,states->adultsInCenter,states->childrenInCenter);
  }
  sem_post(state_mutex);
    sem_wait(child_free_place);// enter the center
  sem_wait(state_mutex);
  }
  states->childrenInCenter++;
  //Entering the center
  FILE_WRITE(file_mutex,output,"%u\t: C %u\t: enter\n",states->actionNumber,intern_id);
  sem_post(state_mutex);
  //-------------------------------enter sequence end------------------------------------

  // simuluje praci
  if (CWT > 0) {
    usleep(rand() % CWT);
  }

  //--------------------------------leave sequence----------------------------------------
  sem_wait(state_mutex);
    FILE_WRITE(file_mutex,output,"%u\t: C %u\t: trying to leave\n",states->actionNumber,intern_id); // process trying to leave center
    FILE_WRITE(file_mutex,output,"%u\t: C %u\t: leave\n",states->actionNumber,intern_id); // write to file that leaved the center
    sem_post(child_free_place); // and now leave the center
    states->childrenInCenter--; // decrement the actual number of childrens in center
    states->childrenLeftCenter++; // increment total number of children that left the center
    if (states->childrenLeftCenter == C && states->adultsLeftCenter == A) { // if this ending procces is the last, then unlock the last mutex
      sem_post(wait_for_ending);
    }
  sem_post(state_mutex);
  //--------------------------------leave sequence end -----------------------------------

  sem_wait(wait_for_ending); // wait for every other procces end
    FILE_WRITE(file_mutex,output,"%u\t: C %u\t: finished\n",states->actionNumber,intern_id );
  sem_post(wait_for_ending);

  exit(0);
}

// adult process
void adult(unsigned intern_id){
  FILE_WRITE(file_mutex,output,"%u\t: A %u\t: started\n",states->actionNumber,intern_id );

  sem_wait(state_mutex); // lock the state changes for every procces
    FILE_WRITE(file_mutex,output,"%u\t: A %u\t: enter\n",states->actionNumber,intern_id);
    sem_post(child_free_place); // make free places for children in center
    sem_post(child_free_place);
    sem_post(child_free_place);
    states->adultsInCenter++; // increment the actual number of adults in center
  sem_post(state_mutex);

  //simulation of work
  if (AWT > 0) {
    usleep(rand() % AWT);
  }

  //--------------------------adult leaving section ---------------------------
  sem_wait(state_mutex);
    FILE_WRITE(file_mutex,output,"%u\t: A %u\t: trying to leave\n",states->actionNumber,intern_id);
    states->adultsLeftCenter++; // increment the number of adults that left the center
    int val; /// free space in center
    sem_getvalue(child_free_place, &val);
    if (val < 3 ) { // if adult can leave center
      FILE_WRITE(file_mutex,output,"%u\t: A %u\t: waiting : %d : %d\n",states->actionNumber,intern_id,states->adultsInCenter,states->childrenInCenter);
      sem_post(state_mutex); // unlock the state changes for alloving childrens to leave cneter
    sem_wait(child_free_place); // wait for children leave the center
    sem_wait(child_free_place);
    sem_wait(child_free_place);
      sem_wait(state_mutex);
    } else {
      sem_wait(child_free_place); // leave the center and
      sem_wait(child_free_place);
      sem_wait(child_free_place);
    }
    states->adultsInCenter--; // decrement actual number of adult in center

    FILE_WRITE(file_mutex,output,"%u\t: A %u\t: leave\n",states->actionNumber,intern_id);

    if (states->childrenLeftCenter == C && states->adultsLeftCenter == A) { // if this is the last proccess that leaving the center
      sem_post(wait_for_ending);
    }
  sem_post(state_mutex);
  //--------------------------adult leaving section end------------------------

  sem_wait(wait_for_ending); // wait for other proccess
  FILE_WRITE(file_mutex,output,"%u\t: A %u\t: finished\n",states->actionNumber,intern_id );
  sem_post(wait_for_ending);

  exit(0);
}
