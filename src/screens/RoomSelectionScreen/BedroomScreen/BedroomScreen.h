#ifndef BEDROOMSCREEN_H
#define BEDROOMSCREEN_H

#include "screens/Screen.h"
#include <lvgl.h>
#include <LittleFS.h>

// Declarar la imagen como variable externa
extern const lv_img_dsc_t DefaultAnimationSpriteSheet;

class BedroomScreen : public Screen {
public:
    void load() override;
    void update() override;
    void handleButtonEvent(const ButtonState& state, const ButtonChange& change) override;

private:
    lv_obj_t* backgroundImage;
    lv_obj_t* roomLabel;
    lv_obj_t* animationImage;
};

#endif