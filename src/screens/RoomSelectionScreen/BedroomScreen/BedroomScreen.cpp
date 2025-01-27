#include "BedroomScreen.h"
#include "../../ScreenManager.h"

// Definir los sprites embebidos (de sprite1 a sprite40)
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
extern ScreenManager screenManager;

void BedroomScreen::load() {
    // Fondo y label
    backgroundImage = lv_img_create(lv_scr_act());
    lv_img_set_src(backgroundImage, &background1);
    lv_obj_set_size(backgroundImage, 128, 160);
    
    roomLabel = lv_label_create(lv_scr_act());
    lv_label_set_text(roomLabel, "x1 Stress Test 40-SPRITES");
    lv_obj_set_style_text_font(roomLabel, &lv_font_montserrat_10, 0);
    lv_obj_align(roomLabel, LV_ALIGN_TOP_LEFT, 2, 2);

    // Crear imagen de animación
    animationImage = lv_img_create(lv_scr_act());
    lv_obj_align(animationImage, LV_ALIGN_CENTER, 0, 0);
    updateSprite();

    currentFrame = 0;
    currentSpriteIndex = 0;
    Serial.println("Animación 40 sprites cargada");
}

void BedroomScreen::update() {
    currentFrame++;
    
    if (currentFrame >= 5) {  // Cambio cada 5 frames
        currentFrame = 0;
        currentSpriteIndex = (currentSpriteIndex + 1) % 40;
        updateSprite();
    }
}

void BedroomScreen::updateSprite() {
    if (useEmbeddedSprites) {
        lv_img_set_src(animationImage, embeddedSprites[currentSpriteIndex]);
    } else {
        lv_img_set_src(animationImage, pngPaths[currentSpriteIndex]);
    }
}

void BedroomScreen::unload() {
    if (animationImage) {
        lv_obj_del(animationImage);
        animationImage = nullptr;
    }

    if (backgroundImage) {
        lv_obj_del(backgroundImage);
        backgroundImage = nullptr;
    }

    if (roomLabel) {
        lv_obj_del(roomLabel);
        roomLabel = nullptr;
    }

    Serial.println("Recursos liberados");
}

void BedroomScreen::handleButtonEvent(const ButtonState& state, const ButtonChange& change) {
    if (change.button1Changed && state.button1Pressed) {
        useEmbeddedSprites = !useEmbeddedSprites;
        updateSprite();  // Actualizar al cambiar modo
        Serial.print("Modo: ");
        Serial.println(useEmbeddedSprites ? "Embedded" : "PNG");
    }
    
    if (change.button3Changed && state.button3Pressed) {
        screenManager.setScreen("RoomSelectionScreen");
    }
}