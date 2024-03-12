// I WANT TO PLAY WITH YOU
//         YOUR FRIEND, AI

#define _DEFAULT_SOURCE
#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>

char LeftRacketInput(void);
char RightRacketInput(void);
char RacketInput(char first_symbol, char second_symbol);
char LookForWinner(int ball_info);

int LeftRacketCheck(int ball_info, char left_racket_pos);
int RightRacketCheck(int ball_info, char right_racket_pos);
int Move(int ball_info);
char GetStructedSpeed(char current_ball_velocity);
int UpdateBall(int ball_info, char left_racket_pos, char right_racket_pos);
char GetDataFromInt(int ball_info, char category_number);
int GetIntFromData(char ball_horizontal_velocity, char ball_vertical_velocity, char ball_x_pos,
                   char ball_y_pos);

void PrintField(char left_racket_pos, char right_racket_pos, char ball_y_pos, char ball_x_pos,
                char first_player_score, char second_player_score);

int main(void) {
    char left_racket_pos = 12;
    char right_racket_pos = 12;
    char ball_x_pos = 40;
    char ball_y_pos = 12;
    int ball_info = 114012;  // 1 - HV, 1 - VV, 40 - BallX, 12 - BallY
    char first_player_score = 0;
    char second_player_score = 0;
    char delta_left_racket_pos = 0;
    char delta_right_racket_pos = 0;
    char winner = 0;
    int ch = 0;

    initscr();
    noecho();
    curs_set(FALSE);
    nodelay(stdscr, TRUE);

    while (1) {
        ball_x_pos = GetDataFromInt(ball_info, 3);
        ball_y_pos = GetDataFromInt(ball_info, 4);
        PrintField(left_racket_pos, right_racket_pos, ball_y_pos, ball_x_pos, first_player_score,
                   second_player_score);
        winner = LookForWinner(ball_info);
        if (winner) {
            printw("\nGoal!\n");
            if (winner == 1) {
                second_player_score++;
                if (second_player_score == 21) {
                    printw("\nCongratulations, second player won!\n");
                    break;
                } else {
                    ball_info = 114012;
                    left_racket_pos = 12;
                    right_racket_pos = 12;
                    getch();
                    continue;
                }
            } else if (winner == -1) {
                first_player_score++;
                if (first_player_score == 21) {
                    printw("\nCongratulations, first player won!\n");
                    break;
                } else {
                    ball_info = 884012;
                    left_racket_pos = 12;
                    right_racket_pos = 12;
                    getch();
                    continue;
                }
            }
        }
        usleep(50000);
        delta_left_racket_pos = 0;
        delta_right_racket_pos = 0;
        ch = getch();
        if (ch != ERR) {
            if (ch == 'a') {
                delta_left_racket_pos = -1;
                if (delta_left_racket_pos) {
                    left_racket_pos = ((delta_left_racket_pos > 0 && left_racket_pos < 23)
                                           ? left_racket_pos + delta_left_racket_pos
                                           : ((delta_left_racket_pos < 0 && left_racket_pos > 1)
                                                  ? left_racket_pos + delta_left_racket_pos
                                                  : left_racket_pos));
                }
                delta_left_racket_pos = 0;
            } else if (ch == 'z') {
                delta_left_racket_pos = 1;
                if (delta_left_racket_pos) {
                    left_racket_pos = ((delta_left_racket_pos > 0 && left_racket_pos < 23)
                                           ? left_racket_pos + delta_left_racket_pos
                                           : ((delta_left_racket_pos < 0 && left_racket_pos > 1)
                                                  ? left_racket_pos + delta_left_racket_pos
                                                  : left_racket_pos));
                }
                delta_left_racket_pos = 0;
            } else if (ch == 'k') {
                delta_right_racket_pos = -1;
                if (delta_right_racket_pos) {
                    right_racket_pos = ((delta_right_racket_pos > 0 && right_racket_pos < 23)
                                            ? right_racket_pos + delta_right_racket_pos
                                            : ((delta_right_racket_pos < 0 && right_racket_pos > 1)
                                                   ? right_racket_pos + delta_right_racket_pos
                                                   : right_racket_pos));
                }
                delta_right_racket_pos = 0;
            } else if (ch == 'm') {
                delta_right_racket_pos = 1;
                if (delta_right_racket_pos) {
                    right_racket_pos = ((delta_right_racket_pos > 0 && right_racket_pos < 23)
                                            ? right_racket_pos + delta_right_racket_pos
                                            : ((delta_right_racket_pos < 0 && right_racket_pos > 1)
                                                   ? right_racket_pos + delta_right_racket_pos
                                                   : right_racket_pos));
                }
                delta_right_racket_pos = 0;
            }
        }

        ball_info = UpdateBall(ball_info, left_racket_pos, right_racket_pos);
    }
    refresh();
    getchar();
    endwin();
    return 0;
}

char RacketInput(char first_symbol, char second_symbol) {
    char key = 0;
    char second = 0;
    char res = 2;  // fprintw(stdin, "\n")
    while (1) {
        if (scanf("%c%c", &key, &second) == 2 && second == '\n') {
            if (key == first_symbol) {
                res = -1;
            } else if (key == second_symbol) {
                res = 1;
            } else if (key == ' ') {
                res = 0;
            }
        }
        if (res == 2) {
            continue;
        } else
            break;
    }
    return res;
}

char LookForWinner(int ball_info) {
    char ball_x_pos = GetDataFromInt(ball_info, 3);
    return (ball_x_pos == 0) ? 1 : ((ball_x_pos == 79) ? -1 : 0);
}

int LeftRacketCheck(int ball_info, char left_racket_pos) {
    char ball_horizontal_velocity = GetDataFromInt(ball_info, 1);
    char ball_vertical_velocity = GetDataFromInt(ball_info, 2);
    char ball_x_pos = GetDataFromInt(ball_info, 3);
    char ball_y_pos = GetDataFromInt(ball_info, 4);
    if (ball_x_pos == 1 && ball_horizontal_velocity < 0) {
        if (ball_y_pos == left_racket_pos - 1) {
            ball_horizontal_velocity = 0 - ball_horizontal_velocity;
            ball_vertical_velocity =
                ball_vertical_velocity > -4 ? ball_vertical_velocity - 1 : ball_vertical_velocity;
        } else if (ball_y_pos == left_racket_pos + 1) {
            ball_horizontal_velocity = 0 - ball_horizontal_velocity;
            ball_vertical_velocity =
                ball_vertical_velocity < 4 ? ball_vertical_velocity + 1 : ball_vertical_velocity;
        } else if (ball_y_pos == left_racket_pos) {
            ball_horizontal_velocity = 0 - ball_horizontal_velocity;
        }
        ball_horizontal_velocity =
            ball_horizontal_velocity >= 0 ? ball_horizontal_velocity : 9 + ball_horizontal_velocity;
        ball_vertical_velocity =
            ball_vertical_velocity >= 0 ? ball_vertical_velocity : 9 + ball_vertical_velocity;
        ball_info = GetIntFromData(ball_horizontal_velocity, ball_vertical_velocity, ball_x_pos, ball_y_pos);
    }
    return ball_info;
}

int RightRacketCheck(int ball_info, char right_racket_pos) {
    char ball_horizontal_velocity = GetDataFromInt(ball_info, 1);
    char ball_vertical_velocity = GetDataFromInt(ball_info, 2);
    char ball_x_pos = GetDataFromInt(ball_info, 3);
    char ball_y_pos = GetDataFromInt(ball_info, 4);
    if (ball_x_pos == 78 && ball_horizontal_velocity > 0) {
        if (ball_y_pos == right_racket_pos - 1) {
            ball_horizontal_velocity = 0 - ball_horizontal_velocity;
            ball_vertical_velocity =
                ball_vertical_velocity > -4 ? ball_vertical_velocity - 1 : ball_vertical_velocity;
        } else if (ball_y_pos == right_racket_pos + 1) {
            ball_horizontal_velocity = 0 - ball_horizontal_velocity;
            ball_vertical_velocity =
                ball_vertical_velocity < 4 ? ball_vertical_velocity + 1 : ball_vertical_velocity;
        } else if (ball_y_pos == right_racket_pos) {
            ball_horizontal_velocity = 0 - ball_horizontal_velocity;
        }
        ball_horizontal_velocity =
            ball_horizontal_velocity >= 0 ? ball_horizontal_velocity : 9 + ball_horizontal_velocity;
        ball_vertical_velocity =
            ball_vertical_velocity >= 0 ? ball_vertical_velocity : 9 + ball_vertical_velocity;
        ball_info = GetIntFromData(ball_horizontal_velocity, ball_vertical_velocity, ball_x_pos, ball_y_pos);
    }
    return ball_info;
}

int Move(int ball_info) {
    char ball_horizontal_velocity = GetDataFromInt(ball_info, 1);
    char ball_vertical_velocity = GetDataFromInt(ball_info, 2);
    char ball_x_pos = GetDataFromInt(ball_info, 3);
    char ball_y_pos = GetDataFromInt(ball_info, 4);
    if (ball_y_pos + ball_vertical_velocity > 24) {
        ball_y_pos = 48 - (ball_y_pos + ball_vertical_velocity);
        ball_vertical_velocity = 0 - ball_vertical_velocity;
    } else if (ball_y_pos + ball_vertical_velocity < 0) {
        ball_y_pos = 0 - (ball_y_pos + ball_vertical_velocity);
        ball_vertical_velocity = 0 - ball_vertical_velocity;
    } else {
        ball_y_pos = ball_y_pos + ball_vertical_velocity;
    }
    if (ball_x_pos + ball_horizontal_velocity > 79) {
        ball_x_pos = 79;
        ball_horizontal_velocity = 0;
        ball_vertical_velocity = 0;
    } else if (ball_x_pos + ball_horizontal_velocity < 0) {
        ball_x_pos = 0;
        ball_horizontal_velocity = 0;
        ball_vertical_velocity = 0;
    } else {
        ball_x_pos = ball_x_pos + ball_horizontal_velocity;
    }
    ball_vertical_velocity = GetStructedSpeed(ball_vertical_velocity);
    ball_horizontal_velocity = GetStructedSpeed(ball_horizontal_velocity);
    return GetIntFromData(ball_horizontal_velocity, ball_vertical_velocity, ball_x_pos, ball_y_pos);
}

char GetStructedSpeed(char current_ball_velocity) {
    return current_ball_velocity >= 0 ? current_ball_velocity : 9 + current_ball_velocity;
}

int UpdateBall(int ball_info, char left_racket_pos, char right_racket_pos) {
    ball_info = LeftRacketCheck(ball_info, left_racket_pos);
    ball_info = RightRacketCheck(ball_info, right_racket_pos);
    ball_info = Move(ball_info);
    return ball_info;
}

void PrintField(char left_racket_pos, char right_racket_pos, char ball_y_pos, char ball_x_pos,
                char first_player_score, char second_player_score) {
    clear();
    printw("\t\t\t\t      Score\nFirst player: %d\t\t\t\t\t\t\tSecond player: %d\n", first_player_score,
           second_player_score);
    for (int i = 0; i < 80; i++, printw("#"))
        ;
    printw("\n");
    for (int y = 0; y < 25; y++) {
        for (int x = 0; x < 80; x++) {
            if ((x == 0 && y <= left_racket_pos + 1 && y >= left_racket_pos - 1) ||
                (x == 79 && y <= right_racket_pos + 1 && y >= right_racket_pos - 1)) {
                printw("|");
            } else if (x == ball_x_pos && y == ball_y_pos) {
                printw("O");
            } else if (x == 39 || x == 40) {
                printw(":");
            } else {
                printw(" ");
            }
        }
        printw("\n");
    }
    for (int i = 0; i < 80; i++, printw("#"))
        ;
    printw("\n");
    refresh();
    return;
}

char GetDataFromInt(int arg, char category_number) {
    char result;
    if (category_number == 1) {
        result = arg / 100000;
        result = result <= 4 ? result : result - 9;
    } else if (category_number == 2) {
        result = (arg / 10000) % 10;
        result = result <= 4 ? result : result - 9;
    } else if (category_number == 3) {
        result = (arg / 100) % 100;
    } else {
        result = arg % 100;
    }
    return result;
}

int GetIntFromData(char ball_horizontal_velocity, char ball_vertical_velocity, char ball_x_pos,
                   char ball_y_pos) {
    return ball_horizontal_velocity * 100000 + ball_vertical_velocity * 10000 + ball_x_pos * 100 + ball_y_pos;
}
