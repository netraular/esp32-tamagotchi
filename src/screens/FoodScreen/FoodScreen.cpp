#include "FoodScreen.h"
#include "../ScreenManager.h"

extern ScreenManager screenManager;

void FoodScreen::load() {
    // Crear una etiqueta con el texto "Food"
    label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "Food");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0); // Centrar el texto en la pantalla

    Serial.println("FoodScreen cargada.");
}

void FoodScreen::update() {
    // No es necesario actualizar nada en esta pantalla
}

void FoodScreen::handleButtonEvent(const ButtonState& state, const ButtonChange& change) {
    // Si se presiona el botón 3, volver a la pantalla MainMenu
    if (change.button3Changed && state.button3Pressed) {
        screenManager.setScreen("MainMenu");
    }
}