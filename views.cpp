#include "views.h"

enum {
	SpeedUp,
	SlowDown,
	Left,
	Right,
	RoadSide
};

void startView(struct surfaces* surfaces, struct colors* colors, struct coords *coords) {
	SDL_ShowCursor(1);
	char text[128];
	SDL_FillRect(surfaces->screen, NULL, colors->szary);
	DrawSurface(surfaces->screen, surfaces->starter, surfaces->screen->w / 2, surfaces->screen->h / 2);
	if (coords->x > surfaces->screen->w / 4 - 60 && coords->x < surfaces->screen->w / 4 + 60 && coords->y > surfaces->screen->h / 3 * 2 + 45 && coords->y < surfaces->screen->h / 3 * 2 + 65) {
		DrawRectangle(surfaces->screen, surfaces->screen->w / 4 - 60, surfaces->screen->h / 3 * 2 + 45, 120, 20, colors->czerwony, colors->jasny_niebieski);
	}
	else {
		DrawRectangle(surfaces->screen, surfaces->screen->w / 4 - 60, surfaces->screen->h / 3 * 2 + 45, 120, 20, colors->czerwony, colors->niebieski);
	}
	if (coords->x > surfaces->screen->w / 4 * 3 - 60 && coords->x < surfaces->screen->w / 4 * 3 + 60 && coords->y > surfaces->screen->h / 3 * 2 + 45 && coords->y < surfaces->screen->h / 3 * 2 + 65) {
		DrawRectangle(surfaces->screen, surfaces->screen->w / 4 * 3 - 60, surfaces->screen->h / 3 * 2 + 45, 120, 20, colors->czerwony, colors->jasny_niebieski);
	}
	else {
		DrawRectangle(surfaces->screen, surfaces->screen->w / 4 * 3 - 60, surfaces->screen->h / 3 * 2 + 45, 120, 20, colors->czerwony, colors->niebieski);
	}
	if (coords->x > surfaces->screen->w / 2 - 60 && coords->x < surfaces->screen->w / 2 + 60 && coords->y > surfaces->screen->h / 3 * 2 + 45 && coords->y < surfaces->screen->h / 3 * 2 + 65) {
		DrawRectangle(surfaces->screen, surfaces->screen->w / 2 - 60, surfaces->screen->h / 3 * 2 + 45, 120, 20, colors->czerwony, colors->jasny_niebieski);
	}
	else {
		DrawRectangle(surfaces->screen, surfaces->screen->w / 2 - 60, surfaces->screen->h / 3 * 2 + 45, 120, 20, colors->czerwony, colors->niebieski);
	}
	sprintf(text, "Rankings");
	DrawString(surfaces->screen, surfaces->screen->w / 4 - strlen(text) * 4, surfaces->screen->h / 3 * 2 + 50, text, surfaces->charset);
	sprintf(text, "Controls");
	DrawString(surfaces->screen, surfaces->screen->w / 4 * 3 - strlen(text) * 4, surfaces->screen->h / 3 * 2 + 50, text, surfaces->charset);
	sprintf(text, "Start game");
	DrawString(surfaces->screen, surfaces->screen->w / 2 - strlen(text) * 4, surfaces->screen->h / 3 * 2 + 50, text, surfaces->charset);
}

void controlsView(struct surfaces* surfaces, struct colors* colors) {
	char text[128];
	int width = SCREEN_WIDTH - 200;
	int height = SCREEN_HEIGHT - 160;
	SDL_ShowCursor(1);
	SDL_FillRect(surfaces->screen, NULL, colors->szary);
	DrawLine(surfaces->screen, 100, 80, height, 0, 1, colors->ciemny_zielony);
	DrawLine(surfaces->screen, 100, SCREEN_HEIGHT - 80, width, 1, 0, colors->ciemny_zielony);
	DrawLine(surfaces->screen, SCREEN_WIDTH - 100, 80, height, 0, 1, colors->ciemny_zielony);
	for (int i = 0; i < 11; i++) {
		DrawLine(surfaces->screen, 100, 80 + i * (height / 11), width, 1, 0, colors->ciemny_zielony);
	}
	DrawLine(surfaces->screen, 100 + width / 10, 80, height, 0, 1, colors->ciemny_zielony);
	DrawLine(surfaces->screen, 100 + width * 55 / 100, 80, height, 0, 1, colors->ciemny_zielony);
}

void drawView(struct surfaces *surfaces, int plansza[MAP_HEIGHT][MAP_WIDTH], int fps, int delay, struct game* game, struct colors* colors, struct powerup *power, struct bullet *bullet, struct enemy *enemy, struct civilian *civilian) {
	char text[128];
	SDL_FillRect(surfaces->screen, NULL, colors->szary);
	for (int i = 0; i < MAP_HEIGHT; i++) {
		DrawRectangle(surfaces->screen, 0, plansza[i][0] * i, plansza[i][1], SCREEN_HEIGHT / MAP_HEIGHT, colors->ciemny_zielony, colors->ciemny_zielony);
		DrawRectangle(surfaces->screen, SCREEN_WIDTH - plansza[i][1], SCREEN_HEIGHT / MAP_HEIGHT * i, plansza[i][1], SCREEN_HEIGHT / MAP_HEIGHT, colors->ciemny_zielony, colors->ciemny_zielony);
	}
	if (power->onmap) DrawSurface(surfaces->screen, surfaces->powerUp, power->x, power->y);
	DrawSurface(surfaces->screen, surfaces->player, SCREEN_WIDTH / 2 + game->posX, SCREEN_HEIGHT / 3 * 2);
	DrawRectangle(surfaces->screen, 0, 0, SCREEN_WIDTH, 40, colors->czerwony, colors->niebieski);
	sprintf(text, "Dawid Glazik, Time elapsed: %.1lf s %.0lf fps Score: %d", game->worldTime, (float)fps, game->score);
	DrawString(surfaces->screen, surfaces->screen->w / 2 - strlen(text) * 4, 10, text, surfaces->charset);
	sprintf(text, "r - ranking, Esc - wyjscie, \030 - przyspieszenie, \031 - zwolnienie");
	DrawString(surfaces->screen, surfaces->screen->w / 2 - strlen(text) * 4, 26, text, surfaces->charset);
	switch (game->gun){
	case 2:
		DrawSurface(surfaces->screen, surfaces->rifle, surfaces->screen->w - 30, surfaces->screen->h - 30);
		DrawLine(surfaces->screen, surfaces->screen->w - 50, surfaces->screen->h - 5, 40 - 40 * (game->worldTime - delay) / 15, 1, 0, colors->niebieski);
		DrawLine(surfaces->screen, surfaces->screen->w - 50, surfaces->screen->h - 5, 40 - 40 * (game->worldTime - delay) / 15, 1, 0, colors->niebieski);
		break;
	case 3:
		DrawSurface(surfaces->screen, surfaces->sniper, surfaces->screen->w - 30, surfaces->screen->h - 30);
		DrawLine(surfaces->screen, surfaces->screen->w - 50, surfaces->screen->h - 5, 40 - 40 * (game->worldTime - delay) / 7, 1, 0, colors->niebieski);
		DrawLine(surfaces->screen, surfaces->screen->w - 50, surfaces->screen->h - 5, 40 - 40 * (game->worldTime - delay) / 7, 1, 0, colors->niebieski);
		break;
	default:
		DrawSurface(surfaces->screen, surfaces->pistol, surfaces->screen->w - 30, surfaces->screen->h - 30);
		DrawLine(surfaces->screen, surfaces->screen->w - 50, surfaces->screen->h - 5, 40, 1, 0, colors->niebieski);
		DrawLine(surfaces->screen, surfaces->screen->w - 50, surfaces->screen->h - 5, 40, 1, 0, colors->niebieski);
		break;
	}
	if (game->lives == 2) DrawSurface(surfaces->screen, surfaces->player, 25, SCREEN_HEIGHT - 30);
	if (bullet->launched) DrawSurface(surfaces->screen, surfaces->bullet, bullet->x, bullet->y);
	if (enemy->onmap && enemy->lives > 0) DrawSurface(surfaces->screen, surfaces->enemy, enemy->x, enemy->y);
	else enemy->y = SCREEN_HEIGHT;
	if (civilian->onmap && civilian->lives > 0) DrawSurface(surfaces->screen, surfaces->civilian, civilian->x, civilian->y);
	else civilian->y = SCREEN_HEIGHT;
}

void finishView(struct surfaces* surfaces, int score, struct colors* colors, struct coords* coords) {
	SDL_ShowCursor(1);
	char text[128];
	SDL_FillRect(surfaces->screen, NULL, colors->szary);
	sprintf(text, "Game Over. Score: %d", score);
	DrawString(surfaces->screen, surfaces->screen->w / 2 - strlen(text) * 4, surfaces->screen->h / 2, text, surfaces->charset);
	if (coords->x > surfaces->screen->w / 2 - 75 && coords->x < surfaces->screen->w / 2 + 75 && coords->y > surfaces->screen->h / 2 + 20 && coords->y < surfaces->screen->h / 2 + 40) {
		DrawRectangle(surfaces->screen, surfaces->screen->w / 2 - 75, surfaces->screen->h / 2 + 20, 150, 20, colors->czerwony, colors->jasny_niebieski);
	}
	else {
		DrawRectangle(surfaces->screen, surfaces->screen->w / 2 - 75, surfaces->screen->h / 2 + 20, 150, 20, colors->czerwony, colors->niebieski);
	}
	sprintf(text, "Add to ranking");
	DrawString(surfaces->screen, surfaces->screen->w / 2 - 40 - strlen(text), surfaces->screen->h / 2 + 26, text, surfaces->charset);
}

void saveLoadView(struct surfaces* surfaces, struct colors* colors, struct toFile* toFile, struct coords* coords, SDL_Event* event) {
	char text[128];
	SDL_ShowCursor(1);
	SDL_FillRect(surfaces->screen, NULL, colors->szary);
	DrawSurface(surfaces->screen, surfaces->saveNload, SCREEN_WIDTH / 2, 36);
	if (coords->x > 170 && coords->x < 470 && coords->y>72 && coords->y < 172) {
		DrawRectangle(surfaces->screen, SCREEN_WIDTH / 2 - 150, 72, 300, 100, colors->czerwony, colors->jasny_niebieski);
	}
	else {
		DrawRectangle(surfaces->screen, SCREEN_WIDTH / 2 - 150, 72, 300, 100, colors->czerwony, colors->niebieski);
	}
	sprintf(text, toFile->slot1);
	DrawString(surfaces->screen, surfaces->screen->w / 2 - strlen(text) * 4, 122, text, surfaces->charset);
	if (coords->x > 170 && coords->x < 470 && coords->y>208 && coords->y < 308) {
		DrawRectangle(surfaces->screen, SCREEN_WIDTH / 2 - 150, 208, 300, 100, colors->czerwony, colors->jasny_niebieski);
	}
	else {
		DrawRectangle(surfaces->screen, SCREEN_WIDTH / 2 - 150, 208, 300, 100, colors->czerwony, colors->niebieski);
	}
	sprintf(text, toFile->slot2);
	DrawString(surfaces->screen, surfaces->screen->w / 2 - strlen(text) * 4, 258, text, surfaces->charset);

	if (coords->x > 170 && coords->x < 470 && coords->y>344 && coords->y < 444) {
		DrawRectangle(surfaces->screen, SCREEN_WIDTH / 2 - 150, 344, 300, 100, colors->czerwony, colors->jasny_niebieski);
	}
	else {
		DrawRectangle(surfaces->screen, SCREEN_WIDTH / 2 - 150, 344, 300, 100, colors->czerwony, colors->niebieski);
	}
	sprintf(text, toFile->slot3);
	DrawString(surfaces->screen, surfaces->screen->w / 2 - strlen(text) * 4, 394, text, surfaces->charset);
}

void rankingView(struct surfaces* surfaces, struct colors* colors, int* sort, double* lista, int sizeOfRanking, int* scroll) {
	char text[128];
	int width = SCREEN_WIDTH - 200;
	int height = SCREEN_HEIGHT - 160;
	SDL_ShowCursor(1);
	SDL_FillRect(surfaces->screen, NULL, colors->szary);
	DrawSurface(surfaces->screen, surfaces->ranking, SCREEN_WIDTH / 2, 40);
	DrawLine(surfaces->screen, 100, 80, height, 0, 1, colors->ciemny_zielony);
	DrawLine(surfaces->screen, 100, SCREEN_HEIGHT - 80, width, 1, 0, colors->ciemny_zielony);
	DrawLine(surfaces->screen, SCREEN_WIDTH - 100, 80, height, 0, 1, colors->ciemny_zielony);
	for (int i = 0; i < 11; i++) {
		DrawLine(surfaces->screen, 100, 80 + i * (height / 11), width, 1, 0, colors->ciemny_zielony);
	}
	DrawLine(surfaces->screen, 100 + width / 10, 80, height, 0, 1, colors->ciemny_zielony);
	DrawLine(surfaces->screen, 100 + width * 55 / 100, 80, height, 0, 1, colors->ciemny_zielony);

	sprintf(text, "Lp.");
	DrawString(surfaces->screen, 125 - strlen(text) * 4, 93, text, surfaces->charset);
	if (*sort == 1) sprintf(text, "Score \031");
	else sprintf(text, "Score");
	DrawString(surfaces->screen, width * 55 / 100 - strlen(text) * 4, 93, text, surfaces->charset);
	if (*sort == 2) sprintf(text, "Time \031");
	else sprintf(text, "Time");
	DrawString(surfaces->screen, width - strlen(text) * 4, 93, text, surfaces->charset);

	sortArray(lista, sizeOfRanking, *sort);
	int j = 1;
	for (int i = *scroll; i < 10 + *scroll; i++) {
		sprintf(text, "%d", i + 1);
		DrawString(surfaces->screen, 125 - strlen(text) * 4, 93 + j * height / 11, text, surfaces->charset);

		sprintf(text, "%.0lf", *(lista + i * 2));
		DrawString(surfaces->screen, width * 55 / 100 - strlen(text) * 4, 93 + j * height / 11, text, surfaces->charset);

		sprintf(text, "%.1lf s", *(lista + i * 2 + 1));
		DrawString(surfaces->screen, width - strlen(text) * 4, 93 + j * height / 11, text, surfaces->charset);
		j++;
	}
}