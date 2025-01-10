#ifndef TESTSCREEN_H
#define TESTSCREEN_H

#include "../Screen.h"
#include "../../components/Keyboard/Keyboard.h"
#include "../../components/InputText/InputText.h"

class TestScreen : public Screen {
public:
    void load() override;
    void update() override;
    void handleButtonEvent(const ButtonState& state, const ButtonChange& change) override;

private:
    Keyboard* keyboard; // Teclado
    InputText* inputText; // Componente InputText
    lv_obj_t* outputLabel; // Etiqueta para mostrar el valor seleccionado
    bool isKeyboardActive; // Indica si el teclado está activo
};

#endif