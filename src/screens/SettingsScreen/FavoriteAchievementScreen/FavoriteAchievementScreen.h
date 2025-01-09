#ifndef FAVORITEACHIEVEMENTSCREEN_H
#define FAVORITEACHIEVEMENTSCREEN_H

#include "../../Screen.h"

class FavoriteAchievementScreen : public Screen {
public:
    void load() override;
    void update() override;
    void handleButtonEvent(const ButtonState& state, const ButtonChange& change) override;

private:
    lv_obj_t* label; // Etiqueta para mostrar el nombre de la ventana
};

#endif