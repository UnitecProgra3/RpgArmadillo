
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<iostream>
#include<map>
#include<vector>

using namespace std;

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event Event;
SDL_Texture *background, *texture_npc, *texture_tile;
SDL_Rect rect_background, rect_character, rect_npc, rect_tile, rect_tileset;

bool collision(SDL_Rect r1, SDL_Rect r2)
{
    if(r1.x+r1.w < r2.x)
        return false;
    if(r1.x > r2.x+r2.w)
        return false;
    if(r1.y+r1.h<r2.y)
        return false;
    if(r1.y > r2.y+r2.h)
        return false;

    return true;
}

int main( int argc, char* args[] )
{
    //Init SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        return 10;
    }
    //Creates a SDL Window
    if((window = SDL_CreateWindow("Image Loading", 100, 100, 500/*WIDTH*/, 250/*HEIGHT*/, SDL_WINDOW_RESIZABLE | SDL_RENDERER_PRESENTVSYNC)) == NULL)
    {
        return 20;
    }
    //SDL Renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED );
    if (renderer == NULL)
    {
        std::cout << SDL_GetError() << std::endl;
        return 30;
    }

    //Init textures
    int w=0,h=0;
    background = IMG_LoadTexture(renderer,"fondo.png");
    SDL_QueryTexture(background, NULL, NULL, &w, &h);
    rect_background.x = 0; rect_background.y = 0; rect_background.w = w; rect_background.h = h;

    char orientation = 'd';// d u l r
    int current_sprite = 0;
    int animation_velocity = 20;
    int velocity = 3;
    int frame = 0;
    map<char,vector<SDL_Texture*> >sprites;
    sprites['u'].push_back(IMG_LoadTexture(renderer, "personaje/up1.png"));
    sprites['u'].push_back(IMG_LoadTexture(renderer, "personaje/up2.png"));
    sprites['d'].push_back(IMG_LoadTexture(renderer, "personaje/down1.png"));
    sprites['d'].push_back(IMG_LoadTexture(renderer, "personaje/down2.png"));
    sprites['l'].push_back(IMG_LoadTexture(renderer, "personaje/left1.png"));
    sprites['l'].push_back(IMG_LoadTexture(renderer, "personaje/left2.png"));
    sprites['r'].push_back(IMG_LoadTexture(renderer, "personaje/right1.png"));
    sprites['r'].push_back(IMG_LoadTexture(renderer, "personaje/right2.png"));

    SDL_QueryTexture(sprites['u'][0], NULL, NULL, &w, &h);
    rect_character.x = 0;
    rect_character.y = 100;
    rect_character.w = w;
    rect_character.h = h;

    texture_npc = IMG_LoadTexture(renderer,"npc.png");
    SDL_QueryTexture(texture_npc, NULL, NULL, &w, &h);
    rect_npc.x = 300;
    rect_npc.y = 100;
    rect_npc.w = w;
    rect_npc.h = h;

    texture_tile = IMG_LoadTexture(renderer,"tile/crypt.png");
    rect_tile.x = 32*15;
    rect_tile.y = 32*13;
    rect_tile.w = 32;
    rect_tile.h = 32;

    SDL_QueryTexture(texture_npc, NULL, NULL, &w, &h);
    rect_tileset.x = 0;
    rect_tileset.y = 0;
    rect_tileset.w = w;
    rect_tileset.h = h;

    int mapa[5][5];

    //Main Loop
    while(true)
    {
        while(SDL_PollEvent(&Event))
        {
            if(Event.type == SDL_QUIT)
            {
                return 0;
            }
        }

        const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );

        if(currentKeyStates[ SDL_SCANCODE_D ])
        {
            rect_character.x+=velocity;
            if(collision(rect_character,rect_npc))
            {
                rect_character.x-=velocity;
            }
            orientation='r';
        }
        if(currentKeyStates[ SDL_SCANCODE_A ])
        {
            rect_character.x-=velocity;
            if(collision(rect_character,rect_npc))
            {
                rect_character.x+=velocity;
            }
            orientation='l';
        }
        if(currentKeyStates[ SDL_SCANCODE_S ])
        {
            rect_character.y+=velocity;
            if(collision(rect_character,rect_npc))
            {
                rect_character.y-=velocity;
            }
            orientation='d';
        }
        if(currentKeyStates[ SDL_SCANCODE_W ])
        {
            rect_character.y-=velocity;
            if(collision(rect_character,rect_npc))
            {
                rect_character.y+=velocity;
            }
            orientation='u';
        }
        if(currentKeyStates[ SDL_SCANCODE_LSHIFT ])
        {
            velocity=6;
            animation_velocity=10;
        }else
        {
            velocity=3;
            animation_velocity=20;
        }

        if(frame%animation_velocity==0)
        {
            current_sprite++;
            if(current_sprite>1)
                current_sprite=0;
        }

        SDL_Delay(17);

        SDL_RenderCopy(renderer, background, NULL, &rect_background);
        SDL_RenderCopy(renderer, texture_tile, &rect_tile, &rect_tileset);
        SDL_RenderCopy(renderer, sprites[orientation][current_sprite], NULL, &rect_character);
        SDL_RenderCopy(renderer, texture_npc, NULL, &rect_npc);
        SDL_RenderPresent(renderer);
        frame++;
    }

	return 0;
}
