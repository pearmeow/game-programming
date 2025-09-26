#include <raylib.h>

// for std max
#include <algorithm>

#include "CS3113/cs3113.h"

enum TeardropStatus { HANGING, RELEASED };

// Global Constants
constexpr int SCREEN_WIDTH = 800, SCREEN_HEIGHT = 450, FPS = 60, SPEED = 200, SHRINK_RATE = 100;

constexpr Vector2 ORIGIN = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2}, TEARDROP_BASE_SIZE = {100.0f, 100.0f},
                  BEAKER_BASE_SIZE = {250.0f, 250.0f}, TEARDROP_INIT_POS = {ORIGIN.x, ORIGIN.y - 130.0f},
                  BEAKER_INIT_POS = {ORIGIN.x + 200.0f, ORIGIN.y + 115.0f};

// @see https://idol.st/idols/ for source
constexpr char TEARDROP_FP[] = "assets/game/drops.png";
constexpr char BEAKER_FP[] = "assets/game/beaker.png";

// Global Variables
AppStatus gAppStatus = RUNNING;
float gAngle = 0.0f, gPreviousTicks = 0.0f;

Vector2 gPosition = TEARDROP_INIT_POS, gTeardropMovement = {0.0f, 0.0f}, gScale = TEARDROP_BASE_SIZE,

        gBeakerPosition = BEAKER_INIT_POS, gBeakerMovement = {0.0f, 0.0f}, gBeakerScale = BEAKER_BASE_SIZE,

        gMousePosition = GetMousePosition(), gMouseScale = {0.0f, 0.0f};

Texture2D gTexture;
Texture2D gBeakerTexture;

TeardropStatus gTeardropStatus = HANGING;

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();
bool isColliding(const Vector2* positionA, const Vector2* scaleA, const Vector2* positionB, const Vector2* scaleB);

// Function Definitions

/**
 * @brief Checks for a square collision between 2 Rectangle objects.
 *
 * @see
 *
 * @param postionA The position of the first object
 * @param scaleA The scale of the first object
 * @param positionB The position of the second object
 * @param scaleB The scale of the second object
 * @return true if a collision is detected,
 * @return false if a collision is not detected
 */
bool isColliding(const Vector2* positionA, const Vector2* scaleA, const Vector2* positionB,
                 const Vector2* scaleB) {
    float xDistance = fabs(positionA->x - positionB->x) - ((scaleA->x + scaleB->x) / 2.0f);
    float yDistance = fabs(positionA->y - positionB->y) - ((scaleA->y + scaleB->y) / 2.0f);

    if (xDistance < 0.0f && yDistance < 0.0f) return true;

    return false;
}

void renderObject(const Texture2D* texture, const Vector2* position, const Vector2* scale) {
    // Whole texture (UV coordinates)
    Rectangle textureArea = {// top-left corner
                             0.0f, 0.0f,

                             // bottom-right corner (of texture)
                             static_cast<float>(texture->width), static_cast<float>(texture->height)};

    // Destination rectangle – centred on gPosition
    Rectangle destinationArea = {position->x, position->y, static_cast<float>(scale->x),
                                 static_cast<float>(scale->y)};

    // Origin inside the source texture (centre of the texture)
    Vector2 originOffset = {static_cast<float>(scale->x) / 2.0f, static_cast<float>(scale->y) / 2.0f};

    // Render the texture on screen
    DrawTexturePro(*texture, textureArea, destinationArea, originOffset, gAngle, WHITE);
}

void initialise() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "User Input / Collision Detection");

    gTexture = LoadTexture(TEARDROP_FP);
    gBeakerTexture = LoadTexture(BEAKER_FP);

    SetTargetFPS(FPS);
}

void processInput() {
    // TODO - Get mouse position
    gMousePosition = GetMousePosition();

    // TODO - Check if mouse coordinates fall within teardrop object
    if (isColliding(&gMousePosition, &gMouseScale, &gPosition, &gScale)) {
        // TODO - If the player presses left mouse button within teardrop, release
        //        teardrop (i.e. use its movement vector)
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            printf("releasing\n");
            gTeardropStatus = RELEASED;
        }
    }

    // to avoid faster diagonal speed
    if (GetLength(&gTeardropMovement) > 1.0f) Normalise(&gTeardropMovement);

    // to close the game
    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;
}

void update() {
    // delta time
    float ticks = (float)GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks = ticks;

    // TODO - Stop horizontal translation once teardrop is released
    if (gTeardropStatus != RELEASED) {
        gPosition.x = gPosition.x + SPEED * cos(GetTime()) * deltaTime;  // moving back and forth on the X-AXIS
    } else {
        // TODO - Add vertical translation for teardrop
        gPosition.y = gPosition.y + SPEED * deltaTime;
    }

    // TODO - If teardrop is colliding with beaker, shrink the size of teardrop
    //        in a way that is FRAME-INDEPENDENT
    if (isColliding(&gPosition, &gScale, &gBeakerPosition, &gBeakerScale)) {
        gScale.x -= SHRINK_RATE * deltaTime;
        gScale.x = std::max(gScale.x, 0.0f);
        gScale.y -= SHRINK_RATE * deltaTime;
        gScale.y = std::max(gScale.x, 0.0f);
    }
}

void render() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // render teardrop
    renderObject(&gTexture, &gPosition, &gScale);

    // render the rupee
    renderObject(&gBeakerTexture, &gBeakerPosition, &gBeakerScale);

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
