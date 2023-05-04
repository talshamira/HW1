#include "HackEnrollment.h"

/* the courses is a regular linked list
 * and for every course there is an IsraeliQueue for the students :(
 */

typedef struct courseList {
    int m_id;
    int m_maxStudents;
    studentList m_queueList;
    struct courseList* m_next;
} *courseList;

typedef struct studentList {
    int m_id; //9 digits
    int m_totalCredit; //int >0
    int m_gpa; //between 0-100
    char* m_name;
    char* m_surname;
    char* m_city; // might delete
    char* m_department; 
    bool m_isHacker;
    struct studentList* m_next;
} *studentList;

typedef struct hackersList {
    int m_id;
    studentList m_hacker;
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
studentList createStudentList(studentList student)
{
    if(!student)
    {
        return NULL;
    }
    studentList list = malloc (sizeof(*list));
    if(!list)
    {
        return NULL;
    }
    list->m_id = student->m_id;
    list->m_totalCredit = student->m_totalCredit;
    list->m_gpa = student->m_gpa;
    list->m_name = student->m_name;
    list->m_surname = student->m_surname;
    list->m_city = student->m_city;
    list->m_department = student->m_department;
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
    list->m_hacker = findStudent(students, id);
    if(!list->m_hacker)
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
        if(gotToEOF != NUM_OF_COURSES_PARAMETERS)
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
    
    char* inputs[NUM_OF_STUDENT_INPUTS] ={NULL} ;
    do
    {
        for(int i = 0; i < NUM_OF_STUDENT_INPUTS; i++)
        {
            inputs[i] = getNextString(students);
            if(!inputs[i])
            {
                for(int j = 0; j < i; j++)
                {
                    free(inputs[j]);
                }
                deleteEnrollmentSystem(hackEnrollment);
                return NULL;
            }
        }
        studentRunner = createStudentList(getNextInt(inputs[ID]), getNextInt(inputs[TOTAL_CREDITS]), 
            getNextInt(inputs[GPA]), inputs[NAME], inputs[SURNAME], inputs[CITY], inputs[DEPARTMENT]);  
        if(!studentRunner)
        {
            for(int i = 0; i < NUM_OF_STUDENT_INPUTS; i++)
            {
                free(inputs[i]);
            }
            deleteEnrollmentSystem(hackEnrollment);
            return NULL;
        }
        studentRunner = studentRunner->m_next;
    }
    while (inputs[0] != EOF);
    
    char* input = NULL;
    hackersList hackerRunner = hackEnrollment->m_hackers;
    courseList coursesListRunner = NULL;
    studentList friendsRuner = NULL;
    studentList rivalsRunner = NULL;
    courseList coursesList = coursesListRunner;
    studentList friends = friendsRuner;
    studentList rivals = rivalsRunner;
    do
    {
        input = getNextString(hackers);
        if(!input)
        {
            deleteEnrollmentSystem(hackEnrollment);
            return NULL;
        }
        int id = getNextInt(input), length = 0;
        do
        {
            free(input);
            input = getNextString(hackers);
            if(!input)
            {
                deleteEnrollmentSystem(hackEnrollment);
                return NULL;
            }
            coursesListRunner = createCourseList(getNextInt(input), 0); 
            if(!coursesList)
            {
                free(input);
                deleteEnrollmentSystem(hackEnrollment);
                return NULL;
            }
            coursesListRunner = coursesListRunner->m_next;
            length = strlen(input);
        }while(input[length -2] != '\n');
        
        do
        {
            free(input);
            input = getNextString(hackers);
            if(!input)
            {
                deleteEnrollmentSystem(hackEnrollment);
                return NULL;
            }
            studentList temp = findStudent(hackEnrollment->m_studentList, getNextInt(input));
            friendsRuner = cloneStudentList(temp);
            if(!friendsRuner)
            {
                free(input);
                destroyCourseList(coursesList);
                destroyStudentList(friends);
                deleteEnrollmentSystem(hackEnrollment);
                return NULL;
            }
            friendsRuner = friendsRuner->m_next;
            length = strlen(input);
        }while(input[length -2] != '\n');
        friendsRuner->m_next = NULL;
        do
        {
            free(input);
            input = getNextString(hackers);
            if(!input)
            {
                deleteEnrollmentSystem(hackEnrollment);
                return NULL;
            }
            studentList temp = findStudent(hackEnrollment->m_studentList, getNextInt(input));
            rivalsRunner = cloneStudentList(temp);
            if(!rivalsRunner)
            {
                free(input);
                destroyCourseList(coursesList);
                destroyStudentList(friends);
                destroyStudentList(rivals);
                deleteEnrollmentSystem(hackEnrollment);
                return NULL;
            }
            rivalsRunner = rivalsRunner->m_next;
            length = strlen(input);
        }while(input[length -2] != END_OF_LINE);
        rivalsRunner->m_next = NULL;
        free(input);
        hackerRunner = createHackersList(id, coursesList, friends, rivals, hackEnrollment->m_studentList);
        if(!hackerRunner)
        {
            destroyCourseList(coursesList);
            destroyStudentList(friends);
            destroyStudentList(rivals);
            deleteEnrollmentSystem(hackEnrollment);
            return NULL;
        }
        hackerRunner = hackerRunner->m_next;
    }while (input != EOF);
}

 int getNextInt(char* line)
{
    int id = 0;
    while(line != SEPERATOR && line != END_OF_LINE)
    {
        id *=  10;
        int temp = (int) line;
        id += temp;
        line++;
    }
    return id;
}

char* getNextString(FILE* file)
{
    char buffer [CHUNK_SIZE];
    char* input = NULL;
    int inputLength = 0, tempLength = 0;
    do
    {
        if(!fgets(buffer, CHUNK_SIZE, file))
        {
            return NULL;
        }
        tempLength = strLength(buffer);
        input = (char *) malloc(sizeof(input)*tempLength+1);
        strcpy(input+inputLength, buffer);
        inputLength += tempLength;
    }while(tempLength == CHUNK_SIZE-1 && (buffer[CHUNK_SIZE-2] != SEPERATOR && buffer[CHUNK_SIZE -2] != END_OF_LINE));
    return input;
}

EnrollmentSystem readEnrollment(EnrollmentSystem sys, FILE* queues)
{
    char* input = NULL;
    do
    {
        input = getNextString(queues);
        if(!input)
        {
            return NULL;
        }
        courseList course = findCourse(sys->m_courseList,   getNextInt(input));
        free(input);
        int length = 0;
        studentList runner = course->m_queueList;
        do
        {
            input = getNextString(queues);
            if(!input)
            {
                return NULL;
            }
            int id = getNextInt(input);
            studentList student = findStudent(sys->m_studentList, id);
            runner = cloneStudentList(student);
            if(!runner)
            {
                free(input);
                destroyStudentList(course->m_queueList);
                return NULL;
            }
            runner = runner->m_next;
        } while (input[length -2] != END_OF_LINE);
        runner->m_next = NULL;
        
    } while (input != EOF);   
}

void changeEnrollmentSystem (EnrollmentSystem sys, bool flagI)
{
    if(flagI)
    {
        FriendshipFunction functions[NUM_OF_FUNCTIONS] = {&nameDistance, &isInFriendList, &idDifference};
    }

    IsraeliQueue tempQueue = IsraeliQueueCreate()
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