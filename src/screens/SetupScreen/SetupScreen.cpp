#include "SetupScreen.h"
#include "../ScreenManager.h"

extern ScreenManager screenManager;

void SetupScreen::load() {

    // Crear una etiqueta con el texto "delete files"
    label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "delete files");
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0); // Centrar el texto
    lv_obj_set_style_text_font(label, &lv_font_montserrat_16, 0); // Fuente mediana
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0); // Centrar en la pantalla

    Serial.println("SetupScreen cargada.");
}

void SetupScreen::update() {
    // No es necesario actualizar nada en esta pantalla
}

void SetupScreen::handleButtonEvent(const ButtonState& state, const ButtonChange& change) {
    // Si se presiona el botón 3, volver a la pantalla PetScreen
    if (change.button3Changed && state.button3Pressed) {
        screenManager.setScreen("PetScreen");
    }
}


