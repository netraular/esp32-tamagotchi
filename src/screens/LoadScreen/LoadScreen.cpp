#include "LoadScreen.h"
#include "../ScreenManager.h"
#include "../../ClockManager/ClockManager.h" // Ruta corregida
#include "../../PersistentDataManager/PersistentDataManager.h"

extern ScreenManager screenManager;

void LoadScreen::load() {
    // Crear una etiqueta para mostrar el mensaje de carga
    label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "Cargando...\nPulsa el botón 2 para PetScreen\nPulsa el botón 3 para MainMenu\nPulsa el botón 1 para guardar la hora actual");
    lv_obj_set_style_text_font(label, &lv_font_montserrat_16, 0);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, -20); // Centrar el texto en la pantalla

    // Crear una etiqueta para mostrar la hora
    clockLabel = lv_label_create(lv_scr_act());
    lv_label_set_text(clockLabel, "00:00:00");
    lv_obj_set_style_text_font(clockLabel, &lv_font_montserrat_24, 0);
    lv_obj_align(clockLabel, LV_ALIGN_CENTER, 0, 20); // Centrar el reloj en la pantalla

    // Inicializar el reloj solo si no se ha inicializado antes
    ClockManager::getInstance().begin();

    Serial.println("LoadScreen cargada.");
}

void LoadScreen::update() {
    // Actualizar el reloj
    ClockManager::getInstance().update();

    // Obtener la hora actual
    const char* timeString = ClockManager::getInstance().getTimeString();

    // Actualizar la etiqueta del reloj
    lv_label_set_text(clockLabel, timeString);
}

void LoadScreen::handleButtonEvent(const ButtonState& state, const ButtonChange& change) {
    // Si se presiona el botón 1, guardar la hora actual en settings.json
    if (change.button1Changed && state.button1Pressed) {
        saveCurrentTime();
    }

    // Si se presiona el botón 2, ir a PetScreen
    if (change.button2Changed && state.button2Pressed) {
        screenManager.setScreen("PetScreen");
    }

    // Si se presiona el botón 3, ir a MainMenu
    if (change.button3Changed && state.button3Pressed) {
        screenManager.setScreen("MainMenu");
    }
}

void LoadScreen::saveCurrentTime() {
    // Obtener la hora actual
    const char* timeString = ClockManager::getInstance().getTimeString();

    // Guardar la hora en settings.json
    if (PersistentDataManager::getInstance().saveDefaultTime(timeString)) {
        Serial.println("Hora guardada correctamente en settings.json.");
    } else {
        Serial.println("Error al guardar la hora en settings.json.");
    }
}