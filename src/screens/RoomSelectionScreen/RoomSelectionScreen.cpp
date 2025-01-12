#include "RoomSelectionScreen.h"
#include "../ScreenManager.h"

extern const lv_img_dsc_t loading1;

extern ScreenManager screenManager;


void RoomSelectionScreen::load() {
    // Inicializar las habitaciones disponibles
    roomNames = {"Comedor", "Cocina", "Baño", "Dormitorio", "Jardín", "Calle"};
    roomImages = {&loading1, &loading1, &loading1, &loading1, &loading1, &loading1};

    // Crear la imagen de fondo
    backgroundImage = lv_img_create(lv_scr_act());
    lv_img_set_src(backgroundImage, roomImages[selectedRoomIndex]);
    lv_obj_set_size(backgroundImage, 128, 160); // Ajustar al tamaño de la pantalla
    lv_obj_align(backgroundImage, LV_ALIGN_CENTER, 0, 0);

    // Crear una etiqueta para mostrar el nombre de la habitación
    roomLabel = lv_label_create(lv_scr_act());
    lv_label_set_text(roomLabel, roomNames[selectedRoomIndex]);
    lv_obj_set_style_text_font(roomLabel, &lv_font_montserrat_16, 0);
    lv_obj_align(roomLabel, LV_ALIGN_BOTTOM_MID, 0, -10); // Alinear en la parte inferior

    Serial.println("RoomSelectionScreen cargada.");
}

void RoomSelectionScreen::update() {
    // No es necesario actualizar nada en esta pantalla
}

void RoomSelectionScreen::handleButtonEvent(const ButtonState& state, const ButtonChange& change) {
    // Navegar entre las habitaciones con el botón 1 (siguiente) y el botón 3 (anterior)
    if (change.button1Changed && state.button1Pressed) {
        selectedRoomIndex = (selectedRoomIndex + 1) % roomNames.size(); // Circular hacia adelante
        updateRoomDisplay();
    }
    if (change.button3Changed && state.button3Pressed) {
        selectedRoomIndex = (selectedRoomIndex - 1 + roomNames.size()) % roomNames.size(); // Circular hacia atrás
        updateRoomDisplay();
    }

    // Seleccionar la habitación con el botón 2
    if (change.button2Changed && state.button2Pressed) {
        const char* selectedRoom = roomNames[selectedRoomIndex];
        if (strcmp(selectedRoom, "Calle") == 0) {
            // Si se selecciona "Calle", volver a la pantalla principal
            screenManager.setScreen("MainMenu");
        } else {
            // Cargar la pantalla de la habitación seleccionada
            screenManager.setScreen(selectedRoom);
        }
    }
}

void RoomSelectionScreen::updateRoomDisplay() {
    // Actualizar la imagen de fondo
    lv_img_set_src(backgroundImage, roomImages[selectedRoomIndex]);

    // Actualizar el nombre de la habitación
    lv_label_set_text(roomLabel, roomNames[selectedRoomIndex]);
}