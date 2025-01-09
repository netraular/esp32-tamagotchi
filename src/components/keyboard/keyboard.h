#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <lvgl.h>
#include <vector>
#include <functional> // Para usar std::function

class Keyboard {
public:
    enum class Mode {
        SelectRow, // Modo de selección de filas
        SelectLetter // Modo de selección de letras
    };

    Keyboard(lv_obj_t* parent);
    void show();
    void hide();
    bool isVisible() const;
    void handleButtonEvent(int button);
    char getSelectedLetter() const; // Obtener la letra seleccionada

    // Callback para notificar la selección de una letra
    void setOnLetterSelectedCallback(std::function<void(char)> callback);

private:
    lv_obj_t* keyboardContainer; // Contenedor del teclado
    std::vector<std::vector<char>> rows; // Filas de letras
    std::vector<lv_obj_t*> rowContainers; // Contenedores de fila
    std::vector<std::vector<lv_obj_t*>> letterButtons; // Botones de letras
    int selectedRowIndex; // Índice de la fila seleccionada
    int selectedLetterIndex; // Índice de la letra seleccionada
    Mode currentMode; // Modo actual (selección de filas o letras)
    std::function<void(char)> onLetterSelectedCallback; // Callback para notificar la selección de una letra

    void createKeyboard();
    void updateSelection();
    void selectRow();
    void selectLetter();
};

#endif