#ifndef WIFISCREEN_H
#define WIFISCREEN_H

#include "../../Screen.h"

class WifiScreen : public Screen {
public:
    void load() override;
    void update() override;
    void handleButtonEvent(const ButtonState& state, const ButtonChange& change) override;

private:
    lv_obj_t* label; // Etiqueta para mostrar el nombre de la ventana
};

#endif