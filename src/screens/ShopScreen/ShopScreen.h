#ifndef SHOPSCREEN_H
#define SHOPSCREEN_H

#include "../Screen.h"
#include <vector>

class ShopScreen : public Screen {
public:
    void load() override;
    void update() override;
    void handleButtonEvent(const ButtonState& state, const ButtonChange& change) override;

private:
    std::vector<lv_obj_t*> foodImages; // Vector para almacenar las imágenes de los alimentos
    std::vector<lv_obj_t*> foodLabels; // Vector para almacenar las etiquetas de los alimentos
    int selectedOption = 0;            // Índice de la opción seleccionada (Food, Items, Clothes)
    int selectedFoodIndex = -1;        // Índice del alimento seleccionado
    bool inFoodMenu = false;           // Indica si estamos en el menú de alimentos
    lv_obj_t* coinsLabel = nullptr;    // Etiqueta para mostrar las monedas

    void showMainMenu();               // Mostrar el menú principal (Food, Items, Clothes)
    void showFoodMenu();               // Mostrar la lista de alimentos
    void updateSelection();            // Actualizar la selección visual
};

#endif