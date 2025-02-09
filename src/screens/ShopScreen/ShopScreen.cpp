#include "ShopScreen.h"
#include "../ScreenManager.h"
#include "../../PersistentDataManager/PersistentDataManager.h"
#include <lvgl.h>

// Declarar las variables externas de las imágenes
extern const lv_img_dsc_t n05ApplePie;
extern const lv_img_dsc_t n07Bread;

extern ScreenManager screenManager;
extern PersistentDataManager persistentDataManager;

// Variables para controlar el parpadeo
bool isCoinsBlinking = false; // Indica si el contador de monedas está parpadeando
unsigned long blinkStartTime = 0; // Tiempo en que comenzó el parpadeo

void ShopScreen::load() {
    // Limpiar los vectores si ya contenían datos
    foodImages.clear();
    foodLabels.clear();
    selectedOption = 0; // Reiniciar la selección
    selectedFoodIndex = -1; // Ningún alimento seleccionado
    inFoodMenu = false; // Empezar en el menú principal

    // Crear un estilo específico para coinsLabel
    static lv_style_t coinsLabelStyle;
    lv_style_init(&coinsLabelStyle);
    lv_style_set_text_color(&coinsLabelStyle, lv_color_hex(0x000000)); // Color negro por defecto

    // Mostrar el menú principal
    showMainMenu();

    Serial.println("ShopScreen cargada.");
}

void ShopScreen::update() {
    // Actualizar el parpadeo del contador de monedas
    if (isCoinsBlinking) {
        unsigned long currentTime = millis();
        if (currentTime - blinkStartTime >= 1000) {
            // Detener el parpadeo después de 1 segundo
            isCoinsBlinking = false;
            lv_obj_set_style_text_color(coinsLabel, lv_color_hex(0x000000), 0); // Restaurar el color a negro
        } else {
            // Alternar el color entre rojo y negro cada 200 ms
            if ((currentTime - blinkStartTime) % 400 < 200) {
                lv_obj_set_style_text_color(coinsLabel, lv_color_hex(0xFF0000), 0); // Rojo
            } else {
                lv_obj_set_style_text_color(coinsLabel, lv_color_hex(0x000000), 0); // Negro
            }
        }
    }
}

void ShopScreen::handleButtonEvent(const ButtonState& state, const ButtonChange& change) {
    // Navegar entre las opciones con el botón 1
    if (change.button1Changed && state.button1Pressed) {
        if (inFoodMenu) {
            // Navegar entre los alimentos
            if (foodLabels.size() > 0) {
                selectedFoodIndex = (selectedFoodIndex + 1) % foodLabels.size();
                Serial.printf("Navegando en el menú Food. Índice seleccionado: %d\n", selectedFoodIndex);
                updateSelection();
            }
        } else {
            // Navegar entre las opciones del menú principal
            selectedOption = (selectedOption + 1) % 3; // Circular entre Food, Items, Clothes
            updateSelection();
        }
    }

    // Seleccionar una opción con el botón 2
    if (change.button2Changed && state.button2Pressed) {
        if (inFoodMenu) {
            // Comprar el alimento seleccionado
            if (selectedFoodIndex >= 0 && selectedFoodIndex < foodLabels.size()) {
                // Cargar los datos de food.json y pet_stats.json
                JsonDocument petStats = persistentDataManager.loadData("/data/pet_stats.json");

                if (petStats.isNull()) {
                    Serial.println("Error al cargar pet_stats.json");
                    return;
                }

                // Obtener el alimento seleccionado
                JsonObject selectedFood = foodData.as<JsonArray>()[selectedFoodIndex];
                int price = selectedFood["price"];
                int coins = petStats["coins"];

                // Verificar si el usuario tiene suficientes monedas
                if (coins >= price) {
                    // Restar el costo de la compra
                    petStats["coins"] = coins - price;

                    // Añadir el alimento a own_food.json
                    JsonDocument ownFoodData = persistentDataManager.loadData("/data/own_food.json");
                    if (ownFoodData.isNull()) {
                        Serial.println("Error al cargar own_food.json");
                        return;
                    }

                    // Buscar si el alimento ya existe en own_food.json
                    bool foodFound = false;
                    for (JsonObject foodItem : ownFoodData.as<JsonArray>()) {
                        if (foodItem["id"] == selectedFood["id"]) {
                            foodItem["quantity"] = foodItem["quantity"].as<int>() + 1;
                            foodFound = true;
                            break;
                        }
                    }

                    // Si el alimento no existe, añadirlo
                    if (!foodFound) {
                        JsonObject newFood = ownFoodData.as<JsonArray>().add<JsonObject>();
                        newFood["id"] = selectedFood["id"];
                        newFood["quantity"] = 1;
                    }

                    // Guardar los cambios en own_food.json y pet_stats.json
                    if (!persistentDataManager.saveData("/data/own_food.json", ownFoodData) ||
                        !persistentDataManager.saveData("/data/pet_stats.json", petStats)) {
                        Serial.println("Error al guardar los archivos JSON");
                        return;
                    }

                    Serial.println("Alimento comprado correctamente");

                    // Actualizar el contador de monedas en la pantalla
                    lv_label_set_text_fmt(coinsLabel, "Coins: %d", petStats["coins"].as<int>());
                } else {
                    Serial.println("No tienes suficientes monedas");

                    // Activar el parpadeo del contador de monedas
                    isCoinsBlinking = true;
                    blinkStartTime = millis();
                }
            }
        } else {
            // Seleccionar una opción del menú principal
            switch (selectedOption) {
                case 0: // Food
                    inFoodMenu = true;
                    showFoodMenu(); // Mostrar el menú de alimentos
                    selectedFoodIndex = 0; // Seleccionar el primer alimento
                    Serial.printf("Entrando al menú Food. Índice seleccionado: %d\n", selectedFoodIndex);
                    updateSelection(); // Resaltar la selección en rojo
                    break;
                case 1: // Items
                    // No hacer nada (aún no implementado)
                    break;
                case 2: // Clothes
                    // No hacer nada (aún no implementado)
                    break;
            }
        }
    }

    // Volver al menú principal o PetScreen con el botón 3
    if (change.button3Changed && state.button3Pressed) {
        if (inFoodMenu) {
            // Volver al menú principal de la tienda
            inFoodMenu = false;
            showMainMenu();

            // Limpiar los vectores de alimentos
            foodLabels.clear();
            foodImages.clear();

            // Restablecer el color del coinsLabel si está parpadeando
            if (isCoinsBlinking) {
                isCoinsBlinking = false;
                lv_obj_set_style_text_color(coinsLabel, lv_color_hex(0x000000), 0); // Restaurar el color a negro
            }
        } else {
            // Volver a PetScreen
            screenManager.setScreen("PetScreen");
        }
    }
}

void ShopScreen::showMainMenu() {
    // Limpiar la pantalla
    lv_obj_clean(lv_scr_act());

    // Crear una etiqueta para el menú principal
    lv_obj_t* menuLabel = lv_label_create(lv_scr_act());
    const char* menuText = "";
    switch (selectedOption) {
        case 0:
            menuText = "> Food\n  Items\n  Clothes";
            break;
        case 1:
            menuText = "  Food\n> Items\n  Clothes";
            break;
        case 2:
            menuText = "  Food\n  Items\n> Clothes";
            break;
    }
    lv_label_set_text(menuLabel, menuText);
    lv_obj_set_style_text_font(menuLabel, &lv_font_montserrat_16, 0);
    lv_obj_align(menuLabel, LV_ALIGN_CENTER, 0, 0);
}

void ShopScreen::showFoodMenu() {
    // Limpiar la pantalla
    lv_obj_clean(lv_scr_act());

    // Cargar datos desde food.json solo si no están cargados
    if (foodData.isNull()) {
        foodData = persistentDataManager.loadData("/data/food.json");
        if (foodData.isNull()) {
            Serial.println("Error al cargar food.json");
            return;
        }
    }

    // Cargar las monedas desde pet_stats.json
    JsonDocument petStats = persistentDataManager.loadData("/data/pet_stats.json");
    if (petStats.isNull()) {
        Serial.println("Error al cargar pet_stats.json");
        return;
    }

    int coins = petStats["coins"]; // Obtener las monedas

    // Mostrar las monedas en la esquina superior derecha
    coinsLabel = lv_label_create(lv_scr_act());
    lv_label_set_text_fmt(coinsLabel, "Coins: %d", coins);
    lv_obj_set_style_text_font(coinsLabel, &lv_font_montserrat_12, 0);
    lv_obj_align(coinsLabel, LV_ALIGN_TOP_RIGHT, -10, 10); // Alinear en la esquina superior derecha

    // Limpiar los vectores antes de añadir nuevos elementos
    foodLabels.clear();
    foodImages.clear();

    // Mostrar los datos de los alimentos en la pantalla
    int yOffset = 40; // Posición vertical inicial (dejamos espacio para las monedas)
    for (JsonObject foodItem : foodData.as<JsonArray>()) {
        const char* name = foodItem["name"];
        int price = foodItem["price"];
        const char* imagePath = foodItem["image"];

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

        // Crear una etiqueta para el nombre y el precio
        lv_obj_t* label = lv_label_create(lv_scr_act());
        lv_label_set_text_fmt(label, "%s - %d coins", name, price);
        lv_obj_set_style_text_font(label, &lv_font_montserrat_12, 0);
        lv_obj_align(label, LV_ALIGN_TOP_LEFT, 50, yOffset); // Alinear a la izquierda
        foodLabels.push_back(label);

        // Incrementar el offset para la siguiente comida
        yOffset += 40; // Espacio entre comidas
    }

    // Seleccionar el primer alimento si hay alimentos disponibles
    if (foodLabels.size() > 0) {
        selectedFoodIndex = 0; // Seleccionar el primer alimento
        Serial.printf("Menú Food cargado. Número de alimentos: %d. Índice seleccionado: %d\n", foodLabels.size(), selectedFoodIndex);
        updateSelection(); // Resaltar la selección en rojo
    } else {
        Serial.println("Menú Food cargado. No hay alimentos disponibles.");
    }
}

void ShopScreen::updateSelection() {
    if (inFoodMenu) {
        // Resaltar el alimento seleccionado
        for (size_t i = 0; i < foodLabels.size(); i++) {
            if (i == selectedFoodIndex) {
                lv_obj_set_style_text_color(foodLabels[i], lv_color_hex(0xFF0000), 0); // Resaltar en rojo
                Serial.printf("Alimento resaltado en rojo: Índice %d\n", i);
            } else {
                lv_obj_set_style_text_color(foodLabels[i], lv_color_hex(0x000000), 0); // Color normal
            }
        }
    } else {
        // Resaltar la opción seleccionada en el menú principal
        lv_obj_t* menuLabel = lv_obj_get_child(lv_scr_act(), 0); // Obtener la etiqueta del menú
        const char* menuText = "";
        switch (selectedOption) {
            case 0:
                menuText = "> Food\n  Items\n  Clothes";
                break;
            case 1:
                menuText = "  Food\n> Items\n  Clothes";
                break;
            case 2:
                menuText = "  Food\n  Items\n> Clothes";
                break;
        }
        lv_label_set_text(menuLabel, menuText);
    }
}