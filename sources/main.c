#include "raylib.h"
#include "resources.h"
#include "raymath.h"
#include <stdio.h>

int main()
{

#pragma region EarlyInit
    GameWindow window = CreateGameWindow(800, 800, 35);
    InitWindow(window.Width, window.Height, "Snake Game");
    SetTargetFPS(60);

    Food food = CreateFood(&window, "../assets/apple.png");

    Color Cream = {255, 245, 225, 255};
    Color SageGreen = {115, 144, 114, 255};
    Color Moss = {58, 77, 57, 255};
    Color SoftBrown = {140, 106, 93, 255};

    Snake* snake = CreateSnake();
    bool earlyGame = true;
    int score = 0;

    MovementTime timeSnake = CreateMovementTime();
    MovementTime timeKey = CreateMovementTime();
#pragma endregion

#pragma region GameLoop

    while (!WindowShouldClose())
    {

        // --------------------------------------------
        // ------------------ EVENTS ------------------
        // --------------------------------------------

        // ---------------- MOVEMENTS ----------------
        if((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) && !Vector2Equals(snake->direction, DOWN) 
            && !Vector2Equals(snake->body->head->next->cellPos, Vector2Add(snake->body->head->cellPos, UP))){
            snake->direction = UP;
        }
        else if((IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) && !Vector2Equals(snake->direction, UP)
                && !Vector2Equals(snake->body->head->next->cellPos, Vector2Add(snake->body->head->cellPos, DOWN))){
            snake->direction = DOWN;
        }
        else if ((IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) && !Vector2Equals(snake->direction, RIGHT)
                && !Vector2Equals(snake->body->head->next->cellPos, Vector2Add(snake->body->head->cellPos, LEFT)))
        {
            snake->direction = LEFT;
        }
        else if ((IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) && !Vector2Equals(snake->direction, LEFT)
                && !Vector2Equals(snake->body->head->next->cellPos, Vector2Add(snake->body->head->cellPos, RIGHT)))
        {
            snake->direction = RIGHT;
        }
        // ---------------- MOVEMENTS ----------------


        // --------------------------------------------
        // ------------------ UPDATE ------------------
        // --------------------------------------------
        

        
        if(timeSnake.eventTriggered(&timeSnake, 0.1)){

            // If food is eaten
            if(Vector2Equals(snake->body->head->cellPos, Vector2Add(food.position, (Vector2){1,1}))){
                food.GenerateRandomPos(&food, &window);
                
                // Loop until food is not inside snake body
                while (snake->body->head->cellPos.x == food.position.x ||
                       snake->body->head->cellPos.y == food.position.y ||
                       snake->body->rear->cellPos.x == food.position.x ||
                       snake->body->rear->cellPos.y == food.position.y){

                    food.GenerateRandomPos(&food, &window);
                    }
                    // Add a cell to the back of the snake
                snake->body->push_back(snake->body, Vector2Subtract(snake->body->rear->cellPos, snake->direction));
                score++;
            }

            // If body collision happened
            Node *current = snake->body->head->next;
            while(current != NULL){
                if(Vector2Equals(current->cellPos, snake->body->head->cellPos)){
                    if(earlyGame){
                        earlyGame = false;
                    } else{
                        exit(0);
                    }
                }
                current = current->next;
            }

            snake->Update(snake, &window);
            // printf("Current Position = {%d, %d}\n", (int)snake->body->head->cellPos.x, (int)snake->body->head->cellPos.y);
        }
        

        // --------------------------------------------
        // ------------------  DRAW  ------------------
        // --------------------------------------------

        BeginDrawing();

        // ---------------- GAME BOARD ----------------
        ClearBackground(SageGreen);
        DrawRectangle(window.grid.cellSize, window.grid.cellSize, window.Width - window.grid.cellSize * 2, 
                      window.Height - window.grid.cellSize*2, Cream);
        DrawText("Snek Game", window.grid.cellSize, 10, 20, RAYWHITE);
        
        DrawText(TextFormat("Score: %i", score), window.Width - (window.grid.cellSize*4), 10, 20, RAYWHITE);
        // ---------------- GAME BOARD ----------------

        // ----------------- ENTITIES -----------------
        food.Draw(&food, &window);
        snake->Draw(snake, &Moss, &SoftBrown, &window);
        // ----------------- ENTITIES -----------------

        EndDrawing();
    }
#pragma endregion
    
    food.Destruct(&food);
    snake->Destruct(snake);
    CloseWindow();
    return 0;
}
