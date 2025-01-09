#include "SleepTimeScreen.h"
#include "../../ScreenManager.h"

extern ScreenManager screenManager;

void SleepTimeScreen::load() {
    // Crear una etiqueta con el nombre de la ventana
    label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "Hora de dormir");
    lv_obj_set_style_text_font(label, &lv_font_montserrat_16, 0);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    Serial.println("SleepTimeScreen cargada.");
}

void SleepTimeScreen::update() {
    // No es necesario actualizar nada en esta pantalla
}

void SleepTimeScreen::handleButtonEvent(const ButtonState& state, const ButtonChange& change) {
    // Volver a SettingsScreen con el botón 3
    if (change.button3Changed && state.button3Pressed) {
        screenManager.setScreen("SettingsScreen");
    }
}