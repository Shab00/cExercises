# Blasteroids

A small, personal skeleton of the "Blasteroids" game from Head First C.
This repo contains a minimal Allegro5-based game loop, a spaceship you can
move and fire simple blasts, and the starting pieces to implement asteroids.

Build (Linux with pkg-config and Allegro5):
```
gcc -o blasteroids src/main.c -Iinclude `pkg-config --cflags allegro-5 allegro_primitives-5 allegro_font-5 allegro_ttf-5` `pkg-config --libs allegro-5 allegro_primitives-5 allegro_font-5 allegro_ttf-5`
```

Run:
```
./blasteroids
```

Controls:
- Left / Right: rotate ship
- Up / Down: accelerate / decelerate
- Space: fire
- Esc: quit

Notes:
- This is a learning skeleton. Asteroids, collision detection, scoring and lives
  are left as exercises to implement incrementally.
