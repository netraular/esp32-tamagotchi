#ifndef RESTARTSCREEN_H
#define RESTARTSCREEN_H

#include "../Screen.h"

class RestartScreen : public Screen {
public:
    void load() override;
    void update() override;
    void handleButtonEvent(const ButtonState& state, const ButtonChange& change) override;

private:
    lv_obj_t* label;
};

#endif