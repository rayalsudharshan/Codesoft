#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

typedef struct {
    int id;
    char name[50];
    char password[50];
    int role; // 0 for student, 1 for instructor
} User;

typedef struct {
    int id;
    char name[50];
    User *instructor;
} Course;

typedef struct {
    int id;
    int student_id;
    int course_id;
    int status; // 0 for absent, 1 for present
} AttendanceRecord;

// Database functions
void create_database();
void create_user_table();
void create_course_table();
void create_attendance_table();

// User functions
User *register_user(const char *name, const char *password, int role);
User *login_user(const char *name, const char *password);

// Course functions
Course *create_course(const char *name, User *instructor);
void add_student_to_course(Course *course, int student_id);
void remove_student_from_course(Course *course, int student_id);

// Attendance functions
void mark_attendance(AttendanceRecord *attendance_record);
void view_attendance_record(AttendanceRecord *attendance_record);

// Error handling functions
void handle_error(const char *message);

int main() {
    // Initialize the database
    create_database();

    // Register a user
    User *user = register_user("John Doe", "password123", 0);
    if (!user) {
        handle_error("Failed to register user.");
        return 1;
    }

    // Login as the user
    User *logged_in_user = login_user("John Doe", "password123");
    if (!logged_in_user) {
        handle_error("Failed to login user.");
        return 1;
    }

    // Create a course
    Course *course = create_course("Introduction to Computer Science", logged_in_user);
    if (!course) {
        handle_error("Failed to create course.");
        return 1;
    }

    // Add a student to the course
    add_student_to_course(course, user->id);

    // Mark attendance for a student
    AttendanceRecord *attendance_record = malloc(sizeof(AttendanceRecord));
    attendance_record->student_id = user->id;
    attendance_record->course_id = course->id;
    mark_attendance(attendance_record);

    // View attendance record for a student
    view_attendance_record(attendance_record);

    // Clean up
    free(user);
    free(logged_in_user);
    free(course);
    free(attendance_record);

    return 0;
}

void create_database() {
    sqlite3 *db;
    int rc = sqlite3_open("attendance.db", &db);
    if (rc) {
        handle_error("Can't open database:");
        sqlite3_close(db);
        exit(1);
    }

    create_user_table(db);
    create_course_table(db);
    create_attendance_table(db);

    sqlite3_close(db);
}

void create_user_table(sqlite3 *db) {
    char *sql = "CREATE TABLE IF NOT EXISTS users ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "name TEXT NOT NULL,"
                "password TEXT NOT NULL,"
                "role INTEGER NOT NULL);";

    char *err_msg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        handle_error("Failed to create user table.");
    }
}

void create_course_table(sqlite3 *db) {
    char *sql = "CREATE