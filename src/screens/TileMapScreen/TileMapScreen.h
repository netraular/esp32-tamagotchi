#ifndef TILEMAP_H
#define TILEMAP_H

#include "../Screen.h"
#include <lvgl.h>
#include <ArduinoJson.h>
#include <vector>  // Para usar std::vector
#include <map>     // Para usar std::map
#include "../ScreenManager.h"

// Declarar sprites y fondo
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
extern const lv_img_dsc_t MapTemplate;

class TileMapScreen: public Screen {
public:
    void load() override;
    void update() override;
    void unload() override;
    void handleButtonEvent(const ButtonState& state, const ButtonChange& change) override;

private:
    lv_obj_t* backgroundImage = nullptr;
    lv_obj_t* animationImage = nullptr;
    lv_obj_t* positionLabel = nullptr;
    int currentFrame = 0;
    int currentSpriteIndex = 0;
    int xPos = 120;  // Posición inicial central (240/2)
    int yPos = 140;  // Posición inicial central (280/2)

    // Estructura para almacenar la animación
    struct AnimationData {
        int index;
        String name;
        int frameWidth;
        int frameHeight;
        std::map<String, JsonObject> sprites;  // Almacena los datos de cada sprite
        std::vector<int> durations;            // Vector para las duraciones
    };

    AnimationData walkAnimation;  // Variable para almacenar la animación

    const lv_img_dsc_t* embeddedSprites[15] = {
        &sprite1, &sprite2, &sprite3, &sprite4, &sprite5,
        &sprite6, &sprite7, &sprite8, &sprite9, &sprite10,
        &sprite11, &sprite12, &sprite13, &sprite14, &sprite15
    };

    void updatePosition();  // Actualizar posición y etiqueta
    bool loadAnimationData(const char* path);  // Método para cargar el JSON
};

#endif