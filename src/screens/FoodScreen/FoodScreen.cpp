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
    selectedFoodIndex = -1; // Reiniciar la selección (ningún elemento seleccionado)

    // Cargar datos desde own_food.json
    JsonDocument ownFoodData = persistentDataManager.loadData("/data/own_food.json");
    if (ownFoodData.isNull()) {
        Serial.println("Error al cargar own_food.json");
        return;
    }

    // Cargar datos desde food.json
    JsonDocument foodData = persistentDataManager.loadData("/data/food.json");
    if (foodData.isNull()) {
        Serial.println("Error al cargar food.json");
        return;
    }

    // Mostrar los datos en la pantalla
    int yOffset = 10; // Posición vertical inicial
    for (JsonObject ownFoodItem : ownFoodData.as<JsonArray>()) {
        int id = ownFoodItem["id"];
        int quantity = ownFoodItem["quantity"];

        // Solo mostrar alimentos con cantidad > 0
        if (quantity > 0) {
            // Buscar los detalles del alimento en food.json
            for (JsonObject foodItem : foodData.as<JsonArray>()) {
                if (foodItem["id"] == id) {
                    const char* name = foodItem["name"];
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
                    break;
                }
            }
        }
    }

    // Si hay alimentos disponibles, seleccionar el primero
    if (foodLabels.size() > 0) {
        selectedFoodIndex = 0; // Seleccionar el primer alimento
        updateSelection(); // Resaltar la selección
    }

    Serial.println("FoodScreen cargada.");
}

void FoodScreen::update() {
    // No es necesario actualizar nada en esta pantalla
}

void FoodScreen::handleButtonEvent(const ButtonState& state, const ButtonChange& change) {
    // Navegar entre las comidas con el botón 1 (solo si hay alimentos disponibles)
    if (change.button1Changed && state.button1Pressed && foodLabels.size() > 0) {
        selectedFoodIndex = (selectedFoodIndex + 1) % foodLabels.size(); // Circular entre las comidas
        updateSelection(); // Actualizar la selección visual
    }

    // Seleccionar una comida con el botón 2 (solo si hay alimentos disponibles)
    if (change.button2Changed && state.button2Pressed && foodLabels.size() > 0) {
        // Cargar los datos actuales de own_food.json
        JsonDocument ownFoodData = persistentDataManager.loadData("/data/own_food.json");
        if (ownFoodData.isNull()) {
            Serial.println("Error al cargar own_food.json");
            return;
        }

        // Reducir la cantidad de la comida seleccionada en 1
        JsonArray ownFoodArray = ownFoodData.as<JsonArray>();
        if (selectedFoodIndex < ownFoodArray.size()) {
            JsonObject selectedFood = ownFoodArray[selectedFoodIndex];
            int quantity = selectedFood["quantity"];
            if (quantity > 0) {
                selectedFood["quantity"] = quantity - 1; // Reducir la cantidad
            }

            // Si la cantidad llega a 0, eliminar el alimento del JSON
            if (selectedFood["quantity"] == 0) {
                ownFoodArray.remove(selectedFoodIndex);
            }
        }

        // Guardar los cambios en own_food.json
        if (!persistentDataManager.saveData("/data/own_food.json", ownFoodData)) {
            Serial.println("Error al guardar own_food.json");
            return;
        }

        // Volver a PetScreen
        screenManager.setScreen("PetScreen");
    }

    // Volver al PetScreen con el botón 3
    if (change.button3Changed && state.button3Pressed) {
        screenManager.setScreen("PetScreen");
    }
}

void FoodScreen::updateSelection() {
    // Resaltar la comida seleccionada
    for (size_t i = 0; i < foodLabels.size(); i++) {
        if (i == selectedFoodIndex) {
            lv_obj_set_style_text_color(foodLabels[i], lv_color_hex(0xFF0000), 0); // Resaltar en rojo
        } else {
            lv_obj_set_style_text_color(foodLabels[i], lv_color_hex(0x000000), 0); // Color normal
        }
    }
}