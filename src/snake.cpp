#include <vector>

#include "main.cpp"
#include "raylib.h"
#include "map.hpp"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "amber/style_amber.h"
#include "ashes/style_ashes.h"
#include "bluish/style_bluish.h"
#include "candy/style_candy.h"
#include "cherry/style_cherry.h"
#include "cyber/style_cyber.h"
#include "enefete/style_enefete.h"
#include "genesis/style_genesis.h"
#include "lavanda/style_lavanda.h"
#include "rltech/style_rltech.h"
#include "sunny/style_sunny.h"
#include "terminal/style_terminal.h"

#define THEMES_COUNT 15 // including default

class Snake : public App {
private:
    int32_t window_width = 1000;
    int32_t window_height = 700;
    int32_t theme = 6;

    uint8_t status = 0;
    bool playing = false;

    Map map = Map(15, 4.0);
    int32_t size = 15;
    int32_t apple_count = 1;
    int32_t apple_pcount = 1;
    int32_t wall_count = 0;
    int32_t wall_pcount = 0;

    void apply_theme() {
        GuiLoadStyleDefault();
        switch (theme) {
        case 0: GuiLoadStyleAmber(); break;
        case 1: GuiLoadStyleAshes(); break;
        case 2: GuiLoadStyleBluish(); break;
        case 3: GuiLoadStyleCandy(); break;
        case 4: GuiLoadStyleCherry(); break;
        case 5: GuiLoadStyleCyber(); break;
        case 6: GuiLoadStyleGenesis(); break;
        case 7: GuiLoadStyleEnefete(); break;
        case 8: GuiLoadStyleLavanda(); break;
        case 9: GuiLoadStyleDefault(); break;
        case 10: GuiLoadStyleRLTech(); break;
        case 11: GuiLoadStyleSunny(); break;
        case 12: GuiLoadStyleTerminal(); break;
        
        default:
            break;
        } GuiSetStyle(DEFAULT, TEXT_SIZE, 32);
    }

    void map_reset() {
        map.set_snake(map.get_size() / 2, map.get_size() / 2);
        map.set_apple(apple_count);
        map.set_wall(wall_count);
    }
public:
    Snake() {
        // SetConfigFlags(FLAG_VSYNC_HINT);
        InitWindow(
            window_width, 
            window_height, 
            "8-bit snake"
        ); SetTargetFPS(24);

        apply_theme();
        
        map_reset();
        map.body_collision = true;
        map.border_collision = true;
    }

    ~Snake() { 
        CloseWindow();
    }

    void run() override {
        int32_t text_width = 0;
        bool on_apple = false;
        bool on_wall = false;
        bool on_tick = false;
        bool on_drop = false;
        const char* options = "amber;ashes;bluish;candy;cherry;cyber;dark;enefete;lavanda;light;rltech;sunny;terminal";
        while (!WindowShouldClose()) {
            switch (status) {
            case 0: // paused
                if (IsKeyPressed(KEY_M)) { 
                    size += 5;
                    if (size > 50) size = 50;
                    map.resize(size);
                    map_reset();
                } else if (IsKeyPressed(KEY_N)) {
                    size -= 5;
                    if (size < 5) size = 5;
                    map.resize(size); 
                    map_reset();
                }

                if (IsKeyPressed(KEY_SPACE)) {
                    playing = true;
                    status = 1;
                } else if (IsKeyPressed(KEY_R)) {
                    map_reset();
                    playing = false;
                } break;
            
            case 1: // playing
                if (IsKeyPressed(KEY_SPACE)) status = 0;

                map.update();
                if (map.snake_alive == false) {
                    status = 2;
                } break;

            case 2: // game over
                if (IsKeyPressed(KEY_SPACE)) {
                    map_reset();
                    playing = false;
                    status = 0;
                } break;
            }

            BeginDrawing();
                ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
                
                if (playing || on_drop) GuiDisable(); else GuiEnable();
                GuiCheckBox({10, 52, 32, 32}, "Body Collision", &map.body_collision);
                GuiCheckBox({10, 94, 32, 32}, "Border Collision", &map.border_collision);
                
                if (GuiSpinner({10, 146, 280, 32}, "", &apple_count, 1, size, on_apple)) on_apple = !on_apple;
                GuiLabel({10, 178, 280, 32}, "Apple Count");
                if (GuiSpinner({10, 220, 280, 32}, "", &wall_count, 0, size, on_wall)) on_wall = !on_wall;
                GuiLabel({10, 252, 280, 32}, "Wall Count");
                if (GuiSpinner({10, 294, 280, 32}, "", &map.tick_rate, 1, 999, on_tick)) { on_tick = !on_tick; }
                GuiLabel({10, 326, 280, 32}, "Tick rate");

                if (apple_count > size) apple_count = size;
                else if (apple_count < 1) apple_count = 1;

                if (wall_count > size) wall_count = size;
                else if (wall_count < 0) wall_count = 0;

                if (map.tick_rate > 999) map.tick_rate = 999;
                else if (map.tick_rate < 1) map.tick_rate = 1;

                if (apple_pcount != apple_count) {
                    apple_pcount = apple_count;
                    map.set_apple(apple_count);
                } if (wall_pcount != wall_count) {
                    wall_pcount = wall_count;
                    map.set_wall(wall_count);
                }
                
                GuiEnable();
                GuiLabel({10, 410, 280, 32}, "Key Bindings:");
                if (status == 0) GuiLabel({10, 452, 280, 32}, "SPACE - start");
                else if (status == 1) GuiLabel({10, 452, 280, 32}, "SPACE - pause");
                else GuiLabel({10, 452, 280, 32}, "SPACE - play again");
                if (status != 0) GuiDisable(); else GuiEnable(); 
                GuiLabel({10, 494, 280, 32}, "R - reset");
                if (playing) GuiDisable(); else GuiEnable(); 
                GuiLabel({10, 536, 280, 32}, "M - size up");
                GuiLabel({10, 578, 280, 32}, "N - size down");
                if (status != 1) GuiDisable(); else GuiEnable();
                GuiLabel({10, 620, 280, 32}, "WASD - direction");
                GuiLabel({10, 662, 280, 32}, "Arrows - direction");
                
                GuiEnable();
                GuiLabel({10, 368, 280, 32}, TextFormat("Score: %d", map.get_snake_length() - 3));
                if (GuiDropdownBox({10, 10, 280, 32}, options, &theme, on_drop)) {
                    on_drop = !on_drop;
                    apply_theme();
                } map.draw({300, 0, 700, 700});
            EndDrawing();
        }
    }
};

App* main_app(int argc, char* argv[]) {
    return new Snake();
}