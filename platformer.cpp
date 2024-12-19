#include "raylib.h"

#include "globals.h"
#include "level.h"
#include "player.h"
#include "graphics.h"
#include "assets.h"
#include "utilities.h"


void update_game() {
    game_frame++;

    switch(game_state) {
        case MENU_STATE:
            if (IsKeyDown(KEY_ENTER)){
                ClearBackground(WHITE);
                game_state = GAME_STATE;
            }
            break;
        case GAME_STATE:
            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
                move_player_horizontally(MOVEMENT_SPEED);
            }

            if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
                move_player_horizontally(-MOVEMENT_SPEED);
            }

            // Calculating collisions to decide whether the player is allowed to jump: don't want them to suction cup to the ceiling or jump midair
            is_player_on_ground = is_colliding({player_pos.x, player_pos.y + 0.1f}, WALL);
            if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W) || IsKeyDown(KEY_SPACE)) && is_player_on_ground) {
                player_y_velocity = -JUMP_STRENGTH;
            }

            if (is_colliding(player_pos, EXIT)) {
                game_state = VICTORY_STATE;
            }

            update_player();
            break;

        case PAUSE_STATE:
            if (IsKeyPressed(KEY_ESCAPE)) {
            draw_pause_menu();
            }
                break;
        case VICTORY_STATE:
            if (IsKeyDown(KEY_ENTER)) {
                game_state = MENU_STATE;
            }
            break;
    }
}

void draw_game() {
    switch (game_state){
        case MENU_STATE:
            draw_menu();
            break;
        case GAME_STATE:
            ClearBackground(BLACK);
            draw_level();
            draw_game_overlay();
            break;
        case PAUSE_STATE:
            ClearBackground(BLACK);
            draw_pause_menu();
            break;
        case VICTORY_STATE:
            ClearBackground(BLACK);
            draw_victory_menu_background();
            draw_victory_menu();
            break;
    }
}



int main() {
    InitWindow(1024, 480, "Platformer");
    SetTargetFPS(60);

    load_fonts();
    load_images();
    load_sounds();
    load_level();

    while (!WindowShouldClose()) {
        BeginDrawing();

        update_game();

        draw_game();

        EndDrawing();
        }

        unload_level();
        unload_sounds();
        unload_images();
        unload_fonts();

        CloseAudioDevice();
        CloseWindow();

        return 0;
    }