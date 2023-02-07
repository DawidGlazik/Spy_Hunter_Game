#pragma once
#include "functions.h"

void driveEvents(SDL_Event* event, struct game* game, struct switches* switches, struct bullet *bullet, bool state[5], double* penalty);

void saveNloadEvents(SDL_Event* event, struct switches* switches, struct coords* coords, struct toFile* toFile, struct game* game);

void rankingEvents(SDL_Event* event, struct switches* switches, struct coords* coords, int* sort, int* scroll, int sizeOfRanking);

void finishEvents(SDL_Surface* screen, SDL_Event* event, struct switches* switches, struct coords* coords, struct game* game, double* lista, int* sizeOfRanking);
