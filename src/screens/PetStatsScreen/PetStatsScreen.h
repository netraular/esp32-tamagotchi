#ifndef PETSTATSSCREEN_H
#define PETSTATSSCREEN_H

#include "../Screen.h"

class PetStatsScreen : public Screen {
public:
    void load() override;
    void update() override;
    void handleButtonEvent(const ButtonState& state, const ButtonChange& change) override;

private:
    lv_obj_t* statsLabel; // Etiqueta para mostrar las estadísticas
};

#endif