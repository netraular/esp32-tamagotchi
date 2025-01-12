#ifndef MAINMENU_H
#define MAINMENU_H

#include "../Screen.h"
#include <lvgl.h>
#include <vector> // Añadir esta línea

class MainMenu : public Screen {
public:
    void load() override;
    void update() override;
    void handleButtonEvent(const ButtonState& state, const ButtonChange& change) override;

private:
    lv_obj_t* list; // Lista de LVGL para las opciones del menú
    lv_obj_t* selectedItem; // Elemento seleccionado actualmente
    std::vector<lv_obj_t*> menuItems; // Vector para almacenar los elementos del menú

    void updateSelection(); // Actualiza la selección visual
};

#endif