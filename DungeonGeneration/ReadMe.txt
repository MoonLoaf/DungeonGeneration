
This is a simple "game" primarily featuring procedural dungeon generation. It's made with C++ and SDL2 and a modified
version of this tileset:

https://blueapollo.itch.io/oracle-1bit-assets

Dungeon generation is based on creating rooms with random coordinates in a grid then connecting them by iterating through a vector of room instances.
This results in paths that sometimes go straight through other rooms, which in turn opens up for the possibility of
rooms having several connections to each other, which provides visually interesting and somewhat chaotic dungeons layouts
that vary widely.

The gameplay is simple WASD movement. Get to the key with the color corresponding to the dungeon exit.

Make sure not to walk on enemies, this resets the player to the dungeon start.

The exit will only work once the player has walked over the correct kind of key.

Currently the game exits once you exit the dungeon, I've played around with a version where once you exit the current dungeon,
you would be placed in a new one. But since this was a bit of a late implementation I realized I'd have to rewrite a lot
of code to properly clean up the previous Grid (Dungeon) instance to avoid memory leaks.

Known bugs:

- Tiles that cant be walked on block corridors/doors, rarely happens