#include "HackEnrollment.h"

/* the courses is a regular linked list
 * and for every course there is an IsraeliQueue for the students :(
 */
#define NUM_OF_FUNCTIONS 3
#define NUM_OF_STUDENT_PARAMETERS 2
#define CHUNK_SIZE 256
#define MAX_ID_SIZE 999999999
#define MIN_ID_SIZE 100000000
#define MAX_GPA 100
#define SEPERATOR ' '

typedef struct courseList {
    int m_id;
    int m_maxStudents;
    IsraeliQueue queueList;
    struct courseList* m_next;
} *courseList;

typedef struct studentList {
    int m_id; //9 digits
    int m_totalCredit; //int >0
    int m_gpa; //between 0-100
    char* m_name;
    char* m_surname;
    char* m_city;
    char* m_department;
    struct studentList* m_next;
} *studentList;

typedef struct hackersList {
    int m_id;
    studentList hacker;
    studentList m_friendList;
    studentList m_rivalList;
    courseList m_courseList;
    struct hackersList* m_next;
}*hackersList;

typedef struct EnrollmentSystem {
    studentList m_studentList;
    courseList m_courseList;
    hackersList m_hackers;
} *EnrollmentSystem;


courseList createCourseList(int id, int maxStudents)
{
    courseList list = malloc (sizeof(*list));
    if(!list)
    {
        return NULL;
    }
    list->m_id = id;
    list->m_maxStudents = maxStudents;
    list->m_next = NULL;
    return list;
}

void destroyCourseList(courseList listHead)
{
    while(listHead)
    {
        courseList toDelete = listHead;
        listHead = listHead->m_next;
        free(toDelete);
    }
}

studentList createStudentList(int id, int totalCredits, int gpa, char* name,char* surname, char* city, char* department)
{
    studentList list = malloc (sizeof(*list));
    if(!list)
    {
        return NULL;
    }
    list->m_id = id;
    list->m_totalCredit = totalCredits;
    list->m_gpa = gpa;
    list->m_name = name;
    list->m_surname = surname;
    list->m_city = city;
    list->m_department = department;
    list->m_next = NULL;
    return list;
}

void destroyStudentList(studentList listHead)
{
    while(listHead)
    {
        studentList toDelete = listHead;
        listHead = listHead->m_next;
        free(toDelete);
    }
}

hackersList createHackersList(int id, courseList courses, studentList friends, studentList rivals, studentList students)
{
    hackersList list = malloc (sizeof(*list));
    if(!list)
    {
        return NULL;
    }
    list->m_id = id;
    list->m_courseList = courses;
    list->m_friendList = friends;
    list->hacker = findStudent(students, id);
    if(!list->hacker)
    {
        free(list);
        return NULL;
    }
    list->m_rivalList = rivals;
    return list;
}

void destroyHackerList(hackersList listHead)
{
    while(listHead)
    {
        hackersList toDelete = listHead;
        listHead = listHead->m_next;
        free(toDelete);
    }
}

studentList findStudent(studentList students, int id)
{
    studentList runner = students;
    while(runner && runner->m_id != id)
    {
        runner = runner->m_next;
    }
    return runner;
}


EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers)
{
    EnrollmentSystem hackEnrollment = (EnrollmentSystem*) malloc(sizeof(*hackEnrollment));
    if(!hackEnrollment)
    {
        return NULL;
    }
    FriendshipFunction friendships[NUM_OF_FUNCTIONS] = {&nameDistance, &isInFriendList, &idDifference};
    studentList studentRunner = hackEnrollment->m_studentList;
    courseList courseRunner = hackEnrollment->m_courseList;
    int gotToEOF = 0;
    do
    {
        int id = 0, maxStudents = 0;
        gotToEOF = fscanf(courses, "%d %d", &id, &maxStudents);
        if(gotToEOF != NUM_OF_STUDENT_PARAMETERS)
        {
            deleteEnrollmentSystem(hackEnrollment);
            return NULL;
        }
        courseRunner = createCourseList(id, maxStudents);
        if(!courseRunner)
        {
            deleteEnrollmentSystem(hackEnrollment);
            return NULL;
        }
        courseRunner = courseRunner->m_next;
    } while(gotToEOF != EOF);
    bool flag = true;
    while (flag)
    {
        char * line = getNextLine(students);
        if(!line)
        {
            return NULL;
        }
        if(line == EOF)
        {
            flag = false;
            free(line);
            break;
        }
        int id  = getNextInt(line), totalCredits = getNextInt(line), gpa = getNextInt(line);
        if(id > MAX_ID_SIZE || id < MIN_ID_SIZE || totalCredits < 0 || gpa < 0 || gpa > MAX_GPA)
        {
            deleteEnrollmentSystem(hackEnrollment);
            return NULL;
        }
        char* name = getNextString(line), surname = getNextString(line), city = getNextString(line);
        char* department = getNextString(line);
        if(!name)
        {
            deleteEnrollmentSystem(hackEnrollment);
            return NULL;
        }
        if(!surname)
        {
            free(name);
            deleteEnrollmentSystem(hackEnrollment);
            return NULL;
        }
        if(!city)
        {
            free(name);
            free(surname);
            deleteEnrollmentSystem(hackEnrollment);
            return NULL;
        }
        if(!department)
        {
            free(name);
            free(surname);
            free(city);
            deleteEnrollmentSystem(hackEnrollment);
            return NULL;
        }
        studentRunner = createStudentList(id, totalCredits, gpa,name, surname, city, department);
        if(!studentRunner)
        {
            free(name);
            free(surname);
            free(city);
            free(department);
            deleteEnrollmentSystem(hackEnrollment);
            return NULL;
        }
        studentRunner = studentRunner->m_next;
        free(line);
    } 
    
}

 int getNextInt(char* line)
{
    int id = 0;
    while(line != SEPERATOR)
    {
        id *=  10;
        int temp = (int) line;
        id += temp;
        line++;
    }
    return id;
}

char* getNextString (char* line)
{
    int counter  = 0;
    while(line[counter] != SEPERATOR)
    {
        counter++;
    }
    char* nextString = (char*) malloc(sizeof(nextString)*counter +1);
    if(!nextString)
    {
        return NULL;
    }
    counter = 0;
    while(line != SEPERATOR)
    {
        nextString[counter] = line;
        line++;
        counter++;
    }
    nextString[counter+1] = '\0';
    return nextString;
}

char* getNextLine(FILE* file)
{
    char buffer [CHUNK_SIZE];
    char* input = NULL;
    int inputLength = 0, tempLength = 0;
    do
    {
        if(!fgets(buffer, CHUNK_SIZE, file))
        {
            enrollmentSystemDestroy(hackEnrollment);
            return NULL;
        }
        tempLength = strLength(buffer);
        input = (char *) malloc(sizeof(input)*tempLength+1);
        strcpy(input+inputLength, buffer);
        inputLength += tempLength;
    }while(tempLength == CHUNK_SIZE-1 && (buffer[CHUNK_SIZE-2] != '\n'));
    return input;
}

/*
    IsraeliQueue listForCourse = IsraeliQueueCreate(friendships, compareFunction, FRIENDSHIP_BAR, RIVELY_BAR);
    if(!listForCourse)
    {
        free(hackEnrollment);
        return NULL;
    }
    */


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

int isInFriendList (hackers hacker, studentList student)
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