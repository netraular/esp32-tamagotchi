#include "MainMenu.h"
#include "../ScreenManager.h"

extern ScreenManager screenManager;

void MainMenu::load() {
    // Crear una lista de LVGL
    list = lv_list_create(lv_scr_act());
    lv_obj_set_size(list, 280, 240); // Ajustar al tamaño de la pantalla
    lv_obj_align(list, LV_ALIGN_CENTER, 0, 0);

    // Añadir opciones al menú
    const char* menuOptions[] = {
        "FoodScreen",
        "ButtonsTestScreen",
        "SetupScreen",
        "RestartScreen",
        "ShopScreen",
        "PetStatsScreen",
        "SettingsScreen",
        "TestScreen",
        "TestScreen2",
        "RoomSelectionScreen",
        "TileMapScreen"
    };

    for (const char* option : menuOptions) {
        lv_obj_t* item = lv_list_add_btn(list, NULL, option); // Añadir botón a la lista
        menuItems.push_back(item); // Guardar el elemento en el vector
    }

    // Seleccionar la primera opción por defecto
    selectedItem = menuItems[0];
    updateSelection();

    Serial.println("MainMenu cargada.");
}

void MainMenu::update() {
    // No es necesario actualizar nada en esta pantalla
}

void MainMenu::handleButtonEvent(const ButtonState& state, const ButtonChange& change) {
    // Navegar entre las opciones con el botón 1 (abajo) y el botón 3 (arriba)
    if (change.button1Changed && state.button1Pressed) {
        // Mover la selección hacia abajo
        int currentIndex = std::distance(menuItems.begin(), std::find(menuItems.begin(), menuItems.end(), selectedItem));
        int nextIndex = (currentIndex + 1) % menuItems.size();
        selectedItem = menuItems[nextIndex];
        updateSelection();
    }
    if (change.button3Changed && state.button3Pressed) {
        // Mover la selección hacia arriba
        int currentIndex = std::distance(menuItems.begin(), std::find(menuItems.begin(), menuItems.end(), selectedItem));
        int prevIndex = (currentIndex - 1 + menuItems.size()) % menuItems.size();
        selectedItem = menuItems[prevIndex];
        updateSelection();
    }

    // Seleccionar una opción con el botón 2
    if (change.button2Changed && state.button2Pressed) {
        // Obtener el texto del elemento seleccionado
        const char* selectedOption = lv_list_get_btn_text(list, selectedItem); // Corregido: pasar 'list' y 'selectedItem'

        // Navegar a la pantalla correspondiente
        screenManager.setScreen(selectedOption);
    }
}

void MainMenu::updateSelection() {
    // Resaltar el elemento seleccionado
    for (lv_obj_t* item : menuItems) {
        if (item == selectedItem) {
            lv_obj_set_style_bg_color(item, lv_color_hex(0x007FFF), 0); // Resaltar en azul
            lv_obj_set_style_text_color(item, lv_color_hex(0xFFFFFF), 0); // Texto en blanco
            lv_obj_scroll_to_view(item, LV_ANIM_ON); // Desplazar para que el elemento sea visible
        } else {
            lv_obj_set_style_bg_color(item, lv_color_hex(0xFFFFFF), 0); // Fondo blanco
            lv_obj_set_style_text_color(item, lv_color_hex(0x000000), 0); // Texto en negro
        }
    }
}