// Copyright (c) 2020 Trevor Bakker
// Biraj Singh GC
// ID: 1001867347


#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

/*** Constants that define parameters of the simulation ***/

#define MAX_SEATS 3        /* Number of seats in the professor's office */
#define professor_LIMIT 10 /* Number of students the professor can help before he needs a break */
#define MAX_STUDENTS 1000  /* Maximum number of students in the simulation */
#define MAX_CONSEC 5       /* Maximum consecutive number of students per class to be served*/

#define CLASSA 0
#define CLASSB 1
#define CLASSC 2
#define CLASSD 3
#define CLASSE 4

/* TODO */
/* Add your synchronization variables here */

pthread_mutex_t studentAMutex;    //To protect critical region
pthread_mutex_t studentBMutex;    //To protect critical region
pthread_mutex_t ProfessorMutex;    //To protect critical region
pthread_cond_t studentLeave;     //To check if any student leaves office 
pthread_cond_t isProfAvailable;  //To check if Prof is available.

/* Basic information about simulation.  They are printed/checked at the end 
 * and in assert statements during execution.
 *
 * You are responsible for maintaining the integrity of these variables in the 
 * code that you develop. 
 */

static int students_in_office;   /* Total numbers of students currently in the office */
static int classa_inoffice;      /* Total numbers of students from class A currently in the office */
static int classb_inoffice;      /* Total numbers of students from class B in the office */
static int students_since_break = 0;

static int consecEntryClassA;    // Store Consecutive entry of class A students  
static int consecEntryClassB;    // Store Consecutive entry of class B students
static int WaitlistA;            // Store Number of Class A students waiting on queue  
static int WaitlistB;            // Store Number of Class B students waiting on queue
static int onBreak;              // Check to see if prof is on break can be 1 or 0

typedef struct {
  int arrival_time;  // time between the arrival of this student and the previous student
  int question_time; // time the student needs to spend with the professor
  int student_id;
  int class;
} student_info;

/* Called at beginning of simulation.  
 * TODO: Create/initialize all synchronization
 * variables and other global variables that you add.
 */
static int initialize(student_info *si, char *filename) {
  students_in_office = 0;
  classa_inoffice = 0;
  classb_inoffice = 0;
  students_since_break = 0;
  consecEntryClassA = 0;
  consecEntryClassA = 0;
  WaitlistA = 0;
  WaitlistB = 0;
  onBreak = 1; //proffessor is initially not in his office

  /* Initialize your synchronization variables (and 
   * other variables you might use) here
   */
  //Used to protect critical region by locking and unlocking
  pthread_mutex_init(&studentAMutex, NULL);
  pthread_mutex_init(&studentBMutex, NULL);
  pthread_mutex_init(&ProfessorMutex, NULL);
  //used to check if prof is available(can send signal)
  pthread_cond_init(&isProfAvailable, NULL);

  //Used to check if any student left office
  pthread_cond_init(&studentLeave, NULL);

  /* Read in the data file and initialize the student array */
  FILE *fp;

  if((fp=fopen(filename, "r")) == NULL) 
  {
    printf("Cannot open input file %s for reading.\n", filename);
    exit(1);
  }

  int i = 0;
  while ( (fscanf(fp, "%d%d%d\n", &(si[i].class), &(si[i].arrival_time), &(si[i].question_time))!=EOF) && 
           i < MAX_STUDENTS ) 
  {
    i++;
  }

 fclose(fp);
 return i;
}
/*Set coditions for class A students to enter the office*/
static int ClassA_enterCondtion(){
  if((classb_inoffice == 0) &&                          // No class B students in office
     (students_in_office < MAX_SEATS) &&                // Only 3 students allowed at a time 
     (consecEntryClassA < MAX_CONSEC || WaitlistB == 0))// Must not exceed 5 studens continiously
    return 1;

  return 0;
}
/*Set coditions for class B students to enter the office*/
static int ClassB_enterCondtion(){
  if((classa_inoffice == 0) &&                         // No class B students in office
     (students_in_office < MAX_SEATS) &&                // Only 3 students allowed at a time 
     (consecEntryClassB < MAX_CONSEC || WaitlistA == 0))// Must not exceed 5 studens continiously
    return 1;

  return 0;
}

/*Set conditions for professor to take break*/
static int take_breakCondition(){
  if(students_in_office == 0 &&                         //Must be no student in office
     students_since_break == professor_LIMIT)           //Has reached limit
    return 1;

  return 0;
}

/*Set condition to check if professor is on break*/
static int on_breakCondition(){
  if(students_since_break >= professor_LIMIT || onBreak) 
    return 1;
  
  return 0;
}

/* Code executed by professor to simulate taking a break 
 * You do not need to add anything here.  
 */
static void take_break() {
  printf("The professor is taking a break now.\n");
  sleep(5);
  assert( students_in_office == 0 );
  students_since_break = 0;
}

/* Code for the professor thread. This is fully implemented except for synchronization
 * with the students.  See the comments within the function for details.
 */
void *professorthread(void *junk) {
  printf("The professor arrived and is starting his office hours\n");

  /* Loop while waiting for students to arrive. */
  while (1) {     
    //lock the critical region                                 
    pthread_mutex_lock(&studentMutex);
    
    //professor has entered the office
    if(onBreak){
      onBreak = 0;
      //Let students know prof is available
      pthread_cond_broadcast(&isProfAvailable);
    }

    //Checch to see if prof should be taking break
    if(take_breakCondition()){
      onBreak = 1;      // Starting break  
      take_break();     // Taking Break
      onBreak = 0;      // Break Ends  
      pthread_cond_broadcast(&isProfAvailable); //Let students know prof is available
    }
    pthread_mutex_unlock(&studentMutex);
    //unlock the critical region 
  }
  pthread_exit(NULL);
}


/* Code executed by a class A student to enter the office.
 * You have to implement this.  Do not delete the assert() statements,
 * but feel free to add your own.
 */
void classa_enter() {
  pthread_mutex_lock(&studentMutex);  //lock the critical region
  WaitlistA++;                        // Begin waiting for the conditions to match

  while(1){
    if(on_breakCondition()){
      //Wait until the professor is available
      //Dont hold resourses while waiting
      pthread_cond_wait(&isProfAvailable,&studentMutex); 
    }
    if(!ClassA_enterCondtion()){
      //Wait until the student leaves office 
      //Dont hold resourses while waiting
      pthread_cond_wait(&studentLeave, &studentMutex);
    }
    //Check condition again to make sure conditions havent changed since initial check
    if((!on_breakCondition()) && 
      ClassA_enterCondtion()){

      students_in_office += 1;
      students_since_break += 1;
      classa_inoffice += 1;

      consecEntryClassB = 0;          //Stop the consecutive entry for classB
      consecEntryClassA++;            //Start or continue class A's consec Entry
      WaitlistA--;                    //Student is no longer waiting and is in the office
      break;
    }
  }
  pthread_mutex_unlock(&studentMutex);
}

/* Code executed by a class B student to enter the office.
 * You have to implement this.  Do not delete the assert() statements,
 * but feel free to add your own.
 */
void classb_enter() {                                                    
 pthread_mutex_lock(&studentMutex);   //lock the critical region
 WaitlistB++;                         //Begin waiting for the conditions to match

  while(1){
    if(on_breakCondition()){
      //Wait until the professor is available
      //Dont hold resourses while waiting
      pthread_cond_wait(&isProfAvailable,&studentMutex); // wait untli  professor is on break
    }
    if(!ClassB_enterCondtion()){
      //Wait until the student leaves office 
      //Dont hold resourses while waiting
      pthread_cond_wait(&studentLeave, &studentMutex); //wait until a student leaves office
    }
    if((!on_breakCondition()) && 
      ClassB_enterCondtion()){   
      //Check condition again to make sure conditions havent changed since initial check
      students_in_office += 1;
      students_since_break += 1;
      classb_inoffice += 1;
  
      consecEntryClassA = 0;          //Stop the consecutive entry for classA
      consecEntryClassB++;            //Start or continue class B's consec Entry
      WaitlistB--;                    //Student is no longer waiting and is in the office
      break;
    }
  }
 pthread_mutex_unlock(&studentMutex);
}

/* Code executed by a student to simulate the time he spends in the office asking questions
 * You do not need to add anything here.  
 */
static void ask_questions(int t) {
  sleep(t);
}


/* Code executed by a class A student when leaving the office.
 * You need to implement this.  Do not delete the assert() statements,
 * but feel free to add as many of your own as you like.
 */
static void classa_leave() {
  pthread_mutex_lock(&studentMutex);        //lock the critical region
  students_in_office -= 1;
  classa_inoffice -= 1;
  pthread_mutex_unlock(&studentMutex);      //unlock the critical region 

  pthread_cond_broadcast(&studentLeave);    //Let students know that there is an empty spot in office

}

/* Code executed by a class B student when leaving the office.
 * You need to implement this.  Do not delete the assert() statements,
 * but feel free to add as many of your own as you like.
 */
static void classb_leave(){
  pthread_mutex_lock(&studentMutex);        //lock the critical region
  students_in_office -= 1;
  classb_inoffice -= 1;
  pthread_mutex_unlock(&studentMutex);      //unlock the critical region 

  pthread_cond_broadcast(&studentLeave);    //Let students know that there is an empty spot in office
}

/* Main code for class A student threads.  
 * You do not need to change anything here, but you can add
 * debug statements to help you during development/debugging.
 */
void* classa_student(void *si){
  student_info *s_info = (student_info*)si;

  /* enter office */
  classa_enter();

  printf("Student %d from class A enters the office\n", s_info->student_id);
  
  assert(students_since_break <= professor_LIMIT);
  assert(students_in_office <= MAX_SEATS && students_in_office >= 0);
  assert(classa_inoffice >= 0 && classa_inoffice <= MAX_SEATS);
  assert(classb_inoffice >= 0 && classb_inoffice <= MAX_SEATS);
  assert(classb_inoffice == 0 );
  
  /* ask questions  --- do not make changes to the 3 lines below*/
  printf("Student %d from class A starts asking questions for %d minutes\n", s_info->student_id, s_info->question_time);
  ask_questions(s_info->question_time);
  printf("Student %d from class A finishes asking questions and prepares to leave\n", s_info->student_id);

  /* leave office */
  classa_leave();  

  printf("Student %d from class A leaves the office\n", s_info->student_id);

  assert(students_in_office <= MAX_SEATS && students_in_office >= 0);
  assert(classb_inoffice >= 0 && classb_inoffice <= MAX_SEATS);
  assert(classa_inoffice >= 0 && classa_inoffice <= MAX_SEATS);


  pthread_exit(NULL);
}

/* Main code for class B student threads.
 * You do not need to change anything here, but you can add
 * debug statements to help you during development/debugging.
 */
void* classb_student(void *si){
  student_info *s_info = (student_info*)si;

  /* enter office */
  classb_enter();

  printf("Student %d from class B enters the office\n", s_info->student_id);
  assert(students_since_break <= professor_LIMIT);
  assert(students_in_office <= MAX_SEATS && students_in_office >= 0);
  assert(classb_inoffice >= 0 && classb_inoffice <= MAX_SEATS);
  assert(classa_inoffice >= 0 && classa_inoffice <= MAX_SEATS);
  assert(classa_inoffice == 0 );

  printf("Student %d from class B starts asking questions for %d minutes\n", s_info->student_id, s_info->question_time);
  ask_questions(s_info->question_time);
  printf("Student %d from class B finishes asking questions and prepares to leave\n", s_info->student_id);

  /* leave office */
  classb_leave();        

  printf("Student %d from class B leaves the office\n", s_info->student_id);

  assert(students_in_office <= MAX_SEATS && students_in_office >= 0);
  assert(classb_inoffice >= 0 && classb_inoffice <= MAX_SEATS);
  assert(classa_inoffice >= 0 && classa_inoffice <= MAX_SEATS);

  pthread_exit(NULL);
}

/* Main function sets up simulation and prints report
 * at the end.
 * GUID: 355F4066-DA3E-4F74-9656-EF8097FBC985
 */
int main(int nargs, char **args) {
  int i;
  int result;
  int student_type;
  int num_students;
  void *status;
  pthread_t professor_tid;
  pthread_t student_tid[MAX_STUDENTS];
  student_info s_info[MAX_STUDENTS];

  if (nargs != 2) {
    printf("Usage: officehour <name of inputfile>\n");
    return EINVAL;
  }

  num_students = initialize(s_info, args[1]);
  if (num_students > MAX_STUDENTS || num_students <= 0) {
    printf("Error:  Bad number of student threads. "
           "Maybe there was a problem with your input file?\n");
    return 1;
  }

  printf("Starting officehour simulation with %d students ...\n",
    num_students);

  result = pthread_create(&professor_tid, NULL, professorthread, NULL);

  if (result) {
    printf("officehour:  pthread_create failed for professor: %s\n", strerror(result));
    exit(1);
  }

  for (i=0; i < num_students; i++) {

    s_info[i].student_id = i;
    sleep(s_info[i].arrival_time);
                
    student_type = random() % 2;

    if (s_info[i].class == CLASSA){
      result = pthread_create(&student_tid[i], NULL, classa_student, (void *)&s_info[i]);
    }
    else { // student_type == CLASSB
      result = pthread_create(&student_tid[i], NULL, classb_student, (void *)&s_info[i]);
    }

    if (result) {
      printf("officehour: thread_fork failed for student %d: %s\n", 
            i, strerror(result));
      exit(1);
    }
  }

  /* wait for all student threads to finish */
  for (i = 0; i < num_students; i++) {
    pthread_join(student_tid[i], &status);
  }

  /* tell the professor to finish. */
  pthread_cancel(professor_tid);

  printf("Office hour simulation done.\n");

  return 0;
}
