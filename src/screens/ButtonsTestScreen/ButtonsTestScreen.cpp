#include <Arduino.h>
#include "config.h"
#include "ButtonsTestScreen.h"
#include "../ScreenManager.h"

extern ScreenManager screenManager;

// Variables para almacenar los objetos de los círculos
lv_obj_t* circle1;
lv_obj_t* circle2;
lv_obj_t* circle3;

// Variables para rastrear el momento en que se presionan los botones
unsigned long button1PressTime = 0;
unsigned long button2PressTime = 0;
unsigned long button3PressTime = 0;

// Margen de tiempo para considerar que los botones se presionaron "a la vez" (en milisegundos)
const unsigned long PRESS_TIME_MARGIN = 50;

// Tiempo que deben estar presionados los tres botones para regresar al menú (3 segundos)
const unsigned long COMBO_HOLD_TIME = 3000;

// Estados de los botones (definidos en ButtonsTestScreen.h)
ButtonStateEnum button1State = STATE_NONE;
ButtonStateEnum button2State = STATE_NONE;
ButtonStateEnum button3State = STATE_NONE;

// Variables para la animación de colores
uint8_t animationStep = 0; // Paso de la animación
const uint8_t ANIMATION_STEPS = 100; // Número de pasos para la animación

// Objeto para el texto "Press all to go to menu"
lv_obj_t* hintLabel;

// Objeto para el contador
lv_obj_t* countdownLabel;
bool isCountingDown = false; // Indica si el contador está activo
unsigned long countdownStartTime = 0; // Tiempo en que comenzó la cuenta regresiva

void ButtonsTestScreen::load() {
    // Crear los círculos
    circle1 = lv_obj_create(lv_scr_act());
    lv_obj_set_size(circle1, 30, 30); // Tamaño del círculo reducido
    lv_obj_set_style_radius(circle1, LV_RADIUS_CIRCLE, 0); // Hacerlo circular
    lv_obj_set_style_bg_color(circle1, lv_color_hex(0x808080), 0); // Color gris
    lv_obj_align(circle1, LV_ALIGN_CENTER, -40, 0); // Posicionar el primer círculo

    circle2 = lv_obj_create(lv_scr_act());
    lv_obj_set_size(circle2, 30, 30); // Tamaño del círculo reducido
    lv_obj_set_style_radius(circle2, LV_RADIUS_CIRCLE, 0); // Hacerlo circular
    lv_obj_set_style_bg_color(circle2, lv_color_hex(0x808080), 0); // Color gris
    lv_obj_align(circle2, LV_ALIGN_CENTER, 0, 0); // Posicionar el segundo círculo

    circle3 = lv_obj_create(lv_scr_act());
    lv_obj_set_size(circle3, 30, 30); // Tamaño del círculo reducido
    lv_obj_set_style_radius(circle3, LV_RADIUS_CIRCLE, 0); // Hacerlo circular
    lv_obj_set_style_bg_color(circle3, lv_color_hex(0x808080), 0); // Color gris
    lv_obj_align(circle3, LV_ALIGN_CENTER, 40, 0); // Posicionar el tercer círculo

    // Crear el texto "Press all to go to menu" en dos líneas
    hintLabel = lv_label_create(lv_scr_act());
    lv_label_set_text(hintLabel, "Press all buttons\nfor 3s to go to menu"); // Texto en dos líneas
    lv_obj_set_style_text_align(hintLabel, LV_TEXT_ALIGN_CENTER, 0); // Centrar el texto
    lv_obj_set_style_text_font(hintLabel, &lv_font_montserrat_12, 0); // Usar una fuente más pequeña
    lv_obj_align(hintLabel, LV_ALIGN_TOP_MID, 0, 10); // Posicionar en la parte superior

    // Crear el contador (inicialmente oculto)
    countdownLabel = lv_label_create(lv_scr_act());
    lv_label_set_text(countdownLabel, "3"); // Iniciar en 3
    lv_obj_set_style_text_align(countdownLabel, LV_TEXT_ALIGN_CENTER, 0); // Centrar el texto
    lv_obj_set_style_text_font(countdownLabel, &lv_font_montserrat_24, 0); // Fuente grande para el contador
    lv_obj_align(countdownLabel, LV_ALIGN_CENTER, 0, 0); // Centrar en la pantalla
    lv_obj_add_flag(countdownLabel, LV_OBJ_FLAG_HIDDEN); // Ocultar inicialmente

    Serial.println("ButtonsTestScreen cargada.");
}

void ButtonsTestScreen::update() {
    // Incrementar el paso de la animación
    animationStep = (animationStep + 1) % ANIMATION_STEPS;

    // Calcular el valor de la animación (0 a 255)
    uint8_t animationValue = (uint8_t)(255 * (sin(2 * PI * animationStep / ANIMATION_STEPS) + 1) / 2);

    // Actualizar el color de los círculos según su estado y la animación
    updateCircleColor(circle1, button1State, animationValue);
    updateCircleColor(circle2, button2State, animationValue);
    updateCircleColor(circle3, button3State, animationValue);

    // Actualizar el contador si está activo
    if (isCountingDown) {
        unsigned long currentTime = millis();
        unsigned long elapsedTime = currentTime - countdownStartTime;

        if (elapsedTime >= COMBO_HOLD_TIME) {
            // Si han pasado 3 segundos, regresar a PetScreen
            screenManager.setScreen("PetScreen");
            isCountingDown = false; // Detener el contador
            lv_obj_add_flag(countdownLabel, LV_OBJ_FLAG_HIDDEN); // Ocultar el contador
        } else {
            // Actualizar el contador cada segundo
            int remainingTime = 3 - (elapsedTime / 1000);
            lv_label_set_text_fmt(countdownLabel, "%d", remainingTime);
        }
    }
}

void ButtonsTestScreen::updateCircleColor(lv_obj_t* circle, ButtonStateEnum state, uint8_t animationValue) {
    lv_color_t baseColor, targetColor;

    switch (state) {
        case STATE_NONE:
            baseColor = lv_color_hex(0x808080); // Gris
            targetColor = lv_color_hex(0x000000); // Negro
            break;
        case STATE_PRESSED:
            baseColor = lv_color_hex(0x00FF00); // Verde
            targetColor = lv_color_hex(0x0000FF); // Azul
            break;
        case STATE_COMBO:
            baseColor = lv_color_hex(0xFF0000); // Rojo
            targetColor = lv_color_hex(0xFFFF00); // Amarillo
            break;
    }

    // Interpolar entre el color base y el color objetivo según el valor de la animación
    lv_color_t interpolatedColor = lv_color_mix(targetColor, baseColor, animationValue);
    lv_obj_set_style_bg_color(circle, interpolatedColor, 0);
}

void ButtonsTestScreen::handleButtonEvent(const ButtonState& state, const ButtonChange& change) {
    // Obtener el tiempo actual
    unsigned long currentTime = millis();

    // Actualizar el tiempo de presión de los botones si se presionan
    if (change.button1Changed && state.button1Pressed) {
        button1PressTime = currentTime;
    }
    if (change.button2Changed && state.button2Pressed) {
        button2PressTime = currentTime;
    }
    if (change.button3Changed && state.button3Pressed) {
        button3PressTime = currentTime;
    }

    // Verificar si los botones se presionaron "a la vez"
    bool buttonsPressedTogether = false;
    if (state.button1Pressed && state.button2Pressed && state.button3Pressed) {
        if (abs((long)(button1PressTime - button2PressTime)) <= PRESS_TIME_MARGIN &&
            abs((long)(button1PressTime - button3PressTime)) <= PRESS_TIME_MARGIN) {
            buttonsPressedTogether = true;
        }
    }

    // Actualizar el estado de los botones
    if (buttonsPressedTogether) {
        // Si los botones se presionaron a la vez, poner los botones presionados en estado COMBO
        if (state.button1Pressed) button1State = STATE_COMBO;
        if (state.button2Pressed) button2State = STATE_COMBO;
        if (state.button3Pressed) button3State = STATE_COMBO;

        // Iniciar el contador si no está activo
        if (!isCountingDown) {
            isCountingDown = true;
            countdownStartTime = currentTime;
            lv_label_set_text(countdownLabel, "3"); // Reiniciar el contador
            lv_obj_clear_flag(countdownLabel, LV_OBJ_FLAG_HIDDEN); // Mostrar el contador
        }
    } else {
        // Si no se presionaron a la vez, detener el contador
        if (isCountingDown) {
            isCountingDown = false;
            lv_obj_add_flag(countdownLabel, LV_OBJ_FLAG_HIDDEN); // Ocultar el contador
        }

        // Actualizar el estado de cada botón individualmente
        if (change.button1Changed) {
            button1State = state.button1Pressed ? STATE_PRESSED : STATE_NONE;
        }
        if (change.button2Changed) {
            button2State = state.button2Pressed ? STATE_PRESSED : STATE_NONE;
        }
        if (change.button3Changed) {
            button3State = state.button3Pressed ? STATE_PRESSED : STATE_NONE;
        }
    }

    // Si un botón no está pulsado, su estado debe ser STATE_NONE, independientemente de su estado anterior
    if (!state.button1Pressed) {
        button1State = STATE_NONE;
    }
    if (!state.button2Pressed) {
        button2State = STATE_NONE;
    }
    if (!state.button3Pressed) {
        button3State = STATE_NONE;
    }
}