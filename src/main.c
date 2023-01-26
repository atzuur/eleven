#include <raylib.h>

int main(void) {

    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [shapes] example - bouncing ball");

    Vector2 ballPosition = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
    Vector2 ballSpeed = {5.0f, 4.0f};
    int ballRadius = 20;

    bool pause = 0;
    int framesCounter = 0;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {

        if (IsKeyPressed(KEY_SPACE))
            pause = !pause;

        if (!pause) {
            ballPosition.x += ballSpeed.x;
            ballPosition.y += ballSpeed.y;

            if ((ballPosition.x >= (GetScreenWidth() - ballRadius)) ||
                (ballPosition.x <= ballRadius))
                ballSpeed.x *= -1.0f;
            if ((ballPosition.y >= (GetScreenHeight() - ballRadius)) ||
                (ballPosition.y <= ballRadius))
                ballSpeed.y *= -1.0f;
        } else
            framesCounter++;

        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawCircleV(ballPosition, (float)ballRadius, MAROON);
        DrawText("PRESS SPACE to PAUSE BALL MOVEMENT", 10, GetScreenHeight() - 25, 20, LIGHTGRAY);

        if (pause && ((framesCounter / 30) % 2))
            DrawText("PAUSED", 350, 200, 30, GRAY);

        DrawFPS(10, 10);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}