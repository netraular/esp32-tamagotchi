#include "TileMapScreen.h"

bool TileMapScreen::loadAnimationData(const char* path) {
    lv_fs_file_t f;
    if (lv_fs_open(&f, path, LV_FS_MODE_RD) != LV_FS_RES_OK) {
        Serial.print("Error al abrir archivo: ");
        Serial.println(path);
        return false;
    }

    // Obtener tamaño del archivo
    uint32_t file_size;
    lv_fs_seek(&f, 0, LV_FS_SEEK_END);
    lv_fs_tell(&f, &file_size);
    lv_fs_seek(&f, 0, LV_FS_SEEK_SET);

    // Crear buffer para el JSON
    char* jsonBuffer = (char*)malloc(file_size + 1);
    if (!jsonBuffer) {
        Serial.println("Error: Memoria insuficiente para el buffer");
        lv_fs_close(&f);
        return false;
    }

    // Leer el archivo completo
    uint32_t bytes_read;
    lv_fs_read(&f, jsonBuffer, file_size, &bytes_read);
    jsonBuffer[bytes_read] = '\0';
    lv_fs_close(&f);

    // Parsear el JSON
    JsonDocument doc;  // Usar JsonDocument en lugar de DynamicJsonDocument
    DeserializationError error = deserializeJson(doc, jsonBuffer);
    free(jsonBuffer);

    if (error) {
        Serial.print("Error al parsear JSON: ");
        Serial.println(error.c_str());
        return false;
    }

    // Almacenar los datos en la estructura
    walkAnimation.index = doc["index"];
    walkAnimation.name = doc["name"].as<String>();
    walkAnimation.frameWidth = doc["framewidth"];
    walkAnimation.frameHeight = doc["frameheight"];

    // Leer los sprites
    JsonObject sprites = doc["sprites"];
    for (JsonPair sprite : sprites) {
        walkAnimation.sprites[sprite.key().c_str()] = sprite.value().as<JsonObject>();
    }

    // Leer las duraciones
    for (int duration : doc["durations"].as<JsonArray>()) {
        walkAnimation.durations.push_back(duration);
    }
    return true;
}

void TileMapScreen::load() {
    // Cargar elementos de la interfaz
    backgroundImage = lv_img_create(lv_scr_act());
    lv_img_set_src(backgroundImage, &MapTemplate);
    lv_obj_set_size(backgroundImage, 240, 280);

    animationImage = lv_img_create(lv_scr_act());
    lv_img_set_src(animationImage, embeddedSprites[0]);
    lv_obj_set_pos(animationImage, xPos, yPos);

    positionLabel = lv_label_create(lv_scr_act());
    lv_label_set_text_fmt(positionLabel, "Pos: (%d, %d)", xPos, yPos);
    lv_obj_align(positionLabel, LV_ALIGN_TOP_LEFT, 2, 2);

    // Cargar datos de la animación
    if (!loadAnimationData("L:/animations/Walk-AnimData.json")) {
        Serial.println("No se pudo cargar la animación");
    }
}

void TileMapScreen::update() {
    if (isAnimating) {
        animationFrameCounter++;
        
        // Actualizar sprite cada 3 frames (ajustable para velocidad)
        if (animationFrameCounter % 3 == 0) {
            int currentSprite = (animationFrameCounter / 3) % totalAnimationFrames;
            lv_img_set_src(animationImage, embeddedSprites[currentSprite]);
        }

        // Terminar animación después de 15 sprites (1-15)
        if (animationFrameCounter >= totalAnimationFrames * 3) {
            isAnimating = false;
            lv_img_set_src(animationImage, embeddedSprites[0]);  // Volver al sprite 1
        }
    } else {
        // Mostrar sprite 1 si no hay animación ni botones presionados
        lv_img_set_src(animationImage, embeddedSprites[0]);
    }
}

void TileMapScreen::handleButtonEvent(const ButtonState& state, const ButtonChange& change) {
    // Solo procesar si no hay animación activa
    if (!isAnimating) {
        // Verificar si algún botón está presionado
        if (state.button1Pressed || state.button2Pressed || state.button3Pressed || state.button4Pressed) {
            // Movimiento
            if (state.button1Pressed) xPos = (xPos > 0) ? xPos - 1 : 0;
            if (state.button4Pressed) xPos = (xPos < 239) ? xPos + 1 : 239;
            if (state.button3Pressed) yPos = (yPos > 0) ? yPos - 1 : 0;
            if (state.button2Pressed) yPos = (yPos < 279) ? yPos + 1 : 279;
            
            updatePosition();
            
            // Iniciar animación
            isAnimating = true;
            animationFrameCounter = 0;
        }
    }
}

void TileMapScreen::updatePosition() {
    // Actualizar posición del sprite
    lv_obj_set_pos(animationImage, xPos, yPos);
    
    // Actualizar texto de la etiqueta
    lv_label_set_text_fmt(positionLabel, "Pos: (%d, %d)", xPos, yPos);
    lv_obj_align(positionLabel, LV_ALIGN_TOP_LEFT, 2, 2);  // Re-alinear para evitar solapamientos
}

void TileMapScreen::unload() {
    if (backgroundImage) lv_obj_del(backgroundImage);
    if (animationImage) lv_obj_del(animationImage);
    if (positionLabel) lv_obj_del(positionLabel);
    
    backgroundImage = nullptr;
    animationImage = nullptr;
    positionLabel = nullptr;
}