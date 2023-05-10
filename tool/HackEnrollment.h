#ifndef HACKENROLMENT_H_
#define HACKENROLMENT_H_
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "IsraeliQueue.h"

#define FRIENDSHIP_BAR 20
#define RIVELRY_BAR 0
#define NUM_OF_FUNCTIONS 4
#define NUM_OF_FUNCTIONS_WITHOUT_I 3
#define NUM_OF_COURSES_PARAMETERS 2
#define NUM_OF_STUDENT_INPUTS 7
#define CHUNK_SIZE 256
#define MAX_ID_SIZE_POSITIVE 999999999
#define MIN_ID_SIZE_POSITIVE 100000000
#define MAX_ID_SIZE_NEGATIVE -10000000
#define MIN_ID_SIZE_NEGATIVE -99999999
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

/**
 * Gets files with information about students, courses and hackers, 
 * Creates EnrollmentSystem with this information.
 * Returns NULL in case of a problem.
 **/
EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers);

/**
 * Gets enrollment system and file with queue 
 * Creates registration queues, adds to the EnrollmentSystem and returns it.
 * Returns NULL in case of a problem.
 **/
EnrollmentSystem readEnrollment(EnrollmentSystem sys, FILE* queues);

/**
 * Gets enrollment system and target file. 
 * Updates the registration queues with the hackers requests if posibble. 
 * If not, writes acordinglly to the target and ends the program.
 **/
void hackEnrollment(EnrollmentSystem sys, FILE* out);

/**
 * Updates if case sensitive
 **/
IsraeliQueueError ifLowerCaseNeeded(EnrollmentSystem sys, bool ifCaseSensitive);

/**
 * deletes the EnrollmentSystem it gets
 **/
void deleteEnrollmentSystem(EnrollmentSystem sys);
#endif