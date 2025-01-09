#ifndef INPUTTEXT_H
#define INPUTTEXT_H

#include <lvgl.h>

class InputText {
public:
    InputText(lv_obj_t* parent, int maxLength);
    lv_obj_t* getTextArea() const;
    void setMaxLength(int maxLength);
    int getMaxLength() const;
    void setPlaceholder(const char* placeholder);
    void setText(const char* text);
    const char* getText() const;

private:
    lv_obj_t* textArea;
    int maxLength;

    static void eventHandler(lv_event_t* e);
};

#endif