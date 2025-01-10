#ifndef INPUTTEXT_H
#define INPUTTEXT_H

#include <lvgl.h>
#include <vector>

class InputText {
public:
    InputText(lv_obj_t* parent, int maxLength);
    void show();
    void hide();
    bool isVisible() const;
    void setText(const char* text);
    const char* getText() const;
    void setMaxLength(int maxLength);
    int getMaxLength() const;
    void setPlaceholder(const char* placeholder);
    lv_obj_t* getContainer() const; // Método para obtener el contenedor

private:
    lv_obj_t* container; // Contenedor de las casillas de texto
    std::vector<lv_obj_t*> boxes; // Vector de casillas de texto
    int maxLength; // Número máximo de caracteres
    char* textBuffer; // Buffer para almacenar el texto

    void createBoxes(lv_obj_t* parent);
    void updateBoxes();
};

#endif