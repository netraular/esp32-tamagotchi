#ifndef TESTSCREEN2_H
#define TESTSCREEN2_H

#include "../Screen.h"
#include "../../components/InputTextWithKeyboard/InputTextWithKeyboard.h"

class TestScreen2 : public Screen {
public:
    void load() override;
    void update() override;
    void handleButtonEvent(const ButtonState& state, const ButtonChange& change) override;

private:
    InputTextWithKeyboard* inputTextWithKeyboard; // Componente InputTextWithKeyboard
    lv_obj_t* outputLabel; // Etiqueta para mostrar el valor seleccionado
};

#endif