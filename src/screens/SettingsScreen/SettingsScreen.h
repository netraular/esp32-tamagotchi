#ifndef SETTINGSSCREEN_H
#define SETTINGSSCREEN_H

#include "../../Screen.h"
#include <vector>

class SettingsScreen : public Screen {
public:
    void load() override;
    void update() override;
    void handleButtonEvent(const ButtonState& state, const ButtonChange& change) override;

private:
    std::vector<const char*> options = {
        "Apodo",
        "Fecha y hora",
        "Hora de dormir",
        "Logro favorito",
        "Objeto favorito",
        "Comida favorita",
        "Wifi",
        "Idioma" // Nueva opción para seleccionar el idioma
    };
    int selectedOption = 0; // Índice de la opción seleccionada
    lv_obj_t* menuLabel;    // Etiqueta para mostrar el menú

    void updateMenu(); // Actualizar el menú visualmente
};

#endif