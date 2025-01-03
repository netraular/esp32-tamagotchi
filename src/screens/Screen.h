#ifndef SCREEN_H
#define SCREEN_H

#include <lvgl.h>

struct ButtonState {
    bool button1Pressed;
    bool button2Pressed;
    bool button3Pressed;
};

struct ButtonChange {
    bool button1Changed;
    bool button2Changed;
    bool button3Changed;
};

struct ButtonChangeBuffer {
    ButtonState state;       // Estado actual de los botones
    ButtonChange change;     // Cambios detectados en los botones
};

class Screen {
public:
    virtual ~Screen() {}
    virtual void load() = 0;
    virtual void update() = 0;
    virtual void handleButtonEvent(const ButtonState& state, const ButtonChange& change) = 0; // Función unificada
};

#endif