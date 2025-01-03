#ifndef FOODSCREEN_H
#define FOODSCREEN_H

#include "../Screen.h"

class FoodScreen : public Screen {
public:
    void load() override;
    void update() override;
    void handleButtonEvent(const ButtonState& state, const ButtonChange& change) override;

private:
    lv_obj_t* label;
};

#endif