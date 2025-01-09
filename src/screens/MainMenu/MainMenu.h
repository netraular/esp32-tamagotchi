#ifndef MAINMENU_H
#define MAINMENU_H

#include "../Screen.h"

class MainMenu : public Screen {
public:
    void load() override;
    void update() override;
    void handleButtonEvent(const ButtonState& state, const ButtonChange& change) override;

private:
    lv_obj_t* menuLabel;
    int selectedOption; // Índice de la opción seleccionada
    void updateMenu();
};

#endif