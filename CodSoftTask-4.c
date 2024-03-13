#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int id;
    char *question;
    char *options[4];
    int correct_answer;
} Question;

typedef struct {
    int score;
    int total_questions;
    int correct_answers;
} QuizResult;

Question questions[] = {
    {1, "What is the capital of France?", {"Paris", "Berlin", "London", "Madrid"}, 0},
    {2, "Which planet is closest to the sun?", {"Mercury", "Venus", "Earth", "Mars"}, 0},
    {3, "What is the largest mammal in the world?", {"Blue whale", "Elephant", "Hippopotamus", "Rhinoceros"}, 0},
    {4, "Which language is the most spoken in the world?", {"English", "Mandarin", "Spanish", "Hindi"}, 0},
    {5, "What is the smallest country in the world?", {"Vatican City", "Monaco", "San Marino", "Liechtenstein"}, 0}
};

QuizResult quiz_result = {0, 0, 0};

void display_question(Question *question) {
    printf("%s\n", question->question);
    for (int i = 0; i < 4; i++) {
        printf("%d. %s\n", i + 1, question->options[i]);
    }
}

int check_answer(Question *question, int answer) {
    if (answer == question->correct_answer + 1) {
        quiz_result.score += 10;
        quiz_result.correct_answers += 1;
        return 1;
    }
    return 0;
}

void display_result() {
    printf("Quiz Result:\n");
    printf("Score: %d\n", quiz_result.score);
    printf("Total Questions: %d\n", quiz_result.total_questions);
    printf("Correct Answers: %d\n", quiz_result.correct_answers);
}

int main() {
    srand(time(NULL));

    int num_questions = sizeof(questions) / sizeof(Question);
    quiz_result.total_questions = num_questions;

    for (int i = 0; i < num_questions; i++) {
        Question *question = &questions[i];
        display_question(question);

        int answer;
        scanf("%d", &answer);

        if (answer < 1 || answer > 4) {
            printf("Invalid answer. Please try again.\n");
            i--;
            continue;
        }

        if (check_answer(question, answer)) {
            printf("Correct!\n");
        } else {
            printf("Incorrect. The correct answer is %s.\n", question->options[question->correct_answer]);
        }
    }

    display_result();

    return 0;
}
