__attribute__((section(".multiboot")))
const unsigned int multiboot_header[] = {
    0x1BADB002, // Multiboot magic number
    0x00,       // Flags (none)
    -(0x1BADB002) // Checksum (negates magic+flags)
};

#include "vga.h"
#include "keyboard.h"

#define WIDTH 40
#define HEIGHT 20
#define PACMAN 'C'
#define WALL '#'
#define FOOD '.'
#define EMPTY ' '
#define DEMON 'X'

char board[HEIGHT][WIDTH];
int pacman_x, pacman_y;
int score = 0;
int food = 0;
int res = 0;

unsigned int seed = 1337;
// Minimal PRNG, safe for bare-metal
int rand() {
    seed = (1103515245 * seed + 12345) & 0x7fffffff;
    return seed;
}

void initialize() {
    // Set boundary walls and empty space
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            board[y][x] = (y == 0 || x == 0 || y == HEIGHT - 1 || x == WIDTH - 1) ? WALL : EMPTY;
        }
    }

    // Place 50 random internal walls
    for (int i = 0; i < 50; i++) {
        int x = rand() % WIDTH;
        int y = rand() % HEIGHT;
        if (board[y][x] == EMPTY) board[y][x] = WALL;
    }

    // Place 10 demons
    for (int i = 0; i < 10; i++) {
        int x = rand() % WIDTH;
        int y = rand() % HEIGHT;
        if (board[y][x] == EMPTY) board[y][x] = DEMON;
    }

    // Place Pacman in the center
    pacman_x = WIDTH / 2;
    pacman_y = HEIGHT / 2;
    board[pacman_y][pacman_x] = PACMAN;

    // Place food dots in a pattern
    for (int y = 1; y < HEIGHT - 1; y++) {
        for (int x = 1; x < WIDTH - 1; x++) {
            if (board[y][x] == EMPTY && (x + y) % 3 == 0) {
                board[y][x] = FOOD;
                food++;
            }
        }
    }
}

void draw() {
    clear_screen();
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            print_char(board[y][x], x, y);
        }
    }
    print_str("Score: ", 0, HEIGHT);
    print_num(score, 7, HEIGHT);
}

void move(int dx, int dy) {
    int nx = pacman_x + dx;
    int ny = pacman_y + dy;

    // Boundary check
    if (nx < 0 || nx >= WIDTH || ny < 0 || ny >= HEIGHT)
        return;

    // Check if next position is a wall
    if (board[ny][nx] == WALL)
        return;

    // Check if next position is a demon
    if (board[ny][nx] == DEMON) {
        res = 1;  // Game over
        return;
    }

    // Check if next position has food
    if (board[ny][nx] == FOOD) {
        score++;
        food--;
    }

    // Move Pacman to new position
    board[pacman_y][pacman_x] = EMPTY;
    pacman_x = nx;
    pacman_y = ny;
    board[pacman_y][pacman_x] = PACMAN;

    // Win condition check
    if (food == 0)
        res = 2;
}

void _start() {
    initialize();
    draw();

    while (1) {
        char key = get_key();

        if (key == 'w') move(0, -1);
        else if (key == 's') move(0, 1);
        else if (key == 'a') move(-1, 0);
        else if (key == 'd') move(1, 0);

        draw();

        if (res == 1) {
            print_str("GAME OVER", WIDTH / 2 - 5, HEIGHT / 2);
            break;
        } else if (res == 2) {
            print_str("YOU WIN!", WIDTH / 2 - 5, HEIGHT / 2);
            break;
        }
    }

    // Halt forever after game ends
    while (1);
}