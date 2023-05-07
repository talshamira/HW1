#include "HackEnrollment.h"

/* the courses is a regular linked list
 * and for every course there is an IsraeliQueue for the students :(
 */

struct courseList {
    int m_id;
    int m_maxStudents;
    IsraeliQueue m_israeliQueue;
    struct courseList* m_next;
};

struct studentList {
    int m_id; //9 digits
    int m_totalCredit; //int >0
    int m_gpa; //between 0-100
    char* m_name;
    char* m_surname;
    char* m_city; // might delete
    char* m_department; 
    hackersList m_isHacker;
    struct studentList* m_next;
};

struct hackersList {
    int m_id;
    studentList m_hacker;
    studentList m_friendList;
    studentList m_rivalList;
    courseList m_courseList;
    struct hackersList* m_next;
};

struct EnrollmentSystem {
    studentList m_studentList;
    courseList m_courseList;
    hackersList m_hackers;
};




studentList findStudent(studentList students, int id);
char* getNextLine(FILE* file);
hackersList cloneHackerList(hackersList hacker);
courseList cloneCourseList (courseList course);
int idDifference(void* student1, void* student2);
int isInFriendList (void* hacker, void* student); 
courseList findCourse(courseList courses, int id);
int compareStudents (void* student1, void* student2);
void printOutput(FILE* out, EnrollmentSystem sys);
int stringDistancei(char* name1, char* name2);
int stringDistance(char* name1, char* name2); 
int nameDistancei(void* hacker, void* student);
int nameDistance(void* hacker, void* student);
int getListLength(courseList course);
void deleteEnrollmentSystem(EnrollmentSystem sys);
char* getNextString (char* line);
int getRidOfSpace (char* input);
void printEnrollmentSystem(EnrollmentSystem sys);

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
    list->m_isHacker = NULL;
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

void deleteEnrollmentSystem(EnrollmentSystem sys)
{
    if(!sys)
    {
        return;
    }
    destroyCourseList(sys->m_courseList);
    destroyHackerList(sys->m_hackers);
    destroyStudentList(sys->m_studentList);
    free(sys);
}
EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers)
{
    EnrollmentSystem hackEnrollment = (EnrollmentSystem) malloc(sizeof(hackEnrollment));
    if(!hackEnrollment)
    {
        return NULL;
    }
    courseList firstCourse = NULL;
    int gotToEOF = 0;
    do
    {
        int id = 0, maxStudents = 0;
        gotToEOF = fscanf(courses, "%d %d", &id, &maxStudents);
        if(gotToEOF != NUM_OF_COURSES_PARAMETERS && gotToEOF != EOF)
        {
            deleteEnrollmentSystem(hackEnrollment);
            return NULL;
        }
        if(id != 0 && maxStudents != 0)
        {
            if(!firstCourse)
            {
                hackEnrollment->m_courseList = createCourseList(id, maxStudents);
                if(!hackEnrollment->m_courseList)
                {
                    deleteEnrollmentSystem(hackEnrollment);
                    return NULL;
                }
                firstCourse = hackEnrollment->m_courseList;
            }
            else
            {
                hackEnrollment->m_courseList->m_next = createCourseList(id, maxStudents);
                if(!hackEnrollment->m_courseList->m_next)
                {
                    deleteEnrollmentSystem(hackEnrollment);
                    return NULL;
                }
                hackEnrollment->m_courseList = hackEnrollment->m_courseList->m_next;
            } 
        }
    } while(gotToEOF != EOF);
    hackEnrollment->m_courseList = firstCourse;
    
    
    studentList firstStudent = NULL;
    char* inputs[NUM_OF_STUDENT_INPUTS] ={NULL} ;
    char* line = getNextLine(students);
    char* begginingOfLine = line;
    while(line)
    {
        /*if(!line)
        {
            deleteEnrollmentSystem(hackEnrollment);
            return NULL;
        }*/
        for(int i = 0; i < NUM_OF_STUDENT_INPUTS; i++)
        {
            inputs[i] = getNextString(line);
            if(!inputs[i])
            {
                for(int j = 0; j < i; j++)
                {
                    free(inputs[j]);
                }
                free(begginingOfLine);
                deleteEnrollmentSystem(hackEnrollment);
                return NULL;
            }
        }
        if(!firstStudent)
        {
            hackEnrollment->m_studentList = createStudentList(atoi(inputs[ID]), atoi(inputs[TOTAL_CREDITS]), 
            atoi(inputs[GPA]), inputs[NAME], inputs[SURNAME], inputs[CITY], inputs[DEPARTMENT]); 
            if(!hackEnrollment->m_studentList)
            {
                for(int i = 0; i < NUM_OF_STUDENT_INPUTS; i++)
                {
                    free(inputs[i]);
                }
                deleteEnrollmentSystem(hackEnrollment);
                free(begginingOfLine);
                return NULL;
            }
            firstStudent = hackEnrollment->m_studentList;
        }
        else
        {
            hackEnrollment->m_studentList->m_next = createStudentList(atoi(inputs[ID]), 
            atoi(inputs[TOTAL_CREDITS]), atoi(inputs[GPA]), inputs[NAME], inputs[SURNAME], 
            inputs[CITY], inputs[DEPARTMENT]); 
            if(!hackEnrollment->m_studentList->m_next)
            {
                for(int i = 0; i < NUM_OF_STUDENT_INPUTS; i++)
                {
                    free(inputs[i]);
                }
                deleteEnrollmentSystem(hackEnrollment);
                free(begginingOfLine);
                return NULL;
            }
            hackEnrollment->m_studentList = hackEnrollment->m_studentList->m_next; 
        }
        free(begginingOfLine);
        line = getNextLine(students);
        begginingOfLine = line;
    }
    hackEnrollment->m_studentList = firstStudent;
    
    
    hackersList firstHacker = NULL;
    courseList hackersFirstCourses = NULL;
    courseList hackersCourseRunner = NULL;
    studentList hackerFriendRunner = NULL;
    studentList hackerRivalsRunner = NULL;
    studentList firstFriends = NULL;
    studentList firstRivals = NULL;
    line = getNextLine(hackers);
    begginingOfLine = line;
    while(*line != END_OF_LINE && *line != EOF)
    {  
        char* tempId = getNextString(line);
        if(!tempId)
        {
            deleteEnrollmentSystem(hackEnrollment);
            free(begginingOfLine);
            return NULL;
        } 
        int id = atoi(tempId);
        free(tempId);
        free(begginingOfLine);
        line = getNextLine(hackers);
        begginingOfLine = line;
        /*if(!line)
        {
            deleteEnrollmentSystem(hackEnrollment);
            return NULL;
        }*/
        while(*line != END_OF_LINE && *line != EOF)
        {
            char* tempCourse = getNextString(line);
            if(!tempCourse)
            {
                deleteEnrollmentSystem(hackEnrollment);
                free(begginingOfLine);
                return NULL;
            }
            line = line + strlen(tempCourse) +getRidOfSpace(line);
            int courseNumber = atoi(tempCourse);
            free(tempCourse);
            courseList wantedCourse = findCourse(hackEnrollment->m_courseList, courseNumber);
            if(!tempCourse)
            {
                deleteEnrollmentSystem(hackEnrollment);
                free(begginingOfLine);
                return NULL;
            }
            if(!hackersFirstCourses)
            {
                hackersCourseRunner = cloneCourseList(wantedCourse);
                if(!hackersCourseRunner)
                {
                    deleteEnrollmentSystem(hackEnrollment);
                    free(begginingOfLine);
                    return NULL;
                }
                hackersFirstCourses = hackersCourseRunner;
            }
            else
            {
                hackersCourseRunner->m_next = cloneCourseList(wantedCourse);
                if(!hackersCourseRunner->m_next)
                {
                    deleteEnrollmentSystem(hackEnrollment);
                    free(begginingOfLine);
                    return NULL;
                }
                hackersCourseRunner = hackersCourseRunner->m_next;
            }
        }
        free(begginingOfLine);
        line = getNextLine(hackers);
        begginingOfLine = line;
        /*if(!line)
        {
            deleteEnrollmentSystem(hackEnrollment);
            return NULL;
        }*/
        while(*line != END_OF_LINE && *line != EOF)
        {
          char* tempStudent = getNextString(line);
            if(!tempStudent)
            {
                deleteEnrollmentSystem(hackEnrollment);
                free(begginingOfLine);
                return NULL;
            } 
            line = line + strlen(tempStudent) +getRidOfSpace(line);
            int studentId = atoi(tempStudent);
            free(tempStudent);
            studentList friend = findStudent(hackEnrollment->m_studentList, studentId);
            if(!friend)
            {
                deleteEnrollmentSystem(hackEnrollment);
                free(begginingOfLine);
                return NULL;
            }
            if(!firstFriends)
            {
                hackerFriendRunner = cloneStudentList(friend);
                if(!hackerFriendRunner)
                {
                    deleteEnrollmentSystem(hackEnrollment);
                    free(begginingOfLine);
                    return NULL;
                }
                firstFriends = hackerFriendRunner;
            }
            else
            {
                hackerFriendRunner->m_next = cloneStudentList(friend);
                if(!hackerFriendRunner->m_next)
                {
                    deleteEnrollmentSystem(hackEnrollment);
                    free(begginingOfLine);
                    return NULL;
                }
                hackerFriendRunner = hackerFriendRunner->m_next;
            }
        }

        free(begginingOfLine);
        line = getNextLine(hackers);
        begginingOfLine = line;
        /*
        if(!line)
        {
            deleteEnrollmentSystem(hackEnrollment);
            return NULL;
        }*/
        while(*line != END_OF_LINE && *line != EOF)
        {
          char* tempStudent = getNextString(line);
            if(!tempStudent)
            {
                deleteEnrollmentSystem(hackEnrollment);
                free(line);
                return NULL;
            }
            line = line + strlen(tempStudent)+getRidOfSpace(line);
            int studentId = atoi(tempStudent);
            free(tempStudent);
            studentList rival = findStudent(hackEnrollment->m_studentList, studentId);
            if(!rival)
            {
                deleteEnrollmentSystem(hackEnrollment);
                free(begginingOfLine);
                return NULL;
            }
            if(!firstRivals)
            {
                hackerRivalsRunner = cloneStudentList(rival);
                if(!hackerRivalsRunner)
                {
                    deleteEnrollmentSystem(hackEnrollment);
                    free(begginingOfLine);
                    return NULL;
                }
                firstRivals = hackerRivalsRunner;
            }
            else
            {
                hackerRivalsRunner->m_next = cloneStudentList(rival);
                if(!hackerRivalsRunner->m_next)
                {
                    deleteEnrollmentSystem(hackEnrollment);
                    free(begginingOfLine);
                    return NULL;
                }
                hackerRivalsRunner = hackerRivalsRunner->m_next;
            }
        }
        if(!firstHacker)
        {
            hackEnrollment->m_hackers = createHackersList(id, firstCourse, firstFriends, firstRivals, 
                                                                                    hackEnrollment->m_studentList);
            if(!hackEnrollment->m_hackers)
            {
                deleteEnrollmentSystem(hackEnrollment);
                free(begginingOfLine);
                return NULL;
            }
            firstHacker = hackEnrollment->m_hackers;
        }
        else
        {
            hackEnrollment->m_hackers->m_next = createHackersList(id, firstCourse, firstFriends, firstRivals, 
                                                                                hackEnrollment->m_studentList);
            if(!hackEnrollment->m_hackers->m_next)
            {
                deleteEnrollmentSystem(hackEnrollment);
                free(begginingOfLine);
                return NULL;
            }
            hackEnrollment->m_hackers = hackEnrollment->m_hackers->m_next;
        }
    }
    hackEnrollment->m_hackers = firstHacker;
    printf("printing system after create enrollment\n");
    printEnrollmentSystem(hackEnrollment);
    return hackEnrollment;
}

char* getNextString (char* line)
{
    if(!line)
    {
        return NULL;
    }
    char* runner = line;
    int counter = 0;
    while(runner[counter] != SEPERATOR && runner[counter] != END_OF_LINE)
    {
        counter++;
    }
    char* string = (char *) malloc(sizeof(string)*counter +1);
    if(!string)
    {
        return NULL;
    }
    for(int i = 0; i < counter; i++)
    {
        string[i] = *line;
        line = line+1;
    }
    string[counter] = '\0';
    return string;
}

/*int getNextInt(char* line)
{
    int id = 0, isNegative =1;
    if(line[0] == '-')
    {
        isNegative = -1;
        line++;
    }
    while(*line != '\0')
    {
        id = id*10;
        int temp = (int) *line;
        id = id+temp;
        line++;
    }
    return id*isNegative;
}*/

char* getNextLine(FILE* file)
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
        tempLength = strlen(buffer);
        input = (char *) malloc(sizeof(input)*tempLength+1);
        strcpy(input+inputLength, buffer);
        inputLength += tempLength;
    }while(tempLength == CHUNK_SIZE-1 &&  buffer[CHUNK_SIZE -2] != END_OF_LINE);
    return input;
}

EnrollmentSystem cloneEnrollmentSystem(EnrollmentSystem sys)
{
    if(!sys)
    {
        return NULL;
    }
    EnrollmentSystem copy = (EnrollmentSystem) malloc(sizeof(copy));
    if(!copy)
    {
        return NULL;
    }
    studentList firstStudent = NULL, studentRunner = sys->m_studentList;
    while(studentRunner)
    {
        if(!firstStudent)
        {
            copy->m_studentList = cloneStudentList(studentRunner);
            if(!copy->m_studentList)
            {
                deleteEnrollmentSystem(copy);
                return NULL;
            }
            firstStudent = copy->m_studentList;
        }
        else
        {
            copy->m_studentList->m_next = cloneStudentList(studentRunner);
            if(!copy->m_studentList->m_next)
            {
                deleteEnrollmentSystem(copy);
                return NULL;
            }
            copy->m_studentList = copy->m_studentList->m_next;
        }
        studentRunner = studentRunner->m_next;
    }
    copy->m_studentList = firstStudent;
    courseList firstCourse = NULL, courseRunner = sys->m_courseList;
    while(courseRunner)
    {
        if(!firstCourse)
        {
            copy->m_courseList = cloneCourseList(courseRunner);
            if(!copy->m_courseList)
            {
                deleteEnrollmentSystem(copy);
                return NULL;
            }
            firstCourse = copy->m_courseList;
        }
        else
        {
            copy->m_courseList->m_next = cloneCourseList(courseRunner);
            if(!copy->m_courseList->m_next)
            {
                deleteEnrollmentSystem(copy);
                return NULL;
            }
            copy->m_courseList = copy->m_courseList->m_next;
        }
        courseRunner = courseRunner->m_next;
    }
    copy->m_courseList = firstCourse;
    hackersList hackersRunnerOld = sys->m_hackers;
    hackersList firstHacker = NULL;
    while(hackersRunnerOld)
    {
        if(!firstHacker)
        {
            copy->m_hackers = cloneHackerList(hackersRunnerOld);
            if(!copy->m_hackers)
            {
                deleteEnrollmentSystem(copy);
                return NULL;
            }
            firstHacker = copy->m_hackers;
        }
        else
        {
            copy->m_hackers->m_next = cloneHackerList(hackersRunnerOld);
            if(!copy->m_hackers->m_next)
            {
                deleteEnrollmentSystem(copy);
                return NULL;
            }
            copy->m_hackers = copy->m_hackers->m_next;
        }
        hackersRunnerOld = hackersRunnerOld->m_next;
    }
    copy->m_hackers = firstHacker;
    printf("printing system after clone enrollment\n");
    printEnrollmentSystem(copy);
    return copy;
}

hackersList cloneHackerList(hackersList hacker)
{
    hackersList list = malloc (sizeof(*list));
    if(!list)
    {
        return NULL;
    }
    list->m_id = hacker->m_id;

    studentList friendRunnerOld = hacker->m_friendList;
    studentList firstFriend = NULL;
    while(friendRunnerOld)
    {
        if(!firstFriend)
        {
            list->m_friendList = cloneStudentList(friendRunnerOld);
            if(!list->m_friendList)
            {
                destroyHackerList(list);
                return NULL;
            }
            firstFriend = list->m_friendList;
        }
        else
        {
            list->m_friendList->m_next = cloneStudentList(friendRunnerOld);
            if(!list->m_friendList->m_next)
            {
                destroyHackerList(list);
                return NULL;
            }
            list->m_friendList = list->m_friendList->m_next;
        }
        friendRunnerOld = friendRunnerOld->m_next;
    }
    list->m_friendList = firstFriend;
    
    studentList rivalRunnerOld = hacker->m_rivalList;
    studentList firstRival = NULL;
    while(rivalRunnerOld)
    {
        if(!firstRival)
        {
            list->m_rivalList = cloneStudentList(rivalRunnerOld);
            if(!list->m_rivalList)
            {
                destroyHackerList(list);
                return NULL;
            }
            firstRival = list->m_rivalList;
        }
        else
        {
            list->m_rivalList->m_next = cloneStudentList(rivalRunnerOld);
            if(!list->m_rivalList->m_next)
            {
                destroyHackerList(list);
                return NULL;
            }
            list->m_rivalList = list->m_rivalList->m_next;
        }
        rivalRunnerOld = rivalRunnerOld->m_next;
    }
    list->m_rivalList = firstRival;

    courseList courseRunnerOld = hacker->m_courseList;
    courseList firstCourse = NULL;
    while(courseRunnerOld)
    {
        if(!firstCourse)
        {
            list->m_courseList = cloneCourseList(courseRunnerOld);
            if(!list->m_courseList)
            {
                destroyHackerList(list);
                return NULL;
            }
            firstCourse = list->m_courseList;
        }
        else
        {
            list->m_courseList->m_next = cloneCourseList(courseRunnerOld);
            if(!list->m_courseList->m_next)
            {
                destroyHackerList(list);
                return NULL;
            }
            list->m_courseList = list->m_courseList->m_next;
        }
        courseRunnerOld = courseRunnerOld->m_next;
    }
    list->m_courseList = firstCourse;
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
    courseList list = createCourseList(course->m_id, course->m_maxStudents);
    if(!list)
    {
        return NULL;
    }
    list->m_next = course->m_next;
    if(!course->m_israeliQueue)
    {
        list->m_israeliQueue = NULL;
    }
    else
    {
        printf("trying to clone israeli queue for course %d\n", list->m_id);
        list->m_israeliQueue = IsraeliQueueClone(course->m_israeliQueue);
        if(!list->m_israeliQueue)
        {
            destroyCourseList(list);
            return NULL;
        }
        printf("succeded in clonning\n");
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
    char* input = getNextLine(queues);
    FriendshipFunction functions[NUM_OF_FUNCTIONS]  = {&idDifference, &isInFriendList, NULL};
    char* begginingOfLine = input;
    while(input != NULL && *input != END_OF_LINE && *input != EOF)
    {
        /*
        if(!input)
        {
            deleteEnrollmentSystem(sysCopy);
            return NULL;
        }*/
        char* tempCourse = getNextString(input);
        if(!tempCourse)
        {
            deleteEnrollmentSystem(sysCopy);
            free(begginingOfLine);
            return NULL;
        }
        input = input + strlen(tempCourse)+getRidOfSpace(input);
        
        courseList course = findCourse(sysCopy->m_courseList, atoi(tempCourse));
        if(!course)
        {
            deleteEnrollmentSystem(sysCopy);
            free(tempCourse);
            free(begginingOfLine);
            return NULL;
        }
        course->m_israeliQueue = IsraeliQueueCreate(functions, &compareStudents, FRIENDSHIP_BAR, RIVELRY_BAR);
        if(!course->m_israeliQueue)
        {
            deleteEnrollmentSystem(sysCopy);
            free(tempCourse);
            free(begginingOfLine);
            return NULL;
        }
        free(tempCourse);
        while(*input != END_OF_LINE && *input != EOF)
        {
            input = input + getRidOfSpace(input);
            char* studentId = getNextString(input);
            if(!studentId)
            {
                deleteEnrollmentSystem(sysCopy);
                free(begginingOfLine);
                return NULL;
            }
            input = input + strlen(studentId) + getRidOfSpace(input);
            studentList student = findStudent(sysCopy->m_studentList, atoi(studentId));
            if(!student)
            {
                deleteEnrollmentSystem(sysCopy);
                free(begginingOfLine);
                return NULL;
            }
            free(studentId);
            if(IsraeliQueueEnqueue(course->m_israeliQueue, student) != ISRAELIQUEUE_SUCCESS)
            {
                deleteEnrollmentSystem(sysCopy);
                free(begginingOfLine);
                return NULL;
            }
        }
        free(begginingOfLine);
        input = getNextLine(queues);
        begginingOfLine = input; 
    }
    deleteEnrollmentSystem(sys);
    if(begginingOfLine)
    {
        free(begginingOfLine);
    }
    printf("printing system after read Enrollment \n");
    printEnrollmentSystem(sysCopy);


    return sysCopy;   
}
void printHacker(hackersList hacker)
{
    printf("printing hacker's %d courses\n", hacker->m_id);
    courseList courseRunner = hacker->m_courseList;
    while(courseRunner)
    {
        printf("course: %d\n", courseRunner->m_id);
        courseRunner = courseRunner->m_next;
    }
    printf("printing friends list\n");
    studentList friends = hacker->m_friendList;
    while(friends)
    {
        printf("friend: %d\n", friends->m_id);
        friends = friends->m_next;
    }
     printf("printing rivals list\n");
    studentList rivals = hacker->m_rivalList;
    while(rivals)
    {
        printf("rival: %d\n", rivals->m_id);
        rivals = rivals->m_next;
    }
}
void printEnrollmentSystem(EnrollmentSystem sys)
{
    courseList courseRunner = sys->m_courseList;
    printf("system courses:\n");
    while(courseRunner)
    {
        printf("%d\n", courseRunner->m_id);
        if(courseRunner->m_israeliQueue)
        {
            printf("course %d has a queue\n",courseRunner->m_id);
        }
        courseRunner = courseRunner->m_next;
    }
    studentList studentRunner = sys->m_studentList;
    printf("system students:\n");
    while(studentRunner)
    {
        printf("%d\n", studentRunner->m_id);
        studentRunner = studentRunner->m_next;
    }
    hackersList hackersRunner = sys->m_hackers;
    printf("system hackers:\n");
    while(hackersRunner)
    {
        printHacker(hackersRunner);
        hackersRunner = hackersRunner->m_next;
    }
}


int getRidOfSpace (char* input)
{
    int counter = 0;
    while(input[counter] == SEPERATOR)
    {
        counter++;
    }
    return counter;
}

courseList findCourse(courseList courses, int id)
{
    if(!courses)
    {
        return NULL;
    }
    courseList runner = courses;
    while(runner && runner->m_id != id)
    {
        runner = runner->m_next;
    }
    return runner;
}


void hackEnrollment(EnrollmentSystem sys, FILE* out)
{
    printf("trying to clone system\n");
    EnrollmentSystem copySys = cloneEnrollmentSystem(sys);
    if(!copySys)
    {
        printf("couldnt get copy\n");
        return;
    }
    printf("cloned system\n");
    hackersList hackerRunner = sys->m_hackers;
    printf("first hacker %d\n", hackerRunner->m_id);
    while(hackerRunner)
    {
        printf("got into loop\n");
        courseList courseRunner = hackerRunner->m_courseList;
        if(!courseRunner)
        {
            printf("hackers course list is null\n");
        }
        printf("first course %d\n", courseRunner->m_id);
        while(courseRunner)
        {
            printf("looking for course in system\n");
            courseList courseWanted = findCourse(sys->m_courseList, courseRunner->m_id);
            if(!courseWanted)
            {
                printf("didnt find course \n");
                EnrollmentSystem temp = sys;
                sys = copySys;
                deleteEnrollmentSystem(temp);
                return;
            }
            printf("found course %d\ntrying to enqueue hacker\n", courseWanted->m_id);
            if(courseWanted->m_israeliQueue)
            {
                if(IsraeliQueueEnqueue(courseWanted->m_israeliQueue, hackerRunner->m_hacker) != ISRAELIQUEUE_SUCCESS)
                {
                    printf("couldnt enqueue hacker\n");
                    EnrollmentSystem temp = sys;
                    sys = copySys;
                    deleteEnrollmentSystem(temp);
                    return;
                }
            }
            
            printf("succeded in enqueing hacker\n");
            courseRunner = courseRunner->m_next;
        }
        hackerRunner = hackerRunner->m_next;
    }
    deleteEnrollmentSystem(copySys);
    EnrollmentSystem copySystem = cloneEnrollmentSystem(sys);
    hackerRunner = sys->m_hackers;
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
            while(!compareStudents(hackerRunner->m_hacker, temp) && IsraeliQueueSize(courseWanted->m_israeliQueue) > 0)
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
                fprintf(out,"%d", hackerRunner->m_id);
                deleteEnrollmentSystem(copySystem);
                return;
            }
        }
        else if(numOfCoursesWanted > 1)
        {
            if(numOfCoursesGot < 2)
            {
                fputs("Cannot satisfy constraints for ", out);
                fprintf(out,"%d", hackerRunner->m_id);
                deleteEnrollmentSystem(copySystem);
                return;
            }
        }
    }
    printOutput(out,copySys);
    deleteEnrollmentSystem(copySys);
}
int getListLength(courseList course)
{
    int counter = 0;
    courseList runner = course;
    while(runner)
    {
        counter++;
        runner = runner->m_next;
    }
    return counter;
}
void printOutput(FILE* out, EnrollmentSystem sys)
{
    courseList courseRunner = sys->m_courseList;
    
    while(courseRunner)
    {
        int counter = 0;
        fprintf(out,"%d", courseRunner->m_id);
        while(IsraeliQueueSize(courseRunner->m_israeliQueue) > 0 && counter < courseRunner->m_maxStudents)
        {
            studentList student = IsraeliQueueDequeue(courseRunner->m_israeliQueue);
            fputs(" ",out);
            fprintf(out,"%d", student->m_id);
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

// might need to get a set as in a hacker and a student or two students
int nameDistancei(void* hacker, void* student)
{
    studentList student1 = hacker, student2 = student;
    int distance = stringDistancei(student1->m_city, student2->m_name);
    distance += stringDistancei(student1->m_surname, student2->m_surname);
    return distance;
}
int nameDistance(void* hacker, void* student)
{
    studentList student1 = hacker, student2 = student;
    int distance = stringDistance(student1->m_city, student2->m_name);
    distance += stringDistance(student1->m_surname, student2->m_surname);
    return distance;
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
            sum = sum + abs(name1[i] - name2[i]);
        }

        i++;
    }

    return sum;
}

int stringDistancei(char* name1, char* name2) //A == a
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
            sum = sum + abs(upperToLowercase(name1[i]) - upperToLowercase(name2[i]));
        }

        i++;
    }

    return sum;
}

int isInFriendList (void* hacker, void* student)
{
    studentList student1 = hacker, student2 = student;
    if(!student1->m_isHacker)
    {
        return 0;
    }
    if(!findStudent(student1->m_isHacker->m_friendList, student2->m_id))
    {
        return 0;
    }
    return FRIENDSHIP_BAR;
}

int idDifference(void* student1, void* student2)
{
    studentList firstStudent = student1, secondStudent = student2;
    return abs(firstStudent->m_id - secondStudent->m_id);
}

int compareStudents (void* student1, void* student2)
{
    studentList firstStudent = student1, secondStudent = student2;
    if(firstStudent->m_id != secondStudent->m_id)
    {
        return false;
    }
    if(firstStudent->m_totalCredit != secondStudent->m_totalCredit)
    {
        return false;
    }
    if(firstStudent->m_gpa != secondStudent->m_gpa)
    {
        return false;
    }
    if(strcmp(firstStudent->m_name, secondStudent->m_name) != 0)
    {
        return false;
    }
    if(strcmp(firstStudent->m_surname, secondStudent->m_surname) != 0)
    {
        return false;
    }
    if(strcmp(firstStudent->m_city, secondStudent->m_city) != 0) //TODO delete if needed
    {
        return false;
    }
    if(strcmp(firstStudent->m_department, secondStudent->m_department) != 0) //TODO delete if needed
    {
        return false;
    }

    return true;
}

IsraeliQueueError ifLowerCaseNeeded(EnrollmentSystem sys, bool ifCaseSensitive)
{
    courseList runner = sys->m_courseList;
    while(runner)
    {
        if(ifCaseSensitive)
        {
            if(runner->m_israeliQueue)
            {
                printf("trying to add friendship measure\n");
                IsraeliQueueError errorQueue= IsraeliQueueAddFriendshipMeasure(runner->m_israeliQueue, &nameDistancei);
                if(errorQueue != ISRAELIQUEUE_SUCCESS)
                {
                    return errorQueue;
                }
            }
            else
            {
                FriendshipFunction functions[NUM_OF_FUNCTIONS]  = {&idDifference, &isInFriendList, &nameDistancei};
                runner->m_israeliQueue = IsraeliQueueCreate(functions, &compareStudents, FRIENDSHIP_BAR, RIVELRY_BAR);
                if(!runner->m_israeliQueue)
                {
                    return ISRAELIQUEUE_ALLOC_FAILED;
                }
            }
        }
        else
        {
            if(runner->m_israeliQueue)
            {
                printf("trying to add friendship measure\n");
                IsraeliQueueError errorQueue = IsraeliQueueAddFriendshipMeasure(runner->m_israeliQueue, &nameDistance);
                if(errorQueue != ISRAELIQUEUE_SUCCESS)
                {
                    return errorQueue;
                }
            }
            else
            {
                FriendshipFunction functions[NUM_OF_FUNCTIONS]  = {&idDifference, &isInFriendList, &nameDistance};
                runner->m_israeliQueue = IsraeliQueueCreate(functions, &compareStudents, FRIENDSHIP_BAR, RIVELRY_BAR);
                if(!runner->m_israeliQueue)
                {
                    return ISRAELIQUEUE_ALLOC_FAILED;
                }
            } 
        }
        runner = runner->m_next;
    }
    printf("printing system after adding i flag\n");
    printEnrollmentSystem(sys);
    return ISRAELIQUEUE_SUCCESS;
}