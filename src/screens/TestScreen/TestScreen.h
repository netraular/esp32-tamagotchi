#ifndef TESTSCREEN_H
#define TESTSCREEN_H

#include "../Screen.h"

class TestScreen : public Screen {
public:
    void load() override;
    void update() override;
    void handleButtonEvent(const ButtonState& state, const ButtonChange& change) override;

private:
    lv_obj_t* img; // Objeto para la imagen
};

#endif