#pragma once

#ifndef RESOURCES_H
#define RESOURCES_H

#include <raylib.h>
#include <stdlib.h>

#pragma region Movement

#define RIGHT (Vector2){1, 0}
#define LEFT (Vector2){-1, 0}
#define UP (Vector2){0, -1}
#define DOWN (Vector2){0, 1}

typedef struct MovementTime{
    double lastUpdateTime;
    bool (*eventTriggered)(struct MovementTime *this, double interval);
} MovementTime;

MovementTime CreateMovementTime();

#pragma endregion

#pragma region GameBoard

typedef struct 
{
    int cellSize;
    int cellCount;
} Grid;

typedef struct GameWindow 
{
    int Width;
    int Height;
    Grid grid;

} GameWindow;

GameWindow CreateGameWindow(int width, int height, int cellSize);

#pragma endregion

#pragma region Deque

typedef struct Node {
    Vector2 cellPos;
    struct Node *prev;
    struct Node *next;
} Node;

typedef struct Deque{
    Node *head;
    Node *rear;
    int size;

    void (*push_front)(struct Deque *this, Vector2 position);
    void (*push_back)(struct Deque *this, Vector2 position);
    void (*pop_front)(struct Deque *this);
    void (*pop_back)(struct Deque *this);
    Vector2 (*front)(struct Deque *this);
    Vector2 (*back)(struct Deque *this);
    void (*destruct)(struct Deque *this);
    bool (*is_empty)(struct Deque *this);
} Deque;

Deque* CreateDeque();

#pragma endregion

#pragma region Snake

typedef struct Snake{
    Deque *body;
    Vector2 direction;

    void (*Draw)(const struct Snake *this, const Color*, const Color*, const struct GameWindow*);
    void (*Update)(struct Snake *this, const struct GameWindow*);
    void (*Destruct)(struct Snake *this);
} Snake;

Snake* CreateSnake();

#pragma endregion

#pragma region Food

typedef struct Food{
    Vector2 position;
    Image image;
    Texture2D texture;

    void (*Draw)(const struct Food* this, const struct GameWindow*);
    void (*GenerateRandomPos)(struct Food *this, const struct GameWindow *);
    void (*Destruct)(struct Food *this);
} Food; 

Food CreateFood(const struct GameWindow *window, const char *imageSource);

#pragma endregion

#endif