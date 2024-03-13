#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sqlite3.h>

#define QUOTES_COUNT 10

typedef struct {
    int id;
    char *quote;
    char *author;
} Quote;

Quote quotes[QUOTES_COUNT] = {
    {1, "The purpose of our lives is to be happy.", "Dalai Lama"},
    {2, "Life is what happens when you’re busy making other plans.", "John Lennon"},
    {3, "Get busy living or get busy dying.", "Stephen King"},
    {4, "Many of life’s failures are people who did not realize how close they were to success when they gave up.", "Thomas A. Edison"},
    {5, "If you want to live a happy life, tie it to a goal, not to people or things.", "Albert Einstein"},
    {6, "Many of life’s failures are people who did not realize how close they were to success when they gave up.", "Thomas A. Edison"},
    {7, "The only way to do great work is to love what you do.", "Steve Jobs"},
    {8, "If you set your goals ridiculously high and it’s a failure, you will fail above everyone else’s success.", "James Cameron"},
    {9, "Life is what happens when you’re busy making other plans.", "John Lennon"},
    {10, "Many of life’s failures are people who did not realize how close they were to success when they gave up.", "Thomas A. Edison"}
};

void display_quote(Quote *quote) {
    printf("\"%s\" - %s\n", quote->quote, quote->author);
}

void share_quote(Quote *quote) {
    printf("Sharing quote: \"%s\" - %s\n", quote->quote, quote->author);
}

void favorite_quote(sqlite3 *db, Quote *quote) {
    char *sql;
    sqlite3_stmt *stmt;

    sql = "INSERT INTO favorites (quote_id, quote, author) VALUES (?, ?, ?);";
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, quote->id);
    sqlite3_bind_text(stmt, 2, quote->quote, -1, NULL);
    sqlite3_bind_text(stmt, 3, quote->author, -1, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void view_favorites(sqlite3 *db) {
    char *sql;
    sqlite3_stmt *stmt;

    sql = "SELECT * FROM favorites;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char *quote = (const char *)sqlite3_column_text(stmt, 1);
        const char *author = (const char *)sqlite3_column_text(stmt, 2);
        printf("\"%s\" - %s\n", quote, author);
    }

    sqlite3_finalize(stmt);
}

int main() {
    sqlite3 *db;
    int rc = sqlite3_open("quotes.db", &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    char *sql;
    sqlite3_stmt *stmt;

    sql = "CREATE TABLE IF NOT EXISTS favorites (quote_id INTEGER PRIMARY KEY, quote TEXT, author TEXT);";
    sqlite3_exec(db, sql, NULL, NULL, NULL);

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    int day = tm->tm_mday;