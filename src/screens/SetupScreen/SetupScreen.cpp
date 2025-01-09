#include "SetupScreen.h"
#include "../ScreenManager.h"
#include "../../PersistentDataManager/PersistentDataManager.h"

extern ScreenManager screenManager;
extern PersistentDataManager persistentDataManager;

void SetupScreen::load() {
    // Crear una etiqueta con el texto "start kodama"
    label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "start kodama");
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

    // Si se presiona el botón 1, resetear los archivos JSON
    if (change.button1Changed && state.button1Pressed) {
        if (persistentDataManager.resetFoodData() && persistentDataManager.resetOwnFoodData() && persistentDataManager.resetPetStats()) {
            Serial.println("Archivos JSON reseteados correctamente");
        } else {
            Serial.println("Error al resetear los archivos JSON");
        }
    }
}