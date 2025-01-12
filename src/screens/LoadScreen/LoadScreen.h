// src/screens/LoadScreen/LoadScreen.h
#ifndef LOADSCREEN_H
#define LOADSCREEN_H

#include "../Screen.h"
#include <lvgl.h>
#include "../../components/BuzzerPlayer/BuzzerPlayer.h"

class LoadScreen : public Screen {
public:
    LoadScreen(); // Declarar el constructor
    void load() override;
    void update() override;
    void handleButtonEvent(const ButtonState& state, const ButtonChange& change) override;

private:
    lv_obj_t* clockLabel;  // Etiqueta para mostrar la hora
    lv_obj_t* backgroundImage; // Imagen de fondo
    BuzzerPlayer buzzerPlayer; // Componente para reproducir sonidos
    void updateClock();    // Función para actualizar la hora
    void saveCurrentTime(); // Función para guardar la hora actual
    void loadRandomBackground(); // Función para cargar una imagen de fondo aleatoria
};

#endif