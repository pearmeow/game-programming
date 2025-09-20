#include <math.h>
#include <raylib.h>

#include <iostream>
#include <vector>

#include "CS3113/cs3113.h"

enum Member { MURDOC, TWO_D, RUSSEL, NOODLE };

// Global Constants
constexpr int SCREEN_WIDTH = 1600 / 2, SCREEN_HEIGHT = 900 / 2, FPS = 60, SIZE = 200, FRAME_LIMIT = 100;
constexpr float MAX_AMP = 10.0f;

constexpr char BG_COLOUR[] = "#000000";
constexpr Vector2 ORIGIN = {SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0};
constexpr Vector2 BASE_SIZE = {(float)SIZE, (float)SIZE};

// Image owned by Gorillaz @see https://gorillaz.com/
constexpr char ALBUM_COVER_FP[] = "assets/demon_days.png";

// Global Variables
AppStatus gAppStatus = RUNNING;
float gScaleFactor = SIZE, gAngle = 0.0f, gPulseTime = 0.0f;
Vector2 gPosition = ORIGIN;
Vector2 gScale = BASE_SIZE;
float gPreviousTicks = 0.0f;
float gPreviousTime = 0.0f;
std::vector<std::vector<float>> gCoords;
int gCurrMember = MURDOC;
Texture2D gTexture;

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();

// Function Definitions
void initialise() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Textures");

    gTexture = LoadTexture(ALBUM_COVER_FP);
    gCoords = {{0.0f, 0.0f, gTexture.width / 2.0f, gTexture.height / 2.0f},
               {gTexture.width / 2.0f, 0.0f, gTexture.width / 2.0f, gTexture.height / 2.0f},
               {0.0f, gTexture.height / 2.0f, gTexture.width / 2.0f, gTexture.height / 1.0f},
               {gTexture.width / 2.0f, gTexture.height / 2.0f, gTexture.width / 1.0f, gTexture.height / 1.0f}};

    SetTargetFPS(FPS);
}

void processInput() {
    if (WindowShouldClose()) gAppStatus = TERMINATED;
}

void update() {
    /**
     * @todo Calculate delta time
     */
    float ticks = (float)GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks = ticks;

    /**
     * @todo Apply delta time to the time-dependent logic
     */
    gPulseTime += 1.0f * deltaTime;

    gScale = {BASE_SIZE.x + MAX_AMP * cos(gPulseTime), BASE_SIZE.y + MAX_AMP * cos(gPulseTime)};

    /**
     * @todo Switch member every 100 fames
     */
    if (ticks > gPreviousTime + 1.0f) {
        gPreviousTime = ticks;
        if (gCurrMember == MURDOC) {
            gCurrMember = TWO_D;
        } else if (gCurrMember == TWO_D) {
            gCurrMember = RUSSEL;
        } else if (gCurrMember == RUSSEL) {
            gCurrMember = NOODLE;
        } else if (gCurrMember == NOODLE) {
            gCurrMember = MURDOC;
        }
    }
}

void render() {
    BeginDrawing();
    ClearBackground(ColorFromHex(BG_COLOUR));

    /**
     * @todo Design your UV coordinates (i.e. textureArea) so that only one
     * member is being rendered onto the screen.
     */
    Rectangle textureArea = {// top-left corner
                             gCoords[gCurrMember][0], gCoords[gCurrMember][1], static_cast<float>(gScale.x),
                             static_cast<float>(gScale.y)};
    // Destination rectangle – centred on gPosition
    Rectangle destinationArea = {gCoords[gCurrMember][2], gCoords[gCurrMember][3], static_cast<float>(gScale.x),
                                 static_cast<float>(gScale.y)};

    // Origin inside the source texture (centre of the texture)
    Vector2 objectOrigin = {static_cast<float>(gScale.x) / 2.0f, static_cast<float>(gScale.y) / 2.0f};

    // Render the texture on screen
    DrawTexturePro(gTexture, textureArea, destinationArea, objectOrigin, gAngle, WHITE);

    EndDrawing();
}

void shutdown() {
    CloseWindow();
}

int main(void) {
    initialise();

    while (gAppStatus == RUNNING) {
        processInput();
        update();
        render();
    }

    shutdown();

    return 0;
}
