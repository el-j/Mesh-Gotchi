#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <NimBLEDevice.h>
#include <TFT_eSPI.h>
#include <Wire.h>
#include <cmath>

enum SystemState { SLEEP, IDLE, ALERT, MENU };

struct Pet {
  float x;
  float y;
  float dx;
  float dy;
  int xp;
  int hunger;
  int frame;
};

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite canvas = TFT_eSprite(&tft);
Adafruit_MPU6050 mpu;
Pet myPet = {86.0f, 160.0f, 0.0f, 0.0f, 0, 0, 0};
SystemState currentState = IDLE;
unsigned long lastFrameMs = 0;

class MeshtasticAdvertisedDeviceCallbacks : public NimBLEAdvertisedDeviceCallbacks {
  void onResult(NimBLEAdvertisedDevice *advertisedDevice) override {
    if (advertisedDevice->getName().find("Meshtastic") != std::string::npos) {
      Serial.printf("Found candidate BLE peer: %s\n", advertisedDevice->getName().c_str());
    }
  }
};

void updatePhysics() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  myPet.dx -= a.acceleration.x * 0.5f;
  myPet.dy += a.acceleration.y * 0.5f;
  myPet.x += myPet.dx;
  myPet.y += myPet.dy;
  myPet.dx *= 0.9f;
  myPet.dy *= 0.9f;

  if (myPet.x <= 0 || myPet.x >= (172 - 64)) {
    myPet.dx *= -0.8f;
  }
  if (myPet.y <= 0 || myPet.y >= (320 - 64)) {
    myPet.dy *= -0.8f;
  }

  const float shake = fabs(a.acceleration.x) + fabs(a.acceleration.y) + fabs(a.acceleration.z);
  if (shake > 25.0f) {
    myPet.xp += 1;
  }

  if (a.acceleration.z < -8.0f) {
    currentState = SLEEP;
  }
}

void drawFrame() {
  canvas.fillSprite(TFT_BLACK);
  canvas.fillRoundRect(static_cast<int>(myPet.x), static_cast<int>(myPet.y), 64, 64, 12, TFT_GREEN);
  canvas.drawString("XP: " + String(myPet.xp), 5, 5, 2);
  canvas.drawString("Hunger: " + String(myPet.hunger), 5, 22, 2);
  canvas.pushSprite(0, 0);
}

void drawAlert() {
  canvas.fillSprite(TFT_BLACK);
  canvas.drawString("Meshtastic Alert!", 10, 30, 2);
  canvas.drawString("Press reset to clear", 10, 55, 2);
  canvas.pushSprite(0, 0);
}

void setup() {
  Serial.begin(115200);

  Wire.begin(20, 19);
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);

  tft.init();
  tft.setRotation(1);
  canvas.createSprite(172, 320);
  canvas.setTextColor(TFT_WHITE, TFT_BLACK);

  NimBLEDevice::init("");
  NimBLEScan *scanner = NimBLEDevice::getScan();
  scanner->setAdvertisedDeviceCallbacks(new MeshtasticAdvertisedDeviceCallbacks());
  scanner->setActiveScan(true);
  scanner->start(2, false);
}

void loop() {
  switch (currentState) {
    case IDLE:
      updatePhysics();
      drawFrame();
      break;
    case ALERT:
      drawAlert();
      break;
    case SLEEP:
      delay(100);
      if (myPet.dy < 0.3f) {
        currentState = IDLE;
      }
      break;
    case MENU:
      drawFrame();
      break;
  }

  myPet.hunger += 1;
  const unsigned long now = millis();
  const unsigned long frameInterval = 33;
  const unsigned long elapsed = now - lastFrameMs;
  if (elapsed < frameInterval) {
    delay(frameInterval - elapsed);
  }
  lastFrameMs = millis();
}
