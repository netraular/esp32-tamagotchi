// src/screens/LoadScreen/LoadScreen.cpp
#include "LoadScreen.h"
#include "../ScreenManager.h"
#include "../../ClockManager/ClockManager.h"
#include "../../PersistentDataManager/PersistentDataManager.h"
#include <lvgl.h>
#include <vector>
#include "esp_system.h" // Para esp_random()
#include "../../assets/sounds/song1.h" // Incluir la canción

// Declarar las variables externas de las imágenes de fondo
extern const lv_img_dsc_t loading1;
extern const lv_img_dsc_t loading6;

// Declarar las variables externas de las imágenes del botón
extern const lv_img_dsc_t GreenButtonUp;
extern const lv_img_dsc_t GreenButtonDown;

extern ScreenManager screenManager;

// Definir el constructor de LoadScreen
LoadScreen::LoadScreen() 
    : buzzerPlayer(BUZZER_PIN) { // Inicializar el BuzzerPlayer con el pin del buzzer
}

void LoadScreen::load() {
    // Limpiar la pantalla si ya había contenido
    lv_obj_clean(lv_scr_act());

    // Cargar una imagen de fondo aleatoria
    loadRandomBackground();

    // Crear una etiqueta para mostrar la hora
    clockLabel = lv_label_create(lv_scr_act());
    lv_label_set_text(clockLabel, "00:00:00");
    lv_obj_set_style_text_font(clockLabel, &lv_font_montserrat_24, 0);
    lv_obj_align(clockLabel, LV_ALIGN_TOP_MID, 0, 40); // Centrar el reloj en la pantalla

    // Crear la imagen del botón
    lv_obj_t* buttonImage = lv_img_create(lv_scr_act());
    lv_img_set_src(buttonImage, &GreenButtonUp); // Imagen inicial del botón
    lv_obj_align(buttonImage, LV_ALIGN_CENTER, -30, 0); // Posición del botón

    // Crear una etiqueta para el texto "Start"
    lv_obj_t* startLabel = lv_label_create(lv_scr_act());
    lv_label_set_text(startLabel, "Start");
    lv_obj_set_style_text_font(startLabel, &lv_font_montserrat_16, 0);
    lv_obj_align_to(startLabel, buttonImage, LV_ALIGN_CENTER, 30, 0); // Posición del texto "Start"

    // Crear la primera animación (GreenButtonUp -> GreenButtonDown)
    static lv_anim_t animDown;
    lv_anim_init(&animDown);
    lv_anim_set_var(&animDown, buttonImage);
    lv_anim_set_time(&animDown, 0); // Duración de la transición a GreenButtonDown (instantánea)
    lv_anim_set_delay(&animDown, 200); // Esperar 700 ms antes de cambiar a GreenButtonDown
    lv_anim_set_exec_cb(&animDown, (lv_anim_exec_xcb_t)[](void* img, int32_t v) {
        lv_obj_t* button = (lv_obj_t*)img;
        lv_img_set_src(button, &GreenButtonDown); // Cambiar a la imagen del botón pulsado
    });
    lv_anim_set_ready_cb(&animDown, [](lv_anim_t* anim) {
        // Cuando la animación de "abajo" termina, iniciar la animación de "arriba"
        lv_anim_t* animUp = (lv_anim_t*)anim->user_data;
        lv_anim_start(animUp);
    });

    // Crear la segunda animación (GreenButtonDown -> GreenButtonUp)
    static lv_anim_t animUp;
    lv_anim_init(&animUp);
    lv_anim_set_var(&animUp, buttonImage);
    lv_anim_set_time(&animUp, 0); // Duración de la transición a GreenButtonUp (instantánea)
    lv_anim_set_delay(&animUp, 1000); // Esperar 300 ms antes de cambiar a GreenButtonUp
    lv_anim_set_exec_cb(&animUp, (lv_anim_exec_xcb_t)[](void* img, int32_t v) {
        lv_obj_t* button = (lv_obj_t*)img;
        lv_img_set_src(button, &GreenButtonUp); // Cambiar a la imagen del botón sin pulsar
    });
    lv_anim_set_ready_cb(&animUp, [](lv_anim_t* anim) {
        // Cuando la animación de "arriba" termina, iniciar la animación de "abajo"
        lv_anim_t* animDown = (lv_anim_t*)anim->user_data;
        lv_anim_start(animDown);
    });

    // Vincular las animaciones entre sí
    animDown.user_data = &animUp;
    animUp.user_data = &animDown;

    // Iniciar la primera animación
    lv_anim_start(&animDown);

    // Inicializar el reloj solo si no se ha inicializado antes
    ClockManager::getInstance().begin();

    // Cargar y reproducir la canción
    buzzerPlayer.loadSong(melody, noteDurations, sizeof(melody) / sizeof(melody[0]));
    buzzerPlayer.play();

    Serial.println("LoadScreen cargada.");
}

void LoadScreen::update() {
    // Actualizar el reloj
    ClockManager::getInstance().update();

    // Obtener la hora actual
    const char* timeString = ClockManager::getInstance().getTimeString();

    // Actualizar la etiqueta del reloj
    lv_label_set_text(clockLabel, timeString);

    // Actualizar la reproducción de la canción
    buzzerPlayer.update();
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
        &loading1, &loading6
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