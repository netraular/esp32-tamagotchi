#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include "../Screen.h"

// Definir el enum ButtonStateEnum
enum ButtonStateEnum {
    STATE_NONE,    // Gris
    STATE_PRESSED, // Verde
    STATE_COMBO    // Rojo
};

class ButtonsTestScreen : public Screen {
public:
    void load() override;
    void update() override;
    void handleButtonEvent(const ButtonState& state, const ButtonChange& change) override;

private:
    void updateCircleColor(lv_obj_t* circle, ButtonStateEnum state, uint8_t animationValue); // Declaración de la nueva función
};

#endif