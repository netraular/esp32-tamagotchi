#ifndef BEDROOMSCREEN_H
#define BEDROOMSCREEN_H

#include "screens/Screen.h"
#include <lvgl.h>
#include <LittleFS.h>

// Declarar todos los sprites embebidos (de sprite1 a sprite40)
extern const lv_img_dsc_t sprite1;
extern const lv_img_dsc_t sprite2;
extern const lv_img_dsc_t sprite3;
extern const lv_img_dsc_t sprite4;
extern const lv_img_dsc_t sprite5;
extern const lv_img_dsc_t sprite6;
extern const lv_img_dsc_t sprite7;
extern const lv_img_dsc_t sprite8;
extern const lv_img_dsc_t sprite9;
extern const lv_img_dsc_t sprite10;
extern const lv_img_dsc_t sprite11;
extern const lv_img_dsc_t sprite12;
extern const lv_img_dsc_t sprite13;
extern const lv_img_dsc_t sprite14;
extern const lv_img_dsc_t sprite15;
extern const lv_img_dsc_t sprite16;
extern const lv_img_dsc_t sprite17;
extern const lv_img_dsc_t sprite18;
extern const lv_img_dsc_t sprite19;
extern const lv_img_dsc_t sprite20;
extern const lv_img_dsc_t sprite21;
extern const lv_img_dsc_t sprite22;
extern const lv_img_dsc_t sprite23;
extern const lv_img_dsc_t sprite24;
extern const lv_img_dsc_t sprite25;
extern const lv_img_dsc_t sprite26;
extern const lv_img_dsc_t sprite27;
extern const lv_img_dsc_t sprite28;
extern const lv_img_dsc_t sprite29;
extern const lv_img_dsc_t sprite30;
extern const lv_img_dsc_t sprite31;
extern const lv_img_dsc_t sprite32;
extern const lv_img_dsc_t sprite33;
extern const lv_img_dsc_t sprite34;
extern const lv_img_dsc_t sprite35;
extern const lv_img_dsc_t sprite36;
extern const lv_img_dsc_t sprite37;
extern const lv_img_dsc_t sprite38;
extern const lv_img_dsc_t sprite39;
extern const lv_img_dsc_t sprite40;

extern const lv_img_dsc_t background1;

class BedroomScreen : public Screen {
public:
    void load() override;
    void update() override;
    void unload() override;
    void handleButtonEvent(const ButtonState& state, const ButtonChange& change) override;

private:
    lv_obj_t* backgroundImage = nullptr;
    lv_obj_t* roomLabel = nullptr;
lv_obj_t* animationImages[20] = {nullptr};  // Array para 20 imágenes    
    bool useEmbeddedSprites = true;
    int currentFrame = 0;
    int currentSpriteIndex = 0;
    int numSprites = 2;  // Nueva variable para controlar el número de sprites

    // Arrays para manejar ambos modos (40 sprites)
    const char* pngPaths[40] = {
        "L:/animations/snorlaxEdited/sprite1.png",
        "L:/animations/snorlaxEdited/sprite2.png",
        "L:/animations/snorlaxEdited/sprite3.png",
        "L:/animations/snorlaxEdited/sprite4.png",
        "L:/animations/snorlaxEdited/sprite5.png",
        "L:/animations/snorlaxEdited/sprite6.png",
        "L:/animations/snorlaxEdited/sprite7.png",
        "L:/animations/snorlaxEdited/sprite8.png",
        "L:/animations/snorlaxEdited/sprite9.png",
        "L:/animations/snorlaxEdited/sprite10.png",
        "L:/animations/snorlaxEdited/sprite11.png",
        "L:/animations/snorlaxEdited/sprite12.png",
        "L:/animations/snorlaxEdited/sprite13.png",
        "L:/animations/snorlaxEdited/sprite14.png",
        "L:/animations/snorlaxEdited/sprite15.png",
        "L:/animations/snorlaxEdited/sprite16.png",
        "L:/animations/snorlaxEdited/sprite17.png",
        "L:/animations/snorlaxEdited/sprite18.png",
        "L:/animations/snorlaxEdited/sprite19.png",
        "L:/animations/snorlaxEdited/sprite20.png",
        "L:/animations/snorlaxEdited/sprite21.png",
        "L:/animations/snorlaxEdited/sprite22.png",
        "L:/animations/snorlaxEdited/sprite23.png",
        "L:/animations/snorlaxEdited/sprite24.png",
        "L:/animations/snorlaxEdited/sprite25.png",
        "L:/animations/snorlaxEdited/sprite26.png",
        "L:/animations/snorlaxEdited/sprite27.png",
        "L:/animations/snorlaxEdited/sprite28.png",
        "L:/animations/snorlaxEdited/sprite29.png",
        "L:/animations/snorlaxEdited/sprite30.png",
        "L:/animations/snorlaxEdited/sprite31.png",
        "L:/animations/snorlaxEdited/sprite32.png",
        "L:/animations/snorlaxEdited/sprite33.png",
        "L:/animations/snorlaxEdited/sprite34.png",
        "L:/animations/snorlaxEdited/sprite35.png",
        "L:/animations/snorlaxEdited/sprite36.png",
        "L:/animations/snorlaxEdited/sprite37.png",
        "L:/animations/snorlaxEdited/sprite38.png",
        "L:/animations/snorlaxEdited/sprite39.png",
        "L:/animations/snorlaxEdited/sprite40.png"
    };
    
    const lv_img_dsc_t* embeddedSprites[40] = {
        &sprite1, &sprite2, &sprite3, &sprite4, &sprite5,
        &sprite6, &sprite7, &sprite8, &sprite9, &sprite10,
        &sprite11, &sprite12, &sprite13, &sprite14, &sprite15,
        &sprite16, &sprite17, &sprite18, &sprite19, &sprite20,
        &sprite21, &sprite22, &sprite23, &sprite24, &sprite25,
        &sprite26, &sprite27, &sprite28, &sprite29, &sprite30,
        &sprite31, &sprite32, &sprite33, &sprite34, &sprite35,
        &sprite36, &sprite37, &sprite38, &sprite39, &sprite40
    };

    void updateSprite();  // Método para actualizar el sprite
};

#endif