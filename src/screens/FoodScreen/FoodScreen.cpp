#include "FoodScreen.h"
#include "../ScreenManager.h"
#include "../../PersistentDataManager/PersistentDataManager.h"

extern ScreenManager screenManager;
extern PersistentDataManager persistentDataManager; // Declarar como extern

void FoodScreen::load() {
    // Cargar datos desde food.json
    JsonDocument foodData = persistentDataManager.loadData("/data/food.json");
    if (foodData.isNull()) {
        Serial.println("Error al cargar food.json");
        return;
    }

    // Mostrar los datos en la pantalla
    for (JsonObject foodItem : foodData.as<JsonArray>()) {
        const char* name = foodItem["name"];
        int quantity = foodItem["quantity"];
        Serial.printf("Comida: %s, Cantidad: %d\n", name, quantity);
    }

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