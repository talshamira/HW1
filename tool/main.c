#include "HackEnrollment.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define OPTION_1_FOR_INPUTS 5
#define OPTION_2_FOR_INPUTS 6
#define LENGTH_OF_FLAG 2
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
void closeAllFiles(FILE* file1, FILE* file2, FILE* file3, FILE* file4, FILE* file5)
{
    fclose(file1);
    fclose(file2);
    fclose(file3);
    fclose(file4);
    fclose(file5);
}

int main(int argc, char** argv)
{
    if (argc != OPTION_1_FOR_INPUTS && argc != OPTION_2_FOR_INPUTS)
    {
        return 0;
    }
    int ifCaseSensitive = 0;
    if(strlen(argv[1]) == LENGTH_OF_FLAG)
    {
        if(argv[1][0] == '-' && argv[1][1] == 'i')
        {
            ifCaseSensitive = 1;
        }
    }
    FILE* studentsFile = fopen(argv[STUDENTS_INDEX + ifCaseSensitive], "r");
    if (!studentsFile)
    {
        return 0;
    }

    FILE* coursesFile = fopen(argv[COURSES_INDEX + ifCaseSensitive], "r");
    if (!coursesFile)
    {
        fclose(studentsFile);
        return 0;
    }
    FILE* hackersFile = fopen(argv[HACKERS_INDEX + ifCaseSensitive], "r");
    if (!hackersFile)
    {
        fclose(studentsFile);
        fclose(coursesFile);
        return 0;
    }

    FILE* queuesFile = fopen(argv[QUEUES_INDEX + ifCaseSensitive], "r");
    if (!queuesFile)
    {
        fclose(studentsFile);
        fclose(coursesFile);
        fclose(hackersFile);
        return 0;
    }

    FILE* target = fopen(argv[TARGET_INDEX + ifCaseSensitive], "w");
    if (!target)
    {
        fclose(studentsFile);
        fclose(coursesFile);
        fclose(hackersFile);
        fclose(queuesFile);
        return 0;
    }
;
    EnrollmentSystem sys = createEnrollment(studentsFile, coursesFile, hackersFile);
    if(!sys)
    {
        closeAllFiles(studentsFile, coursesFile, hackersFile, queuesFile, target);
        return 0;
    }

    EnrollmentSystem newSys = readEnrollment(sys, queuesFile);
    if(!newSys)
    {
        deleteEnrollmentSystem(sys);
        closeAllFiles(studentsFile, coursesFile, hackersFile, queuesFile, target);
        return 0;
    }

    if(ifLowerCaseNeeded(newSys, ifCaseSensitive) != ISRAELIQUEUE_SUCCESS)
    {
        deleteEnrollmentSystem(newSys);
        closeAllFiles(studentsFile, coursesFile, hackersFile, queuesFile, target);
        return 0;
    }

    hackEnrollment(newSys, target);

    closeAllFiles(studentsFile, coursesFile, hackersFile, queuesFile, target);
    return 1;
}

