#include "PetScreen.h"
#include "../ScreenManager.h"

extern ScreenManager screenManager;

void PetScreen::load() {
    // Crear una etiqueta para mostrar la mascota
    petLabel = lv_label_create(lv_scr_act());
    lv_label_set_text(petLabel, "Pet Screen");
    lv_obj_set_style_text_font(petLabel, &lv_font_montserrat_16, 0);
    lv_obj_align(petLabel, LV_ALIGN_CENTER, 0, 0); // Centrar el texto en la pantalla

    Serial.println("PetScreen cargada.");
}

void PetScreen::update() {
    // No es necesario actualizar nada en esta pantalla
}

void PetScreen::handleButtonEvent(const ButtonState& state, const ButtonChange& change) {
    // Si se presiona el botón 3, ir a MainMenu
    if (change.button3Changed && state.button3Pressed) {
        screenManager.setScreen("MainMenu");
    }
}