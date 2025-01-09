#ifndef LANGUAGESCREEN_H
#define LANGUAGESCREEN_H

#include "../../Screen.h"

class LanguageScreen : public Screen {
public:
    void load() override;
    void update() override;
    void handleButtonEvent(const ButtonState& state, const ButtonChange& change) override;

private:
    std::vector<const char*> languages = {
        "Español",
        "Inglés",
        "Francés",
        "Alemán"
    };
    int selectedLanguage = 0; // Índice del idioma seleccionado
    lv_obj_t* menuLabel;      // Etiqueta para mostrar el menú de idiomas

    void updateMenu(); // Actualizar el menú visualmente
};

#endif