#include "common.h"

#include "ocdlibs.h"
#include "raylib.h"

int main(int argc, char** argv) {
    (void) argc; (void) argv;

    lua_State* L = luaL_newstate();
    ocd_openlibs(L);
    int err = luaL_dofile(L, "script.lua");

    if (err) {
        printf("\x1b[31mFATAL ERROR:\x1b[39m\n%s\n",lua_tolstring(L, -1, NULL));
        lua_pop(L, 1);
    }

    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}