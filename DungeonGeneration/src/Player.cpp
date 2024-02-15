#include "Grid.h"
#include "Player.h"
#include <SDL.h>


Player::Player(int startX, int startY, Grid* grid) : x(startX), y(startY), gridRef(grid), StartPos({startX, startY}) {}

void Player::Move(const vector2& direction) {
    int newX = x + direction.x;
    int newY = y + direction.y;

    if (gridRef->IsValidPosition(newX, newY)) {
        const TileType tileType = gridRef->GetTileTypeAt(newX, newY);
        
        switch (tileType)
        {
        case TileType::Ground:
            gridRef->UpdatePlayerPosition(x, y, newX, newY);
            x = newX;
            y = newY;
            break;
        case TileType::Pickup:
            gridRef->UpdatePlayerPosition(x, y, newX, newY);
            y = newY;
            x = newX;
            break;
        case TileType::RedKey:
            gridRef->UpdatePlayerPosition(x, y, newX, newY);
            y = newY;
            x = newX;
            if(gridRef->GetDungeonExitColor() == ExitColor::RedExit) 
            {
                gridRef->UnlockExit();
            }
            break;
        case TileType::BlueKey:
            gridRef->UpdatePlayerPosition(x, y, newX, newY);
            y = newY;
            x = newX;
            if(gridRef->GetDungeonExitColor() == ExitColor::BlueExit) 
            {
                gridRef->UnlockExit();
            }
            break;
        case TileType::YellowKey:
            gridRef->UpdatePlayerPosition(x, y, newX, newY);
            y = newY;
            x = newX;
            if(gridRef->GetDungeonExitColor() == ExitColor::YellowExit) 
            {
                gridRef->UnlockExit();
            }
            break;
        case TileType::Boss:
            gridRef->UpdatePlayerPosition(x, y, StartPos.x, StartPos.y);
            x = StartPos.x;
            y = StartPos.y;
            break;
        case TileType::DoorExitOpen:
                SDL_Quit();
            break;
        case TileType::None:
            return;
        case TileType::Wall:
            return;
        }
    }
}

vector2 Player::GetStartPos() const
{
    return StartPos;
}

