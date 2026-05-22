#define MAX_WORKERS 50
#define MAX_PROJECTS 100
#define MAX_WORKERS_IN_PROJECT 4

//SWITCH - CASE: MENU
#define JOIN_COMPANY 1
#define OPEN_NEW_PROJECT 2
#define WORKER_JOINS_PROJECT 3
#define DISAPLAY_ALL_PROJECTS 4
#define DISAPLAY_ALL_WORKERS 5
#define WORK_ON_PROJECT 6
#define LEAVE_COMPANY 7
#define EXIT 8


// Forward declarations
typedef struct Worker Worker;
typedef struct FeatureNode FeatureNode;
typedef struct Project Project;

struct Worker
{
    char* name;
    Project** projects;
    int projectCount;
};

struct FeatureNode
{
    char* feature;
    struct FeatureNode* next;
};

struct Project
{
    char* name;
    Worker** workers;
    int workerCount;
    FeatureNode* features;
};

//Menu - in main code
char* dynamicString(void);
int isStringEmpty(const char* name);
void clearInputBuffer(void);


//Option 1
void joinCompany(Worker* workers[], int* workerCount);
int isWorkerNameExists(Worker* workers[], int workerCount, const char* name);


//Option 2
void openNewProject(Worker* workers[], int workerCount, Project* projects[], int* projectCount);
int isProjectNameExists(Project* projects[], int projectCount, const char* name);
void displayWorkers(Worker* workers[], int workerCount);

//Option 3
void workerJoinsProject(Worker* workers[], int workerCount, Project* projects[], int projectCount);
void addWorkerToProject(Worker* worker, Project* project);
void displayProjects(Project* projects[], int projectCount, int option); // display Projects With Worker Number (for option 3)

//Option 4
void displayAllProjects(Project* projects[], int projectCount);

//Option 5
void displayAllWorkers(Worker* workers[], int workerCount);

//Option 6
void workOnProject(Worker* workers[], int workerCount, int projectCount);
void addFeatureToProject(Project* project);
void removeFeatureFromProject(Project* project);
int isFeatureExists(FeatureNode* head, const char* featureName);


//Option 7
void leaveCompany(Worker* workers[], int* workerCount, Project* projects[], int* projectCount);
void freeFeatures(FeatureNode* head);
void freeProject(Project* project);


//Option 8 - Exit Software
void freeAllAllocations(Worker* workers[], int workerCount, Project* projects[], int projectCount);