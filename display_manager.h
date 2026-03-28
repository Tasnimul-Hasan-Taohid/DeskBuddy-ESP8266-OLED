#pragma once

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "config.h"

class DisplayManager {
public:
  DisplayManager();
  bool begin();

  void showMessage(const String& line1, const String& line2, int faceIndex);
  void showWaterReminder();
  void showGame(int birdY, int obstacleX, int obstacleGap, int score);
  void showGameOver(int score, int highScore);
  void showBoot();
  void clear();

private:
  Adafruit_SSD1306 _display;

  void _drawFace(int index, int x, int y);
  void _drawBird(int y);
  void _drawObstacle(int x, int gapY, int gapSize);
  void _drawCentered(const String& text, int y, uint8_t size = 1);
  void _drawWrapped(const String& text, int x, int y, int lineW);
};
