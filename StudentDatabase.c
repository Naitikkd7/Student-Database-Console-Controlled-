#include <stdio.h>
#include <string.h>
 
#define MAX_STUDENTS 100
#define DATA_FILE "students.dat"
 
void addstudent();
void addstudentrecord();
void addsearch();
void adddelete();
void loadFromFile();
void saveToFile();
 
typedef struct Student{
    char first_name[50];
    char last_name[50];
    char contact_number[15];
    int Roll_number;
    int section;
    float CGPA;
    char email[50];
    char city[50];
} Student;
 
Student students[MAX_STUDENTS];
int studentCount = 0;
 
int main(){
    int choice = 0;
 
    loadFromFile();
 
    printf("========Welcome to the Student Database Management System========\n");
    while (choice != 5){
        printf("\n1. Add Student\n");
        printf("2. Search Student\n");
        printf("3. Delete Student\n");
        printf("4. Display All Students\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
 
        if (scanf("%d", &choice) != 1){
            /* clear bad input so we don't loop forever */
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        printf("--------------------------------------------------------------\n");
 
        switch (choice) {
            case 1:
                addstudent();
                break;
            case 2:
                addsearch();
                break;
            case 3:
                adddelete();
                break;
            case 4:
                addstudentrecord();
                break;
            case 5:
                printf("Exiting the program.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
 
    return 0;
}
 
/* Reads leftover newline left in the input buffer by scanf("%d", ...)
   so the following fgets() calls don't read an empty line. */
void clearInputBuffer(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
 
void stripNewline(char *str){
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n'){
        str[len - 1] = '\0';
    }
}
 
void addstudent(){
    if (studentCount >= MAX_STUDENTS){
        printf("Student database is full. Cannot add more students.\n");
        return;
    }
 
    Student s;
 
    printf("Enter first name: ");
    clearInputBuffer();
    fgets(s.first_name, sizeof(s.first_name), stdin);
    stripNewline(s.first_name);
 
    printf("Enter last name: ");
    fgets(s.last_name, sizeof(s.last_name), stdin);
    stripNewline(s.last_name);
 
    printf("Enter contact number: ");
    fgets(s.contact_number, sizeof(s.contact_number), stdin);
    stripNewline(s.contact_number);
 
    printf("Enter roll number: ");
    scanf("%d", &s.Roll_number);
 
    /* prevent duplicate roll numbers */
    for (int i = 0; i < studentCount; i++){
        if (students[i].Roll_number == s.Roll_number){
            printf("A student with roll number %d already exists. Student not added.\n", s.Roll_number);
            return;
        }
    }
 
    printf("Enter section: ");
    scanf("%d", &s.section);
 
    printf("Enter CGPA: \n");
    scanf("%f", &s.CGPA);
 
    clearInputBuffer();
    printf("Enter email: ");
    fgets(s.email, sizeof(s.email), stdin);
    stripNewline(s.email);
 
    printf("Enter city: ");
    fgets(s.city, sizeof(s.city), stdin);
    stripNewline(s.city);
 
    students[studentCount] = s;
    studentCount++;
    saveToFile();
 
    printf("Student added successfully.\n");
}
 
void printStudentHeader(){
    printf("%-6s %-12s %-12s %-14s %-8s %-6s %-30s %-15s\n",
           "Roll", "First", "Last", "Contact", "Section", "CGPA", "Email", "City");
    printf("--------------------------------------------------------------------------------------------------\n");
}
 
void printStudent(Student *s){
    printf("%-6d %-12s %-12s %-14s %-8d %-6.2f %-30s %-15s\n",
           s->Roll_number, s->first_name, s->last_name, s->contact_number,
           s->section, s->CGPA, s->email, s->city);
}
 
void addstudentrecord(){
    if (studentCount == 0){
        printf("No student records to display.\n");
        return;
    }
 
    printf("\n---- All Student Records (%d total) ----\n", studentCount);
    printStudentHeader();
    for (int i = 0; i < studentCount; i++){
        printStudent(&students[i]);
    }
}
 
void addsearch(){
    if (studentCount == 0){
        printf("No student records to search.\n");
        return;
    }
 
    int subChoice;
    printf("Search by:\n1. Roll number\n2. First name\nEnter choice: ");
    scanf("%d", &subChoice);
 
    int found = 0;
 
    if (subChoice == 1){
        int roll;
        printf("Enter roll number to search: ");
        scanf("%d", &roll);
 
        for (int i = 0; i < studentCount; i++){
            if (students[i].Roll_number == roll){
                printStudentHeader();
                printStudent(&students[i]);
                found = 1;
                break;
            }
        }
    } else if (subChoice == 2){
        char name[50];
        printf("Enter first name to search: ");
        clearInputBuffer();
        fgets(name, sizeof(name), stdin);
        stripNewline(name);
 
        printStudentHeader();
        for (int i = 0; i < studentCount; i++){
            if (strcmp(students[i].first_name, name) == 0){
                printStudent(&students[i]);
                found = 1;
            }
        }
    } else {
        printf("Invalid search option.\n");
        return;
    }
 
    if (!found){
        printf("No matching student found.\n");
    }
}
 
void adddelete(){
    if (studentCount == 0){
        printf("No student records to delete.\n");
        return;
    }
 
    int roll;
    printf("Enter roll number of the student to delete: ");
    scanf("%d", &roll);
 
    int index = -1;
    for (int i = 0; i < studentCount; i++){
        if (students[i].Roll_number == roll){
            index = i;
            break;
        }
    }
 
    if (index == -1){
        printf("Student with roll number %d not found.\n", roll);
        return;
    }
 
    /* shift all later records one position back */
    for (int i = index; i < studentCount - 1; i++){
        students[i] = students[i + 1];
    }
    studentCount--;
    saveToFile();
 
    printf("Student with roll number %d deleted successfully.\n", roll);
}
 
void saveToFile(){
    FILE *fp = fopen(DATA_FILE, "wb");
    if (fp == NULL){
        printf("Warning: could not save data to file.\n");
        return;
    }
    fwrite(&studentCount, sizeof(int), 1, fp);
    fwrite(students, sizeof(Student), studentCount, fp);
    fclose(fp);
}
 
void loadFromFile(){
    FILE *fp = fopen(DATA_FILE, "rb");
    if (fp == NULL){
        /* no saved data yet, that's fine on first run */
        return;
    }
    fread(&studentCount, sizeof(int), 1, fp);
    fread(students, sizeof(Student), studentCount, fp);
    fclose(fp);
}


