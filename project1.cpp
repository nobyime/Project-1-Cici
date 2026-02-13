/**
* Author: Xiling Wang
* Assignment: Project 1: Draw a Simple 2D Scene
* Date due: 02/14/2026
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "raylib.h"
#include "CS3113/cs3113.h"
#include <math.h>

// Global Constants
constexpr int SCREEN_WIDTH        = 1000,
              SCREEN_HEIGHT       = 600,
              FPS                 = 60;

constexpr Vector2 ORIGIN0 = { SCREEN_WIDTH / 2, 400 };
constexpr Vector2 ORIGIN2 = { 50, SCREEN_HEIGHT / 2 };
constexpr Vector2 ORIGIN3 = { 775, 525 };
constexpr Vector2 ORIGIN4 = { SCREEN_WIDTH / 2, 100 };

constexpr char ME[] = "me.png"; //my bf drew this, he thinks it looks like me
constexpr char CAT_1[] = "cat1.png";
constexpr char CAT_2[] = "cat2.png";
constexpr char CAT_3[] = "cat3.png";

// Global Variables
AppStatus gAppStatus   = RUNNING;
float     gPreviousTicks = 0.0f;

Texture2D gBgTexture;
Texture2D gAcTexture;
Texture2D gMeTexture;
Texture2D gCat1Texture;
Texture2D gCat2Texture;
Texture2D gCat3Texture;

//Me
Vector2 gMePos = ORIGIN0;
Vector2 gMeSize = {300.0f, 300.0f};
constexpr float gMeSpeed = 70.0f,
                gMeAmp = 15.0f,
                gMeBase = ORIGIN0.y;
float gMeDir = 1.0f;
float gMeTime = 0.0f;

//Cat 1: my favourite cat
Vector2 gCat1Pos = ORIGIN0;
Vector2 gCat1Size = {87.0f, 87.0f};
constexpr float gCat1Radius = 160.0f,
                gCat1Speed = 1.2f,
                gCat1Base = ORIGIN0.y;
float gCat1Angle = 0.0f;

//Cat 2: my mom's favourite cat
Vector2 gCat2Pos = ORIGIN2;
Vector2 gCat2Size = {90.0f, 90.0f};
constexpr float gCat2BaseX = ORIGIN2.x,
                gCat2BaseY = ORIGIN2.y,
                gCat2Speed = 300.0f,
                gCat2Coeff = (gCat2BaseY - gCat2BaseX) / 
                    ((SCREEN_WIDTH / 2 - gCat2BaseX) * (SCREEN_WIDTH / 2 - gCat2BaseX));
float gCat2Dir = 1.0f;
float gCat2Time = 0.0f;
float gCat2Scale = 1.0f;

//Cat 3
Vector2 gCat3Pos = ORIGIN3;
Vector2 gCat3NewPos = {875, 425};
Vector2 gCat3Size = {75.9f, 100.75f};
constexpr float gCat3Base = ORIGIN3.x;
constexpr int gCat3Max = 5;
float gCat3Angle = 0.0f;
float gCat3Dir = 1.0f;
float gCat3RDir = 1.0f;
float gCat3Speed = 500.0f;
float gCat3RSpeed = 100.0f;
int gCat3Count = 0;
bool gCat3Move = false;
float gCat3Time = 0.0f;


// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();

// Function Definitions
void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Say Hello to My Cats!");

    gBgTexture = LoadTexture("bg.png");
    gAcTexture = LoadTexture("ac.png"); //it's just part of the background
    gMeTexture = LoadTexture(ME);
    gCat1Texture = LoadTexture(CAT_1);
    gCat2Texture = LoadTexture(CAT_2);
    gCat3Texture = LoadTexture(CAT_3);

    SetTargetFPS(FPS);
}

void processInput() 
{
    if (WindowShouldClose()) gAppStatus = TERMINATED;
}

void update() {
    
    float ticks = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks = ticks;

    //Me
    gMePos.x += gMeDir * gMeSpeed * deltaTime;
    gMeTime += deltaTime;
    gMePos.y = gMeBase + gMeAmp * sinf(gMeTime * 1.5f);

    if(gMePos.x >= 850){
        gMePos.x = 850;
        gMeDir = -1;
    }
    else if(gMePos.x <= 150){
        gMePos.x = 150;
        gMeDir = 1;
    }

    //Cat 1
    gCat1Angle += gCat1Speed * deltaTime;
    gCat1Pos.x = gMePos.x + cos(gCat1Angle) * gCat1Radius;
    gCat1Pos.y = gMePos.y + sin(gCat1Angle) * gCat1Radius;

    //Cat 2
    gCat2Pos.x += gCat2Dir * gCat2Speed * deltaTime;
    gCat2Time += deltaTime;
    if(gCat2Pos.x >= 950){
        gCat2Pos.x = 950;
        gCat2Dir = -1;
    }
    else if(gCat2Pos.x <= 50){
        gCat2Pos.x = 50;
        gCat2Dir = 1;
    }
    gCat2Pos.y = gCat2Coeff * (gCat2Pos.x - SCREEN_WIDTH / 2.0f) * 
        (gCat2Pos.x - SCREEN_WIDTH / 2.0f) + 50;
    gCat2Scale = 0.6f + 0.4f * (gCat2Pos.y / gCat2BaseY);

    //Cat 3
    gCat3Time += deltaTime;
    if(gCat3Move){
        gCat3Pos.x += gCat3Dir * gCat3Speed * deltaTime;
        if(gCat3Pos.x >= gCat3NewPos.x){
            gCat3Pos.x = gCat3NewPos.x;
            gCat3Dir = -1;
            gCat3Move = false;
        }
        else if(gCat3Pos.x <= ORIGIN3.x){
            gCat3Pos.x = ORIGIN3.x;
            gCat3Dir = 1;
            gCat3Move = false;
        }
        gCat3Pos.y = -gCat3Pos.x + 1300;
        gCat3Angle = 0.0f;
    }
    else{
        gCat3Angle += gCat3RDir * gCat3RSpeed * deltaTime;
        if(gCat3Angle >= 10.0f){
            gCat3Angle = 10.0f;
            gCat3RDir = -1;
        }
        else if(gCat3Angle <= -10.0f){
            gCat3Angle = -10.0f;
            gCat3RDir = 1;
            gCat3Count++;
            if(gCat3Count >= gCat3Max){
                gCat3Count = 0;
                gCat3Move = true;
            }
            
        }
    }

    
}

void render()
{
    BeginDrawing();

    //background
    Rectangle textureAreaBg = {
        0.0f, 0.0f,
        static_cast<float>(gBgTexture.width),
        static_cast<float>(gBgTexture.height)
    };
    
    Rectangle destinationAreaBg = {
        0.0f, 0.0f,
        static_cast<float>(SCREEN_WIDTH),
        static_cast<float>(SCREEN_HEIGHT)
    };
    
    Vector2 originBg = {0.0f, 0.0f};

    DrawTexturePro(
        gBgTexture,
        textureAreaBg,
        destinationAreaBg,
        originBg,
        0.0f,
        WHITE
    );

    //Cat 2
    Rectangle textureAreaCat2 = {
        0.0f, 0.0f,
        static_cast<float>(gCat2Texture.width),
        static_cast<float>(gCat2Texture.height)
    };
    
    Rectangle destinationAreaCat2 = {gCat2Pos.x, gCat2Pos.y, 
        gCat2Size.x * gCat2Scale, gCat2Size.y * gCat2Scale};
    
    Vector2 originCat2 = {gCat2Size.x * gCat2Scale / 2.0f, gCat2Size.y * gCat2Scale/ 2.0f};

    DrawTexturePro(
        gCat2Texture,
        textureAreaCat2,
        destinationAreaCat2,
        originCat2,
        0.0f,
        WHITE
    );

    //air conditioner
    Rectangle textureAreaAc = {
        0.0f, 0.0f,
        static_cast<float>(gAcTexture.width),
        static_cast<float>(gAcTexture.height)
    };
    
    Rectangle destinationAreaAc = {492.0f, 125.0f, 300.0f, 100.0f};
    
    Vector2 originAc = {150.0f, 50.0f};

    DrawTexturePro(
        gAcTexture,
        textureAreaAc,
        destinationAreaAc,
        originAc,
        0.0f,
        WHITE
    );

    //Cat 3
    Rectangle textureAreaCat3 = {
        0.0f, 0.0f,
        static_cast<float>(gCat3Texture.width),
        static_cast<float>(gCat3Texture.height)
    };
    
    Rectangle destinationAreaCat3 = {gCat3Pos.x, gCat3Pos.y, gCat3Size.x, gCat3Size.y};
    
    Vector2 originCat3 = {gCat3Size.x / 2.0f, gCat3Size.y / 2.0f};

    DrawTexturePro(
        gCat3Texture,
        textureAreaCat3,
        destinationAreaCat3,
        originCat3,
        gCat3Angle,
        WHITE
    );

    //Cat 1
    Rectangle textureAreaCat1 = {
        0.0f, 0.0f,
        static_cast<float>(gCat1Texture.width),
        static_cast<float>(gCat1Texture.height)
    };
    
    Rectangle destinationAreaCat1 = {gCat1Pos.x, gCat1Pos.y, gCat1Size.x, gCat1Size.y};
    
    Vector2 originCat1 = {gCat1Size.x / 2.0f, gCat1Size.y / 2.0f};

    DrawTexturePro(
        gCat1Texture,
        textureAreaCat1,
        destinationAreaCat1,
        originCat1,
        0.0f,
        WHITE
    );

    //Me
    Rectangle textureAreaMe = {
        0.0f, 0.0f,
        (gMeDir == 1) ? static_cast<float>(-gMeTexture.width) : static_cast<float>(gMeTexture.width),
        static_cast<float>(gMeTexture.height)
    };
    
    Rectangle destinationAreaMe = {gMePos.x, gMePos.y, gMeSize.x, gMeSize.y};
    
    Vector2 originMe = {gMeSize.x / 2.0f, gMeSize.y / 2.0f};

    DrawTexturePro(
        gMeTexture,
        textureAreaMe,
        destinationAreaMe,
        originMe,
        0.0f,
        WHITE
    );

    EndDrawing();
}

void shutdown() 
{ 
    CloseWindow();
    UnloadTexture(gBgTexture);
    UnloadTexture(gAcTexture);
    UnloadTexture(gMeTexture);
    UnloadTexture(gCat1Texture);
    UnloadTexture(gCat2Texture);
    UnloadTexture(gCat3Texture);
}

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();
        render();
    }

    shutdown();

    return 0;
}