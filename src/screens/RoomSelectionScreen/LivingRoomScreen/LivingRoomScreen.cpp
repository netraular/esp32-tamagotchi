#include "LivingRoomScreen.h"
#include "../../ScreenManager.h"

extern const lv_img_dsc_t background4; // Imagen de fondo correspondiente
extern const lv_img_dsc_t bubble; // Imagen de la burbuja
extern ScreenManager screenManager;

void LivingRoomScreen::load() {
    // Crear la imagen de fondo
    backgroundImage = lv_img_create(lv_scr_act());
    lv_img_set_src(backgroundImage, &background4);
    lv_obj_set_size(backgroundImage, 128, 160);
    lv_obj_align(backgroundImage, LV_ALIGN_CENTER, 0, 0);

    // Crear la etiqueta del nombre de la habitación
    roomLabel = lv_label_create(lv_scr_act());
    lv_label_set_text(roomLabel, "Living Room");
    lv_obj_set_style_text_font(roomLabel, &lv_font_montserrat_16, 0);
    lv_obj_align(roomLabel, LV_ALIGN_CENTER, 0, 0);

    // Crear la burbuja en la posición recibida
    bubbleImage = lv_img_create(lv_scr_act());
    lv_img_set_src(bubbleImage, &bubble);
    lv_obj_set_size(bubbleImage, 32, 32);
    lv_obj_set_pos(bubbleImage, bubbleX, bubbleY); // Posicionar la burbuja

    Serial.println("LivingRoomScreen cargada.");
}

void LivingRoomScreen::update() {
    // No es necesario actualizar nada en esta pantalla
}

void LivingRoomScreen::handleButtonEvent(const ButtonState& state, const ButtonChange& change) {
    if (change.button3Changed && state.button3Pressed) {
        screenManager.setScreen("RoomSelectionScreen");
    }
}