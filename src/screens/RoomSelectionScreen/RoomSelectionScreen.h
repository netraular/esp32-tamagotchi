#ifndef ROOMSELECTIONSCREEN_H
#define ROOMSELECTIONSCREEN_H

#include "../Screen.h"
#include <vector>
#include <lvgl.h>

class RoomSelectionScreen : public Screen {
public:
    void load() override;
    void update() override;
    void handleButtonEvent(const ButtonState& state, const ButtonChange& change) override;

private:
    std::vector<const char*> roomNames; // Nombres de las habitaciones
    std::vector<const lv_img_dsc_t*> roomImages; // Imágenes de fondo de las habitaciones
    int selectedRoomIndex = 0; // Índice de la habitación seleccionada
    lv_obj_t* backgroundImage; // Objeto LVGL para la imagen de fondo
    lv_obj_t* roomLabel; // Etiqueta para mostrar el nombre de la habitación

    void updateRoomDisplay(); // Actualiza la imagen de fondo y el nombre de la habitación
};

#endif