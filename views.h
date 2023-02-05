#pragma once
#include "functions.h"

void drawView(SDL_Surface* screen, SDL_Surface* player, SDL_Surface* charset, int plansza[MAP_HEIGHT][MAP_WIDTH], int fps, struct game* game, struct colors* colors, int gameCounter);

void finishView(SDL_Surface* screen, SDL_Surface* charset, int score, struct colors* colors, struct coords* coords);

void saveLoadView(SDL_Surface* screen, SDL_Surface* charset, SDL_Surface* saveNload, struct colors* colors, struct toFile* toFile, struct coords* coords, SDL_Event* event);

void rankingView(SDL_Surface* screen, SDL_Surface* ranking, SDL_Surface* charset, struct colors* colors, int* sort, double* lista, int sizeOfRanking, int* scroll);
