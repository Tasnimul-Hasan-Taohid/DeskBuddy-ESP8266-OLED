#pragma once

#include <Arduino.h>
#include "config.h"

class GameManager {
public:
  GameManager();

  void reset();
  void update();       // Call every GAME_TICK_MS
  void jump();

  bool isAlive();
  bool isGameOver();

  int getBirdY();
  int getObstacleX();
  int getObstacleGap();
  int getScore();
  int getHighScore();

private:
  int  _birdY;
  int  _birdVelocity;
  int  _obstacleX;
  int  _obstacleGapY;
  int  _score;
  int  _highScore;
  bool _alive;

  void _randomizeObstacle();
};
