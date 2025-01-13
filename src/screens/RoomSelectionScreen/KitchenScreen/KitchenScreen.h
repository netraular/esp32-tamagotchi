#ifndef KITCHENSCREEN_H
#define KITCHENSCREEN_H

#include "screens/Screen.h"
#include <lvgl.h>

class KitchenScreen : public Screen {
public:
    void load() override;
    void update() override;
    void handleButtonEvent(const ButtonState& state, const ButtonChange& change) override;

private:
    lv_obj_t* backgroundImage; // Imagen de fondo
    lv_obj_t* roomLabel;       // Etiqueta de la habitación
    lv_obj_t* animimg;         // Objeto de animación de imágenes
};

#endif