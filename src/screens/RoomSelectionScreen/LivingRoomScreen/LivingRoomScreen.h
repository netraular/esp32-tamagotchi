#ifndef LIVINGROOMSCREEN_H
#define LIVINGROOMSCREEN_H

#include "screens/Screen.h"
#include <lvgl.h>

class LivingRoomScreen : public Screen {
public:
    void load() override;
    void update() override;
    void handleButtonEvent(const ButtonState& state, const ButtonChange& change) override;

    // Función para establecer la posición de la burbuja
    void setBubblePosition(int x, int y) {
        bubbleX = x;
        bubbleY = y;
    }

private:
    lv_obj_t* backgroundImage;
    lv_obj_t* roomLabel;
    lv_obj_t* bubbleImage; // Objeto para la burbuja
    int bubbleX = 0; // Posición X de la burbuja
    int bubbleY = 0; // Posición Y de la burbuja
};

#endif