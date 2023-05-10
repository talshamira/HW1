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
hackersList cloneHackerList(hackersList hacker, studentList sysStudents);
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
bool isInRivalList (studentList rivalList, studentList student);

courseList createCourseList(int id, int maxStudents)
{
    courseList list = malloc (sizeof(*list));
    if(list == NULL)
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
    while(listHead != NULL)
    {
        IsraeliQueueDestroy(listHead->m_israeliQueue);
        courseList toDelete = listHead;
        listHead = listHead->m_next;
        free(toDelete);
    }
}

studentList createStudentList(int id, int totalCredits, int gpa, char* name,char* surname, char* city, char* department)
{
    studentList list = malloc (sizeof(*list));
    if(list == NULL)
    {
        return NULL;
    }
    list->m_id = id;
    list->m_totalCredit = totalCredits;
    list->m_gpa = gpa;
    list->m_name = (char*) malloc(strlen(name)+1);
    if(list->m_name == NULL)
    {
        free(list);
        return NULL;
    }
    list->m_surname = (char*) malloc(strlen(surname)+1);
    if(list->m_surname == NULL)
    {
        
        free(list->m_name);
        free(list);
        return NULL;
    }
    list->m_city = (char*) malloc(strlen(city)+1);
    if(list->m_city == NULL)
    {
        free(list->m_surname);
        free(list->m_name);
        free(list);
        return NULL;
    }
    list->m_department = (char*) malloc(strlen(department)+1);
    if(list->m_department == NULL)
    {
        free(list->m_city);
        free(list->m_surname);
        free(list->m_name);
        free(list);
        return NULL;
    }
    strcpy(list->m_name, name);
    strcpy(list->m_surname , surname);
    strcpy(list->m_city , city);
    strcpy(list->m_department ,department);
    list->m_isHacker = NULL;
    list->m_next = NULL;
    return list;
}
studentList cloneStudentList(studentList student)
{
    if(student == NULL)
    {
        return NULL;
    }
    studentList list = malloc (sizeof(*list));
    if(list == NULL)
    {
        return NULL;
    }
    list->m_id = student->m_id;
    list->m_totalCredit = student->m_totalCredit;
    list->m_gpa = student->m_gpa;
    list->m_name = (char*) malloc(strlen(student->m_name)+1);
    if(list->m_name == NULL)
    {
        free(list);
        return NULL;
    }
    list->m_surname = (char*) malloc(strlen(student->m_surname)+1);
    if(list->m_surname == NULL)
    {
        
        free(list->m_name);
        free(list);
        return NULL;
    }
    list->m_city = (char*) malloc(strlen(student->m_city)+1);
    if(list->m_city == NULL)
    {
        free(list->m_surname);
        free(list->m_name);
        free(list);
        return NULL;
    }
    list->m_department = (char*) malloc(strlen(student->m_department)+1);
    if(list->m_department == NULL)
    {
        free(list->m_city);
        free(list->m_surname);
        free(list->m_name);
        free(list);
        return NULL;
    }
    strcpy(list->m_name, student->m_name);
    strcpy(list->m_surname , student->m_surname);
    strcpy(list->m_city , student->m_city);
    strcpy(list->m_department ,student->m_department);
    list->m_isHacker = student->m_isHacker;
    list->m_next = NULL;
    return list;
}

void destroyStudentList(studentList listHead)
{
    while(listHead != NULL)
    {
        studentList toDelete = listHead;
        listHead = listHead->m_next;
        free(toDelete->m_city);
        free(toDelete->m_name);
        free(toDelete->m_surname);
        free(toDelete->m_department);
        free(toDelete);
    }
}

hackersList createHackersList(int id, courseList courses, studentList friends, studentList rivals, studentList students)
{
    hackersList list = malloc (sizeof(*list));
    if(list == NULL)
    {
        return NULL;
    }
    list->m_id = id;
    list->m_courseList = courses;
    list->m_friendList = friends;
    list->m_hacker = findStudent(students, id);
    if(list->m_hacker == NULL)
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
    while(listHead != NULL)
    {
        hackersList toDelete = listHead;
        listHead = listHead->m_next;
        destroyCourseList(toDelete->m_courseList);
        destroyStudentList(toDelete->m_rivalList);
        destroyStudentList(toDelete->m_friendList);
        free(toDelete);
    }
}

studentList findStudent(studentList students, int id)
{
    studentList runner = students;
    while(runner != NULL && runner->m_id != id)
    {
        runner = runner->m_next;
    }
    return runner;
}

void deleteEnrollmentSystem(EnrollmentSystem sys)
{
    if(sys == NULL)
    {
        return;
    }
    destroyCourseList(sys->m_courseList);
    destroyHackerList(sys->m_hackers);
    destroyStudentList(sys->m_studentList);
    free(sys);
}

bool getCourseFromFile (FILE* courses, EnrollmentSystem hackEnrollment)
{
    courseList firstCourse = NULL;
    int gotToEOF = 0;
    FriendshipFunction functions[NUM_OF_FUNCTIONS_WITHOUT_I]  = {&idDifference, &isInFriendList, NULL};
    do
    {
        int id = 0, maxStudents = 0;
        gotToEOF = fscanf(courses, "%d %d", &id, &maxStudents);
        if(gotToEOF != NUM_OF_COURSES_PARAMETERS && gotToEOF != EOF)
        {
            deleteEnrollmentSystem(hackEnrollment);
            return false;
        }
        if(maxStudents != 0)
        {
            if(firstCourse == NULL)
            {
                hackEnrollment->m_courseList = createCourseList(id, maxStudents);
                if(hackEnrollment->m_courseList == NULL)
                {
                    
                    deleteEnrollmentSystem(hackEnrollment);
                    return false;
                }
                firstCourse = hackEnrollment->m_courseList;
            }
            else
            {
                hackEnrollment->m_courseList->m_next = createCourseList(id, maxStudents);
                if(hackEnrollment->m_courseList->m_next == NULL)
                {
                    deleteEnrollmentSystem(hackEnrollment);
                    return false;
                }
                hackEnrollment->m_courseList = hackEnrollment->m_courseList->m_next;
            }
        }
    } while(gotToEOF != EOF);
    hackEnrollment->m_courseList = firstCourse;
    while(firstCourse != NULL)
    {
        firstCourse->m_israeliQueue = IsraeliQueueCreate(functions, &compareStudents, FRIENDSHIP_BAR, RIVELRY_BAR);
        if(firstCourse->m_israeliQueue == NULL)
        {
            deleteEnrollmentSystem(hackEnrollment);
            return false;
        }
        firstCourse = firstCourse->m_next;
    }
    return true;
}


bool getStudentsFromFile (FILE* students, EnrollmentSystem hackEnrollment)
{
    studentList runner = NULL;
    hackEnrollment->m_studentList = NULL;
    char* inputs[NUM_OF_STUDENT_INPUTS] ={NULL} ;
    char* line = getNextLine(students);
    char* begginingOfLine = line;
    while(line !=NULL && *line!=EOF)
    {
        for(int i = 0; i < NUM_OF_STUDENT_INPUTS; i++)
        {
            inputs[i] = getNextString(line);
            if(inputs[i] == NULL)
            {
                for(int j = 0; j <i; j++)
                {
                    free(inputs[j]);
                }
                free(begginingOfLine);
                destroyCourseList(hackEnrollment->m_courseList);
                destroyStudentList(hackEnrollment->m_studentList);
                free(hackEnrollment);
                return false;
            }
            line = line + 1 + strlen(inputs[i]);
        }
        int id = atoi(inputs[ID]), totalCredits = atoi(inputs[TOTAL_CREDITS]), gpa =atoi(inputs[GPA]);
        if(id > 0)
        {
            if(id<MIN_ID_SIZE_POSITIVE || id > MAX_ID_SIZE_POSITIVE || totalCredits < 0 || gpa > MAX_GPA || gpa<0)
            {
                for(int i = 0; i < NUM_OF_STUDENT_INPUTS; i++)
                {
                    free(inputs[i]);
                }
                destroyCourseList(hackEnrollment->m_courseList);
                destroyStudentList(hackEnrollment->m_studentList);
                free(hackEnrollment);
                free(begginingOfLine);
                return false;
            }
        }
        else
        {
            if(id<MIN_ID_SIZE_NEGATIVE || id > MAX_ID_SIZE_NEGATIVE || totalCredits < 0 || gpa > MAX_GPA || gpa<0)
            {
                for(int i = 0; i < NUM_OF_STUDENT_INPUTS; i++)
                {
                    free(inputs[i]);
                }
                destroyCourseList(hackEnrollment->m_courseList);
                destroyStudentList(hackEnrollment->m_studentList);
                free(hackEnrollment);
                free(begginingOfLine);
                return false;
            }
        }
        if(hackEnrollment->m_studentList == NULL)
        {
            hackEnrollment->m_studentList = createStudentList(id,totalCredits, gpa,inputs[NAME], inputs[SURNAME], 
                    inputs[CITY], inputs[DEPARTMENT]); 
            if(hackEnrollment->m_studentList == NULL)
            {
                for(int i = 0; i < NUM_OF_STUDENT_INPUTS; i++)
                {
                    free(inputs[i]);
                }
                destroyCourseList(hackEnrollment->m_courseList);
                destroyStudentList(hackEnrollment->m_studentList);
                free(hackEnrollment);
                free(begginingOfLine);
                return false;
            }
            runner = hackEnrollment->m_studentList;
        }
        else
        {
            runner->m_next = createStudentList(id,totalCredits,gpa,inputs[NAME], 
                                                        inputs[SURNAME], inputs[CITY], inputs[DEPARTMENT]); 
            if(runner->m_next == NULL)
            {
                for(int i = 0; i < NUM_OF_STUDENT_INPUTS; i++)
                {
                    free(inputs[i]);
                }
                destroyCourseList(hackEnrollment->m_courseList);
                destroyStudentList(hackEnrollment->m_studentList);
                free(hackEnrollment);
                free(begginingOfLine);
                return false;
            }
            runner = runner->m_next; 
        }
        free(begginingOfLine);
        for(int i = 0 ; i < NUM_OF_STUDENT_INPUTS; i++)
        {
            free(inputs[i]);
        }
        line = getNextLine(students);
        begginingOfLine = line;
    }
    return true;
}



courseList getCoursesForHackers(FILE* hackers, EnrollmentSystem hackEnrollment, char* line)
{
    line = getNextLine(hackers);
    char* begginingOfLine = line;
    int length = strlen(line);
    courseList hackersFirstCourses = NULL, hackersCourseRunner = NULL;
    while(length > 0)
    {
        printf("line: %s", line);
        char* tempCourse = getNextString(line);
        if(tempCourse == NULL)
        {
            deleteEnrollmentSystem(hackEnrollment);
            free(begginingOfLine);
            return NULL;
        }
        printf("line: %s", line);
        line = line + strlen(tempCourse) +1;
        printf("line: %s", line);
        int courseNumber = atoi(tempCourse);
        free(tempCourse);
        courseList wantedCourse = findCourse(hackEnrollment->m_courseList, courseNumber);
        if(wantedCourse == NULL)
        {
            deleteEnrollmentSystem(hackEnrollment);
            free(begginingOfLine);
            return NULL;
        }
        if(hackersFirstCourses == NULL)
        {
            hackersCourseRunner = cloneCourseList(wantedCourse);
            if(hackersCourseRunner == NULL)
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
            if(hackersCourseRunner->m_next == NULL)
            {
                deleteEnrollmentSystem(hackEnrollment);
                free(begginingOfLine);
                return NULL;
            }
            hackersCourseRunner = hackersCourseRunner->m_next;
        }
        length = strlen(line);
    }
    free(begginingOfLine);
    return hackersFirstCourses;
}
studentList getStudentListForHacker (FILE* hackers, EnrollmentSystem hackEnrollment, char* line)
{
    studentList hackerFriendRunner = NULL;
    studentList firstFriends = NULL;
    line = getNextLine(hackers);
    char* begginingOfLine = line;
    int length = strlen(line);
    while(length > 0)
    {
      char* tempStudent = getNextString(line);
        if(tempStudent == NULL)
        {
            deleteEnrollmentSystem(hackEnrollment);
            free(begginingOfLine);
            return NULL;
        } 
        line = line + strlen(tempStudent) +1;
        int studentId = atoi(tempStudent);
        free(tempStudent);
        studentList friend = findStudent(hackEnrollment->m_studentList, studentId);
        if(friend == NULL)
        {
            deleteEnrollmentSystem(hackEnrollment);
            free(begginingOfLine);
            return NULL;
        }
        if(firstFriends == NULL)
        {
            hackerFriendRunner = cloneStudentList(friend);
            if(hackerFriendRunner == NULL)
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
            if(hackerFriendRunner->m_next == NULL)
            {
                deleteEnrollmentSystem(hackEnrollment);
                free(begginingOfLine);
                return NULL;
            }
            hackerFriendRunner = hackerFriendRunner->m_next;
        }
        length = strlen(line);
    }
    free(begginingOfLine);
    return firstFriends;
}
bool getHackersFromFile(FILE* hackers, EnrollmentSystem hackEnrollment)
{
    hackEnrollment->m_hackers =NULL;
    hackersList firstHacker = NULL;
    char* line = getNextLine(hackers);
    char* begginingOfLine = line;
    while(line != NULL)
    {  
        char* tempId = getNextString(line);
        if(tempId == NULL)
        {
            deleteEnrollmentSystem(hackEnrollment);
            free(begginingOfLine);
            return false;
        } 
        line = line + strlen(tempId) +1;
        int id = atoi(tempId);
        free(tempId);
        free(begginingOfLine);
        courseList hackersFirstCourses = getCoursesForHackers(hackers, hackEnrollment, line);
        if(hackersFirstCourses == NULL)
        {
            return false;
        }
        studentList firstFriends = getStudentListForHacker(hackers, hackEnrollment, line);
        if(firstFriends == NULL)
        {
            return false;
        }
        studentList firstRivals = getStudentListForHacker(hackers, hackEnrollment, line);
        if(firstRivals == NULL)
        {
            return false;
        }
        if(firstHacker == NULL)
        {
            hackEnrollment->m_hackers = createHackersList(id, hackersFirstCourses, firstFriends, firstRivals, 
                                                                                    hackEnrollment->m_studentList);
            if(hackEnrollment->m_hackers == NULL)
            {
                deleteEnrollmentSystem(hackEnrollment);
                //free(begginingOfLine);
                return false;
            }
            firstHacker = hackEnrollment->m_hackers;
            
        }
        else
        {
            hackEnrollment->m_hackers->m_next = createHackersList(id, hackersFirstCourses, firstFriends, firstRivals, 
                                                                                hackEnrollment->m_studentList);
            if(hackEnrollment->m_hackers->m_next == NULL)
            {
                deleteEnrollmentSystem(hackEnrollment);
                //free(begginingOfLine);
                return false;
            }
            hackEnrollment->m_hackers = hackEnrollment->m_hackers->m_next;
        }
        line = getNextLine(hackers);
        begginingOfLine = line;
    }
    if(begginingOfLine != NULL)
    {
       free(begginingOfLine);
    }
    hackEnrollment->m_hackers = firstHacker;
    return true;
}

EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers)
{
    EnrollmentSystem hackEnrollment = (EnrollmentSystem) malloc(sizeof(*hackEnrollment));
    if(hackEnrollment == NULL)
    {
        return NULL;
    }
    if(!getCourseFromFile(courses, hackEnrollment))
    {
        return NULL;
    }
    if(!getStudentsFromFile(students, hackEnrollment))
    {
        return NULL;
    }
    if(!getHackersFromFile(hackers, hackEnrollment))
    {
        return NULL;
    }
    printEnrollmentSystem(hackEnrollment);
    return hackEnrollment;
}

char* getNextString (char* line)
{
    if(line == NULL)
    {
        return NULL;
    }
    char* runner = line;
    int counter = 0, length = strlen(line);
    while(counter <= length && runner[counter] != SEPERATOR && runner[counter] != END_OF_LINE && runner[counter] != EOF)
    {
        counter++;
    }
    char* string = (char *) malloc(sizeof(string)*counter +1);
    if(string == NULL)
    {
        return NULL;
    }
    for(int i = 0; i < counter; i++)
    {
        string[i] = line[i];
    }
    string[counter] = '\0';
    return string;
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
    if(sys == NULL)
    {
        return NULL;
    }
    EnrollmentSystem copy = (EnrollmentSystem) malloc(sizeof(*copy));
    if(copy == NULL)
    {
        return NULL;
    }
    studentList firstStudent = NULL, studentRunner = sys->m_studentList;
    while(studentRunner != NULL)
    {
        if(firstStudent == NULL)
        {
            copy->m_studentList = cloneStudentList(studentRunner);
            if(copy->m_studentList == NULL)
            {
                deleteEnrollmentSystem(copy);
                return NULL;
            }
            firstStudent = copy->m_studentList;
        }
        else
        {
            copy->m_studentList->m_next = cloneStudentList(studentRunner);
            if(copy->m_studentList->m_next == NULL)
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
    while(courseRunner != NULL)
    {
        if(firstCourse == NULL)
        {
            copy->m_courseList = cloneCourseList(courseRunner);
            if(copy->m_courseList == NULL)
            {
                deleteEnrollmentSystem(copy);
                return NULL;
            }
            firstCourse = copy->m_courseList;
        }
        else
        {
            copy->m_courseList->m_next = cloneCourseList(courseRunner);
            if(copy->m_courseList->m_next == NULL)
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
        if(firstHacker == NULL)
        {
            copy->m_hackers = cloneHackerList(hackersRunnerOld, copy->m_studentList);
            if(!copy->m_hackers)
            {
                deleteEnrollmentSystem(copy);
                return NULL;
            }
            firstHacker = copy->m_hackers;
        }
        else
        {
            copy->m_hackers->m_next = cloneHackerList(hackersRunnerOld,copy->m_studentList);
            if(copy->m_hackers->m_next == NULL)
            {
                deleteEnrollmentSystem(copy);
                return NULL;
            }
            copy->m_hackers = copy->m_hackers->m_next;
        }
        hackersRunnerOld = hackersRunnerOld->m_next;
    }
    copy->m_hackers = firstHacker;
    return copy;
}

hackersList cloneHackerList(hackersList hacker, studentList sysStudents)
{
    hackersList list = malloc (sizeof(*list));
    if(list == NULL)
    {
        return NULL;
    }
    list->m_id = hacker->m_id;

    studentList friendRunnerOld = hacker->m_friendList;
    studentList firstFriend = NULL;
    while(friendRunnerOld != NULL)
    {
        if(firstFriend == NULL)
        {
            list->m_friendList = cloneStudentList(friendRunnerOld);
            if(list->m_friendList == NULL)
            {
                destroyHackerList(list);
                return NULL;
            }
            firstFriend = list->m_friendList;
        }
        else
        {
            list->m_friendList->m_next = cloneStudentList(friendRunnerOld);
            if(list->m_friendList->m_next == NULL)
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
    while(rivalRunnerOld != NULL)
    {
        if(firstRival == NULL)
        {
            list->m_rivalList = cloneStudentList(rivalRunnerOld);
            if(list->m_rivalList == NULL)
            {
                destroyHackerList(list);
                return NULL;
            }
            firstRival = list->m_rivalList;
        }
        else
        {
            list->m_rivalList->m_next = cloneStudentList(rivalRunnerOld);
            if(list->m_rivalList->m_next == NULL)
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
    while(courseRunnerOld != NULL)
    {
        if(firstCourse == NULL)
        {
            list->m_courseList = cloneCourseList(courseRunnerOld);
            if(list->m_courseList == NULL)
            {
                destroyHackerList(list);
                return NULL;
            }
            firstCourse = list->m_courseList;
        }
        else
        {
            list->m_courseList->m_next = cloneCourseList(courseRunnerOld);
            if(list->m_courseList->m_next == NULL)
            {
                destroyHackerList(list);
                return NULL;
            }
            list->m_courseList = list->m_courseList->m_next;
        }
        courseRunnerOld = courseRunnerOld->m_next;
    }
    list->m_courseList = firstCourse;
    list->m_hacker = findStudent(sysStudents, hacker->m_id);
    list->m_next = NULL;
    return list;
}


courseList cloneCourseList (courseList course)
{
    if(course == NULL)
    {
        return NULL;
    }
    courseList list = createCourseList(course->m_id, course->m_maxStudents);
    if(list == NULL)
    {
        return NULL;
    }
    list->m_next = NULL;
    if(course->m_israeliQueue == NULL)
    {
        list->m_israeliQueue = NULL;
    }
    else
    {
        list->m_israeliQueue = IsraeliQueueClone(course->m_israeliQueue);
        if(list->m_israeliQueue == NULL)
        {
            destroyCourseList(list);
            return NULL;
        }
    }
    return list;
}


EnrollmentSystem readEnrollment(EnrollmentSystem sys, FILE* queues)
{
    EnrollmentSystem sysCopy = cloneEnrollmentSystem(sys);
    if(sysCopy == NULL)
    {
        return NULL;
    }
    char* input = getNextLine(queues);
    char* begginingOfLine = input;
    int length = strlen(input);
    while(length >0)
    {
        /*
        if(!input)
        {
            deleteEnrollmentSystem(sysCopy);
            return NULL;
        }*/
        char* tempCourse = getNextString(input);
        if(tempCourse ==  NULL)
        {
            deleteEnrollmentSystem(sysCopy);
            free(begginingOfLine);
            return NULL;
        }
        input = input + strlen(tempCourse)+1;
        
        courseList course = findCourse(sysCopy->m_courseList, atoi(tempCourse));
        if(course == NULL)
        {
            deleteEnrollmentSystem(sysCopy);
            free(tempCourse);
            free(begginingOfLine);
            return NULL;
        }
        free(tempCourse);
        length = strlen(input);
        while(length >0)
        {
            input = input + getRidOfSpace(input);
            char* studentId = getNextString(input);
            if(studentId == NULL)
            {
                deleteEnrollmentSystem(sysCopy);
                free(begginingOfLine);
                return NULL;
            }
            input = input + strlen(studentId) + 1;
            studentList student = findStudent(sysCopy->m_studentList, atoi(studentId));
            if(student == NULL)
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
            length = strlen(input);
        }
        free(begginingOfLine);
        input = getNextLine(queues);
        begginingOfLine = input; 
    }
    deleteEnrollmentSystem(sys);
    if(begginingOfLine != NULL)
    {
        free(begginingOfLine);
    }
    return sysCopy;   
}
//********************************************************************************************************************* TODO remove unneccery functions
void printHacker(hackersList hacker)
{
    printf("printing hacker's %d courses\n", hacker->m_id);
    courseList courseRunner = hacker->m_courseList;
    while(courseRunner != NULL)
    {
        printf("course: %d\n", courseRunner->m_id);
        courseRunner = courseRunner->m_next;
    }
    printf("printing friends list\n");
    studentList friends = hacker->m_friendList;
    while(friends != NULL)
    {
        printf("friend: %d\n", friends->m_id);
        friends = friends->m_next;
    }
     printf("printing rivals list\n");
    studentList rivals = hacker->m_rivalList;
    while(rivals != NULL)
    {
        printf("rival: %d\n", rivals->m_id);
        rivals = rivals->m_next;
    }
}
void printEnrollmentSystem(EnrollmentSystem sys)
{
    courseList courseRunner = sys->m_courseList;
    printf("system courses:\n");
    while(courseRunner != NULL)
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
    while(studentRunner != NULL)
    {
        printf("%d\n", studentRunner->m_id);
        studentRunner = studentRunner->m_next;
    }
    hackersList hackersRunner = sys->m_hackers;
    printf("system hackers:\n");
    while(hackersRunner != NULL)
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
    if(courses == NULL)
    {
        return NULL;
    }
    courseList runner = courses;
    while(runner != NULL && runner->m_id != id)
    {
        runner = runner->m_next;
    }
    return runner;
}

bool insertHackers (EnrollmentSystem sys)
{
    EnrollmentSystem copySys = cloneEnrollmentSystem(sys);
    if(copySys == NULL)
    {
        return false;
    }
    hackersList hackerRunner = sys->m_hackers;
    while(hackerRunner != NULL)
    {
        studentList hacker = findStudent(sys->m_studentList, hackerRunner->m_id);
        if(hacker == NULL)
        {
            EnrollmentSystem temp = sys;
            sys = copySys;
            deleteEnrollmentSystem(temp);
            return false;
        }
        courseList courseRunner = hackerRunner->m_courseList;
        while(courseRunner != NULL)
        {
            courseList courseWanted = findCourse(sys->m_courseList, courseRunner->m_id);
            if(courseWanted == NULL)
            {
                EnrollmentSystem temp = sys;
                sys = copySys;
                deleteEnrollmentSystem(temp);
                return false;
            }
            if(courseWanted->m_israeliQueue != NULL)
            {
                printf("enquing hacker %d to course %d\n",hacker->m_id, courseWanted->m_id);
                if(IsraeliQueueEnqueue(courseWanted->m_israeliQueue, hacker) != ISRAELIQUEUE_SUCCESS)
                {
                    EnrollmentSystem temp = sys;
                    sys = copySys;
                    deleteEnrollmentSystem(temp);
                    return false;
                }
            }
            
            courseRunner = courseRunner->m_next;
        }
        hackerRunner = hackerRunner->m_next;
    }
    deleteEnrollmentSystem(copySys);
    return true;
}
void hackEnrollment(EnrollmentSystem sys, FILE* out)
{
    if(!insertHackers(sys))
    {
        return;
    }
    EnrollmentSystem copySystem = cloneEnrollmentSystem(sys);
    hackersList hackerRunner = sys->m_hackers;
    while(hackerRunner != NULL)
    {
       studentList hacker = findStudent(sys->m_studentList, hackerRunner->m_id);
        if(hacker == NULL)
        {
            EnrollmentSystem temp = sys;
            sys = copySystem;
            deleteEnrollmentSystem(temp);
            return;
        }
        int numOfCoursesWanted = getListLength(hackerRunner->m_courseList);
        int numOfCoursesGot = 0;
        courseList courseRunner = hackerRunner->m_courseList;
        while(courseRunner != NULL)
        {
            int counter = 0;
            courseList courseWanted = findCourse(sys->m_courseList, courseRunner->m_id);
            if(courseWanted->m_israeliQueue != NULL)
            {
                studentList temp = IsraeliQueueDequeue(courseWanted->m_israeliQueue);
                while(temp!=NULL && !compareStudents(hacker, temp) && IsraeliQueueSize(courseWanted->m_israeliQueue)>0)
                {
                    temp = IsraeliQueueDequeue(courseWanted->m_israeliQueue);
                    counter++;
                }
                if(counter < courseWanted->m_maxStudents)
                {
                    numOfCoursesGot++;
                }
            }
            courseRunner = courseRunner->m_next;
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
        hackerRunner = hackerRunner->m_next;
    }
    printOutput(out,copySystem);
    deleteEnrollmentSystem(copySystem);
}

int getListLength(courseList course)
{
    int counter = 0;
    courseList runner = course;
    while(runner != NULL)
    {
        counter++;
        runner = runner->m_next;
    }
    return counter;
}
void printOutput(FILE* out, EnrollmentSystem sys)
{
    courseList courseRunner = sys->m_courseList;
    while(courseRunner != NULL)
    {
        int counter = 0;
        if(IsraeliQueueSize(courseRunner->m_israeliQueue) >1 )
        {
             fprintf(out,"%d", courseRunner->m_id);
            while(IsraeliQueueSize(courseRunner->m_israeliQueue) >0 && counter < courseRunner->m_maxStudents)
            {
                studentList student = IsraeliQueueDequeue(courseRunner->m_israeliQueue);
                if(student != NULL)
                {
                    fputs(" ",out);
                    fprintf(out,"%d", student->m_id);
                }
            }
            if(courseRunner->m_next != NULL)
            {
                fputs("\n",out);
            }
        }
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


int nameDistancei(void* hacker, void* student)
{
    studentList student1 = hacker, student2 = student;
    if(student1->m_isHacker == NULL && student2->m_isHacker == NULL)
    {
        return RIVELRY_BAR-1;
    }
    if(student1->m_isHacker != NULL)
    {
        if(isInRivalList(student1->m_isHacker->m_rivalList, student2))
        {
            return RIVELRY_BAR-1;
        }
    }
    else
    {
        if(isInRivalList(student2->m_isHacker->m_rivalList, student2))
        {
            return RIVELRY_BAR-1;
        }
    }
    int distance = stringDistancei(student1->m_name, student2->m_name);
    distance += stringDistancei(student1->m_surname, student2->m_surname);
    return distance;
}
int nameDistance(void* hacker, void* student)
{
    studentList student1 = hacker, student2 = student;
    if(student1->m_isHacker == NULL && student2->m_isHacker == NULL)
    {
        return RIVELRY_BAR-1;
    }
    if(student1->m_isHacker != NULL)
    {
        if(isInRivalList(student1->m_isHacker->m_rivalList, student2))
        {
            return RIVELRY_BAR-1;
        }
    }
    else
    {
        if(isInRivalList(student2->m_isHacker->m_rivalList, student2))
        {
            return RIVELRY_BAR-1;
        }
    }
    int distance = stringDistance(student1->m_name, student2->m_name);
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
bool isInRivalList (studentList rivalList, studentList student)
{
    if(!findStudent(rivalList, student->m_id))
    {
        return false;
    }
    return true;
}
int isInFriendList (void* hacker, void* student)
{
    studentList student1 = hacker, student2 = student;
    if(student1->m_isHacker == NULL && student2->m_isHacker == NULL)
    {
        return RIVELRY_BAR-1;
    }
    if(student1->m_isHacker != NULL)
    {
        if(isInRivalList(student1->m_isHacker->m_rivalList, student2) || 
                            !findStudent(student1->m_isHacker->m_friendList, student2->m_id))
        {
            return RIVELRY_BAR-1;
        }
    }
    else
    {
        if(isInRivalList(student2->m_isHacker->m_rivalList, student2) || 
                            !findStudent(student2->m_isHacker->m_friendList, student1->m_id))
        {
            return RIVELRY_BAR-1;
        }
    }
   
    return FRIENDSHIP_BAR+1;
}

int idDifference(void* student1, void* student2)
{
    studentList firstStudent = student1, secondStudent = student2;
    if(firstStudent->m_isHacker == NULL && secondStudent->m_isHacker == NULL)
    {
        return RIVELRY_BAR-1;
    }
    if(firstStudent->m_isHacker != NULL)
    {
        if(isInRivalList(firstStudent->m_isHacker->m_rivalList, secondStudent))
        {
            return RIVELRY_BAR-1;
        }
    }
    else
    {
        if(isInRivalList(secondStudent->m_isHacker->m_rivalList, firstStudent))
        {
            return RIVELRY_BAR-1;
        }
    }
    return abs(firstStudent->m_id - secondStudent->m_id);
}

int compareStudents (void* student1, void* student2)
{
    if(student1 == NULL || student2 == NULL)
    {
        return false;
    }
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
    while(runner != NULL)
    {
        if(ifCaseSensitive)
        {
            if(runner->m_israeliQueue != NULL)
            {
                IsraeliQueueError errorQueue= IsraeliQueueAddFriendshipMeasure(runner->m_israeliQueue, &nameDistancei);
                if(errorQueue != ISRAELIQUEUE_SUCCESS)
                {
                    return errorQueue;
                }
            }
            else
            {
                FriendshipFunction functions[NUM_OF_FUNCTIONS] = {&idDifference, &isInFriendList, &nameDistancei, NULL};
                runner->m_israeliQueue = IsraeliQueueCreate(functions, &compareStudents, FRIENDSHIP_BAR, RIVELRY_BAR);
                if(!runner->m_israeliQueue)
                {
                    return ISRAELIQUEUE_ALLOC_FAILED;
                }
            }
        }
        else
        {
            if(runner->m_israeliQueue != NULL)
            {
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
    return ISRAELIQUEUE_SUCCESS;
}