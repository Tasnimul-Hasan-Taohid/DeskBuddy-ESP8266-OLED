#pragma once

// ─── OLED Display ───────────────────────────────────────────────
#define OLED_SDA          4        // D2
#define OLED_SCL          5        // D1
#define OLED_WIDTH        128
#define OLED_HEIGHT       64
#define OLED_RESET        -1
#define OLED_I2C_ADDR     0x3C

// ─── Button ─────────────────────────────────────────────────────
#define BUTTON_PIN        0        // D3 / GPIO0 (FLASH button, active LOW)
#define DEBOUNCE_MS       50

// ─── Timings ────────────────────────────────────────────────────
#define MESSAGE_DISPLAY_MS     4000   // How long each message shows (ms)
#define WATER_REMINDER_MS     1800000 // Every 30 minutes
#define WATER_DISPLAY_MS       6000   // How long water reminder shows
#define MOOD_CHANGE_MS        10000   // Mood face changes every 10s
#define IDLE_RETURN_MS        15000   // Return to message mode after game idle

// ─── Game ───────────────────────────────────────────────────────
#define GAME_GRAVITY        2
#define GAME_JUMP_STRENGTH  8
#define GAME_TICK_MS        80        // Game loop speed (ms)
#define OBSTACLE_SPEED      3

// ─── Debug ──────────────────────────────────────────────────────
#define DEBUG_BAUD        115200

// ─── App Modes ──────────────────────────────────────────────────
enum AppMode {
  MODE_MESSAGE,     // Rotating cute messages + face
  MODE_WATER,       // Water reminder alert
  MODE_GAME,        // Dino-style jump game
  MODE_GAME_OVER    // Game over screen
};
