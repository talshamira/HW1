#include "HackEnrollment.h"

/* the courses is a regular linked list
 * and for every course there is an IsraeliQueue for the students :(
 */
#define NUM_OF_FUNCTIONS 3

typedef struct courseList {
    int m_id;
    int m_maxStudents;
    IsraeliQueue queueList;
    struct courseList* m_next;
} *courseList;

typedef struct studentList {
    int m_id; //9 digits
    int m_total_credit; //int >0
    int m_gpa; //between 0-100
    char* m_name;
    char* m_surname;
    char* m_city;
    char* m_department;
    struct studentList* m_next;
} *studentList;

typedef struct hackers {
    char* m_id;
    studentList m_friendList;
    studentList m_rivalList;
    courseList m_courseList;
};

typedef struct HackEnrollment {
    studentList m_studentList;
    courseList m_courseList;
    hackers m_hackers;
} *HackEnrollment;


EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers)
{
    HackEnrollment hackEnrollment = (HackEnrollment*) malloc(sizeof(*hackEnrollment));
    if(!hackEnrollment)
    {
        return NULL;
    }
    FriendshipFunction friendships[NUM_OF_FUNCTIONS] = {&nameDistance, &isInFriendList, &idDifference};
    //char buffer [CHUNK_SIZE];
    //while(fgets(buffer, CHUNK_SIZE, ))
    courseList runner = hackEnrollment->m_courseList;
    int gotToEOF = 0;
    do
    {
        gotToEOF = fscanf(courses, "%d %d", runner->m_id, runner->m_maxStudents);
        if(gotToEOF != NUM_OF_STUDENT_PARAMETERS)
        {
            free(hackEnrollment);
            return NULL;
        }
        runner = runner->m_next;
    } while(gotToEOF != EOF);
    IsraeliQueue listForCourse = IsraeliQueueCreate(friendships, compareFunction, FRIENDSHIP_BAR, RIVELY_BAR);
    if(!listForCourse)
    {
        free(hackEnrollment);
        return NULL;
    }

    hack_enrollmant->students_file = students;
    hack_enrollmant->courses_file = courses;
}


int nameDistance(char* name1, char* name2) // might need to get a set as in a hacker and a student or two students
{
    int ascii_value1 = 0;
    int ascii_value2 = 0;
    int i = 0;

    while (name1[i])
    {
        ascii_value1 = ascii_value1 + name1[i];
        i++;
    }

    i = 0;
    while (name2[i])
    {
        ascii_value2 = ascii_value2 + name2[i];
        i++;
    }

    return abs(ascii_value1 - ascii_value2);
}

int isInFriendList (hacker hacker, studentList student)
{

}

int idDifference(int Id1, int Id2)
{
    return abs(Id1 - Id2);
}

bool compareFunction (studentList student1, studentList student2)
{
    //TODO compareFunction
    if(value1 == value2)
    {
        return true;
    }

    return false;
}