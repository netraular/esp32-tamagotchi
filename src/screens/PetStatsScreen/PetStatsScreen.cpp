#include "PetStatsScreen.h"
#include "../ScreenManager.h"
#include "../../PersistentDataManager/PersistentDataManager.h"
#include <lvgl.h>

extern ScreenManager screenManager;
extern PersistentDataManager persistentDataManager;

void PetStatsScreen::load() {
    // Limpiar la pantalla
    lv_obj_clean(lv_scr_act());

    // Cargar datos desde pet_stats.json
    JsonDocument petStats = persistentDataManager.loadData("/data/pet_stats.json");
    if (petStats.isNull()) {
        Serial.println("Error al cargar pet_stats.json");
        return;
    }

    // Obtener las estadísticas de la mascota
    const char* name = petStats["name"];
    int health = petStats["health"];
    int hunger = petStats["hunger"];
    int happiness = petStats["happiness"];
    int coins = petStats["coins"];
    const char* birthdate = petStats["birthdate"];
    int evolution = petStats["evolution"];
    bool alive = petStats["alive"];

    // Crear una etiqueta para mostrar las estadísticas
    statsLabel = lv_label_create(lv_scr_act());
    lv_label_set_text_fmt(
        statsLabel,
        "Name: %s\n"
        "Health: %d\n"
        "Hunger: %d\n"
        "Happiness: %d\n"
        "Coins: %d\n"
        "Birthdate: %s\n"
        "Evolution: %d\n"
        "Status: %s",
        name,
        health,
        hunger,
        happiness,
        coins,
        birthdate,
        evolution,
        alive ? "Alive" : "Not Alive"
    );
    lv_obj_set_style_text_font(statsLabel, &lv_font_montserrat_12, 0);
    lv_obj_align(statsLabel, LV_ALIGN_CENTER, 0, 0); // Centrar el texto en la pantalla

    Serial.println("PetStatsScreen cargada.");
}

void PetStatsScreen::update() {
    // No es necesario actualizar nada en esta pantalla
}

void PetStatsScreen::handleButtonEvent(const ButtonState& state, const ButtonChange& change) {
    // Volver al menú principal con el botón 3
    if (change.button3Changed && state.button3Pressed) {
        screenManager.setScreen("MainMenu");
    }
}