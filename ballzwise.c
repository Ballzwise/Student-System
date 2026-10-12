
    /* ============================================================
   STUDENT MANAGEMENT SYSTEM
   A console-based program to add, view, search, update, and
   delete student records, with save/load to a file.
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---------- Configuration ---------- */
#define MAX_STUDENTS 100
#define NAME_LEN 50
#define GENDER_LEN 10
#define DEPT_LEN 50
#define FILENAME "students.dat"

/* ---------- Student record structure ---------- */
typedef struct {
    int id;
    char name[NAME_LEN];
    int age;
    char gender[GENDER_LEN];
    char department[DEPT_LEN];
    int level;
    float gpa;
} Student;

/* ---------- Global storage ----------
   A simple array holds all students while the program runs.
   'studentCount' tracks how many slots are currently in use. */
Student students[MAX_STUDENTS];
int studentCount = 0;

/* ---------- Function prototypes ---------- */
void showMenu(void);
void addStudent(void);
void viewStudents(void);
void searchStudent(void);
void updateStudent(void);
void deleteStudent(void);
int findStudentIndexById(int id);
void clearInputBuffer(void);
int readIntSafely(const char *prompt);
float readFloatSafely(const char *prompt);
void loadStudentsFromFile(void);
void saveStudentsToFile(void);

/* ============================================================
   main()
   Loads any previously saved students, then repeatedly shows
   the menu and performs the action the user selects, until
   the user chooses Exit (option 6).
   ============================================================ */
int main(void) {
    loadStudentsFromFile();

    int choice;
    int running = 1;

    while (running) {
        showMenu();
        choice = readIntSafely("Enter your choice: ");

        switch (choice) {
            case 1:
                addStudent();
                break;
            case 2:
                viewStudents();
                break;
            case 3:
                searchStudent();
                break;
            case 4:
                updateStudent();
                break;
            case 5:
                deleteStudent();
                break;
            case 6:
                saveStudentsToFile();
                printf("\nThank you for using the Student Management System.\n");
                running = 0;
                break;
            default:
                printf("\nInvalid choice. Please enter a number between 1 and 6.\n");
                break;
        }
    }

    return 0;
}

/* ============================================================
   showMenu()
   Prints the main menu options.
   ============================================================ */
void showMenu(void) {
    printf("\n====================================\n");
    printf("      STUDENT MANAGEMENT SYSTEM\n");
    printf("====================================\n");
    printf("1. Add Student\n");
    printf("2. View All Students\n");
    printf("3. Search Student\n");
    printf("4. Update Student\n");
    printf("5. Delete Student\n");
    printf("6. Exit\n");
}

/* ============================================================
   clearInputBuffer()
   Clears leftover characters (like a stray newline) from stdin
   after a scanf call, so the next input isn't corrupted.
   ============================================================ */
void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* discard */
    }
}

/* ============================================================
   readIntSafely()
   Prompts for an integer and keeps asking until the user
   enters a valid whole number. Protects the program from
   crashing on bad input (e.g. letters typed instead of digits).
   ============================================================ */
int readIntSafely(const char *prompt) {
    int value;
    int result;

    printf("%s", prompt);
    result = scanf("%d", &value);

    while (result != 1) {
        clearInputBuffer();
        printf("Invalid input. Please enter a whole number: ");
        result = scanf("%d", &value);
    }
    clearInputBuffer();
    return value;
}

/* ============================================================
   readFloatSafely()
   Same idea as readIntSafely(), but for decimal numbers (GPA).
   ============================================================ */
float readFloatSafely(const char *prompt) {
    float value;
    int result;

    printf("%s", prompt);
    result = scanf("%f", &value);

    while (result != 1) {
        clearInputBuffer();
        printf("Invalid input. Please enter a number (e.g. 3.75): ");
        result = scanf("%f", &value);
    }
    clearInputBuffer();
    return value;
}

/* ============================================================
   findStudentIndexById()
   Searches the students array for a matching ID.
   Returns the array index if found, or -1 if not found.
   Used by search, update, and delete so we don't repeat the
   same search loop three times.
   ============================================================ */
int findStudentIndexById(int id) {
    for (int i = 0; i < studentCount; i++) {
        if (students[i].id == id) {
            return i;
        }
    }
    return -1;
}

/* ============================================================
   addStudent()
   Collects a new student's details from the user and stores
   them in the next free slot in the array.
   ============================================================ */
void addStudent(void) {
    if (studentCount >= MAX_STUDENTS) {
        printf("\nStorage full. Cannot add more students.\n");
        return;
    }

    Student newStudent;

    printf("\n--- Add Student ---\n");
    newStudent.id = readIntSafely("Enter Student ID: ");

    /* Prevent duplicate IDs */
    if (findStudentIndexById(newStudent.id) != -1) {
        printf("A student with this ID already exists.\n");
        return;
    }

    printf("Enter Full Name: ");
    fgets(newStudent.name, NAME_LEN, stdin);
    newStudent.name[strcspn(newStudent.name, "\n")] = '\0'; /* strip newline */

    newStudent.age = readIntSafely("Enter Age: ");

    printf("Enter Gender: ");
    fgets(newStudent.gender, GENDER_LEN, stdin);
    newStudent.gender[strcspn(newStudent.gender, "\n")] = '\0';

    printf("Enter Department: ");
    fgets(newStudent.department, DEPT_LEN, stdin);
    newStudent.department[strcspn(newStudent.department, "\n")] = '\0';

    newStudent.level = readIntSafely("Enter Level (e.g. 100, 200, 300): ");
    newStudent.gpa = readFloatSafely("Enter GPA: ");

    students[studentCount] = newStudent;
    studentCount++;

    saveStudentsToFile();
    printf("\nStudent added successfully.\n");
}

/* ============================================================
   viewStudents()
   Displays every stored student in a table.
   ============================================================ */
void viewStudents(void) {
    printf("\n--- All Students ---\n");

    if (studentCount == 0) {
        printf("No students registered yet.\n");
        return;
    }

    printf("%-6s %-20s %-5s %-6s %-20s %-6s %-5s\n",
           "ID", "Name", "Age", "Level", "Department", "GPA", "Gen.");
    printf("--------------------------------------------------------------------------\n");

    for (int i = 0; i < studentCount; i++) {
        printf("%-6d %-20s %-5d %-6d %-20s %-5.2f %-5s\n",
               students[i].id,
               students[i].name,
               students[i].age,
               students[i].level,
               students[i].department,
               students[i].gpa,
               students[i].gender);
    }
}

/* ============================================================
   searchStudent()
   Asks for an ID and, if found, prints that student's full
   details. Otherwise reports "Student not found."
   ============================================================ */
void searchStudent(void) {
    printf("\n--- Search Student ---\n");
    int id = readIntSafely("Enter Student ID: ");

    int index = findStudentIndexById(id);

    if (index == -1) {
        printf("Student not found.\n");
        return;
    }

    Student s = students[index];
    printf("\nStudent Found:\n");
    printf("ID:         %d\n", s.id);
    printf("Name:       %s\n", s.name);
    printf("Age:        %d\n", s.age);
    printf("Gender:     %s\n", s.gender);
    printf("Department: %s\n", s.department);
    printf("Level:      %d\n", s.level);
    printf("GPA:        %.2f\n", s.gpa);
}

/* ============================================================
   updateStudent()
   Asks for an ID, and if the student exists, lets the user
   change every field except the Student ID itself.
   ============================================================ */
void updateStudent(void) {
    printf("\n--- Update Student ---\n");
    int id = readIntSafely("Enter Student ID: ");

    int index = findStudentIndexById(id);

    if (index == -1) {
        printf("Student not found.\n");
        return;
    }

    printf("Student found. Enter new details below.\n");

    printf("Enter new Name: ");
    fgets(students[index].name, NAME_LEN, stdin);
    students[index].name[strcspn(students[index].name, "\n")] = '\0';

    students[index].age = readIntSafely("Enter new Age: ");

    printf("Enter new Department: ");
    fgets(students[index].department, DEPT_LEN, stdin);
    students[index].department[strcspn(students[index].department, "\n")] = '\0';

    students[index].level = readIntSafely("Enter new Level: ");
    students[index].gpa = readFloatSafely("Enter new GPA: ");

    saveStudentsToFile();
    printf("\nStudent record updated successfully.\n");
}

/* ============================================================
   deleteStudent()
   Asks for an ID, confirms with the user, then removes that
   student by shifting all later records one place left.
   ============================================================ */
void deleteStudent(void) {
    printf("\n--- Delete Student ---\n");
    int id = readIntSafely("Enter Student ID: ");

    int index = findStudentIndexById(id);

    if (index == -1) {
        printf("Student not found.\n");
        return;
    }

    printf("Are you sure you want to delete this student? (Y/N): ");
    char confirm;
    scanf(" %c", &confirm);
    clearInputBuffer();

    if (confirm == 'Y' || confirm == 'y') {
        for (int i = index; i < studentCount - 1; i++) {
            students[i] = students[i + 1];
        }
        studentCount--;
        saveStudentsToFile();
        printf("Student deleted successfully.\n");
    } else {
        printf("Deletion cancelled.\n");
    }
}

/* ============================================================
   saveStudentsToFile()
   Writes the entire students array to a binary file so the
   data survives after the program closes.
   ============================================================ */
void saveStudentsToFile(void) {
    FILE *file = fopen(FILENAME, "wb");

    if (file == NULL) {
        printf("Error: could not open file to save data.\n");
        return;
    }

    fwrite(&studentCount, sizeof(int), 1, file);
    fwrite(students, sizeof(Student), studentCount, file);

    fclose(file);
}

/* ============================================================
   loadStudentsFromFile()
   Reads previously saved student records back into memory
   when the program starts. If no file exists yet (first run),
   the program just starts with an empty list.
   ============================================================ */
void loadStudentsFromFile(void) {
    FILE *file = fopen(FILENAME, "rb");

    if (file == NULL) {
        /* No saved data yet — that's fine on first run. */
        studentCount = 0;
        return;
    }

    fread(&studentCount, sizeof(int), 1, file);
    fread(students, sizeof(Student), studentCount, file);

    fclose(file);
}
