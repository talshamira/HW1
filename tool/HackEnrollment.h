#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "IsraeliQueue.h"

#define FRIENDSHIP_BAR 20
#define RIVELY_BAR 0

typedef struct EnrollmentSystem * EnrollmentSystem;

/**Creates EnrollmentSystem with info of the students and the courses
 * .*/
EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers);

EnrollmentSystem readEnrollment(EnrollmentSystem sys, FILE* queues);

void hackEnrollment(EnrollmentSystem sys, FILE* out);