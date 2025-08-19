#include "main.cpp"
#include "raylib.h"

class Snake : public App {
private:
    int32_t window_width = 800;
    int32_t window_height = 600;

public:
    Snake() { 
        InitWindow(
            window_width, 
            window_height, 
            "8-bit snake"
        );
    }

    ~Snake() { 
        CloseWindow();
    }

    void run() override { 
        while (!WindowShouldClose()) {
            BeginDrawing();
                ClearBackground(WHITE);
                DrawText("Hello, World!", 10, 10, 50, BLACK);
            EndDrawing();
        }
    }

};

App* main_app(int argc, char* argv[]) {
    return new Snake();
}