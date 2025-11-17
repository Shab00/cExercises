#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "game.h"

#define BASE_RADIUS 20.0f

void draw_asteroid(const Asteroid *a)
{
    if (!a || a->gone) return;

    ALLEGRO_TRANSFORM t;
    al_identity_transform(&t);
    al_translate_transform(&t, a->sx, a->sy);
    al_rotate_transform(&t, deg_to_rad(a->twist));
    al_scale_transform(&t, a->scale, a->scale);
    al_use_transform(&t);

    ALLEGRO_COLOR col = a->color;
    al_draw_line(-20, 20, -25, 5, col, 2.0f);
    al_draw_line(-25, 5, -25, -10, col, 2.0f);
    al_draw_line(-25, -10, -5, -10, col, 2.0f);
    al_draw_line(-5, -10, -10, -20, col, 2.0f);
    al_draw_line(-10, -20, 5, -20, col, 2.0f);
    al_draw_line(5, -20, 20, -10, col, 2.0f);
    al_draw_line(20, -10, 20, -5, col, 2.0f);
    al_draw_line(20, -5, 0, 0, col, 2.0f);
    al_draw_line(0, 0, 20, 10, col, 2.0f);
    al_draw_line(20, 10, 10, 20, col, 2.0f);
    al_draw_line(10, 20, 0, 15, col, 2.0f);
    al_draw_line(0, 15, -20, 20, col, 2.0f);

    ALLEGRO_TRANSFORM id;
    al_identity_transform(&id);
    al_use_transform(&id);
}

void update_asteroid(Asteroid *a, float dt, int screen_w, int screen_h)
{
    if (!a || a->gone) return;

    a->twist += a->rot_velocity * dt;
    if (a->twist >= 360.0f) a->twist -= 360.0f;
    if (a->twist < 0.0f) a->twist += 360.0f;

    float rad = deg_to_rad(a->heading);
    float vx = cosf(rad) * a->speed;
    float vy = sinf(rad) * a->speed;
    a->sx += vx * dt;
    a->sy += vy * dt;

    if (a->sx < 0) a->sx += screen_w;
    if (a->sx >= screen_w) a->sx -= screen_w;
    if (a->sy < 0) a->sy += screen_h;
    if (a->sy >= screen_h) a->sy -= screen_h;
}
