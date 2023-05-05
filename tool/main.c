//
// Created by user on 03/05/2023.
//
#include "HackEnrollment.h"

#define OPTION_1_FOR_INPUTS 5
#define OPTION_2_FOR_INPUTS 6

#define STUDENTS_INDEX 1
#define COURSES_INDEX 2
#define HACKERS_INDEX 3
#define QUEUES_INDEX 4
#define TARGET_INDEX 5
/*
 * argv[1] - flag (optional)
 * argv[2] - students
 * argv[3] - courses
 * argv[4] - hackers
 * argv[5] - queues
 * argv[6] - target
 */

int main(int argc, char** argv)
{
    if (argc != OPTION_1_FOR_INPUTS && argc != OPTION_2_FOR_INPUTS)
    {
        return 0;
    }
    int if_change_letters = 0;
    if(argv[1] == "-i")
    {
        if_change_letters = 1;
    }
    FILE* studentsFile = fopen(argv[STUDENTS_INDEX + if_change_letters], "r");
    if (!studentsFile)
    {
        return 0;
    }

    FILE* coursesFile = fopen(argv[COURSES_INDEX + if_change_letters], "r");
    if (!coursesFile)
    {
        fclose(studentsFile);
        return 0;
    }
    FILE* hackersFile = fopen(argv[HACKERS_INDEX + if_change_letters], "r");
    if (!hackersFile)
    {
        fclose(studentsFile);
        fclose(coursesFile);
        return 0;
    }

    FILE* queuesFile = fopen(argv[QUEUES_INDEX + if_change_letters], "r");
    if (!queuesFile)
    {
        fclose(studentsFile);
        fclose(coursesFile);
        fclose(hackersFile);
        return 0;
    }

    FILE* target = fopen(argv[TARGET_INDEX + if_change_letters], "w");
    if (!target)
    {
        fclose(studentsFile);
        fclose(coursesFile);
        fclose(hackersFile);
        fclose(queuesFile);
        return 0;
    }
    EnrollmentSystem sys = createEnrollment(studentsFile, coursesFile, hackersFile);
    EnrollmentSystem newSys = readEnrollment(sys, queuesFile);

    functionThatDoShit();

    hackEnrollment(newSys, target);

    fclose(studentsFile);
    fclose(coursesFile);
    fclose(hackersFile);
    fclose(queuesFile);
    fclose(target);
    return 1;
}