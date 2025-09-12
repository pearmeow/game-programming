/**
 * @file
 * @author Perry Huang
 * @date 2025-
 */

#include "raylib.h"

// Enums
enum AppStatus { TERMINATED, RUNNING };
enum Direction { LEFT, RIGHT };

// Global Constants
constexpr int SCREEN_WIDTH = 800, SCREEN_HEIGHT = 450, FPS = 60, SIDES = 4;

constexpr Vector2 ORIGIN = {SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0};

// Global Variables
AppStatus gAppStatus = RUNNING;
float gAngle = 0.0f;
float gScaleFactor = 100;
Direction gDirection = LEFT;
Vector2 gPosition = {0.0f, 0.0f};

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();

// Function Definitions
void initialise() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello raylib!");

    SetTargetFPS(FPS);
}

void processInput() {
    if (WindowShouldClose()) gAppStatus = TERMINATED;
}

void update() {
    // scaling
    gScaleFactor *= 1.001;

    // translation
    if (gPosition.x < SCREEN_WIDTH) {
        gPosition.x += 1.0f;
    }
    if (gPosition.y < SCREEN_HEIGHT) {
        gPosition.y += 0.3f;
    }

    // rotation
    gAngle += 1.0f * gDirection == RIGHT ? 1 : -1;
    if (gAngle > 20.0f) {
        gDirection = LEFT;
    }
    if (gAngle < -20.0f) {
        gDirection = RIGHT;
    }
}

void render() {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawPoly(gPosition, SIDES, gScaleFactor, gAngle, ORANGE);

    EndDrawing();
}

void shutdown() {
    CloseWindow();  // Close window and OpenGL context
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
