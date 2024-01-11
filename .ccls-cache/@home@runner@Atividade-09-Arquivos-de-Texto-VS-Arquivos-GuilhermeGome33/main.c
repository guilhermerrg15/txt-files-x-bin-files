#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

typedef struct task {
    int id;
    char title[50];
    int status;
} Task;

// Function prototypes
int generateRandomId();
void listTasksText(const char *filename);
void addTaskText(const char *filename, Task task);
void updateTaskText(const char *filename, Task task);
void deleteTaskText(const char *filename, int taskId);
void listTasksBinary(const char *filename);
void addTaskBinary(const char *filename, Task task);
void updateTaskBinary(const char *filename, int taskId, Task updatedTask);
void deleteTaskBinary(const char *filename, int taskId);

int main() {
    srand(time(NULL));

    // Test cases for text file operations
    const char *textFileName = "tasks.txt";
    printf("Text File Operations:\n");

    // Test case 1: List tasks from the text file
    listTasksText(textFileName);

    Task newTask1 = {generateRandomId(), "Task done", 1};
    Task updatedTask1 = {generateRandomId(), "Task 1 Updated", 1};

    // Test case 2: Add a new task to the text file
    addTaskText(textFileName, newTask1);

    // Test case 3: List tasks after addition in text file
    listTasksText(textFileName);

    // Test case 4: Update a task in the text file
    updateTaskText(textFileName, updatedTask1);

    // Test case 5: Delete a task from the text file
    deleteTaskText(textFileName, updatedTask1.id);

    // Test case 6: List tasks after deletion in text file
    listTasksText(textFileName);

    // Test cases for binary file operations
    const char *binaryFileName = "tasks_bin.bat";
    printf("\nBinary File Operations:\n");

    // Test case 7: List tasks from the binary file
    listTasksBinary(binaryFileName);

    Task newTask2 = {generateRandomId(), "Task 4", 0};
    Task updatedTask2 = {generateRandomId(), "Task 2 Updated", 0};

    // Test case 8: Add a new task to the binary file
    addTaskBinary(binaryFileName, newTask2);

    // Test case 9: Update a task in the binary file
    updateTaskBinary(binaryFileName, updatedTask2.id, updatedTask2);

    // Test case 10: Delete a task from the binary file
    deleteTaskBinary(binaryFileName, updatedTask2.id);

    return 0;
}

// Function to generate a random ID
int generateRandomId() {
    return rand();
}

// Function to list all tasks from the text file and measure the time spent
void listTasksText(const char *filename) {
    clock_t start = clock();

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    Task task;
    while (fscanf(file, "%d,%49[^,],%d\n", &task.id, task.title, &task.status) == 3) {
        printf("ID: %d, Title: %s, Status: %s\n", task.id, task.title, task.status ? "Done" : "Not done");
    }

    fclose(file);

    clock_t end = clock();
    double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time spent listing tasks in text file: %f seconds\n", elapsed_time);
}

// Function to add a new task to the text file and measure the time spent
void addTaskText(const char *filename, Task task) {
    clock_t start = clock();

    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    task.id = generateRandomId(); // Generate a random ID for the task

    fprintf(file, "%d,%s,%d\n", task.id, task.title, task.status);
    fclose(file);

    clock_t end = clock();
    double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time spent adding task to text file: %f seconds\n", elapsed_time);
}

// Function to update a task in the text file and measure the time spent
void updateTaskText(const char *filename, Task task) {
    clock_t start = clock();

    FILE *file = fopen(filename, "r+");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    Task currentTask;
    while (fscanf(file, "%d,%49[^,],%d\n", &currentTask.id, currentTask.title, &currentTask.status) == 3) {
        if (currentTask.id == task.id) {
            fseek(file, -strlen(currentTask.title) - 5, SEEK_CUR);
            fprintf(file, "%d,%s,%d", task.id, task.title, task.status);
            break;
        }
    }

    fclose(file);

    clock_t end = clock();
    double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time spent updating task in text file: %f seconds\n", elapsed_time);
}

// Function to delete a task from the text file and measure the time spent
void deleteTaskText(const char *filename, int taskId) {
    clock_t start = clock();

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        perror("Error opening temporary file");
        fclose(file);
        return;
    }

    Task task;
    int found = 0;
    while (fscanf(file, "%d,%49[^,],%d\n", &task.id, task.title, &task.status) == 3) {
        if (task.id == taskId) {
            found = 1;
        } else {
            fprintf(tempFile, "%d,%s,%d\n", task.id, task.title, task.status);
        }
    }

    fclose(file);
    fclose(tempFile);

    if (found) {
        remove(filename);
        rename("temp.txt", filename);
    } else {
        remove("temp.txt");
        printf("Task with ID %d not found.\n", taskId);
    }

    clock_t end = clock();
    double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time spent deleting task in text file: %f seconds\n", elapsed_time);
}

// Function to list all tasks from the binary file and measure the time spent
void listTasksBinary(const char *filename) {
    clock_t start = clock();

    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    Task task;
    while (fread(&task, sizeof(Task), 1, file) == 1) {
        printf("ID: %d, Title: %s, Status: %s\n", task.id, task.title, task.status ? "Completed" : "Pending");
    }

    fclose(file);

    clock_t end = clock();
    double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time spent listing tasks in binary file: %f seconds\n", elapsed_time);
}

// Function to add a new task to the binary file and measure the time spent
void addTaskBinary(const char *filename, Task task) {
    clock_t start = clock();

    FILE *file = fopen(filename, "ab");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    task.id = generateRandomId(); // Generate a random ID for the task

    fwrite(&task, sizeof(Task), 1, file);
    fclose(file);

    clock_t end = clock();
    double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time spent adding task to binary file: %f seconds\n", elapsed_time);
}

// Function to update a task in the binary file and measure the time spent
void updateTaskBinary(const char *filename, int taskId, Task updatedTask) {
    clock_t start = clock();

    FILE *file = fopen(filename, "r+b");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    Task task;
    int found = 0;
    while (fread(&task, sizeof(Task), 1, file) == 1) {
        if (task.id == taskId) {
            fseek(file, -sizeof(Task), SEEK_CUR);
            fwrite(&updatedTask, sizeof(Task), 1, file);
            found = 1;
            break;
        }
    }

    fclose(file);

    if (!found) {
        printf("Task with ID %d not found.\n", taskId);
    }

    clock_t end = clock();
    double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time spent updating task in binary file: %f seconds\n", elapsed_time);
}

// Function to delete a task from the binary file and measure the time spent
void deleteTaskBinary(const char *filename, int taskId) {
    clock_t start = clock();

    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    FILE *tempFile = fopen("temp.bin", "wb");
    if (tempFile == NULL) {
        perror("Error opening temporary file");
        fclose(file);
        return;
    }

    Task task;
    int found = 0;
    while (fread(&task, sizeof(Task), 1, file) == 1) {
        if (task.id == taskId) {
            found = 1;
        } else {
            fwrite(&task, sizeof(Task), 1, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    if (found) {
        remove(filename);
        rename("temp.bin", filename);
    } else {
        remove("temp.bin");
        printf("Task with ID %d not found.\n", taskId);
    }

    clock_t end = clock();
    double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time spent deleting task in binary file: %f seconds\n", elapsed_time);
}
