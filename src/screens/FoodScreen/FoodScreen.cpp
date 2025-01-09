#include "FoodScreen.h"
#include "../ScreenManager.h"
#include "../../PersistentDataManager/PersistentDataManager.h"
#include <lvgl.h>

// Declarar las variables externas de las imágenes
extern const lv_img_dsc_t n05ApplePie;
extern const lv_img_dsc_t n07Bread;

extern ScreenManager screenManager;
extern PersistentDataManager persistentDataManager;

void FoodScreen::load() {
    // Limpiar los vectores si ya contenían datos
    foodImages.clear();
    foodLabels.clear();

    // Cargar datos desde food.json
    JsonDocument foodData = persistentDataManager.loadData("/data/food.json");
    if (foodData.isNull()) {
        Serial.println("Error al cargar food.json");
        return;
    }

    // Mostrar los datos en la pantalla
    int yOffset = 10; // Posición vertical inicial
    for (JsonObject foodItem : foodData.as<JsonArray>()) {
        const char* name = foodItem["name"];
        int quantity = foodItem["quantity"];
        const char* imagePath = foodItem["image"];

        // Crear una etiqueta para el nombre y la cantidad
        lv_obj_t* label = lv_label_create(lv_scr_act());
        lv_label_set_text_fmt(label, "%s x%d", name, quantity);
        lv_obj_set_style_text_font(label, &lv_font_montserrat_12, 0);
        lv_obj_align(label, LV_ALIGN_TOP_LEFT, 50, yOffset); // Alinear a la izquierda
        foodLabels.push_back(label);

        // Crear una imagen para la comida
        lv_obj_t* img = lv_img_create(lv_scr_act());
        if (strcmp(imagePath, "/assets/food/n05ApplePie.c") == 0) {
            lv_img_set_src(img, &n05ApplePie);
        } else if (strcmp(imagePath, "/assets/food/n07Bread.c") == 0) {
            lv_img_set_src(img, &n07Bread);
        }
        lv_obj_set_size(img, 32, 32); // Tamaño de la imagen
        lv_obj_align(img, LV_ALIGN_TOP_LEFT, 10, yOffset); // Alinear a la izquierda
        foodImages.push_back(img);

        // Incrementar el offset para la siguiente comida
        yOffset += 40; // Espacio entre comidas
    }

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