#include "raylib.h"

#include "globals.h"
#include "level.h"
#include "player.h"
#include "enemy.h"
#include "graphics.h"
#include "assets.h"
#include "utilities.h"

///2 enemy, 3 more levels, explosion of the bomb, pause_menu, pictures on menu,
/// work on victory menu, add reverse movements, add keys for doors, add collecting hearts, add transportation,
/// logo for game, moving enemy, menu icon(start, exit, history), add conversation with cats, collect peace of book,
/// at the end display the book, in which there is a message ///




void update_menu_sound();
void update_game() {
    game_frame++;

    switch(game_state) {
        case MENU_STATE:
            update_menu_sound();

            if (IsKeyPressed(KEY_ENTER)){
                game_state = GAME_STATE;
                StopMusicStream(menu_music);
                PlayMusicStream(level_music);
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

            update_player();

            if (!is_music_playing){
                PlayMusicStream(level_music);
                is_music_playing = true;
                StopMusicStream(menu_music);
                StopMusicStream(victory_music);

            }

            if(IsKeyPressed(KEY_ESCAPE)){
                game_state = PAUSE_STATE;
            }

            break;

        case PAUSE_STATE:
            if (!is_music_playing){
                PlayMusicStream(gameover_music);
                is_music_playing = true;
                StopMusicStream(level_music);
                StopMusicStream(victory_music);
                StopMusicStream(menu_music);
            }

            if (IsKeyPressed(KEY_ENTER)) {
                game_state = GAME_STATE;
                StopMusicStream(gameover_music);
                PlayMusicStream(level_music);

            }
                break;
        case VICTORY_STATE:
            if (!is_music_playing){
                StopMusicStream(level_music);
                StopMusicStream(menu_music);
                PlayMusicStream(victory_music);
                is_music_playing = true;
            }

            if (IsKeyDown(KEY_ENTER)) {
                reset_game();
                StopMusicStream(victory_music);
            }

            break;

        case GAME_OVER_STATE:
            if (IsKeyPressed(KEY_SPACE)){
                reset_game();
                StopMusicStream(victory_music);
            }
    }

    UpdateMusicStream(menu_music);
    UpdateMusicStream(level_music);
    UpdateMusicStream(victory_music);
    UpdateMusicStream(gameover_music);
}

void draw_game() {
    switch (game_state){
        case MENU_STATE:
            ClearBackground(BLACK);
            draw_menu();
            break;
        case GAME_STATE:
            ClearBackground(BLACK);
            draw_level();
            draw_player();
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
        case GAME_OVER_STATE:
            ClearBackground(BLACK);
            draw_game_over_state();
    }
}


int main() {
    InitWindow(1024, 480, "Platformer");
    SetTargetFPS(60);

    SetExitKey(KEY_NULL);

    load_fonts();
    load_images(Texture2D());
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


void update_menu_sound() {

    if (!IsMusicStreamPlaying(menu_music)) {
        StopMusicStream(level_music);
        StopMusicStream(victory_music);
        StopMusicStream(gameover_music);

        PlayMusicStream(menu_music);
    }
}

