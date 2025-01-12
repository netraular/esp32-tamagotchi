#ifndef BEDROOMSCREEN_H
#define BEDROOMSCREEN_H

#include "screens/Screen.h"
#include <lvgl.h>

class BedroomScreen : public Screen {
public:
    void load() override;
    void update() override;
    void handleButtonEvent(const ButtonState& state, const ButtonChange& change) override;

private:
    lv_obj_t* backgroundImage;
    lv_obj_t* roomLabel;
};

#endif