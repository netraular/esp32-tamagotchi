#ifndef INPUTTEXT_H
#define INPUTTEXT_H

#include <lvgl.h>
#include <vector>
#include <string>

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
    lv_obj_t* getContainer() const;
    void setSelectedBox(int index); // Seleccionar una casilla
    int insertChar(char c);       // Insertar un carácter en la casilla seleccionada
    int getSelectedBoxIndex() const; // Obtener el índice de la casilla seleccionada
    int moveNext();        // Avanzar al siguiente índice
    int movePrevious();    // Retroceder al índice anterior

private:
    lv_obj_t* container; // Contenedor de las casillas de texto
    std::vector<lv_obj_t*> boxes; // Vector de casillas de texto
    int maxLength; // Número máximo de caracteres
    char* textBuffer; // Buffer para almacenar el texto
    int selectedBoxIndex; // Índice de la casilla seleccionada

    void createBoxes(lv_obj_t* parent);
    void updateBoxes();
    void updateSelection(); // Actualizar la selección visual
    void clearSelection();  // Deseleccionar todas las casillas
};

#endif