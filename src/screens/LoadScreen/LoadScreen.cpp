#include "LoadScreen.h"
#include "../ScreenManager.h"
#include "../../ClockManager/ClockManager.h"
#include "../../PersistentDataManager/PersistentDataManager.h"
#include <lvgl.h>
#include <vector>
#include "esp_system.h" // Para esp_random()

// Declarar las variables externas de las imágenes de fondo
extern const lv_img_dsc_t loading1;
extern const lv_img_dsc_t loading2;
extern const lv_img_dsc_t loading3;
extern const lv_img_dsc_t loading4;
extern const lv_img_dsc_t loading5;
extern const lv_img_dsc_t loading6;

extern ScreenManager screenManager;

void LoadScreen::load() {
    // Limpiar la pantalla si ya había contenido
    lv_obj_clean(lv_scr_act());

    // Cargar una imagen de fondo aleatoria
    loadRandomBackground();

    // Crear una etiqueta para mostrar la hora
    clockLabel = lv_label_create(lv_scr_act());
    lv_label_set_text(clockLabel, "00:00:00");
    lv_obj_set_style_text_font(clockLabel, &lv_font_montserrat_24, 0);
    lv_obj_align(clockLabel, LV_ALIGN_TOP_MID, 0, 20); // Centrar el reloj en la pantalla

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

void LoadScreen::loadRandomBackground() {
    // Vector con las imágenes de fondo disponibles
    std::vector<const lv_img_dsc_t*> backgrounds = {
        &loading1, &loading2, &loading3, &loading4, &loading5,
        &loading6
    };

    // Generar un número aleatorio usando esp_random()
    uint32_t randomNumber = esp_random();

    // Seleccionar una imagen de fondo aleatoria
    int randomIndex = randomNumber % backgrounds.size();
    const lv_img_dsc_t* selectedBackground = backgrounds[randomIndex];

    // Crear la imagen de fondo
    backgroundImage = lv_img_create(lv_scr_act());
    lv_img_set_src(backgroundImage, selectedBackground);
    lv_obj_set_size(backgroundImage, 128, 160); // Tamaño de la imagen
    lv_obj_align(backgroundImage, LV_ALIGN_CENTER, 0, 0); // Centrar la imagen en la pantalla
    lv_obj_move_background(backgroundImage); // Mover la imagen al fondo
}