#ifndef OUTSIDESCREEN_H
#define OUTSIDESCREEN_H

#include "screens/Screen.h"
#include <lvgl.h>

class OutsideScreen : public Screen {
public:
    void load() override;
    void update() override;
    void handleButtonEvent(const ButtonState& state, const ButtonChange& change) override;

private:
    lv_obj_t* backgroundImage;
    lv_obj_t* roomLabel;
};

#endif