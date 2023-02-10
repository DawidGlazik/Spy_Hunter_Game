#pragma once
#include "functions.h"

void startView(struct surfaces* surfaces, struct colors* colors, struct coords* coords);

void controlsView(struct surfaces* surfaces, struct colors* colors);

void drawView(struct surfaces *surfaces, int plansza[MAP_HEIGHT][MAP_WIDTH], int fps, int delay, struct game* game, struct colors* colors, struct powerup *power, struct bullet *bullet, struct enemy* enemy, struct civilian* civilian);

void finishView(struct surfaces* surfaces, int score, struct colors* colors, struct coords* coords);

void saveLoadView(struct surfaces* surfaces, struct colors* colors, struct toFile* toFile, struct coords* coords, SDL_Event* event, int option);

void rankingView(struct surfaces* surfaces, struct colors* colors, int* sort, double* lista, int sizeOfRanking, int* scroll);
