#ifndef LOADSCREEN_H
#define LOADSCREEN_H

#include "../Screen.h"
#include <time.h> // Para manejar la hora

class LoadScreen : public Screen {
public:
    void load() override;
    void update() override;
    void handleButtonEvent(const ButtonState& state, const ButtonChange& change) override;

private:
    lv_obj_t* label;       // Etiqueta para mostrar el mensaje de carga
    lv_obj_t* clockLabel;  // Etiqueta para mostrar la hora
    void updateClock();    // Función para actualizar la hora
    void setupTime();      // Función para configurar la hora
    void saveCurrentTime(); // Función para guardar la hora actual
    void loadDefaultTime(); // Función para cargar la hora por defecto desde settings.json
    void attemptNtpSync();  // Función para intentar obtener la hora por NTP en segundo plano
};

#endif