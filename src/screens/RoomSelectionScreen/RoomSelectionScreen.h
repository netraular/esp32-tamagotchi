#ifndef ROOMSELECTIONSCREEN_H
#define ROOMSELECTIONSCREEN_H

#include "screens/Screen.h"
#include <vector>
#include <lvgl.h>

class RoomSelectionScreen : public Screen {
public:
    void load() override;
    void update() override;
    void handleButtonEvent(const ButtonState& state, const ButtonChange& change) override;

    // Función para obtener la posición actual de la burbuja
    int getBubbleX() const { return lv_obj_get_x(bubbleImage); }
    int getBubbleY() const { return lv_obj_get_y(bubbleImage); }

private:
    std::vector<const char*> roomNames; // Nombres de las habitaciones
    std::vector<const lv_img_dsc_t*> roomImages; // Imágenes de fondo de las habitaciones
    int selectedRoomIndex = 3; // Índice de la habitación seleccionada (LivingRoom por defecto)
    lv_obj_t* backgroundImage; // Objeto LVGL para la imagen de fondo
    lv_obj_t* roomLabel; // Etiqueta para mostrar el nombre de la habitación
    lv_obj_t* arrowLeft; // Flecha izquierda
    lv_obj_t* arrowRight; // Flecha derecha
    lv_obj_t* bubbleImage; // Imagen "bubble" en la esquina superior derecha
    lv_anim_t animLeft; // Animación para la flecha izquierda
    lv_anim_t animRight; // Animación para la flecha derecha
    lv_anim_t animBubbleX; // Animación horizontal para la burbuja
    lv_anim_t animBubbleY; // Animación vertical para la burbuja

    void updateRoomDisplay(); // Actualiza la imagen de fondo, el nombre de la habitación y las flechas
};

#endif