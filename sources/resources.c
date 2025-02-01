#include <stdlib.h>
#include "resources.h"
#include <raymath.h>
#include <stdio.h>
#pragma region GameBoard

GameWindow CreateGameWindow(int width, int height, int cellSize){
    GameWindow window;
    window.Width = width;
    window.Height = height;
    window.grid.cellSize = cellSize;
    window.grid.cellCount = width/cellSize;

    return window;
}

#pragma endregion

#pragma region Food

void DrawFood(const struct Food* this, const struct GameWindow *window)
{
    DrawTextureEx(this->texture, (Vector2){(this->position.x) * window->grid.cellSize, 
                  (this->position.y) * window->grid.cellSize}, 0.0f, 1.5f, WHITE);
    // DrawTexture(this->texture, (int)this->position.x * window->grid.cellSize,
    //             (int)this->position.y * window->grid.cellSize, WHITE);
}

void GenerateRandomPos(struct Food *this, const struct GameWindow *window)
{
    float x = GetRandomValue(1, window->grid.cellCount - 2);
    float y = GetRandomValue(1, window->grid.cellCount - 2);
    this->position = (Vector2){x, y};
}


void FoodDestructor(Food* this){
    if (!this)
        return;
    UnloadTexture(this->texture);

    free(this);
}

Food CreateFood(const struct GameWindow *window, const char *imageSource){
    Food food;
    GenerateRandomPos(&food, window);
    food.image = LoadImage(imageSource);
    food.texture = LoadTextureFromImage(food.image);
    UnloadImage(food.image);

    food.Draw = DrawFood;
    food.GenerateRandomPos = GenerateRandomPos;
    food.Destruct = FoodDestructor;
    return food;
}

#pragma endregion

#pragma region Deque

void Deque_push_front(struct Deque *this, Vector2 position)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (!newNode)
    {
        printf("Error: Memory allocation failed in push_front.\n");
        return;
    }

    newNode->cellPos = position;
    newNode->prev = NULL;
    newNode->next = this->head;

    if (this->head == NULL)
    {
        this->rear = newNode;
    }
    else
    {
        this->head->prev = newNode;
    }

    this->head = newNode;
    this->size++;
}

void Deque_push_back(struct Deque *this, Vector2 position)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (!newNode)
    {
        printf("Error: Memory allocation failed in push_back.\n");
        return;
    }

    newNode->cellPos = position;
    newNode->next = NULL;
    newNode->prev = this->rear;

    if (this->rear == NULL)
    {
        this->head = newNode;
    }
    else
    {
        this->rear->next = newNode;
    }

    this->rear = newNode;
    this->size++;
}

void Deque_pop_front(struct Deque *this){
    if (this->head == NULL)
        return;

    Node *temp = this->head;
    this->head = this->head->next;

    if(this->head == NULL){
        this->rear = NULL;
    } else{
        this->head->prev = NULL;
    }

    free(temp);
    this->size--;
}

void Deque_pop_back(struct Deque *this){
    if (this->rear == NULL) return;

    Node *temp = this->rear;
    this->rear = this->rear->prev;

    if(this->rear == NULL){
        this->head = NULL;
    } else {
        this->rear->next = NULL;
    }

    free(temp);
    this->size--;
}

Vector2 Deque_front(struct Deque *this){
    if (this->head == NULL)
        return (Vector2){-1, -1};
    return this->head->cellPos;
}

Vector2 Deque_back(struct Deque *this){
    if (this->rear == NULL)
        return (Vector2){-1, -1};
    return this->rear->cellPos;
}

bool Deque_is_empty(struct Deque *this){
    return this->head == NULL;
}

void Deque_destruct(struct Deque *this){
    while (!this->is_empty(this)){
        this->pop_front(this);
    }

    free(this);
}

Deque* CreateDeque()
{
    Deque *this = (Deque *)malloc(sizeof(Deque));

    this->head = NULL;
    this->rear = NULL;
    this->size = 0;

    this->push_front = Deque_push_front;
    this->push_back = Deque_push_back;
    this->pop_front = Deque_pop_front;
    this->pop_back = Deque_pop_back;
    this->front = Deque_front;
    this->back = Deque_back;
    this->is_empty = Deque_is_empty;
    this->destruct = Deque_destruct;

    return this;
}

#pragma endregion

#pragma region Snake

void DrawSnake(const struct Snake *this, const Color *colorBody, const Color *colorHead, const struct GameWindow *window){
    Node* current = this->body->head;
    while(current != NULL){
        float x = current->cellPos.x;
        float y = current->cellPos.y;
        if(current == this->body->head){
        DrawRectangleRounded((Rectangle){x * window->grid.cellSize, y * window->grid.cellSize,
                             window->grid.cellSize, window->grid.cellSize}, 0.75, 6, *colorHead);    
        }
        else{
            DrawRectangleRounded((Rectangle){x * window->grid.cellSize, y * window->grid.cellSize,
                                window->grid.cellSize, window->grid.cellSize}, 0.75, 6, *colorBody);
        }
        current = current->next;
    }
}

void UpdateSnake(struct Snake *this, const struct GameWindow *window){
    Vector2 target = Vector2Add(this->body->head->cellPos, this->direction);
    if(Vector2Equals(this->direction, UP) && target.y * window->grid.cellSize <= 0){
        target.y = window->grid.cellCount - 1;
    }
    if (Vector2Equals(this->direction, DOWN) && target.y * window->grid.cellSize > window->Height - window->grid.cellSize)
    {
        target.y = 1;
    }
    if (Vector2Equals(this->direction, LEFT) && target.x * window->grid.cellSize <= 0)
    {
        target.x = window->grid.cellCount - 1;
    }
    if (Vector2Equals(this->direction, RIGHT) && target.x * window->grid.cellSize > window->Width - window->grid.cellSize)
    {
        target.x = 1;
    }
    this->body->pop_back(this->body);
    this->body->push_front(this->body, target);
}

void DestructSnake(struct Snake *this){
    if(!this)
        return;
    this->body->destruct(this->body);
    free(this);
}

Snake *CreateSnake()
{
    Snake *snake = (Snake *)malloc(sizeof(Snake));

    snake->body = CreateDeque();
    snake->direction = RIGHT;

    snake->body->push_front(snake->body, (Vector2){5, 5});
    snake->body->push_front(snake->body, (Vector2){4, 5});
    snake->body->push_front(snake->body, (Vector2){3, 5});

    snake->Draw = DrawSnake;
    snake->Update = UpdateSnake;
    snake->Destruct = DestructSnake;

    return snake;
}

#pragma endregion

#pragma region MovementTime
    bool MovementEventTriggered(struct MovementTime *this, double interval) {
        double currentTime = GetTime();
        if(currentTime - this->lastUpdateTime >= interval){
            this->lastUpdateTime = currentTime;
            return true;
        }
        return false;
    }


    MovementTime CreateMovementTime(){
        MovementTime this;
        this.lastUpdateTime = 0;
        this.eventTriggered = MovementEventTriggered;

        return this;
    }
#pragma endregion