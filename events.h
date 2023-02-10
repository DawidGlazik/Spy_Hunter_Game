#pragma once
#include "functions.h"

void starterEvents(SDL_Surface* screen, SDL_Event* event, struct switches* switches, struct coords* coords);

void controlsEvents(SDL_Event* event, struct switches* switches, struct coords* coords);

void driveEvents(SDL_Event* event, struct game* game, struct switches* switches, struct bullet *bullet, bool state[6], struct enemy* enemy, struct civilian* civilian);

void saveNloadEvents(SDL_Event* event, struct switches* switches, struct coords* coords, struct toFile* toFile, struct game* game, struct enemy* enemy, struct civilian* civilian);

void rankingEvents(SDL_Event* event, struct switches* switches, struct coords* coords, int* sort, int* scroll, int sizeOfRanking);

void finishEvents(SDL_Surface* screen, SDL_Event* event, struct switches* switches, struct coords* coords, struct game* game, double* lista, int* sizeOfRanking);
