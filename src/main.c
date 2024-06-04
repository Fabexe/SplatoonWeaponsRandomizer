#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#define WEAPONS_NBS

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

void initSDL() {
    // Initialiser SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("Erreur de l'initialisation de SDL : %s\n", SDL_GetError());
        return 0;
    }

    // Initialiser SDL_mixer
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
    
    // Initialiser SDL_ttf
    if (TTF_Init() < 0) {
        printf("Erreur de l'initialisation de SDL_ttf : %s\n", TTF_GetError());
        SDL_Quit();
        return 0;
    }

    // Créer une fenêtre
    gWindow = SDL_CreateWindow("Splatoon 3 Weapon Randomizer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 500, 690, SDL_WINDOW_SHOWN);

    // Créer le renderer
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

    // Définition de la couleur de fond
    SDL_RenderClear(gRenderer);
    SDL_RenderPresent(gRenderer);

}

TTF_Font* gFontPolice(const char *chemin, int taille) {
    TTF_Font *police = TTF_OpenFont(chemin, taille);
    if (!police) {
        printf("Erreur de chargement de la police : %s\n", TTF_GetError());
    }
    return police;
}

void closeSDL() {
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

void menuLoop() {
    SDL_Event gEventEnd;
    int gVariableEnd = 0;

    // Charger le fond d'écran
    SDL_Surface* gSurfaceBackground = IMG_Load("sprites/background.jpg");
    SDL_Texture* gTextureBackground = SDL_CreateTextureFromSurface(gRenderer, gSurfaceBackground);
    if (gSurfaceBackground == NULL) {
        printf("Erreur du chargement de <background.jpg> : %s \n", IMG_GetError());
    }
    SDL_Rect gRectBackground;
    gRectBackground.x = 0;
    gRectBackground.y = 0;
    gRectBackground.w = 500;
    gRectBackground.h = 690;
    SDL_FreeSurface(gSurfaceBackground);

    // Charger le titre
    TTF_Font* gFontTitre = gFontPolice("fonts/SpFont.otf", 72);
    SDL_Surface* gSurfaceTitre = TTF_RenderText_Solid(gFontTitre, "Splatoon 3 Weapon Randomizer", (SDL_Color){255, 255, 255, 255});
    SDL_Texture* gTextureTitre = SDL_CreateTextureFromSurface(gRenderer, gSurfaceTitre);
    SDL_Rect gRectTitre;
    gRectTitre.x = 20;
    gRectTitre.y = 10;
    gRectTitre.w = 460;
    gRectTitre.h = 30;
    SDL_FreeSurface(gSurfaceTitre);

    // Chargement des images
    SDL_Texture* gTextureImages[WEAPONS_NBS];
    for (int i = 0; i < WEAPONS_NBS; ++i) {
        char filename[133];
        sprintf(filename, "weapon%d.png", i+1);
        gTextureImages[i] = IMG_LoadTexture(gRenderer, filename);
        if (gTextureImages[i] == NULL) {
            printf("Failed to load image %d! SDL_Error: %s\n", i, SDL_GetError());
            return 1;
        }
    }

    // Chargement des sons
    Mix_Music* gMusicReload = Mix_LoadMUS("sounds/select.mp3");

    while (!gVariableEnd) {
        while (SDL_PollEvent(&gEventEnd) != 0) {
            if (gEventEnd.type == SDL_QUIT) {
                gVariableEnd = 1;
            }

        // Afficher le rendu
        SDL_RenderCopy(gRenderer, gTextureBackground, NULL, &gRectBackground);
        SDL_RenderCopy(gRenderer, gTextureTitre, NULL, &gRectTitre);

        // Mettre à jour l'affichage
        SDL_RenderPresent(gRenderer);

        // Ajouter une légère pause pour contrôler la vitesse du jeu
        SDL_Delay(16);

        }
    }
}

int main(int argc, char* args[]) {
    // Initialiser SDL
    initSDL();

    // Lancer le menu du jeu
    menuLoop();

    // Fermer SDL
    closeSDL();

    return 0;
}