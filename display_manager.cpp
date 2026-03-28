#include "display_manager.h"

DisplayManager::DisplayManager()
  : _display(OLED_WIDTH, OLED_HEIGHT, &Wire, OLED_RESET) {}

bool DisplayManager::begin() {
  Wire.begin(OLED_SDA, OLED_SCL);
  if (!_display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDR)) {
    Serial.println("[DISPLAY] Init failed");
    return false;
  }
  _display.clearDisplay();
  _display.setTextColor(SSD1306_WHITE);
  _display.display();
  Serial.println("[DISPLAY] OK");
  return true;
}

void DisplayManager::clear() {
  _display.clearDisplay();
  _display.display();
}

// ─── Boot Screen ───────────────────────────────────────────────
void DisplayManager::showBoot() {
  _display.clearDisplay();
  _drawCentered("DeskBuddy", 10, 2);
  _drawCentered("v1.0", 30);
  _drawCentered("Hello! (^.^)", 45);
  _display.display();
}

// ─── Message Mode ──────────────────────────────────────────────
void DisplayManager::showMessage(const String& line1, const String& line2, int faceIndex) {
  _display.clearDisplay();

  // Face on the left (32x32 area)
  _drawFace(faceIndex, 2, 8);

  // Message on the right
  _display.setTextSize(1);
  _drawWrapped(line1, 38, 8, 14);
  if (line2.length() > 0) {
    _drawWrapped(line2, 38, 30, 14);
  }

  // Thin bottom border line
  _display.drawFastHLine(0, 62, 128, SSD1306_WHITE);
  _display.display();
}

// ─── Water Reminder ────────────────────────────────────────────
void DisplayManager::showWaterReminder() {
  _display.clearDisplay();

  // Draw a cute water drop shape
  // Drop body (circle)
  _display.fillCircle(20, 38, 12, SSD1306_WHITE);
  // Drop tip (triangle above circle)
  for (int i = 0; i < 10; i++) {
    _display.drawLine(20, 26 - i, 20 - (i / 2), 26, SSD1306_WHITE);
    _display.drawLine(20, 26 - i, 20 + (i / 2), 26, SSD1306_WHITE);
  }
  // Shine dot
  _display.fillCircle(15, 34, 2, SSD1306_BLACK);

  // Text
  _display.setTextSize(1);
  _drawCentered("Hey! Drink some", 4);
  _drawCentered("water, okay?", 15);
  _drawCentered("(>.<) Hydrate!", 50);

  _display.display();
}

// ─── Game Screen ───────────────────────────────────────────────
void DisplayManager::showGame(int birdY, int obstacleX, int obstacleGap, int score) {
  _display.clearDisplay();

  // Ground line
  _display.drawFastHLine(0, 55, 128, SSD1306_WHITE);

  // Bird (little square with an eye)
  _drawBird(birdY);

  // Obstacle
  _drawObstacle(obstacleX, obstacleGap, 16);

  // Score
  _display.setTextSize(1);
  _display.setCursor(90, 2);
  _display.print("Pts:");
  _display.print(score);

  _display.display();
}

// ─── Game Over ─────────────────────────────────────────────────
void DisplayManager::showGameOver(int score, int highScore) {
  _display.clearDisplay();
  _drawCentered("Game Over!", 4, 1);

  // Sad face
  _drawFace(3, 48, 14);

  _display.setTextSize(1);
  String s1 = "Score:  " + String(score);
  String s2 = "Best:   " + String(highScore);
  _drawCentered(s1, 40);
  _drawCentered(s2, 50);

  _drawCentered("Press to retry", 57);
  _display.display();
}

// ─── Private Helpers ───────────────────────────────────────────

/*
  Face index:
  0 = happy   (^.^)
  1 = wink    (^_~)
  2 = love    (^3^)
  3 = sad     (;_;)
  4 = blush   (>///<)
  5 = excited (*.*) 
*/
void DisplayManager::_drawFace(int index, int x, int y) {
  // Outer face circle
  _display.drawCircle(x + 15, y + 16, 15, SSD1306_WHITE);

  switch (index) {
    case 0: // happy
      // eyes
      _display.fillCircle(x + 9,  y + 12, 2, SSD1306_WHITE);
      _display.fillCircle(x + 21, y + 12, 2, SSD1306_WHITE);
      // smile arc (manual)
      for (int i = -4; i <= 4; i++)
        _display.drawPixel(x + 15 + i, y + 22 + (i * i) / 8, SSD1306_WHITE);
      break;

    case 1: // wink
      // left eye closed (line)
      _display.drawFastHLine(x + 7, y + 12, 4, SSD1306_WHITE);
      // right eye open
      _display.fillCircle(x + 21, y + 12, 2, SSD1306_WHITE);
      // smile
      for (int i = -4; i <= 4; i++)
        _display.drawPixel(x + 15 + i, y + 22 + (i * i) / 8, SSD1306_WHITE);
      break;

    case 2: // love
      // heart eyes (simplified)
      _display.fillCircle(x + 9,  y + 12, 3, SSD1306_WHITE);
      _display.fillCircle(x + 21, y + 12, 3, SSD1306_WHITE);
      // tiny pupil
      _display.fillCircle(x + 9,  y + 12, 1, SSD1306_BLACK);
      _display.fillCircle(x + 21, y + 12, 1, SSD1306_BLACK);
      // smile big
      for (int i = -5; i <= 5; i++)
        _display.drawPixel(x + 15 + i, y + 23 + (i * i) / 6, SSD1306_WHITE);
      break;

    case 3: // sad
      // eyes
      _display.fillCircle(x + 9,  y + 14, 2, SSD1306_WHITE);
      _display.fillCircle(x + 21, y + 14, 2, SSD1306_WHITE);
      // frown
      for (int i = -4; i <= 4; i++)
        _display.drawPixel(x + 15 + i, y + 24 - (i * i) / 8, SSD1306_WHITE);
      break;

    case 4: // blush
      // eyes
      _display.fillCircle(x + 9,  y + 12, 2, SSD1306_WHITE);
      _display.fillCircle(x + 21, y + 12, 2, SSD1306_WHITE);
      // blush marks
      _display.drawFastHLine(x + 4,  y + 17, 4, SSD1306_WHITE);
      _display.drawFastHLine(x + 22, y + 17, 4, SSD1306_WHITE);
      // tiny smile
      for (int i = -3; i <= 3; i++)
        _display.drawPixel(x + 15 + i, y + 21 + (i * i) / 6, SSD1306_WHITE);
      break;

    case 5: // excited (wide eyes)
      _display.drawCircle(x + 9,  y + 12, 3, SSD1306_WHITE);
      _display.drawCircle(x + 21, y + 12, 3, SSD1306_WHITE);
      _display.fillCircle(x + 9,  y + 12, 1, SSD1306_WHITE);
      _display.fillCircle(x + 21, y + 12, 1, SSD1306_WHITE);
      // open "O" mouth
      _display.drawCircle(x + 15, y + 22, 3, SSD1306_WHITE);
      break;

    default:
      // fallback to happy
      _display.fillCircle(x + 9,  y + 12, 2, SSD1306_WHITE);
      _display.fillCircle(x + 21, y + 12, 2, SSD1306_WHITE);
      for (int i = -4; i <= 4; i++)
        _display.drawPixel(x + 15 + i, y + 22 + (i * i) / 8, SSD1306_WHITE);
      break;
  }
}

void DisplayManager::_drawBird(int y) {
  // 8x8 pixel bird (simple flappy-style)
  _display.fillRect(12, y, 8, 6, SSD1306_WHITE);     // body
  _display.drawPixel(19, y + 1, SSD1306_BLACK);       // eye
  _display.fillRect(10, y + 2, 3, 2, SSD1306_WHITE);  // wing
}

void DisplayManager::_drawObstacle(int x, int gapY, int gapSize) {
  if (x < -10 || x > OLED_WIDTH) return;
  // Top pillar
  if (gapY > 0)
    _display.fillRect(x, 0, 8, gapY, SSD1306_WHITE);
  // Bottom pillar
  int bottomTop = gapY + gapSize;
  if (bottomTop < 55)
    _display.fillRect(x, bottomTop, 8, 55 - bottomTop, SSD1306_WHITE);
  // Cap decorations
  _display.fillRect(x - 1, gapY - 4, 10, 4, SSD1306_WHITE);
  _display.fillRect(x - 1, gapY + gapSize, 10, 4, SSD1306_WHITE);
}

void DisplayManager::_drawCentered(const String& text, int y, uint8_t size) {
  _display.setTextSize(size);
  int16_t x1, y1;
  uint16_t w, h;
  _display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
  int x = max(0, (int)((OLED_WIDTH - w) / 2));
  _display.setCursor(x, y);
  _display.print(text);
}

void DisplayManager::_drawWrapped(const String& text, int x, int y, int lineW) {
  _display.setTextSize(1);
  String word = "";
  int curX = x, curY = y;
  int charW = 6;

  for (int i = 0; i <= (int)text.length(); i++) {
    char c = (i < (int)text.length()) ? text[i] : ' ';
    if (c == ' ' || i == (int)text.length()) {
      if (curX + (int)word.length() * charW > x + lineW * charW) {
        curX = x;
        curY += 9;
        if (curY > OLED_HEIGHT - 9) break;
      }
      _display.setCursor(curX, curY);
      _display.print(word);
      curX += (word.length() + 1) * charW;
      word = "";
    } else {
      word += c;
    }
  }
}
