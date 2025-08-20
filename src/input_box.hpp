#ifndef INPUT_BOX_H
#define INPUT_BOX_H

#include <string>

#include "raylib.h"

enum InputType {
    TEXT,
    NUMBER 
};

class InputBox {
private:
    enum InputType type = TEXT;
    std::string text = "";
    int32_t max_char = 0;
    Rectangle rect = { 0 };
    bool focus = false;

public:
    InputBox(InputType type, int32_t max_char, Rectangle rect);
    ~InputBox();

    void update();
    void draw();
    std::string str();
    const char* c_str();
    int32_t num();
};

#endif // INPUT_BOX_H