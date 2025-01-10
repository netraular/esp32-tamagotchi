#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <lvgl.h>
#include <vector>
#include <functional>
#include <string>

class Keyboard {
public:
    enum class Mode {
        SelectRow,
        SelectLetter
    };

    Keyboard(lv_obj_t* parent);
    void show();
    void hide();
    bool isVisible() const;
    void handleButtonEvent(int button);
    char getSelectedLetter() const;

    void setOnLetterSelectedCallback(std::function<void(std::string)> callback);

    bool isSelectingLetter() const;
    bool isSelectingRow() const;

private:
    lv_obj_t* keyboardContainer;
    std::vector<std::vector<char>> rows;
    std::vector<lv_obj_t*> rowContainers;
    std::vector<std::vector<lv_obj_t*>> letterButtons;
    int selectedRowIndex;
    int selectedLetterIndex;
    Mode currentMode;
    std::function<void(std::string)> onLetterSelectedCallback;

    int visibleRowStart; // Índice de la primera fila visible
    int visibleRowCount; // Número de filas visibles

    void createKeyboard();
    void updateSelection();
    void selectRow();
    void selectLetter();
    void updateVisibleRows(); // Actualizar las filas visibles
};

#endif