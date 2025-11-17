#ifndef GAME_H
#define GAME_H

#include <allegro5/allegro.h>

typedef struct {
    float sx;
    float sy;
    float heading;
    float speed;
    int gone;
    ALLEGRO_COLOR color;
} Spaceship;

typedef struct {
    float sx;
    float sy;
    float heading;
    float speed;
    int gone;
    ALLEGRO_COLOR color;
} Blast;

typedef struct {
    float sx;
    float sy;
    float heading;
    float twist;
    float speed;
    float rot_velocity;
    float scale;
    int gone;
    ALLEGRO_COLOR color;
} Asteroid;

static inline float deg_to_rad(float deg) {
    return deg * (3.14159265358979323846f / 180.0f);
}

#endif
