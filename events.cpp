#include "events.h"

enum {
	SpeedUp,
	SlowDown,
	Left,
	Right,
	RoadSide
};

void driveEvents(SDL_Event* event, struct game* game, struct switches* switches, bool state[5], double* penalty, int *gameCounter) {
	while (SDL_PollEvent(event)) {
		switch (event->type) {
		case SDL_KEYDOWN:
			if (event->key.keysym.sym == SDLK_ESCAPE) switches->quit = true;
			else if (event->key.keysym.sym == SDLK_UP) state[SpeedUp] = true;
			else if (event->key.keysym.sym == SDLK_DOWN) state[SlowDown] = true;
			else if (event->key.keysym.sym == SDLK_LEFT) state[Left] = true;
			else if (event->key.keysym.sym == SDLK_RIGHT) state[Right] = true;
			else if (event->key.keysym.sym == SDLK_n) {
				game->score = 0;
				game->distance = 0;
				game->worldTime = 0;
				switches->finish = false;
				switches->pause = false;
				game->temp = 0.0;
				game->lives = 1000;
				*penalty = 0.0;
				startBoard(game->plansza);
			}
			else if (event->key.keysym.sym == SDLK_p) {
				if (!switches->finish) switches->pause = !switches->pause;
			}
			else if (event->key.keysym.sym == SDLK_f) {
				switches->pause = false;
				switches->finish = true;
				*gameCounter += 1;
			}
			else if (event->key.keysym.sym == SDLK_s) switches->save = true;
			else if (event->key.keysym.sym == SDLK_l) switches->load = true;
			else if (event->key.keysym.sym == SDLK_r) switches->ranking = true;
			break;
		case SDL_KEYUP:
			if (event->key.keysym.sym == SDLK_UP) state[SpeedUp] = false;
			else if (event->key.keysym.sym == SDLK_DOWN) state[SlowDown] = false;
			else if (event->key.keysym.sym == SDLK_LEFT) state[Left] = false;
			else if (event->key.keysym.sym == SDLK_RIGHT) state[Right] = false;
			break;
		case SDL_QUIT:
			switches->quit = true;
			break;
		}
	}
}

void saveNloadEvents(SDL_Event* event, struct switches* switches, struct coords* coords, struct toFile* toFile, struct game* game) {
	static char saving1[128];
	static char saving2[128];
	static char saving3[128];
	while (SDL_PollEvent(event)) {
		switch (event->type) {
		case SDL_KEYDOWN:
			if (event->key.keysym.sym == SDLK_ESCAPE) {
				switches->save = false;
				switches->load = false;
			}
		case SDL_MOUSEMOTION:
			SDL_GetMouseState(&coords->x, &coords->y);
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event->button.button == SDL_BUTTON_LEFT && (switches->save || switches->load)) {
				struct tm* cur_time;
				time_t czas;
				time(&czas);
				cur_time = localtime(&czas);
				if (coords->x > 170 && coords->x < 470 && coords->y>72 && coords->y < 172) {
					if (switches->load && toFile->slot1 != "Pusty1") loadFile(game, 1);
					else if (switches->save) {
						strftime(saving1, sizeof(saving1), "%x-%X", cur_time);
						toFile->slot1 = saving1;
						saveFile(game, 1, toFile);
					}
				}
				if (coords->x > 170 && coords->x < 470 && coords->y>208 && coords->y < 308) {
					if (switches->load && toFile->slot2 != "Pusty2") loadFile(game, 2);
					else if (switches->save) {
						strftime(saving2, sizeof(saving2), "%x-%X", cur_time);
						toFile->slot2 = saving2;
						saveFile(game, 2, toFile);
					}
				}
				if (coords->x > 170 && coords->x < 470 && coords->y>344 && coords->y < 444) {
					if (switches->load && toFile->slot3 != "Pusty3") loadFile(game, 3);
					else if (switches->save) {
						strftime(saving3, sizeof(saving3), "%x-%X", cur_time);
						toFile->slot3 = saving3;
						saveFile(game, 3, toFile);
					}
				}
				switches->save = false;
				switches->load = false;
			}
			break;
		case SDL_QUIT:
			switches->quit = true;
			break;
		};
	};
}

void rankingEvents(SDL_Event* event, struct switches* switches, struct coords* coords, int* sort, int* scroll, int sizeOfRanking) {
	while (SDL_PollEvent(event)) {
		switch (event->type) {
		case SDL_KEYDOWN:
			if (event->key.keysym.sym == SDLK_ESCAPE) switches->ranking = false;
			else if (event->key.keysym.sym == SDLK_p) *sort = 1;
			else if (event->key.keysym.sym == SDLK_t) *sort = 2;
		case SDL_MOUSEMOTION:
			SDL_GetMouseState(&coords->x, &coords->y);
			break;
		case SDL_MOUSEWHEEL:
			if (coords->x > 100 && coords->x < SCREEN_WIDTH - 100 && coords->y > 80 && coords->y < SCREEN_HEIGHT - 80) {
				if (*scroll <= sizeOfRanking - 11 && event->wheel.y < 0) *scroll += 1;
				else if (*scroll > 0 && event->wheel.y > 0) *scroll -= 1;
			}
			break;
		case SDL_QUIT:
			switches->quit = true;
			break;
		};
	};
}

void finishEvents(SDL_Surface* screen, SDL_Event* event, struct switches* switches, struct coords* coords, struct game* game, double* lista, int* sizeOfRanking) {
	while (SDL_PollEvent(event)) {
		switch (event->type) {
		case SDL_KEYDOWN:
			if (event->key.keysym.sym == SDLK_ESCAPE) switches->quit = true;
			else if (event->key.keysym.sym == SDLK_n) {
				game->score = 0;
				game->distance = 0;
				game->worldTime = 0;
				switches->finish = false;
				switches->pause = false;
				game->temp = 0.0;
				game->lives = 1000;
				startBoard(game->plansza);
			}
		case SDL_MOUSEMOTION:
			SDL_GetMouseState(&coords->x, &coords->y);
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event->button.button == SDL_BUTTON_LEFT) {
				if (coords->x > screen->w / 2 - 75 && coords->x < screen->w / 2 + 75 && coords->y > screen->h / 2 + 20 && coords->y < screen->h / 2 + 40) {
					*(lista + (*sizeOfRanking) * 2) = game->score;
					*(lista + (*sizeOfRanking) * 2 + 1) = game->worldTime;
					*sizeOfRanking += 1;
					switches->quit = true;
				}
				break;
		case SDL_QUIT:
			switches->quit = true;
			break;
			};
		};
	}
}