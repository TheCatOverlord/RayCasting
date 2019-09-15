#include <raylib.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdio.h>

#define NUM_OF_RAYS 360

typedef enum GameScreen { LOGO, TITLE, GAMEPLAY } GameScreen;
typedef enum RayRender { EndPoint, Gradient, EndPointGradient, inverseGradient, lines } RayRender;

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
    int stepMultiple;

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
    const int frameRate = 60;

    InitWindow(screenWidth, screenHeight, "2D RayCasting");

    srand(time(NULL));

    // Game varibles
    GameScreen screen = LOGO;
    int framesCounter = 0;
    RayRender render = EndPoint;
    bool debugLog = false;
    bool drawMap = false;
    int map[100] = { 1,1,1,1,1,1,1,1,1,1,
                     1,0,0,0,0,0,0,0,0,1,
                     1,0,0,0,0,0,0,0,0,1,
                     1,0,0,0,0,0,0,0,0,1,
                     1,0,0,0,0,0,0,0,0,1,
                     1,0,0,0,0,0,0,0,0,1,
                     1,0,0,0,0,0,0,0,0,1,
                     1,0,0,0,0,0,0,0,0,1,
                     1,0,0,0,0,0,0,0,0,1,
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
        rays[i].stepMultiple = 10;
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
    
    SetTargetFPS(frameRate);

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
                
                switch (render)
                {
                    case EndPoint:
                    { if (IsKeyPressed(KEY_R)) render = Gradient; } break;
                    
                    case Gradient:
                    { if (IsKeyPressed(KEY_R)) render = EndPointGradient; } break;

                    case EndPointGradient:
                    { if (IsKeyPressed(KEY_R)) render = inverseGradient; } break;

                    case inverseGradient:
                    { if (IsKeyPressed(KEY_R)) render = lines; } break;

                    case lines:
                    { if (IsKeyPressed(KEY_R)) render = EndPoint; } break;
                
                    default: break;
                }

                if (IsKeyPressed(KEY_X))
                {     
                    for (int i = 0; i < NUM_OF_RAYS; i++)
                    {
                        rays[i].stepMultiple++;
                    }
                }
                if (IsKeyPressed(KEY_Z))
                {
                    for (int i = 0; i < NUM_OF_RAYS; i++)
                    {
                        if (rays[i].stepMultiple != 1) rays[i].stepMultiple--;
                    }
                }
                if (IsKeyPressed(KEY_C)) debugLog = !debugLog;
                if (IsKeyPressed(KEY_F)) drawMap = !drawMap;

                // Calculate the rays
                for (int i = 0; i < NUM_OF_RAYS; i++)
                {
                    rays[i].draw = false;
                    rays[i].numberOfSteps = 0;
                    rays[i].startPosition = player.position;
                    rays[i].endPosition = player.position;
                    rays[i].hit = false;
                    rays[i].steps.x = (cos((PI/180) * rays[i].angle)) * rays[i].stepMultiple;
                    rays[i].steps.y = (sin((PI/180) * rays[i].angle)) * rays[i].stepMultiple;
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


                    // Yes, probably not the best practice but im tired ok.
                    if (rays[i].length > 290)
                    { rays[i].stepMultiple = 11; }
                    if (rays[i].length > 260 && rays[i].length < 290)
                    { rays[i].stepMultiple = 10; }
                    if (rays[i].length > 230 && rays[i].length < 260)
                    { rays[i].stepMultiple = 9; }
                    if (rays[i].length > 210 && rays[i].length < 230)
                    { rays[i].stepMultiple = 8; }
                    if (rays[i].length > 180 && rays[i].length < 210)
                    { rays[i].stepMultiple = 7; }
                    if (rays[i].length > 150 && rays[i].length < 180)
                    { rays[i].stepMultiple = 6; }
                    if (rays[i].length > 120 && rays[i].length < 150)
                    { rays[i].stepMultiple = 5; }
                    if (rays[i].length > 90 && rays[i].length < 120)
                    { rays[i].stepMultiple = 4; }
                    if (rays[i].length > 60 && rays[i].length < 90)
                    { rays[i].stepMultiple = 3; }
                    if (rays[i].length > 30 && rays[i].length < 60)
                    { rays[i].stepMultiple = 2; }
                    if (rays[i].length > 0 && rays[i].length < 30)
                    { rays[i].stepMultiple = 1; }
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
                    switch (render)
                    {
                        case EndPoint:
                        {
                            for (int i = 0; i < NUM_OF_RAYS; i++)
                            {
                                if (rays[i].draw)
                                {
                                    DrawPixel(rays[i].endPosition.x, rays[i].endPosition.y, WHITE);
                                }
                            }
                        } break;

                        case Gradient:
                        {
                            for (int i = 0; i < NUM_OF_RAYS; i++)
                            {
                                if(rays[i].draw)
                                {
                                    for (int j = 0; j < rays[i].numberOfSteps; j++)
                                    {
                                        int color = ((float)j / (float)rays[i].numberOfSteps) * -255;
                                        Color rayColor = (Color) { color,color,color,color };
                                        DrawPixel((rays[i].steps.x * j) + rays[i].startPosition.x, (rays[i].steps.y * j) + rays[i].startPosition.y, rayColor);
                                    }
                                }
                            }
                            
                        } break;

                        case EndPointGradient:
                        {
                            for (int i = 0; i < NUM_OF_RAYS; i++)
                            {
                                if (rays[i].draw)
                                {
                                    for (int j = 0; j < rays[i].numberOfSteps; j++)
                                    {
                                        int color = ((float)j / (float)rays[i].numberOfSteps) * -255;
                                        Color rayColor = (Color) { color,color,color,color };
                                        DrawPixel((rays[i].steps.x * j) + rays[i].startPosition.x, (rays[i].steps.y * j) + rays[i].startPosition.y, rayColor);
                                    }
                                    DrawPixel(rays[i].endPosition.x, rays[i].endPosition.y, WHITE);
                                }
                            }
                        } break;

                        case inverseGradient:
                        {
                            for (int i = 0; i < NUM_OF_RAYS; i++)
                            {
                                if (rays[i].draw)
                                {
                                    for (int j = 0; j < rays[i].numberOfSteps; j++)
                                    {
                                        int color = ((float)j / (float)rays[i].numberOfSteps) * 255;
                                        Color rayColor = (Color) { color,color,color,color };
                                        DrawPixel((rays[i].steps.x * j) + rays[i].startPosition.x, (rays[i].steps.y * j) + rays[i].startPosition.y, rayColor);
                                    }
                                }
                            }
                            
                        } break;

                        case lines:
                        {
                            for (int i = 0; i < NUM_OF_RAYS; i++)
                            {
                                if (rays[i].draw)
                                {
                                    DrawLine(rays[i].startPosition.x, rays[i].startPosition.y, rays[i].endPosition.x, rays[i].endPosition.y, WHITE);
                                }
                            }
                            
                        } break;
                
                        default: break;
                    }
                    
                    if (drawMap)
                    {
                        for (int i = 0; i < 100; i++)
                        {
                            if ( mapCubes[i].draw)
                            {
                                DrawRectangle(mapCubes[i].cubeBounds.x, mapCubes[i].cubeBounds.y, mapCubes[i].cubeBounds.width, mapCubes[i].cubeBounds.height, DARKGRAY);
                            }
                        }
                    }
                    
                    DrawPixel(player.position.x, player.position.y, GREEN);

                    if (debugLog)
                    {
                    
                        float addedLength = 0.0f;
                        for (int i = 0; i < NUM_OF_RAYS; i++)
                        {
                            addedLength += rays[i].length;
                        }
                        addedLength = addedLength / frayCount;

                        float averageSteps = 0.0f;
                        for (int i = 0; i < NUM_OF_RAYS; i++)
                        {
                            averageSteps += rays[i].numberOfSteps;
                        }
                        averageSteps = averageSteps / frayCount;

                        float averageMultiple = 0.0f;
                        for (int i = 0; i < NUM_OF_RAYS; i++)
                        {
                            averageMultiple += rays[i].stepMultiple;
                        }
                        averageMultiple = averageMultiple / frayCount;

                        switch (render)
                        {
                            case EndPoint:
                            { DrawText("Ray Renderer: EndPoints", 20, 480, 10, GREEN); } break;

                            case Gradient:
                            { DrawText("Ray Renderer: Gradient", 20, 480, 10, GREEN); } break;

                            case EndPointGradient:
                            { DrawText("Ray Renderer: Gradient + EndPoints", 20, 480, 10, GREEN); } break;

                            case lines:
                            { DrawText("Ray Renderer: Lines", 20, 480, 10, GREEN); } break;

                            case inverseGradient:
                            { DrawText("Ray Renderer: Inverse Gradient", 20, 480, 10, GREEN); } break;
                        
                        default:
                            break;
                        }
                        DrawText(FormatText("Player Speed: %0.2f, %0.2f", player.speed.x, player.speed.y), 20, 490, 10, GREEN);
                        DrawText(FormatText("Player Position: %0.2f,%0.2f", player.position.x, player.position.y), 20, 500, 10, GREEN);
                        DrawText(FormatText("Average Ray Length: %0.2f", addedLength / rays[1].stepMultiple), 20, 510, 10, GREEN);
                        DrawText(FormatText("Average Steps: %0.2f", averageSteps), 20,520,10,GREEN);
                        DrawText(FormatText("Frame Time: %0.2f ms", GetFrameTime() * 1000), 20, 530, 10, GREEN);
                        DrawText(FormatText("Target Frame Time: %0.2f ms", (1/(float)frameRate) * 1000), 20, 540, 10, GREEN);
                        DrawText(FormatText("Time Scale: %0.2f", GetFrameTime() / (1 / (float) frameRate)), 20, 550, 10, GREEN);
                        DrawText(FormatText("Set Frame Rate: %d", frameRate), 20, 560, 10, GREEN);
                        DrawText(FormatText("Step Multiple: %0.2f", averageMultiple), 20, 570, 10, GREEN);
                        DrawText(FormatText("Number of Rays: %d", NUM_OF_RAYS), 20, 580, 10, GREEN);
                        DrawFPS(20,20);
                    }
                    
                } break;
            
                default: break;
            }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}