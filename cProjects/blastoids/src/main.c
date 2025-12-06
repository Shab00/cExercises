#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>

#include "game.h"

void draw_asteroid(const Asteroid *a);
void update_asteroid(Asteroid *a, float dt, int screen_w, int screen_h);

#define SCREEN_W 800
#define SCREEN_H 600
#define FPS 60.0f

#define BLAST_MAX 24
#define BLAST_SPEED 420.0f
#define BLAST_LIFETIME 1.5f
#define BLAST_RADIUS 3.0f

#define MAX_ASTEROIDS 6

static void spawn_blast(Blast blasts[], float blast_life[], const Spaceship *ship)
{
    int slot = -1;
    for (int i = 0; i < BLAST_MAX; ++i) {
        if (blasts[i].gone) { slot = i; break; }
    }
    if (slot < 0) return;

    float rad = deg_to_rad(ship->heading - 90.0f);
    const float forward = 14.0f;
    blasts[slot].sx = ship->sx + cosf(rad) * forward;
    blasts[slot].sy = ship->sy + sinf(rad) * forward;
    blasts[slot].heading = ship->heading;
    blasts[slot].speed = BLAST_SPEED;
    blasts[slot].gone = 0;
    blasts[slot].color = al_map_rgb(255, 200, 50);
    blast_life[slot] = BLAST_LIFETIME;
}

static void init_asteroids(Asteroid asteroids[], int count)
{
    for (int i = 0; i < count; ++i) {
        asteroids[i].sx = (float)(50 + rand() % (SCREEN_W - 100));
        asteroids[i].sy = (float)(50 + rand() % (SCREEN_H - 100));
        asteroids[i].heading = (float)(rand() % 360);
        asteroids[i].twist = (float)(rand() % 360);
        asteroids[i].speed = 30.0f + (rand() % 90);
        asteroids[i].rot_velocity = -60.0f + (rand() % 120);
        asteroids[i].scale = 0.8f + ((rand() % 50) / 100.0f);
        asteroids[i].gone = 0;
        asteroids[i].color = al_map_rgb(180 + rand()%75, 160 + rand()%80, 140 + rand()%90);
    }
}

int main(void)
{
    srand((unsigned)time(NULL));

    if (!al_init()) {
        fprintf(stderr, "al_init failed\n");
        return 1;
    }
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_keyboard();

    ALLEGRO_DISPLAY *display = al_create_display(SCREEN_W, SCREEN_H);
    if (!display) {
        fprintf(stderr, "al_create_display failed\n");
        return 1;
    }

    al_init_image_addon();

    ALLEGRO_BITMAP *bg = al_load_bitmap("spaceImage.jpg");
    if (!bg) {
        fprintf(stderr, "Warning: failed to load background 'spaceImage.jpg' — using solid color\n");
    }

    ALLEGRO_BITMAP *ship_bmp = al_load_bitmap("spaceShip.jpg");
    if (!ship_bmp) {
        fprintf(stderr, "Warning: failed to load ship image 'spaceShip.jpg' — using primitive ship\n");
    }

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    ALLEGRO_FONT *font = al_create_builtin_font();

    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));

    Spaceship ship = {
        .sx = SCREEN_W / 2.0f,
        .sy = SCREEN_H / 2.0f,
        .heading = 0.0f,
        .speed = 0.0f,
        .gone = 0,
    };
    ship.color = al_map_rgb(0, 255, 0);

    Asteroid asteroids[MAX_ASTEROIDS];
    init_asteroids(asteroids, MAX_ASTEROIDS);

    Blast blasts[BLAST_MAX];
    float blast_life[BLAST_MAX];
    for (int i = 0; i < BLAST_MAX; ++i) {
        blasts[i].gone = 1;
        blast_life[i] = 0.0f;
        blasts[i].color = al_map_rgb(255,200,50);
    }

    int score = 0;
    int ship_hits = 0;
    const int ship_max_hits = 3;
    float ship_invul_timer = 0.0f;

    bool keys[ALLEGRO_KEY_MAX];
    memset(keys, 0, sizeof(keys));

    bool running = true;
    bool redraw = true;
    bool game_over = false;

    al_start_timer(timer);
    double last_time = al_get_time();

    while (running) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(queue, &ev);

        if (ev.type == ALLEGRO_EVENT_TIMER) {
            double now = al_get_time();
            float dt = (float)(now - last_time);
            last_time = now;

            if (!game_over) {
                const float SHIP_ACCEL = 200.0f;
                const float SHIP_MAX_SPEED = 220.0f;
                const float SHIP_ROT_SPEED = 180.0f;
                const float SHIP_FRICTION = 100.0f;
                const float SHIP_BRAKE = 300.0f;

                if (ship_invul_timer > 0.0f) ship_invul_timer -= dt;

                if (keys[ALLEGRO_KEY_LEFT]) ship.heading -= SHIP_ROT_SPEED * dt;
                if (keys[ALLEGRO_KEY_RIGHT]) ship.heading += SHIP_ROT_SPEED * dt;

                if (keys[ALLEGRO_KEY_UP]) {
                    ship.speed += SHIP_ACCEL * dt;
                    if (ship.speed > SHIP_MAX_SPEED) ship.speed = SHIP_MAX_SPEED;
                } else {
                    ship.speed -= SHIP_FRICTION * dt;
                    if (ship.speed < 0.0f) ship.speed = 0.0f;
                }
                if (keys[ALLEGRO_KEY_DOWN]) {
                    ship.speed -= SHIP_BRAKE * dt;
                    if (ship.speed < 0.0f) ship.speed = 0.0f;
                }

                float rad = deg_to_rad(ship.heading - 90.0f);
                float vx = cosf(rad) * ship.speed;
                float vy = sinf(rad) * ship.speed;
                ship.sx += vx * dt;
                ship.sy += vy * dt;

                if (ship.sx < 0) ship.sx += SCREEN_W;
                if (ship.sx >= SCREEN_W) ship.sx -= SCREEN_W;
                if (ship.sy < 0) ship.sy += SCREEN_H;
                if (ship.sy >= SCREEN_H) ship.sy -= SCREEN_H;

                for (int i = 0; i < MAX_ASTEROIDS; ++i) {
                    if (!asteroids[i].gone) {
                        update_asteroid(&asteroids[i], dt, SCREEN_W, SCREEN_H);
                    }
                }

                for (int i = 0; i < BLAST_MAX; ++i) {
                    if (blasts[i].gone) continue;
                    float br = deg_to_rad(blasts[i].heading - 90.0f);
                    float bvx = cosf(br) * blasts[i].speed;
                    float bvy = sinf(br) * blasts[i].speed;
                    blasts[i].sx += bvx * dt;
                    blasts[i].sy += bvy * dt;

                    if (blasts[i].sx < 0) blasts[i].sx += SCREEN_W;
                    if (blasts[i].sx >= SCREEN_W) blasts[i].sx -= SCREEN_W;
                    if (blasts[i].sy < 0) blasts[i].sy += SCREEN_H;
                    if (blasts[i].sy >= SCREEN_H) blasts[i].sy -= SCREEN_H;

                    blast_life[i] -= dt;
                    if (blast_life[i] <= 0.0f) blasts[i].gone = 1;
                }

                for (int b = 0; b < BLAST_MAX; ++b) {
                    if (blasts[b].gone) continue;
                    for (int a = 0; a < MAX_ASTEROIDS; ++a) {
                        if (asteroids[a].gone) continue;
                        float dx = blasts[b].sx - asteroids[a].sx;
                        float dy = blasts[b].sy - asteroids[a].sy;
                        float dist2 = dx*dx + dy*dy;
                        float ar = 25.0f * asteroids[a].scale;
                        float hit_r = ar + BLAST_RADIUS;
                        if (dist2 <= hit_r * hit_r) {
                            asteroids[a].gone = 1;
                            blasts[b].gone = 1;
                            score += 100;
                            break;
                        }
                    }
                }

                if (ship_invul_timer <= 0.0f) {
                    const float ship_radius = 10.0f;
                    for (int a = 0; a < MAX_ASTEROIDS; ++a) {
                        if (asteroids[a].gone) continue;
                        float dx = ship.sx - asteroids[a].sx;
                        float dy = ship.sy - asteroids[a].sy;
                        float dist2 = dx*dx + dy*dy;
                        float ar = 25.0f * asteroids[a].scale;
                        float hit_r = ar + ship_radius;
                        if (dist2 <= hit_r * hit_r) {
                            ship_hits += 1;
                            ship_invul_timer = 2.0f;
                            ship.sx = SCREEN_W / 2.0f;
                            ship.sy = SCREEN_H / 2.0f;
                            ship.speed = 0.0f;
                            asteroids[a].gone = 1;
                            break;
                        }
                    }
                }

                if (ship_hits >= ship_max_hits) {
                    game_over = true;
                }
            } 

            redraw = true;
        } else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        } else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            keys[ev.keyboard.keycode] = true;
            if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) running = false;
            if (!game_over && ev.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                spawn_blast(blasts, blast_life, &ship);
            }
            if (game_over && ev.keyboard.keycode == ALLEGRO_KEY_R) {
                ship_hits = 0;
                ship_invul_timer = 0.0f;
                ship.sx = SCREEN_W / 2.0f;
                ship.sy = SCREEN_H / 2.0f;
                ship.speed = 0.0f;
                score = 0;
                init_asteroids(asteroids, MAX_ASTEROIDS);
                for (int i = 0; i < BLAST_MAX; ++i) blasts[i].gone = 1, blast_life[i] = 0.0f;
                game_over = false;
            }
        } else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
            keys[ev.keyboard.keycode] = false;
        }

        if (redraw && al_is_event_queue_empty(queue)) {
            redraw = false;

            if (bg) {
                al_draw_scaled_bitmap(bg, 0, 0, al_get_bitmap_width(bg), al_get_bitmap_height(bg),
                                      0, 0, SCREEN_W, SCREEN_H, 0);
            } else {
                al_clear_to_color(al_map_rgb(0,0,0));
            }

            for (int i = 0; i < MAX_ASTEROIDS; ++i) {
                if (!asteroids[i].gone) draw_asteroid(&asteroids[i]);
            }

            for (int i = 0; i < BLAST_MAX; ++i) {
                if (blasts[i].gone) continue;
                al_draw_filled_circle(blasts[i].sx, blasts[i].sy, BLAST_RADIUS, blasts[i].color);
            }

            if (ship_invul_timer > 0.0f) {
                if (fmod(ship_invul_timer, 0.25f) > 0.12f) {
                } else {
                    if (ship_bmp) {
                        float cx = al_get_bitmap_width(ship_bmp) / 2.0f;
                        float cy = al_get_bitmap_height(ship_bmp) / 2.0f;
                        al_draw_rotated_bitmap(ship_bmp, cx, cy, ship.sx, ship.sy, deg_to_rad(ship.heading), 0);
                    } else {
                        ALLEGRO_TRANSFORM t;
                        al_identity_transform(&t);
                        al_rotate_transform(&t, deg_to_rad(ship.heading));
                        al_translate_transform(&t, ship.sx, ship.sy);
                        al_use_transform(&t);

                        al_draw_line(-8, 9, 0, -11, ship.color, 3.0f);
                        al_draw_line(0, -11, 8, 9, ship.color, 3.0f);
                        al_draw_line(-6, 4, -1, 4, ship.color, 3.0f);
                        al_draw_line(6, 4, 1, 4, ship.color, 3.0f);

                        ALLEGRO_TRANSFORM id;
                        al_identity_transform(&id);
                        al_use_transform(&id);
                    }
                }
            } else {
                if (ship_bmp) {
                    float cx = al_get_bitmap_width(ship_bmp) / 2.0f;
                    float cy = al_get_bitmap_height(ship_bmp) / 2.0f;
                    al_draw_rotated_bitmap(ship_bmp, cx, cy, ship.sx, ship.sy, deg_to_rad(ship.heading), 0);
                } else {
                    ALLEGRO_TRANSFORM t;
                    al_identity_transform(&t);
                    al_rotate_transform(&t, deg_to_rad(ship.heading));
                    al_translate_transform(&t, ship.sx, ship.sy);
                    al_use_transform(&t);

                    al_draw_line(-8, 9, 0, -11, ship.color, 3.0f);
                    al_draw_line(0, -11, 8, 9, ship.color, 3.0f);
                    al_draw_line(-6, 4, -1, 4, ship.color, 3.0f);
                    al_draw_line(6, 4, 1, 4, ship.color, 3.0f);

                    ALLEGRO_TRANSFORM id;
                    al_identity_transform(&id);
                    al_use_transform(&id);
                }
            }

            al_draw_textf(font, al_map_rgb(255,255,255), 10, 10, 0, "Score: %d", score);
            al_draw_textf(font, al_map_rgb(255,255,255), 10, 26, 0, "Hits: %d/%d", ship_hits, ship_max_hits);
            int free_slots = 0;
            for (int i = 0; i < BLAST_MAX; ++i) if (blasts[i].gone) ++free_slots;
            al_draw_textf(font, al_map_rgb(255,255,255), 10, 42, 0, "Blasts free: %d/%d (Space to fire)", free_slots, BLAST_MAX);

            if (game_over) {
                al_draw_textf(font, al_map_rgb(255,0,0), SCREEN_W/2 - 80, SCREEN_H/2 - 10, 0, "GAME OVER");
                al_draw_textf(font, al_map_rgb(255,255,255), SCREEN_W/2 - 140, SCREEN_H/2 + 10, 0, "Press R to restart or Esc to quit");
            }

            al_flip_display();
        }
    }

    if (bg) al_destroy_bitmap(bg);
    if (ship_bmp) al_destroy_bitmap(ship_bmp);
    al_shutdown_image_addon();

    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    al_destroy_display(display);
    return 0;
}
