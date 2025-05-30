#include "raylib.h"
#include <string>

int loses = 0;
int wins = 0;
int contacts = 0;
float waittime = 0.0f;
const float vel = 800.0f;
bool win = true;
Vector2 ballvel = {0.0f, 0.0f};
Vector2 rectprop = {50.0f, 250.0f};

bool collisionWallsTop(Vector2 pos) {

    if (pos.y <= 0.1f) return true;

    else return false;
}

bool collisionWallsBottom(Vector2 pos) {

    if (pos.y >= 830.0f) return true;

    else return false;
}

void control(Vector2 &pos, float dt) {

    if (IsKeyDown(KEY_W) && !collisionWallsTop(pos)) pos.y -= vel * dt;
    if (IsKeyDown(KEY_S) && !collisionWallsBottom(pos)) pos.y += vel * dt;
}

void ballSetVel(Vector2 &ballpos) {

    ballpos = {960.0f, 540.0f};

    if (win) ballvel.x = 10.0f;
    else if (!win) ballvel.x = -10.0f;

    ballvel.y = (float)(GetRandomValue(-10, 10));

    waittime = 0;
}

bool timer(float time, float dt) {

    waittime += dt;

    if (waittime < time) return false;
    else return true;
}

void ballPhysics(Vector2 &ballpos, Vector2 botpos, Vector2 plrpos, float dt) {

    if (ballpos.x <= (plrpos.x + rectprop.x) &&
    (ballpos.x - ballvel.x) >= plrpos.x &&
    ballpos.y <= (plrpos.y + rectprop.y) &&
    ballpos.y >= plrpos.y) {

        if (contacts < 1) ballvel.x = -ballvel.x * 2;
        else ballvel.x = -ballvel.x * 1.02;
 
        contacts++;
    }

    else if (ballpos.x <= (botpos.x + rectprop.x) &&
    (ballpos.x - ballvel.x) >= botpos.x &&
    ballpos.y <= (botpos.y + rectprop.y) &&
    ballpos.y >= botpos.y) {

        if (contacts < 1) ballvel.x = -ballvel.x * 2;
        else ballvel.x = -ballvel.x * 1.02;

        contacts++;
    }

    if (ballpos.y <= 10.0f) ballvel.y = -ballvel.y;
    else if (ballpos.y >= 1060.0f) ballvel.y = -ballvel.y;

    ballpos.x -= ballvel.x;
    ballpos.y += ballvel.y;
}

void botPhysics(Vector2 &botpos, Vector2 ballpos, float dt) {

    if (ballpos.y > (botpos.y + rectprop.y / 2) && !collisionWallsBottom(botpos)) botpos.y += vel * dt;
    else if (ballpos.y < (botpos.y + rectprop.y / 2) && !collisionWallsTop(botpos)) botpos.y -= vel * dt;
}

bool outOfMap(Vector2 ballpos) {

    if (ballpos.x <= 0.0f) {
        win = false;
        loses++;
        return true;
    }

    else if (ballpos.x >= 1920.0f) {
        win = true;
        wins++;
        return true;
    }

    else return false;
}

bool checkWin() {

    if (wins == 10) return true;
    else return false;
}

bool checkLose() {

    if (loses == 10) return true;
    else return false;
}

int main() {

    Vector2 plrpos = {100.0f, 540.0f};
    Vector2 botpos = {1820.0f, 540.0f};
    Vector2 ballpos = {960.0f, 540.0f};
    std::string score;

    InitWindow(1920, 1080, "Pong");

    SetTargetFPS(60);

    ballSetVel(ballpos); 

    while (!WindowShouldClose()) {

        float dt = GetFrameTime();
        score = std::to_string(wins) + " : " + std::to_string(loses);

        if (checkWin()) {

            ballSetVel(ballpos);

            wins = 0;
            loses = 0;

            while (!WindowShouldClose()) {

                if (IsKeyPressed(KEY_R)) break;
                
                BeginDrawing();
                ClearBackground(BLACK);
                DrawText("You Win!", 910, 540, 50, WHITE);
                DrawText("Press R to restart", 900, 900, 20, WHITE);
                EndDrawing();
            }

            EndDrawing();

            continue;
        }

        else if (checkLose()) {

            ballSetVel(ballpos);

            wins = 0;
            loses = 0;

            while (!WindowShouldClose()) {

                if (IsKeyPressed(KEY_R)) break;
                
                BeginDrawing();
                ClearBackground(BLACK);
                DrawText("You Lose!", 910, 540, 50, WHITE);
                DrawText("Press R to restart", 900, 900, 20, WHITE);
                EndDrawing();
            }

            EndDrawing();

            continue;
        }

        else {

            control(plrpos, dt);
            botPhysics(botpos, ballpos, dt);
                
            if (timer(1.5f, dt)) ballPhysics(ballpos, botpos, plrpos, dt);

            if (outOfMap(ballpos)) {

                ballSetVel(ballpos);
                contacts = 0;
            }
            
            BeginDrawing();
            ClearBackground(BLACK);

            DrawText(score.c_str(), 910, 900, 50, WHITE);
            DrawCircleV(ballpos, 20.0f, WHITE);
            DrawRectangleV(plrpos, rectprop, WHITE);
            DrawRectangleV(botpos, rectprop, WHITE);
                    
            EndDrawing(); 
        }
    }

    CloseWindow();

    return 0;
}