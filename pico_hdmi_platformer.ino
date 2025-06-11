#include <PicoDVI.h>

#include "Level.h"
#include "Tiles.h"

// Display config
DVIGFX16 display(DVI_RES_320x240p60, picodvi_dvi_cfg);

// Game constants
const int PIXEL_SIZE = 10;
const int GRID_WIDTH = 50;
const int GRID_HEIGHT = 10;

const int X_OFFSET = 60 * 0;
const int Y_OFFSET = 30 * 0;

const int VISIBLE_WIDTH = 16 * 2;
const int VISIBLE_HEIGHT = 12 * 2;

const double GRAVITY = 1.0;
const int JUMP_FORCE = -4;

int cameraX = 0;
int cameraY = 0;


enum TileType {
  EMPTY = 0,
  PLAYER = 1,
  GOAL = 2,
  OBSTACLE = 3,
  PLATFORM = 4,
  ENEMY = 5
};

TileType lastDrawn[VISIBLE_HEIGHT][VISIBLE_WIDTH];

// Player variables
int playerX = 2;
int playerY = 2;
int playerVelY = 0;
bool isJumping = false;
bool isOnGround = false;
uint8_t levelOrig[10][40] = {};
const int levY = 10;
const int levX = 40;
void setup() {
  for (int y = 0; y < levY; y++) {
    for (int x = 0; x < levX; x++) {
      levelOrig[y][x] = levelMap[y][x];
    }
  }

  memset(lastDrawn, -1, sizeof(lastDrawn));

  Serial.begin(115200);
  if (!display.begin()) {
    pinMode(LED_BUILTIN, OUTPUT);
    while (true) digitalWrite(LED_BUILTIN, (millis() / 500) & 1);
  }

  levelMap[playerY][playerX] = PLAYER;
  updateCamera();
  drawFullMap();
}

int prevPlayerX = playerX;
int prevPlayerY = playerY;

void loop() {
  handleInput();
  applyGravity();

  updateCamera();

  // Only redraw if player moved or camera changed
  if (playerX != prevPlayerX || playerY != prevPlayerY || cameraMoved()) {
    drawFullMap();

    prevPlayerX = playerX;
    prevPlayerY = playerY;
  }

  delay(30);
}

bool cameraMoved() {
  static int lastCameraX = -1;
  static int lastCameraY = -1;

  if (cameraX != lastCameraX || cameraY != lastCameraY) {
    lastCameraX = cameraX;
    lastCameraY = cameraY;
    return true;
  }
  return false;
}
void handleInput() {
  if (Serial.available()) {
    char c = tolower(Serial.read());
    int dx = 0;

    if (c == 'a') dx = -1;
    if (c == 'd') dx = 1;
    if (c == 'w' && isOnGround) {
      playerVelY = JUMP_FORCE;
      isJumping = true;
      isOnGround = false;
      Serial.println("JUMP!");
    }

    movePlayer(dx, 0);
  }
}

void applyGravity() {
  playerVelY += GRAVITY;
  if (playerVelY > 5) playerVelY = 5;

  if (isSolid(playerX, playerY + 1)) {
    if (playerVelY > 0) {
      playerVelY = 0;
      isOnGround = true;
      isJumping = false;
    }
  } else {
    isOnGround = false;
  }

  int dy = (playerVelY > 0) ? 1 : (playerVelY < 0 ? -1 : 0);
  movePlayer(0, dy);
}

bool isSolid(int x, int y) {
  return y >= GRID_HEIGHT || (x >= 0 && x < GRID_WIDTH &&
                              (levelMap[y][x] == OBSTACLE || levelMap[y][x] == PLATFORM));
}

void movePlayer(int dx, int dy) {
  int newX = playerX + dx;
  int newY = playerY + dy;

  // Prevent out-of-bounds
  if (newX < 0 || newX >= GRID_WIDTH || newY < 0 || newY >= GRID_HEIGHT) return;

  // Check for enemy collision
  if (levelMap[newY][newX] == ENEMY) {
    Serial.println("You were hit by an enemy!");
    restartGame();
    return;
  }

  // Clear old player tile before moving
  markForRedraw(playerX, playerY);
  levelMap[playerY][playerX] = EMPTY;

  // Move horizontally if no solid tile
  if (dx != 0 && !isSolid(newX, playerY)) {
    playerX = newX;
  }

  // Move vertically if no solid tile
  if (dy != 0 && !isSolid(playerX, newY)) {
    playerY = newY;
  }

  // Mark new position for redraw
  markForRedraw(playerX, playerY);

  // Update level map player position
  levelMap[playerY][playerX] = PLAYER;

  if (levelMap[playerY][playerX] == GOAL) {
    Serial.println("Goal reached!");
  }
}

void updateCamera() {
  cameraX = playerX - VISIBLE_WIDTH / 2;
  cameraY = playerY - VISIBLE_HEIGHT / 2;

  if (cameraX < 0) cameraX = 0;
  if (cameraY < 0) cameraY = 0;
  if (cameraX > GRID_WIDTH - VISIBLE_WIDTH) cameraX = GRID_WIDTH - VISIBLE_WIDTH;
  if (cameraY > GRID_HEIGHT - VISIBLE_HEIGHT) cameraY = GRID_HEIGHT - VISIBLE_HEIGHT;
}

void drawFullMap() {
  for (int y = 0; y < VISIBLE_HEIGHT; y++) {
    for (int x = 0; x < VISIBLE_WIDTH; x++) {
      int mapX = cameraX + x;
      int mapY = cameraY + y;
      TileType currentTile = EMPTY;

      if (mapX >= 0 && mapX < GRID_WIDTH && mapY >= 0 && mapY < GRID_HEIGHT) {
        currentTile = (TileType)levelMap[mapY][mapX];
      }

      if (lastDrawn[y][x] != currentTile) {
        drawTile(x, y, currentTile);
        lastDrawn[y][x] = currentTile;
      }
    }
  }
}

void markForRedraw(int mapX, int mapY) {
  int screenX = mapX - cameraX;
  int screenY = mapY - cameraY;
  if (screenX >= 0 && screenX < VISIBLE_WIDTH && screenY >= 0 && screenY < VISIBLE_HEIGHT) {
    lastDrawn[screenY][screenX] = (TileType) - 1;
  }
}

int score = 0;
int lives = 3;
void loadLevel() {
  score = 0;
  lives = 3;
  for (int y = 0; y < levY; y++) {
    for (int x = 0; x < levX; x++) {
      markForRedraw(x, y);
    }
  }
}

void restartGame() {
  Serial.println("Restarting game...");

  loadLevel();

  for (int y = 0; y < levY; y++) {
    for (int x = 0; x < levX; x++) {
      levelMap[y][x] = levelOrig[y][x];
    }
  }

  playerX = 2;
  playerY = 2;
  playerVelY = 0;
  isJumping = false;
  isOnGround = false;
  levelMap[playerY][playerX] = PLAYER;

  updateCamera();
  drawFullMap();
}

void drawTile(int x, int y, TileType tile) {
  clearTileArea(x, y);

  const uint16_t* tileBitmap = nullptr;
  int tileSize = PIXEL_SIZE;

  switch (tile) {
    case PLAYER:   tileBitmap = (const uint16_t*)tilePlayer; break;
    case GOAL:     tileBitmap = (const uint16_t*)tileGoal; break;
    case OBSTACLE: tileBitmap = (const uint16_t*)tileObstacle; break;
    case PLATFORM: tileBitmap = (const uint16_t*)tilePlatform; break;
    case ENEMY:    tileBitmap = (const uint16_t*)tileEnemy; break;
    default:       return;
  }

  if (tileBitmap) {
    display.drawRGBBitmap(
      x * PIXEL_SIZE + X_OFFSET,
      y * PIXEL_SIZE + Y_OFFSET,
      tileBitmap,
      tileSize,
      tileSize
    );
  }
}

#define BACKGROUND_COLOR 0xFFFF

void clearTileArea(int x, int y) {
  display.fillRect(
    x * PIXEL_SIZE + X_OFFSET,
    y * PIXEL_SIZE + Y_OFFSET,
    PIXEL_SIZE,
    PIXEL_SIZE,
    BACKGROUND_COLOR
  );
}
