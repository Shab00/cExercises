#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

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

int main(void)
{
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

    Asteroid ast;
    ast.sx = 100.0f;
    ast.sy = 120.0f;
    ast.heading = 45.0f;
    ast.twist = 0.0f;
    ast.speed = 60.0f;
    ast.rot_velocity = 45.0f;
    ast.scale = 1.0f;
    ast.gone = 0;
    ast.color = al_map_rgb(200, 200, 200);

    bool keys[ALLEGRO_KEY_MAX];
    memset(keys, 0, sizeof(keys));

    bool running = true;
    bool redraw = true;
    al_start_timer(timer);
    double last_time = al_get_time();

    while (running) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(queue, &ev);

        if (ev.type == ALLEGRO_EVENT_TIMER) {
            double now = al_get_time();
            float dt = (float)(now - last_time);
            last_time = now;

            const float SHIP_ACCEL = 200.0f;
            const float SHIP_MAX_SPEED = 220.0f;
            const float SHIP_ROT_SPEED = 180.0f;
            const float SHIP_FRICTION = 100.0f;
            const float SHIP_BRAKE = 300.0f;

            if (keys[ALLEGRO_KEY_LEFT]) {
                ship.heading -= SHIP_ROT_SPEED * dt;
            }
            if (keys[ALLEGRO_KEY_RIGHT]) {
                ship.heading += SHIP_ROT_SPEED * dt;
            }

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

            float rad = (ship.heading - 90.0f) * (3.14159265358979323846f / 180.0f);
            float vx = cosf(rad) * ship.speed;
            float vy = sinf(rad) * ship.speed;
            ship.sx += vx * dt;
            ship.sy += vy * dt;

            if (ship.sx < 0) ship.sx += SCREEN_W;
            if (ship.sx >= SCREEN_W) ship.sx -= SCREEN_W;
            if (ship.sy < 0) ship.sy += SCREEN_H;
            if (ship.sy >= SCREEN_H) ship.sy -= SCREEN_H;

            update_asteroid(&ast, dt, SCREEN_W, SCREEN_H);

            redraw = true;
        } else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        } else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            keys[ev.keyboard.keycode] = true;
            if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) running = false;
        } else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
            keys[ev.keyboard.keycode] = false;
        }

        if (redraw && al_is_event_queue_empty(queue)) {
            redraw = false;

            if (bg) {
                al_draw_scaled_bitmap(bg,
                                      0, 0,
                                      al_get_bitmap_width(bg),
                                      al_get_bitmap_height(bg),
                                      0, 0,
                                      SCREEN_W, SCREEN_H,
                                      0);
            } else {
                al_clear_to_color(al_map_rgb(0,0,0));
            }

            draw_asteroid(&ast);

            if (ship_bmp) {
                float cx = al_get_bitmap_width(ship_bmp) / 2.0f;
                float cy = al_get_bitmap_height(ship_bmp) / 2.0f;
                al_draw_rotated_bitmap(ship_bmp, cx, cy, ship.sx, ship.sy, deg_to_rad(ship.heading), 0);
            } else {
                ALLEGRO_TRANSFORM t;
                al_identity_transform(&t);
                al_rotate_transform(&t, ship.heading * (3.14159265358979323846f/180.0f));
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

            al_draw_textf(font, al_map_rgb(255,255,255), 10, 10, 0, "Ship speed: %.1f", ship.speed);

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
