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
    for(int i = 0; i < 20; i++) {
        animationImages[i] = lv_img_create(lv_scr_act());
        int col = i % 4;       // 4 columnas (0-3)
        int row = i / 4;       // 5 filas (0-4)
        int offsetX = 32 * col;  // Centrado en cada celda de 32px
        int offsetY = 32 * row;
        lv_obj_align(animationImages[i], LV_ALIGN_TOP_LEFT, offsetX, offsetY);
    }
    updateSprite();

    currentFrame = 0;
    currentSpriteIndex = 0;
    numSprites = 2;  // Inicializar con 2 sprites
    Serial.println("Animación 40 sprites cargada");
}

void BedroomScreen::update() {
    currentFrame++;
    
    if (currentFrame >= 5) {  // Cambio cada 5 frames
        currentFrame = 0;
        currentSpriteIndex = (currentSpriteIndex + 1) % numSprites; // Usar numSprites
        updateSprite();
    }
}

void BedroomScreen::updateSprite() {
    for(int i = 0; i < 20; i++) {
        if(useEmbeddedSprites) {
            lv_img_set_src(animationImages[i], embeddedSprites[currentSpriteIndex]);
        } else {
            lv_img_set_src(animationImages[i], pngPaths[currentSpriteIndex]);
        }
    }
}
void BedroomScreen::unload() {
    // Eliminar objetos LVGL
    for(int i = 0; i < 20; i++) {
        if(animationImages[i]) {
            lv_obj_del(animationImages[i]);
            animationImages[i] = nullptr;
        }
    }
    
    if (backgroundImage) {
        lv_obj_del(backgroundImage);
        backgroundImage = nullptr;
    }

    if (roomLabel) {
        lv_obj_del(roomLabel);
        roomLabel = nullptr;
    }

    // Liberar caché de imágenes PNG
    for(int i = 0; i < numSprites; i++) {
        lv_image_cache_drop(pngPaths[i]);  // Liberar cada imagen del cache
    }

    Serial.println("Recursos liberados");
}

void BedroomScreen::handleButtonEvent(const ButtonState& state, const ButtonChange& change) {
    if (change.button2Changed && state.button2Pressed) {
        useEmbeddedSprites = !useEmbeddedSprites;
        updateSprite();
        Serial.print("Modo: ");
        Serial.println(useEmbeddedSprites ? "Embedded" : "PNG");
    }
    
    // Botón 1: Decrementar sprites
    if (change.button1Changed && state.button1Pressed) {
        if (numSprites > 2) {
            numSprites--;
        }
        Serial.print("Sprites: ");
        Serial.println(numSprites);
    }
    
    // Botón 3: Incrementar sprites
    if (change.button3Changed && state.button3Pressed) {
        if (numSprites < 40) {
            numSprites++;
        }
        Serial.print("Sprites: ");
        Serial.println(numSprites);
    }
    if (change.button4Changed && state.button4Pressed) {
        screenManager.setScreen("RoomSelectionScreen");
    }
}