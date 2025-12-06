#ifndef GAME_H
#define GAME_H

#include <allegro5/allegro.h>

/* Spaceship structure */
typedef struct {
    float sx;
    float sy;
    float heading;     /* degrees */
    float speed;       /* pixels/sec */
    int gone;
    ALLEGRO_COLOR color;
} Spaceship;

/* Blast structure */
typedef struct {
    float sx;
    float sy;
    float heading; /* degrees */
    float speed;   /* pixels/sec */
    int gone;
    ALLEGRO_COLOR color;
} Blast;

/* Asteroid structure */
typedef struct {
    float sx;
    float sy;
    float heading;      /* travel heading (degrees) */
    float twist;        /* current visual rotation (degrees) */
    float speed;        /* pixels/sec */
    float rot_velocity; /* degrees/sec for twist */
    float scale;        /* 1.0 = base size */
    int gone;
    ALLEGRO_COLOR color;
} Asteroid;

/* small helper: degrees -> radians */
static inline float deg_to_rad(float deg) {
    return deg * (3.14159265358979323846f / 180.0f);
}

#endif /* GAME_H */
