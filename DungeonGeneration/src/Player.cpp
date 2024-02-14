#include "Grid.h"
#include "Player.h"

Player::Player(int startX, int startY, Grid* grid) : x(startX), y(startY), gridRef(grid) {}

 void Player::moveUp() {
     if (y > 0 && (gridRef->IsValidPosition(x, y - 1)) && 
         (gridRef->GetTileTypeAt(x, y - 1) == TileType::Ground || gridRef->GetTileTypeAt(x, y - 1) == TileType::Pickup)) {
         gridRef->UpdatePlayerPosition(x, y, x, y - 1);
         y--;
         }
 }

 void Player::moveDown() {
     if (y < gridRef->GetHeight() - 1 && (gridRef->IsValidPosition(x, y + 1)) && 
         (gridRef->GetTileTypeAt(x, y + 1) == TileType::Ground || gridRef->GetTileTypeAt(x, y + 1) == TileType::Pickup)) {
         gridRef->UpdatePlayerPosition(x, y, x, y + 1);
         y++;
         }
 }

 void Player::moveLeft() {
     if (x > 0 && (gridRef->IsValidPosition(x - 1, y)) && 
         (gridRef->GetTileTypeAt(x - 1, y) == TileType::Ground || gridRef->GetTileTypeAt(x - 1, y) == TileType::Pickup)) {
         gridRef->UpdatePlayerPosition(x, y, x - 1, y);
         x--;
         }
 }

 void Player::moveRight() {
     if (x < gridRef->GetWidth() - 1 && (gridRef->IsValidPosition(x + 1, y)) && 
         (gridRef->GetTileTypeAt(x + 1, y) == TileType::Ground || gridRef->GetTileTypeAt(x + 1, y) == TileType::Pickup)) {
         gridRef->UpdatePlayerPosition(x, y, x + 1, y);
         x++;
         }
}

