#include "game_manager.h"

GameManager::GameManager()
  : _birdY(24), _birdVelocity(0),
    _obstacleX(128), _obstacleGapY(16),
    _score(0), _highScore(0), _alive(true) {}

void GameManager::reset() {
  _birdY        = 24;
  _birdVelocity = 0;
  _obstacleX    = 128;
  _score        = 0;
  _alive        = true;
  _randomizeObstacle();
  Serial.println("[GAME] Reset");
}

void GameManager::jump() {
  if (!_alive) return;
  _birdVelocity = -GAME_JUMP_STRENGTH;
  Serial.println("[GAME] Jump");
}

void GameManager::update() {
  if (!_alive) return;

  // Apply gravity
  _birdVelocity += GAME_GRAVITY;
  _birdY        += _birdVelocity;

  // Floor / ceiling collision
  if (_birdY >= 49) { // ground line at y=55, bird is 6px tall
    _birdY = 49;
    _alive = false;
  }
  if (_birdY < 0) {
    _birdY = 0;
    _birdVelocity = 0;
  }

  // Move obstacle
  _obstacleX -= OBSTACLE_SPEED;

  // Passed the obstacle — score
  if (_obstacleX == 12) {
    _score++;
    Serial.print("[GAME] Score: ");
    Serial.println(_score);
  }

  // Off screen — reset obstacle
  if (_obstacleX < -12) {
    _randomizeObstacle();
  }

  // Collision detection
  // Bird is rect: x=12, y=_birdY, w=8, h=6
  // Obstacle: x=_obstacleX, w=8
  //   Top pillar:    y=0,             h=_obstacleGapY
  //   Bottom pillar: y=_obstacleGapY+16, h=to ground
  bool xOverlap = (_obstacleX < 20 && _obstacleX + 8 > 12);
  if (xOverlap) {
    bool hitTop    = (_birdY < _obstacleGapY);
    bool hitBottom = (_birdY + 6 > _obstacleGapY + 16);
    if (hitTop || hitBottom) {
      _alive = false;
      if (_score > _highScore) _highScore = _score;
      Serial.println("[GAME] Collision!");
    }
  }
}

bool GameManager::isAlive()    { return _alive; }
bool GameManager::isGameOver() { return !_alive; }
int  GameManager::getBirdY()   { return _birdY; }
int  GameManager::getObstacleX()  { return _obstacleX; }
int  GameManager::getObstacleGap(){ return _obstacleGapY; }
int  GameManager::getScore()      { return _score; }
int  GameManager::getHighScore()  { return _highScore; }

void GameManager::_randomizeObstacle() {
  _obstacleX    = OLED_WIDTH + 10;
  // Gap Y: keep between 6 and 33 so there's always room top and bottom
  _obstacleGapY = 6 + (millis() % 28);
}
