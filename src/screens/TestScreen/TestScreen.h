#ifndef TESTSCREEN_H
#define TESTSCREEN_H

#include "../Screen.h"
#include "../../components/Keyboard/Keyboard.h"

class TestScreen : public Screen {
public:
    void load() override;
    void update() override;
    void handleButtonEvent(const ButtonState& state, const ButtonChange& change) override;

private:
    Keyboard* keyboard; // Teclado
    lv_obj_t* outputLabel; // Etiqueta para mostrar el valor seleccionado
};

#endif