#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ex_5.h"

int main() {
    // Initialization
    Worker* workers[MAX_WORKERS] = { NULL };
    Project* projects[MAX_PROJECTS] = { NULL };
    int workerCount = 0;
    int projectCount = 0;

    //A while loop until the user chose option 8
    while (1) {
        printf("Welcome!\n"
            "Please select the desired option:\n"
            "1. Join the company\n"
            "2. Open a new project\n"
            "3. Join an existing project\n"
            "4. See what projects are in work\n"
            "5. See which people are in what projects\n"
            "6. Work on existing project\n"
            "7. Leave the company\n"
            "8. Exit\n"
            "Enter your choice: ");

        int yourChoice;
        scanf("%d", &yourChoice);


        // Execute the appropriate function based on the user's choice
        switch (yourChoice) {
        case JOIN_COMPANY:
            joinCompany(workers, &workerCount);
            break;

        case OPEN_NEW_PROJECT:
            openNewProject(workers, workerCount, projects, &projectCount);
            break;

        case WORKER_JOINS_PROJECT:
            workerJoinsProject(workers, workerCount, projects, projectCount);
            break;

        case DISAPLAY_ALL_PROJECTS:
            displayAllProjects(projects, projectCount);
            break;

        case DISAPLAY_ALL_WORKERS:
            displayAllWorkers(workers, workerCount);
            break;

        case WORK_ON_PROJECT:
            workOnProject(workers, workerCount, projectCount);
            break;

        case LEAVE_COMPANY:
            leaveCompany(workers, &workerCount, projects, &projectCount);
            break;

        case EXIT:
            freeAllAllocations(workers, workerCount, projects, projectCount);
            printf("Exiting...\n");
            return 0;
            break;

        default:
            // If the user enters an invalid choice, print an error message
            printf("Invalid choice. Please try again.\n\n");
            break;
        }
    }
}

// Function to clear the input buffer
void clearInputBuffer() {
    scanf("%*[^\n]"); // Read and discard characters until a newline is encountered
    scanf("%*c");     // Read and discard the newline character
}

// Function to get a dynamic string from the user (will use for every input that is not a choice)
char* dynamicString() {
    clearInputBuffer();
    // Allocate initial memory for dString
    char* dString = (char*)malloc(sizeof(char));
    if (dString == NULL) {
        printf("Error: Memory allocation failed.\n");
        exit(1);
    }

    // Read characters until a newline is encountered
    int bufferSize = 0;
    char c;
    // Check the return value of scanf and compare the character read
        // Check the return value of scanf and compare the character read 
    while (scanf("%c", &c) == 1 && c != '\n' && c != '\r') {
        dString[bufferSize++] = c;

        // Reallocate memory for dString
        char* temp = (char*)realloc(dString, (bufferSize + 1) * sizeof(char));
        if (temp == NULL) {
            printf("Error: Memory allocation failed.\n");
            free(dString);
            exit(1);
        }

        dString = temp;
    }
    dString[bufferSize] = '\0'; // Null-terminate the string
    return dString;
}

// Function to check if a string is empty
int isStringEmpty(const char* name) {
    if (name == NULL) {
        // Handle NULL pointer as an empty string
        return 1;
    }

    // Check each character in the string
    while (*name != '\0') {
        // If character is not a space, tab, newline, or carriage return, string is not empty
        if (*name != ' ' && *name != '\t' && *name != '\n' && *name != '\r') {
            return 0;
        }
        name++;
    }

    // All characters are whitespace or the string is empty
    return 1;
}

//************************************* Option 1 - Add Workers ****************************************************************************
void joinCompany(Worker* workers[], int* workerCount) {
    // Check if the worker count is not at his max
    if (*workerCount >= MAX_WORKERS) {
        printf("Cannot add more workers. Maximum limit reached.\n");
        return;
    }

    // Prompt the user for the worker's name
    printf("Enter the name of the new worker: ");
    char* tempName = dynamicString();
    // Check if the name is not empty
    if (isStringEmpty(tempName)) {
        printf("Invalid name\n");
        free(tempName);
        return;
    }

    // Check for duplicate worker names
    if (isWorkerNameExists(workers, *workerCount, tempName)) {
        printf("A worker with this name already exists.\n"
            "There is not enough space in this company for both of us\n");
        free(tempName); // Free allocated memory for tempName
        return;
    }

    // Allocate memory for a new worker
    Worker* newWorker = (Worker*)malloc(sizeof(Worker));

    // Check if memory allocation was successful
    if (newWorker == NULL) {
        printf("Error: Memory allocation failed.\n");
        free(tempName); // Free allocated memory for tempName
        exit(1);
    }

    // Allocate memory for the worker's name
    newWorker->name = (char*)malloc((strlen(tempName) + 1) * sizeof(char));

    // Check if memory allocation was successful
    if (newWorker->name == NULL) {
        printf("Error: Memory allocation failed.\n");
        free(newWorker);
        free(tempName);
        exit(1);
    }

    // Copy the entered name to the allocated memory
    strcpy(newWorker->name, tempName);

    // Initialize other fields of the worker
    newWorker->projects = NULL;
    newWorker->projectCount = 0;

    // Add the new worker to the array
    workers[*workerCount] = newWorker;

    // Increment the worker count
    (*workerCount)++;

    // Free allocated memory for tempName
    free(tempName);
}

int isWorkerNameExists(Worker* workers[], int workerCount, const char* name) {
    for (int i = 0; i < workerCount; i++) {
        if (strcmp(workers[i]->name, name) == 0) {
            // Worker with the same name found
            return 1;
        }
    }
    // Worker with the given name does not exist
    return 0;
}

//******************************************************************** Option 2 - Create Projcets ******************************************
// Function to open a new project and assign a worker to it
void openNewProject(Worker* workers[], int workerCount, Project* projects[], int* projectCount) {
    // Check if the maximum number of projects has been reached
    if (*projectCount >= MAX_PROJECTS) {
        printf("Cannot open more projects. Maximum limit reached.\n");
        return;
    }
    // Check if there are any workers in the company
    if (workerCount <= 0) {
        printf("There are no workers in the company yet, please join the company first.\n");
        return;
    }

    // Display all workers and ask the user to select a worker
    printf("Who are you? Choose a number:\n");
    displayWorkers(workers, workerCount);
    printf("Enter your choice: ");
    int workerNumber;
    scanf("%d", &workerNumber);

    // Check if the selected worker is within the valid range
    if (workerNumber < 1 || workerNumber > workerCount) {
        printf("Invalid selection.\n");
        return;
    }

    // Prompt the user for the project name
    printf("Enter the name of the new project: ");
    char* tempProjectName = dynamicString();
    // Check if the project name is not empty
    if (isStringEmpty(tempProjectName)) {
        printf("Invalid name\n");
        free(tempProjectName);
        return;
    }

    // Check if a project with the same name already exists
    if (isProjectNameExists(projects, *projectCount, tempProjectName)) {
        printf("A project with this name already exists.\n"
            "Better be more creative\n");
        free(tempProjectName); // Free allocated memory for tempName
        return;
    }

    // Allocate memory for a new project
    Project* newProject = (Project*)malloc(sizeof(Project));
    // Check if memory allocation was successful
    if (newProject == NULL) {
        printf("Error: Memory allocation failed.\n");
        free(tempProjectName);
        exit(1);
    }

    // Allocate memory for the project's name
    newProject->name = (char*)malloc((strlen(tempProjectName) + 1) * sizeof(char));
    // Check if memory allocation was successful
    if (newProject->name == NULL) {
        printf("Error: Memory allocation failed.\n");
        free(newProject);
        free(tempProjectName);
        exit(1);
    }

    // Copy the entered name to the allocated memory
    strcpy(newProject->name, tempProjectName);

    // Initialize other fields of the project 
    newProject->workers = (Worker**)malloc(sizeof(Worker*) * MAX_WORKERS);
    if (newProject->workers == NULL) {
        printf("Error: Memory allocation failed.\n");
        free(newProject->name);
        free(newProject);
        free(tempProjectName);
        exit(1);
    }

    newProject->workerCount = 0;
    newProject->features = NULL;

    // Add the new project to the array
    projects[*projectCount] = newProject;

    // Increment the project count
    (*projectCount)++;


    // Add the selected worker to the project
    newProject->workers[newProject->workerCount++] = workers[workerNumber - 1];

    // Add the selected project to the worker
    Worker* selectedWorker = workers[workerNumber - 1];
    Project** tempProjects = (Project**)realloc(selectedWorker->projects, (selectedWorker->projectCount + 1) * sizeof(Project*));

    // Check if realloc was successful, else free 
    if (tempProjects == NULL) {
        printf("Error: Memory allocation failed.\n");
        free(newProject->workers);
        free(newProject->name);
        free(newProject);
        free(tempProjectName);
        exit(1);
    }
    // Assign the selected worker to the selected project
    selectedWorker->projects = tempProjects;
    // Increase the project count of the selected worker
    selectedWorker->projects[selectedWorker->projectCount++] = newProject;

    free(tempProjectName);

}


// Check if the project not already exist
int isProjectNameExists(Project* projects[], int projectCount, const char* name) {
    for (int i = 0; i < projectCount; i++) {
        if (strcmp(projects[i]->name, name) == 0) {
            // Project with the same name found
            return 1;
        }
    }
    // Project with the given name does not exist
    return 0;
}

// Display the workers
void displayWorkers(Worker* workers[], int workerCount) {
    for (int i = 0; i < workerCount; i++) {
        printf("%d. %s\n", i + 1, workers[i]->name);
    }
}

//******************************************************************** Option 3 - Join Projects ******************************************
// Function to add a worker to an open project
void workerJoinsProject(Worker* workers[], int workerCount, Project* projects[], int projectCount) {
    // Check if there are any workers in the company
    if (workerCount <= 0) {
        printf("There are no workers in the company yet, please join the company first.\n");
        return;
    }
    // Check if there are any projects in the company
    if (projectCount <= 0) {
        printf("There are no projects in the company yet, please create one first.\n");
        return;
    }

    // Display all workers and ask the user to select a worker to join a project
    printf("Select a worker to join a project:\n");
    displayWorkers(workers, workerCount);
    printf("Enter the worker's number: ");

    int workerNumber;
    scanf("%d", &workerNumber);

    // Check if the selected worker is within the valid range
    if (workerNumber < 1 || workerNumber > workerCount) {
        printf("Invalid worker selection.\n");
        return;
    }

    // Retrieve the selected worker
    Worker* selectedWorker = workers[workerNumber - 1];

    // Display all projects and ask the user to select a project for the worker to join
    printf("Select a project to join:\n");
    displayProjects(projects, projectCount, WORKER_JOINS_PROJECT);
    printf("Enter the project's number: ");

    int projectNumber;
    scanf("%d", &projectNumber);

    // Check if the selected project is within the valid range
    if (projectNumber < 1 || projectNumber > projectCount) {
        printf("Invalid project selection.\n");
        return;
    }

    // Retrieve the selected project
    Project* selectedProject = projects[projectNumber - 1];

    // Add the selected worker to the selected project
    addWorkerToProject(selectedWorker, selectedProject);
}

// Function to add the selected worker to the selected project
void addWorkerToProject(Worker* worker, Project* project) {
    // Check if the worker is already in the project
    for (int i = 0; i < project->workerCount; i++) {
        if (project->workers[i] == worker) {
            printf("This worker is already part of the project '%s'.\n", project->name);
            return;
        }
    }

    // Check if the project is full
    if (project->workerCount == MAX_WORKERS_IN_PROJECT) {
        printf("Project is full.");
        return;
    }

    // Increase the size of the workers array in the project
    Worker** tempWorkers = realloc(project->workers, (project->workerCount + 1) * sizeof(Worker*));
    if (tempWorkers == NULL) {
        printf("Error: Memory allocation failed.\n");
        exit(1);
    }
    // Add the worker to the project
    project->workers = tempWorkers;
    project->workers[project->workerCount] = worker;
    project->workerCount++;

    // Increase the size of the projects array in the worker
    Project** tempProjects = (Project**)realloc(worker->projects, (worker->projectCount + 1) * sizeof(Project*));
    if (tempProjects == NULL) {
        printf("Error: Memory allocation failed.\n");
        // If realloc fails for worker->projects, free the memory allocated for project->workers
        free(tempWorkers);
        exit(1);
    }

    // Add the project to the worker's list of projects, and increase the project count for the worker
    worker->projects = tempProjects;
    worker->projects[worker->projectCount] = project;
    worker->projectCount++;
}

// Function to display all projects
void displayProjects(Project* projects[], int projectCount, int option) {
    // Check if there are any projects to display
    if (projectCount <= 0) {
        printf("There are no projects to display.\n");
        return;
    }
    // A special projects display for option 3
    if (option == WORKER_JOINS_PROJECT) {
        // Display each project with its number and the number of workers
        for (int i = 0; i < projectCount; i++) {
            printf("%d. %s (Workers: %d)\n", i + 1, projects[i]->name, projects[i]->workerCount);
        }
    }
    else {
        // Display each project with its number
        for (int i = 0; i < projectCount; i++) {
            printf("%d. %s\n", i + 1, projects[i]->name);
        }
    }
}


//******************************************************************** Option 4 - See all Projects ******************************************
// Function to display all projects
void displayAllProjects(Project* projects[], int projectCount) {
    // Check if there are any projects in the company
    if (projectCount <= 0) {
        printf("There are no projects to display.\n");
        return;
    }

    // Loop through all projects
    for (int i = 0; i < projectCount; i++) {
        // Print the name of the project
        printf("Project: %s\n", projects[i]->name);
        printf("  Workers in this project:\n");

        // Loop through all workers in the project
        for (int c = 0; c < projects[i]->workerCount; c++) {
            // Print the name of each worker
            printf("    - %s\n", projects[i]->workers[c]->name);
        }
        printf("  Features of the project:\n");

        // Check if the project has any features
        if (projects[i]->features == NULL) {
            printf("    No features\n");
        }
        else {
            // If the project has features, loop through all features
            FeatureNode* currentFeature = projects[i]->features;
            while (currentFeature != NULL) {
                // Print each feature
                printf("    - %s\n", currentFeature->feature);
                currentFeature = currentFeature->next;
            }
        }
        printf("\n");
    }
}


//******************************************************************** Option 5 - See all People ******************************************
// Function to display all workers
void displayAllWorkers(Worker* workers[], int workerCount) {
    // Check if there are any workers in the company
    if (workerCount <= 0) {
        printf("There are no workers to display.\n");
        return;
    }

    // Loop through all workers
    for (int i = 0; i < workerCount; i++) {
        // Print the name of the worker
        printf("Worker: %s\n", workers[i]->name);

        // Check if the worker is involved in any projects
        if (workers[i]->projectCount > 0) {
            printf("  Projects this worker is involved in:\n");

            // Loop through all projects the worker is involved in
            for (int c = 0; c < workers[i]->projectCount; c++) {
                // Print the name of each project
                printf("    - %s\n", workers[i]->projects[c]->name);
            }
        }
        else {
            // If the worker is not involved in any projects, print a message indicating this
            printf("  Not involved in any projects.\n");
        }
        printf("\n");
    }
}


//******************************************************************** Option 6 - Work on Projcets - add/remove Feauters ******************************************
// Function to add or remove features from a project
void workOnProject(Worker* workers[], int workerCount, int projectCount) {
    // Check if there are any workers/projects in the company
    if (workerCount <= 0) {
        printf("There are no workers in the company yet, please join the company first.\n");
        return;
    }
    if (projectCount <= 0) {
        printf("There are no projects in the company yet, please create a project first.\n");
        return;
    }

    printf("Select a worker:\n");
    displayWorkers(workers, workerCount);
    printf("Enter the worker's number: ");

    int workerNumber;
    scanf("%d", &workerNumber);

    // Check if the selected worker is within the valid range
    if (workerNumber < 1 || workerNumber > workerCount) {
        printf("Invalid worker selection.\n");
        return;
    }

    // Retrieve the selected worker
    Worker* selectedWorker = workers[workerNumber - 1];

    if (selectedWorker->projectCount <= 0) {
        printf("This worker is not involved in any projects.\n");
        return;
    }

    printf("Select a project:\n");
    displayProjects(selectedWorker->projects, selectedWorker->projectCount, WORK_ON_PROJECT);
    printf("Enter the project number: ");

    int projectNumber;
    scanf("%d", &projectNumber);

    // Check if the selected project is within the valid range
    if (projectNumber < 1 || projectNumber > selectedWorker->projectCount) {
        printf("Invalid project selection.\n");
        return;
    }

    // Retrieve the selected project
    Project* selectedProject = selectedWorker->projects[projectNumber - 1];

    printf("Do you want to add or remove a feature? (add/remove): ");

    char yourChoice[10]; // Allocate a buffer for the choice
    scanf("%9s", yourChoice); // Read into the buffer, leaving room for the null character
    yourChoice[9] = '\0'; // Manually set the last character to null


    if (strcmp(yourChoice, "add") == 0) {
        addFeatureToProject(selectedProject);
    }
    else if (strcmp(yourChoice, "remove") == 0) {
        removeFeatureFromProject(selectedProject);
    }
    else {
        // If the user enters an invalid choice, print an error message
        printf("Invalid choice.\n\n");
    }
}
// Function to add feature to a project
void addFeatureToProject(Project* project) {
    printf("Enter the new feature: ");
    char* newFeature = dynamicString();

    // Check if the feature name is not empty
    if (isStringEmpty(newFeature)) {
        printf("Invalid name\n");
        free(newFeature);
        return;
    }

    // Check if the feature already exists in the project
    if (isFeatureExists(project->features, newFeature)) {
        free(newFeature);  // Free the allocated memory for newFeature
        return;  // Do not proceed with adding the feature
    }

    // Allocate memory for a new FeatureNode
    FeatureNode* newFeatureNode = (FeatureNode*)malloc(sizeof(FeatureNode));
    if (newFeatureNode == NULL) {
        printf("Error: Memory allocation failed.\n");
        free(newFeature);
        exit(1);
    }

    // Set the feature data for the new FeatureNode
    newFeatureNode->feature = newFeature;
    newFeatureNode->next = NULL;  // Set the next pointer to NULL as it will be the last node

    // If the features list is empty, set the new node as the head
    if (project->features == NULL) {
        project->features = newFeatureNode;
    }
    else {
        // Otherwise, traverse the list to find the last node
        FeatureNode* current = project->features;
        while (current->next != NULL) {
            current = current->next;
        }
        // Add the new node after the last node
        current->next = newFeatureNode;
    }
}


// Function to check if a feature already exist in a project
int isFeatureExists(FeatureNode* head, const char* featureName) {
    // Start with the head of the feature list
    FeatureNode* currentFeature = head;

    // Loop through all features in the list
    while (currentFeature != NULL) {
        // If the current feature's name matches the given feature name
        if (strcmp(currentFeature->feature, featureName) == 0) {
            // Print a message indicating that the feature already exists
            printf("This feature already exists in the project.\n");
            // Return 1 to indicate that the feature exists
            return 1;
        }
        // Move to the next feature in the list
        currentFeature = currentFeature->next;
    }
    // If no matching feature was found, return 0 to indicate that the feature does not exist
    return 0;
}



// Function to remove a feature from a project
void removeFeatureFromProject(Project* project) {
    // Check if there are any features to remove
    if (project->features == NULL) {
        printf("There are no features to remove.\n");
        return;
    }

    // Start with the head of the feature list
    FeatureNode* currentFeature = project->features;

    // Ask the user to select a feature to remove
    printf("Select a feature to remove:\n");

    // Count the number of features in the list
    int featuresCount = 0;
    while (currentFeature != NULL) {
        featuresCount++;
        // Print each feature with its corresponding number
        printf("%d. %s\n", featuresCount, currentFeature->feature);
        // Move to the next feature in the list
        currentFeature = currentFeature->next;
    }

    // Ask the user to select a feature to remove
    printf("Enter your choice: ");

    int removeChoice;
    scanf("%d", &removeChoice);

    // Check if the selected feature is within the valid range
    if (removeChoice < 1 || removeChoice > featuresCount) {
        printf("Invalid selection.\n");
        return;
    }

    // Reset the currentFeature pointer to the head of the features list
    currentFeature = project->features;

    // If there is only one feature in the list
    if (project->features->next == NULL) {
        // Free the memory for the feature string and the feature node
        free(project->features->feature);
        free(project->features);
        // Set the head to NULL as there are no features left
        project->features = NULL;
        return;
    }


    // If the user wants to remove the first feature
    if (removeChoice == 1) {
        // Update the head to the next feature
        project->features = currentFeature->next;
        // Free the memory of the feature string and the removed feature
        free(currentFeature->feature);
        free(currentFeature);
        return;
    }

    // If the user wants to remove the last feature
    if (removeChoice == featuresCount) {
        // Move the currentFeature pointer to the second-to-last feature
        for (int i = 1; i < featuresCount - 1; i++) {
            currentFeature = currentFeature->next;
        }
        // Free the memory of the last feature
        free(currentFeature->next->feature);
        free(currentFeature->next);
        // Set the next pointer of the second-to-last feature to NULL
        currentFeature->next = NULL;
        return;
    }

    // If the user wants to remove a feature other than the first and the last
    for (int i = 1; i < removeChoice; i++) {
        // Move the currentFeature pointer to the feature before the one to be removed
        currentFeature = currentFeature->next;
    }

    // If the currentFeature pointer is NULL, return
    if (currentFeature == NULL) return;

    // Point to the feature to be removed
    FeatureNode* temp = currentFeature->next;
    // Update the next pointer to skip the removed feature
    currentFeature->next = temp->next;
    // Free the memory of the removed feature
    free(temp->feature);
    free(temp);
}


//******************************************************************** Option 7 - Leave the Company ******************************************
// Function to remove a worker from the company and update the projects and workers arrays accordingly
void leaveCompany(Worker* workers[], int* workerCount, Project* projects[], int* projectCount) {
    // Check if there are any workers in the company
    if (*workerCount <= 0) {
        printf("There are no workers in the company yet, please join the company first.\n");
        return;
    }
    // Display all workers and ask the user to select a worker to leave the company
    printf("Select a worker to leave the company: \n");
    displayWorkers(workers, *workerCount);
    printf("Enter the worker's number: ");

    int workerNumber;
    scanf("%d", &workerNumber);

    // Check if the selected worker is within the valid range
    if (workerNumber < 1 || workerNumber > *workerCount) {
        printf("Invalid worker selection.\n");
        return;
    }

    // Retrieve the selected worker
    Worker* selectedWorker = workers[workerNumber - 1];

    // If the worker is involved in any projects
    if (selectedWorker->projectCount > 0) {
        // Iterate over each project the worker is involved in
        for (int c = 0; c < selectedWorker->projectCount; c++) {
            // Find the worker in the project's workers list and remove them
            for (int j = 0; j < selectedWorker->projects[c]->workerCount; j++) {
                // If the worker is found in the project's workers list
                if (selectedWorker->projects[c]->workers[j] == selectedWorker) {
                    // Shift all subsequent workers in the list to fill the gap left by the removed worker
                    for (int k = j; k < selectedWorker->projects[c]->workerCount - 1; k++) {
                        selectedWorker->projects[c]->workers[k] = selectedWorker->projects[c]->workers[k + 1];
                    }
                    // Decrease the count of workers in the project
                    selectedWorker->projects[c]->workerCount--;
                    break;
                }
            }
            // If the project has only one worker (the one to be removed)
            if (selectedWorker->projects[c]->workerCount == 0) {
                // If the project has any features, free them
                if (selectedWorker->projects[c]->features != 0) {
                    freeFeatures(selectedWorker->projects[c]->features);
                    selectedWorker->projects[c]->features = NULL;
                }
                // Free the project
                freeProject(selectedWorker->projects[c]);
                // Find the project in the global projects list and remove it
                for (int i = 0; i < *projectCount; ++i) {
                    if (projects[i] == selectedWorker->projects[c]) {
                        // Shift all subsequent projects in the list to fill the gap left by the removed project
                        for (int p = i; p < *projectCount - 1; ++p) {
                            projects[p] = projects[p + 1];
                        }
                    }
                }
                // Decrease the count of projects
                (*projectCount)--;
            }
        }
        // Free the memory allocated for the worker's projects
        free(selectedWorker->projects);
    }

    // Free the selected worker's name and the worker itself
    free(selectedWorker->name);
    free(selectedWorker);

    // Update the workers array after removing the worker
    for (int i = workerNumber - 1; i < *workerCount - 1; ++i) {
        workers[i] = workers[i + 1];
    }
    (*workerCount)--;
}


// Function to free the memory allocated for the features linked list
void freeFeatures(FeatureNode* head) {
    // Start with the head of the features linked list
    FeatureNode* currentFeature = head;
    // Loop through all features in the list
    while (currentFeature != NULL) {
        // Store the current feature in a temporary variable
        FeatureNode* temp = currentFeature;
        // Move the currentFeature pointer to the next feature
        currentFeature = currentFeature->next;
        // Free the memory allocated for the feature string
        free(temp->feature);
        // Free the memory allocated for the feature node
        free(temp);
    }
}

// Function to free the memory allocated for a project
void freeProject(Project* project) {
    // Free the memory allocated for the project name
    free(project->name);

    // Free the memory allocated for the array of worker pointers
    free(project->workers);

    // Free the memory allocated for the project itself
    free(project);
}
//******************************************************************** Option 8 - EXIST  ******************************************
// Function to free all dynamically allocated memory for workers and projects
void freeAllAllocations(Worker* workers[], int workerCount, Project* projects[], int projectCount) {
    // Loop through all projects
    for (int i = 0; i < projectCount; i++) {
        // Check if the project is not NULL
        if (projects[i] != NULL) {
            // If the project has features, free them
            if (projects[i]->features != NULL) {
                freeFeatures(projects[i]->features);
            }
            // Free the project
            freeProject(projects[i]);
        }
    }

    // Loop through all workers
    for (int i = 0; i < workerCount; i++) {
        // Check if the worker is not NULL
        if (workers[i] != NULL) {
            // Free the worker's name
            free(workers[i]->name);
            // Free the worker's projects
            free(workers[i]->projects);
            // Free the worker
            free(workers[i]);
        }
    }
}


