#include "views.h"

enum {
	SpeedUp,
	SlowDown,
	Left,
	Right,
	RoadSide
};

void drawView(SDL_Surface* screen, SDL_Surface* player, SDL_Surface* charset, int plansza[MAP_HEIGHT][MAP_WIDTH], int fps, struct game* game, struct colors* colors, int gameCounter) {
	char text[128];
	SDL_FillRect(screen, NULL, colors->szary);
	for (int i = 0; i < MAP_HEIGHT; i++) {
		DrawRectangle(screen, 0, plansza[i][0] * i, plansza[i][1], SCREEN_HEIGHT / MAP_HEIGHT, colors->ciemny_zielony, colors->ciemny_zielony);
		DrawRectangle(screen, SCREEN_WIDTH - plansza[i][1], SCREEN_HEIGHT / MAP_HEIGHT * i, plansza[i][1], SCREEN_HEIGHT / MAP_HEIGHT, colors->ciemny_zielony, colors->ciemny_zielony);
	}
	DrawSurface(screen, player, SCREEN_WIDTH / 2 + game->posX, SCREEN_HEIGHT / 3 * 2);
	DrawRectangle(screen, 0, 0, SCREEN_WIDTH, 40, colors->czerwony, colors->niebieski);
	sprintf(text, "Dawid Glazik 193069, czas trwania = %.1lf s  %.0lf fps wynik: %d", game->worldTime, (float)fps, game->score);
	DrawString(screen, screen->w / 2 - strlen(text) * 4, 10, text, charset);
	sprintf(text, "r - ranking, Esc - wyjscie, \030 - przyspieszenie, \031 - zwolnienie");
	DrawString(screen, screen->w / 2 - strlen(text) * 4, 26, text, charset);
	sprintf(text, "abcdefgghimo");
	DrawString(screen, screen->w - strlen(text) * 8, screen->h - 10, text, charset);
	sprintf(text, "Licznik gier: %d", gameCounter);
	DrawString(screen, strlen(text), screen->h - 10, text, charset);
	if (game->lives == 2) DrawSurface(screen, player, 40, SCREEN_HEIGHT - 40);
}

void finishView(SDL_Surface* screen, SDL_Surface* charset, int score, struct colors* colors, struct coords* coords) {
	SDL_ShowCursor(1);
	char text[128];
	SDL_FillRect(screen, NULL, colors->szary);
	sprintf(text, "Koniec gry. Wynik: %d", score);
	DrawString(screen, screen->w / 2 - strlen(text) * 4, screen->h / 2, text, charset);
	if (coords->x > screen->w / 2 - 75 && coords->x < screen->w / 2 + 75 && coords->y > screen->h / 2 + 20 && coords->y < screen->h / 2 + 40) {
		DrawRectangle(screen, screen->w / 2 - 75, screen->h / 2 + 20, 150, 20, colors->czerwony, colors->jasny_niebieski);
	}
	else {
		DrawRectangle(screen, screen->w / 2 - 75, screen->h / 2 + 20, 150, 20, colors->czerwony, colors->niebieski);
	}
	sprintf(text, "Dodaj do rankingu");
	DrawString(screen, screen->w / 2 - 50 - strlen(text), screen->h / 2 + 26, text, charset);
}

void saveLoadView(SDL_Surface* screen, SDL_Surface* charset, SDL_Surface* saveNload, struct colors* colors, struct toFile* toFile, struct coords* coords, SDL_Event* event) {
	char text[128];
	SDL_ShowCursor(1);
	SDL_FillRect(screen, NULL, colors->szary);
	DrawSurface(screen, saveNload, SCREEN_WIDTH / 2, 36);
	if (coords->x > 170 && coords->x < 470 && coords->y>72 && coords->y < 172) {
		DrawRectangle(screen, SCREEN_WIDTH / 2 - 150, 72, 300, 100, colors->czerwony, colors->jasny_niebieski);
	}
	else {
		DrawRectangle(screen, SCREEN_WIDTH / 2 - 150, 72, 300, 100, colors->czerwony, colors->niebieski);
	}
	sprintf(text, toFile->slot1);
	DrawString(screen, screen->w / 2 - strlen(text) * 4, 122, text, charset);
	if (coords->x > 170 && coords->x < 470 && coords->y>208 && coords->y < 308) {
		DrawRectangle(screen, SCREEN_WIDTH / 2 - 150, 208, 300, 100, colors->czerwony, colors->jasny_niebieski);
	}
	else {
		DrawRectangle(screen, SCREEN_WIDTH / 2 - 150, 208, 300, 100, colors->czerwony, colors->niebieski);
	}
	sprintf(text, toFile->slot2);
	DrawString(screen, screen->w / 2 - strlen(text) * 4, 258, text, charset);

	if (coords->x > 170 && coords->x < 470 && coords->y>344 && coords->y < 444) {
		DrawRectangle(screen, SCREEN_WIDTH / 2 - 150, 344, 300, 100, colors->czerwony, colors->jasny_niebieski);
	}
	else {
		DrawRectangle(screen, SCREEN_WIDTH / 2 - 150, 344, 300, 100, colors->czerwony, colors->niebieski);
	}
	sprintf(text, toFile->slot3);
	DrawString(screen, screen->w / 2 - strlen(text) * 4, 394, text, charset);
}

void rankingView(SDL_Surface* screen, SDL_Surface* ranking, SDL_Surface* charset, struct colors* colors, int* sort, double* lista, int sizeOfRanking, int* scroll) {
	char text[128];
	int width = SCREEN_WIDTH - 200;
	int height = SCREEN_HEIGHT - 160;
	SDL_ShowCursor(1);
	SDL_FillRect(screen, NULL, colors->szary);
	DrawSurface(screen, ranking, SCREEN_WIDTH / 2, 40);
	DrawLine(screen, 100, 80, height, 0, 1, colors->ciemny_zielony);
	DrawLine(screen, 100, SCREEN_HEIGHT - 80, width, 1, 0, colors->ciemny_zielony);
	DrawLine(screen, SCREEN_WIDTH - 100, 80, height, 0, 1, colors->ciemny_zielony);
	for (int i = 0; i < 11; i++) {
		DrawLine(screen, 100, 80 + i * (height / 11), width, 1, 0, colors->ciemny_zielony);
	}
	DrawLine(screen, 100 + width / 10, 80, height, 0, 1, colors->ciemny_zielony);
	DrawLine(screen, 100 + width * 55 / 100, 80, height, 0, 1, colors->ciemny_zielony);

	sprintf(text, "Lp.");
	DrawString(screen, 125 - strlen(text) * 4, 93, text, charset);
	if (*sort == 1) sprintf(text, "Wynik \031");
	else sprintf(text, "Wynik");
	DrawString(screen, width * 55 / 100 - strlen(text) * 4, 93, text, charset);
	if (*sort == 2) sprintf(text, "Czas \031");
	else sprintf(text, "Czas");
	DrawString(screen, width - strlen(text) * 4, 93, text, charset);

	sortArray(lista, sizeOfRanking, *sort);
	int j = 1;
	for (int i = *scroll; i < 10 + *scroll; i++) {
		sprintf(text, "%d", i + 1);
		DrawString(screen, 125 - strlen(text) * 4, 93 + j * height / 11, text, charset);

		sprintf(text, "%.0lf", *(lista + i * 2));
		DrawString(screen, width * 55 / 100 - strlen(text) * 4, 93 + j * height / 11, text, charset);

		sprintf(text, "%.1lf s", *(lista + i * 2 + 1));
		DrawString(screen, width - strlen(text) * 4, 93 + j * height / 11, text, charset);
		j++;
	}
}