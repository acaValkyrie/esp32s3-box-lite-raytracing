#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BNO055.h>
#include "lgfx.h"
#include "vector.h"

Adafruit_BNO055 accel_sensor = Adafruit_BNO055(55, 0x28, &Wire);

static LGFX display;
static LGFX_Sprite sprite(&display);

Vector3 accel;
Vector3 velocity;
Vector3 position;

Vector3 pos_max;
Vector3 pos_min;

int x_draw;
int y_draw;
const int r = 10;

void setup(void) {
  display.init();
  display.setTextSize(1);
  // display.printf("Hello, World!\n");
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

  velocity = Vector3(0, 0, 0);
  position = Vector3(0, 0, 0);
  const int margin = r + 1;
  pos_max = Vector3(display.width() - margin, display.height() - margin, 10);
  pos_min = Vector3(margin, margin, 0);
  x_draw = position.x;
  y_draw = position.y;
  sprite.createSprite(display.width(), display.height());
}

void loop(void) {
  imu::Vector<3> accelermetor = accel_sensor.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
  accel.x = accelermetor.x() * 0.1;
  accel.y = (accelermetor.y()*(0.7) + accelermetor.z()*(-0.7)) * 0.1;
  accel.z = (accelermetor.y()*(-0.7) + accelermetor.z()*(-0.7)) * 0.1;
  velocity.x += accel.x;
  velocity.y += accel.y;
  velocity.z += accel.z;
  position.x += velocity.x;
  position.y += velocity.y;
  position.z += velocity.z;

  const float bounce = -0.5;
  if (position.x > pos_max.x){ position.x = pos_max.x; velocity.x *= bounce; }
  if (position.x < pos_min.x){ position.x = pos_min.x; velocity.x *= bounce; }
  if (position.y > pos_max.y){ position.y = pos_max.y; velocity.y *= bounce; }
  if (position.y < pos_min.y){ position.y = pos_min.y; velocity.y *= bounce; }
  if (position.z > pos_max.z){ position.z = pos_max.z; velocity.z *= bounce; }
  if (position.z < pos_min.z){ position.z = pos_min.z; velocity.z *= bounce; }

  const int x_before = x_draw;
  const int y_before = y_draw;
  x_draw = position.x;
  y_draw = position.y;
  sprite.fillCircle(x_before, y_before, r, 0x0000);
  sprite.fillCircle(x_draw, y_draw, r, 0xFFFF);
  sprite.pushSprite(&display, 0, 0);
  delay(1);
}