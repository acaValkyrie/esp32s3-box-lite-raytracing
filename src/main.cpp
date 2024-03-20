#include <Arduino.h>
#include <LovyanGFX.hpp>
#include <Wire.h>
#include <Adafruit_BNO055.h>

class LGFX: public lgfx::LGFX_Device{
  lgfx::Panel_ST7789 _panel_instance;
  lgfx::Bus_SPI _bus_instance;
  public:
  LGFX(void){
    {
      auto cfg = _bus_instance.config();
      cfg.spi_host = SPI3_HOST;
      cfg.spi_mode = 3;
      cfg.freq_write = 40000000;
      cfg.freq_read = 16000000;
      cfg.spi_3wire = true;
      cfg.use_lock = true;
      cfg.dma_channel = SPI_DMA_CH_AUTO;
      cfg.pin_sclk = 7;
      cfg.pin_mosi = 6;
      cfg.pin_miso = -1;
      cfg.pin_dc = 4;
      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }
    {
      auto cfg = _panel_instance.config();
      cfg.pin_cs = 5;
      cfg.pin_rst = 48;
      cfg.pin_busy = -1;

      cfg.panel_width = 240;
      cfg.panel_height = 320;
      cfg.offset_x = 0;
      cfg.offset_y = 0;
      cfg.offset_rotation = 3;
      cfg.dummy_read_pixel = 8;
      cfg.dummy_read_bits = 1;
      cfg.readable = true;
      cfg.invert = true;
      cfg.rgb_order = false;
      cfg.dlen_16bit = false;
      cfg.bus_shared = false;
      _panel_instance.config(cfg);
    }
    setPanel(&_panel_instance);
  }
};

LGFX display;
Adafruit_BNO055 accel_sensor = Adafruit_BNO055(55, 0x28, &Wire);
class Vector3{
  public:
  float x;
  float y;
  float z;
};
Vector3 accel;
Vector3 velocity;
Vector3 position;

void setup(void) {
  display.init();
  display.setTextSize(1);
  display.printf("Hello, World!\n");
  const int sda = 41;
  const int scl = 40;
  pinMode(sda, INPUT_PULLUP);
  pinMode(scl, INPUT_PULLUP);
  Wire.begin(41, 40);
  if (!accel_sensor.begin()) // センサの初期化
  {
    display.printf("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!\n");
    while (1);
  }

  velocity.x = velocity.y = velocity.z = 0;
  position.x = position.y = position.z = 0;
}

void loop(void) {
  display.setCursor(0, 0);
  imu::Vector<3> accelermetor = accel_sensor.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
  accel.x = accelermetor.x() * 0.1;
  accel.y = accelermetor.y() * 0.1;
  accel.z = accelermetor.z() * 0.1;
  velocity.x += accel.x;
  velocity.y += accel.y;
  velocity.z += accel.z;
  position.x += velocity.x;
  position.y += velocity.y;
  position.z += velocity.z;
  display.printf("a x: %.2f \ty: %.2f \tz: %.2f\n", accel.x, accel.y, accel.z);
  display.printf("v x: %.2f \ty: %.2f \tz: %.2f\n", velocity.x, velocity.y, velocity.z);
  display.printf("p x: %.2f \ty: %.2f \tz: %.2f\n", position.x, position.y, position.z);

  delay(10);
}