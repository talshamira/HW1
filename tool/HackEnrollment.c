#include "HackEnrollment.h"

/* the courses is a regular linked list
 * and for every course there is an IsraeliQueue for the students :(
 */

typedef struct courseList {
    int m_id;
    int m_maxStudents;
    IsraeliQueue m_israeliQueue;
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
    list->m_isHacker = false;
    list->m_next = NULL;
    return list;
}
studentList cloneStudentList(studentList student)
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
    list->m_isHacker = student->m_isHacker;
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
    list->m_next = NULL;
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
        }while(input[length -2] != END_OF_LINE);
        
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
        hackerRunner->m_hacker->m_isHacker = true;
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

EnrollmentSystem cloneEnrollmentSystem(EnrollmentSystem sys)
{
    if(!sys)
    {
        return NULL;
    }
    EnrollmentSystem copy = (EnrollmentSystem*) malloc(sizeof(copy));
    if(!copy)
    {
        return NULL;
    }
    if(!studentCopy(sys->m_studentList, copy->m_studentList) || !courseCopy(sys->m_courseList, copy->m_courseList))
    {
        deleteEnrollmentSystem(copy);
        return NULL;
    }
    hackersList hackersRunnerOld = sys->m_hackers;
    hackersList hackersRunnerNew = copy->m_hackers;
    while(hackersRunnerOld)
    {
        hackersRunnerNew = cloneHackerList(hackersRunnerOld);
        if(!hackersRunnerNew)
        {
            deleteEnrollmentSystem(copy);
            return NULL;
        }
        hackersRunnerNew = hackersRunnerNew->m_next;
        hackersRunnerOld = hackersRunnerOld->m_next;
    }
}

bool courseCopy (courseList oldCourse, courseList newCourse)
{
    courseList courseRunnerOld = oldCourse;
    courseList courseRunnerNew = newCourse;
    while(courseRunnerOld)
    {
        courseRunnerNew = cloneCourseList(courseRunnerOld);
        if(!courseRunnerNew)
        {
            return false;
        }
        courseRunnerNew = courseRunnerNew->m_next;
        courseRunnerOld = courseRunnerOld->m_next;
    }
    return true;
}

bool studentCopy(studentList oldStudent, studentList newStudent)
{
    studentList studentRunnerOld = oldStudent;
    studentList studentRunnerNew = newStudent;
    while(studentRunnerOld)
    {
        studentRunnerNew = cloneStudentList(studentRunnerOld);
        if(!studentRunnerNew)
        {
            return false;
        }
        studentRunnerNew = studentRunnerNew->m_next;
        studentRunnerOld = studentRunnerOld->m_next;
    }
    return true;
}

hackersList cloneHackerList(hackersList hacker)
{
    hackersList list = malloc (sizeof(*list));
    if(!list)
    {
        return NULL;
    }
    list->m_id = hacker->m_id;
    if(!courseCopy(hacker->m_courseList, list->m_courseList) || !studentCopy(hacker->m_friendList, list->m_friendList) 
        || !studentCopy(hacker->m_rivalList, list->m_rivalList))
    {
        destroyHackerList(list);
        return NULL;
    }
    list->m_hacker = hacker->m_hacker;
    list->m_next = NULL;
    return list;
}

courseList cloneCourseList (courseList course)
{
    if(!course)
    {
        return NULL;
    }
    courseList list = malloc (sizeof(*list));
    if(!list)
    {
        return NULL;
    }
    list->m_id = course->m_id;
    list->m_maxStudents = course->m_maxStudents;
    list->m_next = course->m_next;
    list->m_israeliQueue = IsraeliQueueClone(course->m_israeliQueue);
    if(!list->m_israeliQueue)
    {
        destroyCourseList(list);
        return NULL;
    }
    return list;
}


EnrollmentSystem readEnrollment(EnrollmentSystem sys, FILE* queues)
{
    EnrollmentSystem sysCopy = cloneEnrollmentSystem(sys);
    if(!sysCopy)
    {
        return NULL;
    }
    char* input = NULL;
    FriendshipFunction functions[NUM_OF_FUNCTIONS]  = {&idDifference, &isInFriendList, NULL};
    do
    {
        input = getNextString(queues);
        if(!input)
        {
            deleteEnrollmentSystem(sysCopy);
            return NULL;
        }
        courseList course = findCourse(sysCopy->m_courseList, getNextInt(input));
        free(input);
        int length = 0;
        course->m_israeliQueue = IsraeliQueueCreate(&functions, &compareFunction, FRIENDSHIP_BAR, RIVELRY_BAR);
        if(!course->m_israeliQueue)
        {
            deleteEnrollmentSystem(sysCopy);
            return NULL;
        }
        do
        {
            input = getNextString(queues);
            if(!input)
            {
                deleteEnrollmentSystem(sysCopy);
                return NULL;
            }
            int id = getNextInt(input);
            studentList student = findStudent(sysCopy->m_studentList, id);
            if(IsraeliQueueEnqueue(course->m_israeliQueue, student) != ISRAELIQUEUE_SUCCESS)
            {
                deleteEnrollmentSystem(sysCopy);
                free(input);
                return NULL;
            }
        } while (input[length -2] != END_OF_LINE);
    } while (input != EOF);
    deleteEnrollmentSystem(sys);
    return sysCopy;   
}

courseList findCourse(courseList courses, int id)
{
    courseList runner = courses;
    while(runner && runner->m_id != id)
    {
        runner = runner->m_next;
    }
    if(!runner)
    {
        return NULL;
    }
    return runner;
}


void hackEnrollment(EnrollmentSystem sys, FILE* out)
{
    EnrollmentSystem copySys = cloneEnrollmentSystem(sys);
    if(!copySys)
    {
        return NULL;
    }
    hackersList hackerRunner = sys->m_hackers;
    while(hackerRunner)
    {
        courseList courseRunner = hackerRunner->m_courseList;
        
        while(courseRunner)
        {
            courseList courseWanted = findCourse(sys->m_courseList, courseRunner->m_id);
            if(!courseWanted)
            {
                EnrollmentSystem temp = sys;
                sys = copySys;
                deleteEnrollmentSystem(temp);
                return;
            }
            if(IsraeliQueueEnqueue(courseWanted->m_israeliQueue, hackerRunner->m_hacker) != ISRAELIQUEUE_SUCCESS)
            {
                EnrollmentSystem temp = sys;
                sys = copySys;
                deleteEnrollmentSystem(temp);
                return;
            }
            courseRunner = courseRunner->m_next;
        }
        hackerRunner = hackerRunner->m_next;
    }
    deleteEnrollmentSystem(copySys);
    EnrollmentSystem copySys = cloneEnrollmentSystem(sys);
    hackersList hackerRunner = sys->m_hackers;
    while(hackerRunner)
    {
        int numOfCoursesWanted = getListLength(hackerRunner->m_courseList);
        int numOfCoursesGot = 0;
        courseList courseRunner = hackerRunner->m_courseList;
        while(courseRunner)
        {
            int counter = 0;
            courseList courseWanted = findCourse(sys->m_courseList, courseRunner->m_id);
            studentList temp = IsraeliQueueDequeue(courseWanted->m_israeliQueue);
            while(!compareFunction(hackerRunner->m_hacker, temp) && IsraeliQueueSize(courseWanted->m_israeliQueue) > 0)
            {
                free(temp);
                temp = IsraeliQueueDequeue(courseWanted->m_israeliQueue);
                counter++;
            }
            if(!temp)
            {
                free(temp);
            }
            if(counter < courseWanted->m_maxStudents)
            {
                numOfCoursesGot++;
            }
        }
        if(numOfCoursesWanted ==1)
        {
            if(numOfCoursesGot < 1)
            {
                fputs("Cannot satisfy constraints for ", out);
                fputs(hackerRunner->m_id, out);
                deleteEnrollmentSystem(copySys);
                return;
            }
        }
        else if(numOfCoursesWanted > 1)
        {
            if(numOfCoursesGot < 2)
            {
                fputs("Cannot satisfy constraints for ", out);
                fputs(hackerRunner->m_id, out);
                deleteEnrollmentSystem(copySys);
                return;
            }
        }
    }
    printOutput(out,copySys);
    deleteEnrollmentSystem(copySys);
}

void printOutput(FILE* out, EnrollmentSystem sys)
{
    courseList courseRunner = sys->m_courseList;
    
    while(courseRunner)
    {
        int counter = 0;
        fputs(courseRunner->m_id,out);
        while(IsraeliQueueSize(courseRunner->m_israeliQueue) > 0 && counter < courseRunner->m_maxStudents)
        {
            studentList student = IsraeliQueueDequeue(courseRunner->m_israeliQueue);
            fputs(" ",out);
            fputs(student->m_id, out);
            free(student);
        }
        fputs("\n",out);
        courseRunner = courseRunner->m_next;
    }
}

char upperToLowercase(char ch)
{
    if(ch >= FIRST_CAPS_LETTER && ch <= LAST_CAPS_LETTER)
    {
        ch = ch + ADD_TO_CAPS_FOR_SMALL;
    }
    return ch;
}


int nameDistancei(hackersList hacker, studentList student)
{
    return stringDistancei(hacker->m_hacker->m_name[i], student->m_name[i]);
}
int nameDistancei(hackersList hacker, studentList student)
{
    return stringDistance(hacker->m_hacker->m_name[i], student->m_name[i]);
}


int stringDistance(char* name1, char* name2) //A != a
{
    int sum = 0;
    int i = 0;

    while (name1[i] || name2[i])
    {
        if(!name1[i])
        {
            sum = sum + upperToLowercase(name2[i]);
        }
        else if(!name2[i])
        {
            sum = sum + upperToLowercase(name1[i]);
        }
        else
        {
            sum = sum + abs(name1[i] - name2[i])
        }

        i++;
    }

    return sum;
}

int stringDistancei(hackersList hacker, studentList student) //A == a
{
    int sum = 0;
    int i = 0;

    while (hacker->m_hacker->m_name[i] || student->m_name[i])
    {
        if(!name1[i])
        {
            sum = sum + upperToLowercase(name2[i]);
        }
        else if(!name2[i])
        {
            sum = sum + upperToLowercase(name1[i]);
        }
        else
        {
            sum = sum + abs(upperToLowercase(name1[i]) - upperToLowercase(name2[i]))
        }

        i++;
    }

    return sum;
}

int isInFriendList (hackersList hacker, studentList student)
{
    if(!findStudent(hacker->m_friendList, student->m_id))
    {
        return false;
    }
    return true;
}

int idDifference(int Id1, int Id2)
{
    return abs(Id1 - Id2);
}

bool compareStudents (studentList student1, studentList student2)
{
    if(student1->m_id != student2->m_id)
    {
        return false;
    }
    if(student1->m_totalCredit != student2->m_totalCredit)
    {
        return false;
    }
    if(student1->m_gpa != student2->m_gpa)
    {
        return false;
    }
    if(strcmp(student1->m_name, student2->m_name) != 0)
    {
        return false;
    }
    if(strcmp(student1->m_surname, student2->m_surname) != 0)
    {
        return false;
    }
    if(strcmp(student1->m_city, student2->m_city) != 0) //TODO delete if needed
    {
        return false;
    }
    if(strcmp(student1->m_department, student2->m_department) != 0) //TODO delete if needed
    {
        return false;
    }

    return true;
}

void ifLowerCaseNeeded(EnrollmentSystem sys, bool ifCaseSensitive)
{
    courseList runner = sys->m_courseList;
    while(runner)
    {
        if(ifCaseSensitive)
        {
            IsraeliQueueError errorQueue= IsraeliQueueAddFriendshipMeasure(runner->m_israeliQueue, &nameDistancei);
        }
        else
        {
            IsraeliQueueError errorQueue = IsraeliQueueAddFriendshipMeasure(runner->m_israeliQueue, &nameDistance);
        }
        runner = runner->m_next;
    }
}