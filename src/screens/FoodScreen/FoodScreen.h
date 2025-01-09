#ifndef FOODSCREEN_H
#define FOODSCREEN_H

#include "../Screen.h"
#include <vector>

class FoodScreen : public Screen {
public:
    void load() override;
    void update() override;
    void handleButtonEvent(const ButtonState& state, const ButtonChange& change) override;

private:
    std::vector<lv_obj_t*> foodImages; // Vector para almacenar las imágenes de las comidas
    std::vector<lv_obj_t*> foodLabels; // Vector para almacenar las etiquetas de las comidas
    int selectedFoodIndex = 0;         // Índice de la comida seleccionada
    void updateSelection();            // Función para actualizar la selección visual
};

#endif