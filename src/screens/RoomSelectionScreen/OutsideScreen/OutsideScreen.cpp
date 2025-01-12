#include "OutsideScreen.h"
#include "../../ScreenManager.h"

extern const lv_img_dsc_t background6; // Imagen de fondo correspondiente
extern ScreenManager screenManager;

void OutsideScreen::load() {
    backgroundImage = lv_img_create(lv_scr_act());
    lv_img_set_src(backgroundImage, &background6);
    lv_obj_set_size(backgroundImage, 128, 160);
    lv_obj_align(backgroundImage, LV_ALIGN_CENTER, 0, 0);

    roomLabel = lv_label_create(lv_scr_act());
    lv_label_set_text(roomLabel, "Outside");
    lv_obj_set_style_text_font(roomLabel, &lv_font_montserrat_16, 0);
    lv_obj_align(roomLabel, LV_ALIGN_CENTER, 0, 0);

    Serial.println("OutsideScreen cargada.");
}

void OutsideScreen::update() {
    // No es necesario actualizar nada en esta pantalla
}

void OutsideScreen::handleButtonEvent(const ButtonState& state, const ButtonChange& change) {
    if (change.button3Changed && state.button3Pressed) {
        screenManager.setScreen("RoomSelectionScreen");
    }
}