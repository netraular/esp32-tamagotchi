#include "PetScreen.h"
#include "../ScreenManager.h"
#include "../../ClockManager/ClockManager.h" // Incluir ClockManager

extern ScreenManager screenManager;

void PetScreen::load() {
    // Crear una etiqueta para mostrar la mascota
    petLabel = lv_label_create(lv_scr_act());
    lv_label_set_text(petLabel, "Pet Screen");
    lv_obj_set_style_text_font(petLabel, &lv_font_montserrat_16, 0);
    lv_obj_align(petLabel, LV_ALIGN_CENTER, 0, -20); // Centrar el texto en la pantalla

    // Crear una etiqueta para mostrar la hora
    clockLabel = lv_label_create(lv_scr_act());
    lv_label_set_text(clockLabel, "00:00:00");
    lv_obj_set_style_text_font(clockLabel, &lv_font_montserrat_24, 0);
    lv_obj_align(clockLabel, LV_ALIGN_TOP_RIGHT, -10, 10); // Alinear en la esquina superior derecha

    Serial.println("PetScreen cargada.");
}

void PetScreen::update() {
    // Obtener la hora actual
    const char* timeString = ClockManager::getInstance().getTimeString();

    // Actualizar la etiqueta del reloj
    lv_label_set_text(clockLabel, timeString);
}

void PetScreen::handleButtonEvent(const ButtonState& state, const ButtonChange& change) {
    // Si se presiona el botón 3, ir a MainMenu
    if (change.button3Changed && state.button3Pressed) {
        screenManager.setScreen("MainMenu");
    }
}