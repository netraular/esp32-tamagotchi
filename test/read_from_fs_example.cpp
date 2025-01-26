#include <Arduino.h>
#include <LittleFS.h>
#include <lvgl.h>

// Callbacks para LVGL (versión corregida)
static void* lvgl_fs_open(lv_fs_drv_t* drv, const char* path, lv_fs_mode_t mode) {
    const char* mode_str = "";
    if (mode == LV_FS_MODE_RD) mode_str = "r";
    else if (mode == LV_FS_MODE_WR) mode_str = "w";
    else if (mode == (LV_FS_MODE_WR | LV_FS_MODE_RD)) mode_str = "w+";

    fs::File* file = new fs::File(LittleFS.open(path, mode_str));
    return (*file) ? (void*)file : nullptr;  // Corregido el chequeo de archivo
}

static lv_fs_res_t lvgl_fs_close(lv_fs_drv_t* drv, void* file_p) {
    fs::File* file = (fs::File*)file_p;
    if (file) {
        file->close();
        delete file;
    }
    return LV_FS_RES_OK;
}

static lv_fs_res_t lvgl_fs_read(lv_fs_drv_t* drv, void* file_p, void* buf, uint32_t btr, uint32_t* br) {
    fs::File* file = (fs::File*)file_p;
    *br = file->readBytes((char*)buf, btr);
    return LV_FS_RES_OK;
}

static lv_fs_res_t lvgl_fs_seek(lv_fs_drv_t* drv, void* file_p, uint32_t pos, lv_fs_whence_t whence) {
    fs::File* file = (fs::File*)file_p;
    switch(whence) {
        case LV_FS_SEEK_SET: file->seek(pos); break;
        case LV_FS_SEEK_CUR: file->seek(file->position() + pos); break;
        case LV_FS_SEEK_END: file->seek(file->size() + pos); break;
        default: return LV_FS_RES_INV_PARAM;
    }
    return LV_FS_RES_OK;
}

static lv_fs_res_t lvgl_fs_tell(lv_fs_drv_t* drv, void* file_p, uint32_t* pos_p) {
    fs::File* file = (fs::File*)file_p;
    *pos_p = file->position();
    return LV_FS_RES_OK;
}

void setup() {
    Serial.begin(115200);
    delay(2000);  // Espera para inicializar serial
    
    // 1. Inicializar LittleFS
    if (!LittleFS.begin(true, "/littlefs", 10, "littlefs")) {
        Serial.println("Error al inicializar LittleFS!");
        ESP.restart();
    }

    // 2. Inicializar LVGL
    lv_init();

    // 3. Registrar driver de archivos
    static lv_fs_drv_t fs_drv;
    lv_fs_drv_init(&fs_drv);
    fs_drv.letter = 'L';
    fs_drv.open_cb = lvgl_fs_open;
    fs_drv.close_cb = lvgl_fs_close;
    fs_drv.read_cb = lvgl_fs_read;
    fs_drv.seek_cb = lvgl_fs_seek;
    fs_drv.tell_cb = lvgl_fs_tell;
    lv_fs_drv_register(&fs_drv);

    // 4. Probar lectura de archivo
    lv_fs_file_t f;
    if(lv_fs_open(&f, "L:/test.txt", LV_FS_MODE_RD) == LV_FS_RES_OK) {
        char buffer[256];
        uint32_t bytes_read;
        lv_fs_read(&f, buffer, sizeof(buffer), &bytes_read);
        buffer[bytes_read] = '\0';
        
        Serial.println("Contenido del archivo:");
        Serial.println(buffer);
        
        lv_fs_close(&f);
    } else {
        Serial.println("Error al abrir el archivo");
    }
}

void loop() {
    lv_tick_inc(5);
    lv_task_handler();
    delay(5);
}