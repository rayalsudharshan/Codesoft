#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#define SQLITE_DB "todo.db"

typedef struct {
    int id;
    char title[100];
    char description[200];
    int priority;
    char due_date[20];
    int completed;
} Task;

void create_database() {
    sqlite3 *db;
    int rc = sqlite3_open(SQLITE_DB, &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return;
    }
    char *sql = "CREATE TABLE IF NOT EXISTS tasks ("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                 "title TEXT NOT NULL,"
                 "description TEXT,"
                 "priority INTEGER,"
                 "due_date TEXT,"
                 "completed INTEGER);";
    rc = sqlite3_exec(db, sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
    }
    sqlite3_close(db);
}

void add_task(const char *title, const char *description, int priority, const char *due_date, int completed) {
    sqlite3 *db;
    int rc = sqlite3_open(SQLITE_DB, &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return;
    }
    char *sql = "INSERT INTO tasks (title, description, priority, due_date, completed) VALUES (?, ?, ?, ?, ?);";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }
    sqlite3_bind_text(stmt, 1, title, -1, NULL);
    sqlite3_bind_text(stmt, 2, description, -1, NULL);
    sqlite3_bind_int(stmt, 3, priority);
    sqlite3_bind_text(stmt, 4, due_date, -1, NULL);
    sqlite3_bind_int(stmt, 5, completed);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void edit_task(int id, const char *title, const char *description, int priority, const char *due_date, int completed) {
    sqlite3 *db;
    int rc = sqlite3_open(SQLITE_DB, &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return;
    }
    char *sql = "UPDATE tasks SET title = ?, description = ?, priority = ?, due_date = ?, completed = ? WHERE id = ?;";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }
    sqlite3_bind_text(stmt, 1, title, -1, NULL);
    sqlite3_bind_text(stmt, 2, description, -1, NULL);
    sqlite3_bind_int(stmt, 3, priority);