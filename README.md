# 🤖 DeskBuddy — Your OLED Desk Companion

> A tiny ESP8266 gadget that sits on your desk, sends you cute messages, reminds you to drink water, and lets you play a one-button game when you need a break.

No WiFi needed. No app. No cloud. Just vibes.

---

## What It Does

| Feature | Details |
|---|---|
| 💬 Cute Messages | 25 rotating motivational messages with animated pixel faces |
| 💧 Water Reminder | Reminds you to hydrate every 30 minutes |
| 🎮 One-Button Game | Flappy-style jump game, playable with a single button |
| 😊 Animated Faces | 6 face expressions: happy, wink, love, sad, blush, excited |
| 🏆 High Score | Tracks your best game score across rounds |

---

## Hardware

### Components

| Part | Details |
|---|---|
| Board | ESP8266 (NodeMCU or Wemos D1 Mini) |
| Display | 0.96" OLED, SSD1306, 128×64 px, I2C |
| Button | Any momentary push button (normally open) |
| Power | Micro-USB 5V |

### Wiring

| ESP8266 Pin | Connected To | Description |
|---|---|---|
| D1 (GPIO5) | OLED SCL | I2C Clock |
| D2 (GPIO4) | OLED SDA | I2C Data |
| 3V3 | OLED VCC | Power |
| GND | OLED GND + Button pin 2 | Ground |
| D3 (GPIO0) | Button pin 1 | Input (active LOW) |

> **Note:** GPIO0 is the built-in FLASH button on NodeMCU/D1 Mini boards — no external button needed to get started!

---

## Project Structure

```
ESP8266-OLED-DeskBuddy/
├── main.ino               ← App loop + mode switching
├── config.h               ← All settings and constants
├── display_manager.h
├── display_manager.cpp    ← OLED drawing (faces, game, messages)
├── button_manager.h
├── button_manager.cpp     ← Debounced button + hold detection
├── message_manager.h
├── message_manager.cpp    ← 25 cute messages + face assignments
├── game_manager.h
├── game_manager.cpp       ← Flappy-style game physics + collision
└── README.md
```

---

## Library Installation

Open Arduino IDE → **Sketch → Include Library → Manage Libraries**

| Library | Author |
|---|---|
| Adafruit GFX | Adafruit |
| Adafruit SSD1306 | Adafruit |

That's it — no other external libraries needed.

---

## Flash & Run

1. Open `main.ino` in Arduino IDE
2. **Tools → Board** → `NodeMCU 1.0` or `LOLIN(Wemos) D1 Mini`
3. **Tools → Port** → select your port
4. Click **Upload**
5. Done — DeskBuddy boots in 2 seconds

---

## How to Use

### Message Mode (default)
- DeskBuddy cycles through 25 cute messages automatically every 4 seconds
- **Press button once** → skip to next message
- **Hold button 2 seconds** → enter game mode

### Water Reminder
- Every 30 minutes, DeskBuddy interrupts with a drink water reminder
- **Press button** or wait 6 seconds → dismiss and return to messages

### Game Mode
- A one-button Flappy-style game
- **Press button** → bird jumps
- Avoid the pillars — score goes up each time you pass one
- Hit a pillar → Game Over screen shows your score and best score

### Game Over Screen
- **Press button once** → retry the game
- **Hold button 2 seconds** → back to message mode
- **Wait 20 seconds** → auto-return to message mode

---

## OLED Display Layouts

### Message Mode
```
┌──────────────────────────┐
│  ○     │ You're doing    │
│ (^.^)  │ amazing!        │
│  ○     │                 │
│_________________________ │
└──────────────────────────┘
```

### Water Reminder
```
┌──────────────────────────┐
│  Hey! Drink some         │
│  water, okay?            │
│    💧                    │
│  (>.<) Hydrate!          │
└──────────────────────────┘
```

### Game Mode
```
┌──────────────────────────┐  Pts: 3
│         ████             │
│         █  █             │
│  [bird]    █  █          │
│         ████             │
│_________________________ │
└──────────────────────────┘
```

### Game Over
```
┌──────────────────────────┐
│       Game Over!         │
│        (;_;)             │
│     Score:  7            │
│     Best:  12            │
│    Press to retry        │
└──────────────────────────┘
```

---

## Configuration

Edit `config.h` to change behaviour:

```cpp
// How long each message stays on screen (ms)
#define MESSAGE_DISPLAY_MS     4000

// Water reminder interval — default 30 min
#define WATER_REMINDER_MS     1800000

// Game speed — lower = faster
#define GAME_TICK_MS          80

// Obstacle speed (pixels per tick)
#define OBSTACLE_SPEED        3
```

---

## Face Reference

| Index | Expression | When Used |
|---|---|---|
| 0 | Happy `(^.^)` | General positive messages |
| 1 | Wink `(^_~)` | Playful messages |
| 2 | Love `(^3^)` | Encouraging messages |
| 3 | Sad `(;_;)` | Game over screen |
| 4 | Blush `(>///<)` | Shy/sweet messages |
| 5 | Excited `(*.*)`| Hype messages |

---

## Add Your Own Messages

Open `message_manager.cpp` and add entries to the `_messages[]` array:

```cpp
{ "Your text line 1", "Your text line 2", 0 },
//                                         ^ face index (0–5)
```

Keep each line under ~14 characters for best display.

---

## Troubleshooting

**Display is blank**
- Check SDA→D2 and SCL→D1 wiring
- Verify 3.3V power (not 5V!) to OLED
- Confirm I2C address is `0x3C` in `config.h`

**Button not responding**
- GPIO0 is active LOW with internal pull-up — button should connect GPIO0 to GND
- Open Serial Monitor at 115200 baud and look for `[BTN]` logs

**Game feels too fast/slow**
- Increase `GAME_TICK_MS` in `config.h` to slow down
- Decrease `OBSTACLE_SPEED` for easier gameplay

---

## License

MIT License — do whatever you want with it.

---

## Credits

Built with ❤️ using ESP8266 Arduino core + Adafruit SSD1306.
