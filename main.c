#include <raylib.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdio.h>

#define NUM_OF_RAYS 180

typedef enum GameScren { LOGO, TITLE, GAMEPLAY } GameScreen;

typedef struct Player
{
    Vector2 position;
    Vector2 speed;
    
} Player;

typedef struct RayToCast
{
    Vector2 startPosition;
    Vector2 endPosition;
    float angle;
    bool hit;
    float length;
    Vector2 steps;
    int numberOfSteps;
    bool draw;

} RayToCast;

typedef struct MapCube
{
    Rectangle cubeBounds;
    bool draw; 
} MapCube;

int main(void)
{
    // init

    const int screenWidth = 600;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "2D RayCasting");

    srand(time(NULL));

    // Game varibles
    GameScreen screen = LOGO;
    int framesCounter = 0;
    bool drawRay = false;
    int map[100] = { 1,1,1,1,1,1,1,1,1,1,
                     1,0,1,0,0,0,0,0,0,1,
                     1,0,1,1,1,1,0,1,1,1,
                     1,0,0,0,0,0,0,0,0,1,
                     1,0,1,0,1,0,1,0,0,1,
                     1,0,1,0,1,0,1,0,0,1,
                     1,0,1,0,1,0,1,0,0,1,
                     1,0,1,0,1,0,1,1,1,1,
                     1,0,0,0,1,0,0,0,0,1,
                     1,1,1,1,1,1,1,1,1,1 };

    // check structs
    Player player = { 0 };
    Rectangle cubes[10] = { 0 };
    RayToCast rays[NUM_OF_RAYS] = { 0 };
    MapCube mapCubes[100] = { 0 };

    // init player
    player.position = (Vector2) { 90, 90 };
    player.speed = (Vector2) { 0.0f, 0.0f };

    // init cubes
    for (int i = 0; i < 10; i++)
    {
        cubes[i].height = 20;
        cubes[i].width = 20;
        cubes[i].x = (rand() % screenWidth/2) + screenWidth/4;
        cubes[i].y = (rand() % screenHeight/2) + screenHeight/4;
    }
    float frayCount = NUM_OF_RAYS;
    //init rays
    for (int i = 0; i < NUM_OF_RAYS; i++)
    {
        rays[i].angle = (360.0f/frayCount) * i;
        rays[i].hit = false;
        rays[i].startPosition = (Vector2) { 0.0f,0.0f };
        rays[i].endPosition = (Vector2) { 0.0f, 0.0f };
        rays[i].length = 0.0f;
        rays[i].steps = (Vector2) { 0.0f, 0.0f };
        rays[i].numberOfSteps = 0;
        rays[i].draw = false;
    }

    //init map
    for (int i = 0; i < 100; i++)
    {
        mapCubes[i].cubeBounds.height = 60;
        mapCubes[i].cubeBounds.width = 60;
        mapCubes[i].cubeBounds.x = (i % 10) * 60;
        mapCubes[i].cubeBounds.y = (i / 10 ) * 60;
        if (map[i] == 1)
        {
            mapCubes[i].draw = true;
        }
        else if (map[i] == 0)
        {
            mapCubes[i].draw = false;
        }
    }
    
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // Update
        switch (screen)
        {
            case LOGO:
            {
                framesCounter++;
                if ( framesCounter > 180)
                {
                    screen = TITLE;
                    framesCounter = 0;
                }
            } break;

            case TITLE:
            {
                framesCounter++;
                if(IsKeyPressed(KEY_ENTER)) screen = GAMEPLAY;
            } break;

            case GAMEPLAY:
            {
                // Player movement
                if (IsKeyDown(KEY_W)) player.speed.y -= 0.35f;
                if (IsKeyDown(KEY_S)) player.speed.y += 0.35f;
                if (IsKeyDown(KEY_A)) player.speed.x -= 0.35f;
                if (IsKeyDown(KEY_D)) player.speed.x += 0.35f;
                if (IsKeyDown(KEY_UP)) player.speed.y -= 0.35f;
                if (IsKeyDown(KEY_DOWN)) player.speed.y += 0.35f;
                if (IsKeyDown(KEY_LEFT)) player.speed.x -= 0.35f;
                if (IsKeyDown(KEY_RIGHT)) player.speed.x += 0.35f;
                
                for (int i = 0; i < 100; i++)
                {
                    if (CheckCollisionPointRec(player.position, mapCubes[i].cubeBounds) == false)
                    {
                        player.position.y += player.speed.y / 100;
                        player.position.x += player.speed.x / 100;
                    }   
                }
                if (player.speed.x > 0) player.speed.x -= 0.1f;
                if (player.speed.x < 0) player.speed.x += 0.1f;
                if (player.speed.y > 0) player.speed.y -= 0.1f;
                if (player.speed.y < 0) player.speed.y += 0.1f;
                
                if (IsKeyPressed(KEY_R)) drawRay = !drawRay;

                // Calculate the rays
                for (int i = 0; i < NUM_OF_RAYS; i++)
                {
                    rays[i].draw = false;
                    rays[i].numberOfSteps = 0;
                    rays[i].startPosition = player.position;
                    rays[i].endPosition = player.position;
                    rays[i].hit = false;
                    rays[i].steps.x = cos((PI/180) * rays[i].angle);
                    rays[i].steps.y = sin((PI/180) * rays[i].angle);
                    while (!rays[i].hit)
                    {
                        rays[i].endPosition.x += rays[i].steps.x;
                        rays[i].endPosition.y += rays[i].steps.y;
                        rays[i].numberOfSteps++;
                        if (rays[i].endPosition.x > GetScreenWidth() || rays[i].endPosition.x < 0)
                        {
                            rays[i].hit = true;
                        }
                        if (rays[i].endPosition.y > GetScreenHeight() || rays[i].endPosition.y < 0)
                        {
                            rays[i].hit = true;
                        }
                        for (int j = 0; j < 100; j++)
                        {
                            if (mapCubes[j].draw)
                            {
                                if(CheckCollisionPointRec(rays[i].endPosition, mapCubes[j].cubeBounds))
                                {
                                    rays[i].hit = true;
                                    rays[i].draw = true;
                                }
                            }
                            
                            // if (CheckCollisionPointRec(rays[i].endPosition, cubes[j]))
                            // {
                            //     rays[i].hit = true;
                            //     rays[i].draw = true;
                            // }
                        }
                    }
                    rays[i].length = sqrt(pow(rays[i].endPosition.x - rays[i].startPosition.x, 2) + pow(rays[i].endPosition.y - rays[i].startPosition.y, 2));
                       
                }
                
            } break;
            
            default: break;
        }

        // draw
        BeginDrawing();
            ClearBackground(BLACK);

            switch (screen)
            {
                case LOGO:
                {
                    DrawText("Firewire.dev", 20, 20, 20, LIGHTGRAY);
                } break;

                case TITLE:
                {
                    DrawText("Ray Casting Test", GetScreenWidth()/2 - MeasureText("Ray Casting Test", 40)/2, GetScreenHeight()/2, 40, GRAY);
                    if ((framesCounter/30)%2 == 0) DrawText("PRESS [ENTER] to START", GetScreenWidth()/2 - MeasureText("PRESS [ENTER] to START", 20)/2, GetScreenHeight()/2 + 60, 20, DARKGRAY);
                } break;

                case GAMEPLAY:
                {
                    for (int i = 0; i < NUM_OF_RAYS; i++)
                    {
                        if (rays[i].draw)
                        {
                            if (drawRay)
                            {
                                for (int j = 0; j < rays[i].numberOfSteps; j++)
                                {
                                    int color = ((float)j / (float)rays[i].numberOfSteps) * -255;
                                    Color rayColor = (Color) { color,color,color,color };
                                    DrawPixel((rays[i].steps.x * j) + rays[i].startPosition.x, (rays[i].steps.y * j) + rays[i].startPosition.y, rayColor);
                                    DrawPixel(rays[i].endPosition.x, rays[i].endPosition.y, WHITE);
                                }
                            }
                            else 
                            {
                                DrawPixel(rays[i].endPosition.x, rays[i].endPosition.y, WHITE);
                            }
                        }
                    }
                    
                    DrawPixel(player.position.x, player.position.y, GREEN);

                    DrawFPS(20,20);
                    
                    
                } break;
            
                default: break;
            }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}