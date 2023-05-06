#ifndef HACKENROLMENT_H_
#define HACKENROLMENT_H_
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "IsraeliQueue.h"

#define FRIENDSHIP_BAR 20
#define RIVELRY_BAR 0
#define NUM_OF_FUNCTIONS 3
#define NUM_OF_COURSES_PARAMETERS 2
#define NUM_OF_STUDENT_INPUTS 7
#define CHUNK_SIZE 256
#define MAX_ID_SIZE 999999999
#define MIN_ID_SIZE 100000000
#define MAX_GPA 100
#define SEPERATOR ' '
#define ID 0
#define TOTAL_CREDITS 1
#define GPA 2
#define NAME 3
#define SURNAME 4
#define CITY 5
#define DEPARTMENT 6
#define END_OF_LINE '\n'
#define FIRST_CAPS_LETTER 'A'
#define LAST_CAPS_LETTER 'Z'
#define ADD_TO_CAPS_FOR_SMALL 32


typedef struct EnrollmentSystem * EnrollmentSystem;
typedef struct hackersList * hackersList;
typedef struct studentList * studentList;
typedef struct courseList * courseList;

/**Creates EnrollmentSystem with info of the students and the courses
 * .*/
EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers);

EnrollmentSystem readEnrollment(EnrollmentSystem sys, FILE* queues);

void hackEnrollment(EnrollmentSystem sys, FILE* out);
IsraeliQueueError ifLowerCaseNeeded(EnrollmentSystem sys, bool ifCaseSensitive);
void deleteEnrollmentSystem(EnrollmentSystem sys);
#endif