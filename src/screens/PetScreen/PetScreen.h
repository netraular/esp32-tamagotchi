#ifndef PETSCREEN_H
#define PETSCREEN_H

#include "../Screen.h"

class PetScreen : public Screen {
public:
    void load() override;
    void update() override;
    void handleButtonEvent(const ButtonState& state, const ButtonChange& change) override;

private:
    lv_obj_t* petLabel; // Etiqueta para mostrar la mascota
};

#endif