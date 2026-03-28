#include "config.h"
#include "display_manager.h"
#include "button_manager.h"
#include "message_manager.h"
#include "game_manager.h"

// ─── Managers ──────────────────────────────────────────────────
DisplayManager display;
ButtonManager  button;
MessageManager messages;
GameManager    game;

// ─── App State ─────────────────────────────────────────────────
AppMode currentMode    = MODE_MESSAGE;
AppMode previousMode   = MODE_MESSAGE;

// Message mode timing
unsigned long lastMessageTime  = 0;

// Water reminder timing
unsigned long lastWaterTime    = 0;
unsigned long waterDisplayStart = 0;

// Game timing
unsigned long lastGameTick     = 0;
unsigned long gameOverTime     = 0;

// ─── Helpers ───────────────────────────────────────────────────
void setMode(AppMode m) {
  previousMode = currentMode;
  currentMode  = m;
  Serial.print("[MODE] -> ");
  const char* names[] = {"MESSAGE","WATER","GAME","GAME_OVER"};
  Serial.println(names[(int)m]);
}

// ──────────────────────────────────────────────────────────────
void setup() {
  Serial.begin(DEBUG_BAUD);
  delay(100);
  Serial.println("\n\n== DeskBuddy v1.0 ==");

  if (!display.begin()) {
    Serial.println("[SETUP] Display FATAL");
    while (1) yield();
  }

  button.begin();

  display.showBoot();
  delay(2000);

  // Show first message immediately
  const BuddyMessage& msg = messages.current();
  display.showMessage(msg.line1, msg.line2, msg.faceIndex);
  lastMessageTime = millis();
  lastWaterTime   = millis();

  Serial.println("[SETUP] Done");
}

void loop() {
  button.update();

  unsigned long now = millis();

  // ── Water reminder: interrupt any mode except game ──────────
  if (currentMode != MODE_GAME && currentMode != MODE_GAME_OVER) {
    if (now - lastWaterTime >= WATER_REMINDER_MS) {
      lastWaterTime    = now;
      waterDisplayStart = now;
      setMode(MODE_WATER);
    }
  }

  // ── Button: hold 2 seconds in message mode to enter game ───
  if (currentMode == MODE_MESSAGE && button.isHeld(2000)) {
    game.reset();
    lastGameTick = now;
    setMode(MODE_GAME);
    delay(200); // absorb the held press so it doesn't immediately jump
    return;
  }

  // ── Mode logic ─────────────────────────────────────────────
  switch (currentMode) {

    // ── MESSAGE mode ─────────────────────────────────────────
    case MODE_MESSAGE: {
      if (button.wasPressed()) {
        messages.next();
        const BuddyMessage& msg = messages.current();
        display.showMessage(msg.line1, msg.line2, msg.faceIndex);
        lastMessageTime = now;
        break;
      }
      // Auto-advance messages
      if (now - lastMessageTime >= MESSAGE_DISPLAY_MS) {
        messages.next();
        lastMessageTime = now;
      }
      const BuddyMessage& msg = messages.current();
      display.showMessage(msg.line1, msg.line2, msg.faceIndex);
      break;
    }

    // ── WATER mode ───────────────────────────────────────────
    case MODE_WATER: {
      display.showWaterReminder();
      bool dismissed = button.wasPressed();
      bool expired   = (now - waterDisplayStart >= WATER_DISPLAY_MS);
      if (dismissed || expired) {
        setMode(MODE_MESSAGE);
        lastMessageTime = now;
      }
      break;
    }

    // ── GAME mode ────────────────────────────────────────────
    case MODE_GAME: {
      // Button press = jump
      if (button.wasPressed()) {
        game.jump();
      }

      // Game tick
      if (now - lastGameTick >= GAME_TICK_MS) {
        lastGameTick = now;
        game.update();
      }

      if (game.isGameOver()) {
        gameOverTime = now;
        setMode(MODE_GAME_OVER);
        display.showGameOver(game.getScore(), game.getHighScore());
        break;
      }

      display.showGame(
        game.getBirdY(),
        game.getObstacleX(),
        game.getObstacleGap(),
        game.getScore()
      );
      break;
    }

    // ── GAME OVER mode ───────────────────────────────────────
    case MODE_GAME_OVER: {
      display.showGameOver(game.getScore(), game.getHighScore());

      if (button.wasPressed()) {
        // Press once = retry game
        game.reset();
        lastGameTick = now;
        setMode(MODE_GAME);
        break;
      }

      // Hold button 2s from game over = back to messages
      if (button.isHeld(2000)) {
        setMode(MODE_MESSAGE);
        lastMessageTime = now;
        break;
      }

      // Auto-return to message mode after 20s idle
      if (now - gameOverTime >= 20000) {
        setMode(MODE_MESSAGE);
        lastMessageTime = now;
      }
      break;
    }
  }

  delay(10);
  yield();
}
