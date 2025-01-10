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
    void setPlaceholder(const char* placeholder);
    lv_obj_t* getContainer() const;
    void setSelectedBox(int index); // Seleccionar una casilla
    void insertChar(char c);       // Insertar un carácter en la casilla seleccionada
    int getSelectedBoxIndex() const; // Obtener el índice de la casilla seleccionada
    std::string moveNext();        // Avanzar al siguiente índice
    std::string movePrevious();    // Retroceder al índice anterior
    bool isAnyBoxSelected() const; // Verificar si alguna casilla está seleccionada

private:
    lv_obj_t* container; // Contenedor de las casillas de texto
    std::vector<lv_obj_t*> boxes; // Vector de casillas de texto
    int maxLength; // Número máximo de caracteres
    char* textBuffer; // Buffer para almacenar el texto
    int selectedBoxIndex; // Índice de la casilla seleccionada
    bool isAnyBoxSelectedFlag; // Indica si alguna casilla está seleccionada

    void createBoxes(lv_obj_t* parent);
    void updateBoxes();
    void updateSelection(); // Actualizar la selección visual
    void clearSelection();  // Deseleccionar todas las casillas
};

#endif