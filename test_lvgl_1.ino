#include <lvgl.h>
#include <Arduino_GFX_Library.h>
#include <TAMC_GT911.h>
#include <Wire.h>
#include "Arial_16.c"

// LVGL font
LV_FONT_DECLARE(Arial);

// Ekran - parametry
#define TFT_WIDTH     800
#define TFT_HEIGHT    480
#define TFT_BL        2

// Dotyk - piny
#define TOUCH_SDA     19
#define TOUCH_SCL     20
#define TOUCH_RST     38
#define TOUCH_INT     -1

// LVGL buffer
static lv_color_t buf[TFT_WIDTH * 40];
static lv_display_t *lv_disp;

// Ekran RGB
Arduino_ESP32RGBPanel *bus = new Arduino_ESP32RGBPanel(
  /* DE, VSYNC, HSYNC, PCLK */ 41, 40, 39, 42,
  /* R0-R4 */ 14, 21, 47, 48, 45,
  /* G0-G5 */ 9, 46, 3, 8, 16, 1,
  /* B0-B4 */ 15, 7, 6, 5, 4,
  /* width, height */ TFT_WIDTH, TFT_HEIGHT,
  /* hsync */ 210, 30, 16,
  /* vsync */ 22, 13, 10,
  /* pclk flags, freq, autoflush */ 1, 16000000, true
);

Arduino_RGB_Display *gfx = new Arduino_RGB_Display(TFT_WIDTH, TFT_HEIGHT, bus, 0, true);

// Dotyk GT911
TAMC_GT911 touch(TOUCH_SDA, TOUCH_SCL, TOUCH_INT, TOUCH_RST, TFT_WIDTH, TFT_HEIGHT);
int licznik_1 = 0;

// Funkcja: odczyt dotyku dla LVGL
void my_touch_read(lv_indev_t *indev, lv_indev_data_t *data) {
  touch.read();
  if (touch.isTouched) {
    data->point.x = touch.points[0].x;
    data->point.y = touch.points[0].y;
    data->state = LV_INDEV_STATE_PRESSED;
  } else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}

// Funkcja: flush ekranu dla LVGL
void my_flush_cb(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map) {
  gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)px_map,
                          area->x2 - area->x1 + 1,
                          area->y2 - area->y1 + 1);
  lv_display_flush_ready(disp);
}

// UI
lv_obj_t *label;

void btn_event_cb(lv_event_t *e) {
  licznik_1++;
  char buf[64];
  snprintf(buf, sizeof(buf), "naciśnięto %d", licznik_1);
  lv_label_set_text(label, buf);
}

void setup_ui() {
  lv_obj_t *btn = lv_btn_create(lv_screen_active());
  
  lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_CLICKED, NULL);

// Stylowanie przycisku
  lv_obj_set_style_bg_color(btn, lv_palette_main(LV_PALETTE_PINK), 0);
  lv_obj_set_style_bg_opa(btn, LV_OPA_COVER, 0); // pełne krycie
  lv_obj_set_style_radius(btn, 10, 0);           // zaokrąglenie rogów
  lv_obj_set_size(btn, 200, 60);                 // rozmiar przycisku
  lv_obj_set_pos(btn, 0, 0);
  label = lv_label_create(btn);
  lv_obj_center(label);
  lv_obj_set_style_text_font(label, &Arial, 0);
  lv_label_set_text(label, "Kliknij mnie");
}

// Setup
void setup() {
  Serial.begin(115200);

  // Podświetlenie TFT
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  // Inicjalizacja ekranu i dotyku
  gfx->begin();
  gfx->fillScreen(BLACK);
  Wire.begin(TOUCH_SDA, TOUCH_SCL);
  touch.begin();
  touch.setRotation(ROTATION_INVERTED);

  // Inicjalizacja LVGL
  lv_init();
  lv_tick_set_cb([]() { return (uint32_t)millis(); });

  // Ekran w LVGL
  lv_disp = lv_display_create(TFT_WIDTH, TFT_HEIGHT);
  lv_display_set_flush_cb(lv_disp, my_flush_cb);
  lv_display_set_buffers(lv_disp, buf, NULL, sizeof(buf), LV_DISPLAY_RENDER_MODE_PARTIAL);

  // Dotyk w LVGL
  lv_indev_t *indev = lv_indev_create();
  lv_indev_set_disp(indev, lv_disp);
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev, my_touch_read);

  // UI
  setup_ui();
}

// Pętla główna
void loop() {
  lv_task_handler();
  delay(5);
}