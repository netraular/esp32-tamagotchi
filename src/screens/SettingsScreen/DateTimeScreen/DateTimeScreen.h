#ifndef DATETIMESCREEN_H
#define DATETIMESCREEN_H

#include "../../Screen.h"

class DateTimeScreen : public Screen {
public:
    void load() override;
    void update() override;
    void handleButtonEvent(const ButtonState& state, const ButtonChange& change) override;

private:
    lv_obj_t* label; // Etiqueta para mostrar el nombre de la ventana
};

#endif