#include "functions.h"

enum {
	SpeedUp,
	SlowDown,
	Left,
	Right,
	RoadSide,
	Block
};

void DrawString(SDL_Surface* screen, int x, int y, const char* text, SDL_Surface* charset) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while (*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
	};
};

void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y) {
	SDL_Rect dest;
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
};

void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color) {
	int bpp = surface->format->BytesPerPixel;
	Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32*)p = color;
};

void DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color) {
	for (int i = 0; i < l; i++) {
		DrawPixel(screen, x, y, color);
		x += dx;
		y += dy;
	};
};

void DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k, Uint32 outlineColor, Uint32 fillColor) {
	int i;
	DrawLine(screen, x, y, k, 0, 1, outlineColor);
	DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(screen, x, y, l, 1, 0, outlineColor);
	DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
	for (i = y + 1; i < y + k - 1; i++)
		DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
};



void startBoard(int plansza[MAP_HEIGHT][MAP_WIDTH]) {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (j == 0) plansza[i][j] = SCREEN_HEIGHT / MAP_HEIGHT;
			else plansza[i][j] = SCREEN_WIDTH / 3;
		}
	}
}

void mapMovement(struct game* game, struct powerup *power, struct enemy* enemy, struct civilian* civilian) {
	int seed = rand() % 100;
	int seed2 = rand() % 5000;
	int seed3 = rand() % (SCREEN_WIDTH - 2 * game->plansza[3][1]);
	if (seed % 2500 == 0 && !power->onmap) {
		power->x = game->plansza[3][1] + seed3;
		power->y = game->plansza[5][0];
		power->onmap = true;
	}
	for (int i = MAP_HEIGHT - 2; i >= 0; i--) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			game->plansza[i + 1][j] = game->plansza[i][j];
		}
	}
	if (seed < 25 && game->plansza[1][1] > SCREEN_WIDTH / 4) game->plansza[0][1] = game->plansza[1][1] - GRASS_DIFF;
	else if (seed < 50 && game->plansza[1][1] < SCREEN_WIDTH * 2 / 5) game->plansza[0][1] = game->plansza[1][1] + GRASS_DIFF;
	else game->plansza[0][1] = game->plansza[1][1];
	game->temp = game->worldTime;
	if (power->y + 24 < SCREEN_HEIGHT) power->y += 24;
	else power->onmap = false;
	if (seed >= 50 && seed % 10 == 0 && !enemy->onmap) {
		enemy->onmap = true;
		enemy->x = game->plansza[3][1] + seed3;
		enemy->y = 48;
		enemy->lives = 2;
	}
	else if (seed % 10 == 0 && !civilian->onmap) {
		civilian->onmap = true;
		civilian->x = game->plansza[3][1] + seed3;
		civilian->y = 48;
		civilian->lives = 2;
	}
	if (enemy->y + 30 < SCREEN_HEIGHT) enemy->y += 15;
	else enemy->onmap = false;
	if (civilian->y + 30 < SCREEN_HEIGHT) civilian->y += 15;
	else civilian->onmap = false;
}

int loadPicture(SDL_Surface* any, struct surfaces *surfaces) {
	if (any == NULL) {
		printf("SDL_LoadBMP() error: %s\n", SDL_GetError());
		endProgram(surfaces);
		return 1;
	};
}

void endProgram(struct surfaces *surfaces) {
	SDL_FreeSurface(surfaces->screen);
	SDL_FreeSurface(surfaces->charset);
	SDL_FreeSurface(surfaces->pauza);
	SDL_FreeSurface(surfaces->player);
	SDL_FreeSurface(surfaces->ranking);
	SDL_FreeSurface(surfaces->save);
	SDL_FreeSurface(surfaces->powerUp);
	SDL_FreeSurface(surfaces->pistol);
	SDL_FreeSurface(surfaces->rifle);
	SDL_FreeSurface(surfaces->sniper);
	SDL_FreeSurface(surfaces->enemy);
	SDL_FreeSurface(surfaces->civilian);
	SDL_FreeSurface(surfaces->bullet);
	SDL_FreeSurface(surfaces->controls);
	SDL_FreeSurface(surfaces->load);
	SDL_FreeSurface(surfaces->starter);
	SDL_DestroyTexture(surfaces->scrtex);
	SDL_DestroyRenderer(surfaces->renderer);
	SDL_DestroyWindow(surfaces->window);
	SDL_Quit();
}

void saveFile(const struct game* game, int option, struct toFile* toFile, struct enemy* enemy, struct civilian* civilian) {
	char* str[30];
	if (option == 1) *str = "saves/zapis1.bin";
	else if (option == 2) *str = "saves/zapis2.bin";
	else *str = "saves/zapis3.bin";
	FILE* plik;
	if ((plik = fopen(*str, "w")) != NULL) {
		fwrite(game, sizeof(struct game), 1, plik);
		fwrite(enemy, sizeof(struct enemy), 1, plik);
		fwrite(civilian, sizeof(struct civilian), 1, plik);
		fclose(plik);
	}
}

void loadFile(struct game* game, int option, struct enemy* enemy, struct civilian* civilian) {
	char* str[30];
	if (option == 1) *str = "saves/zapis1.bin";
	else if (option == 2) *str = "saves/zapis2.bin";
	else *str = "saves/zapis3.bin";
	FILE* plik;
	if ((plik = fopen(*str, "r")) != NULL) {
		fread(game, sizeof(struct game), 1, plik);
		fread(enemy, sizeof(struct enemy), 1, plik);
		fread(civilian, sizeof(struct civilian), 1, plik);
		fclose(plik);
	}
}

int checkCollision(struct surfaces *surfaces, struct game* game, bool* pause, bool state[6]) {
	for (int i = 0; i < 3; i++) {
		int position = SCREEN_WIDTH / 2 + game->posX;
		int grass = game->plansza[11 + i][1];
		if (position < grass || position > SCREEN_WIDTH - grass) {
			if (position > grass - ROADSIDE && position < SCREEN_WIDTH - grass + ROADSIDE) {
				state[RoadSide] = true;
				return 0;
			}
			else {
				carExplode(surfaces, game, pause);
				return 1;
			}
		}
		else state[RoadSide] = false;
	}
	return 0;
}

void carExplode(struct surfaces* surfaces, struct game* game, bool* pause) {
	surfaces->player = SDL_LoadBMP("images/car_bum.bmp");
	loadPicture(surfaces->player, surfaces);
	DrawSurface(surfaces->screen, surfaces->player, SCREEN_WIDTH / 2 + game->posX, SCREEN_HEIGHT / 3 * 2);
	SDL_UpdateTexture(surfaces->scrtex, NULL, surfaces->screen->pixels, surfaces->screen->pitch);
	SDL_RenderCopy(surfaces->renderer, surfaces->scrtex, NULL, NULL);
	SDL_RenderPresent(surfaces->renderer);
	*pause = true;
	SDL_Delay(2000);
	game->posX = 0;
	*pause = false;
}

int checkPowerUp(double *delay, struct game* game, struct powerup* power) {
	if (game->gun == 2 && game->worldTime - *delay > 15) {
		game->gun = 1;
		*delay = 0.0;
	}
	else if (game->gun == 3 && game->worldTime - *delay > 7) {
		game->gun = 1;
		*delay = 0.0;
	}
	int seed = rand() % 100;
	int carPosition = game->posX + SCREEN_WIDTH / 2;
	if (carPosition > power->x - 20 && carPosition < power->x + 20 && power->y > SCREEN_HEIGHT * 2 / 3 - 20 && power->y < SCREEN_HEIGHT * 2 / 3) {
		power->onmap = false;
		if (seed < 80 && game->gun == 1) {
			game->gun = 2;
			return 1;
		}
		else if (game->gun == 1) {
			game->gun = 3;
			return 1;
		}
	}
	return 0;
}

void refresh(struct surfaces *surfaces) {
	SDL_UpdateTexture(surfaces->scrtex, NULL, surfaces->screen->pixels, surfaces->screen->pitch);
	SDL_RenderCopy(surfaces->renderer, surfaces->scrtex, NULL, NULL);
	SDL_RenderPresent(surfaces->renderer);
}

int initialize(struct surfaces *surfaces, struct colors* colors) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}

	//int rc = SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP, &window, &renderer); // tryb pełnoekranowy
	int rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &surfaces->window, &surfaces->renderer);
	if (rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return 1;
	};

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(surfaces->renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(surfaces->renderer, 0, 0, 0, 255);

	SDL_SetWindowTitle(surfaces->window, "Spy Hunter");
	surfaces->screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	surfaces->scrtex = SDL_CreateTexture(surfaces->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

	initColors(surfaces->screen, colors);
}

void initColors(SDL_Surface* screen, struct colors* colors) {
	colors->czerwony = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
	colors->niebieski = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);
	colors->jasny_niebieski = SDL_MapRGB(screen->format, 0x4D, 0xD6, 0xEB);
	colors->ciemny_zielony = SDL_MapRGB(screen->format, 0x15, 0x65, 0x0C);
	colors->szary = SDL_MapRGB(screen->format, 0x7A, 0x7A, 0x7A);
}

void setBMPs(struct surfaces* surfaces) {
	surfaces->charset = SDL_LoadBMP("images/cs8x8.bmp");
	loadPicture(surfaces->charset, surfaces);
	SDL_SetColorKey(surfaces->charset, true, 0x000000);

	surfaces->starter = SDL_LoadBMP("images/starter.bmp");
	loadPicture(surfaces->starter, surfaces);

	surfaces->pauza = SDL_LoadBMP("images/pauza.bmp");
	loadPicture(surfaces->pauza, surfaces);

	surfaces->save = SDL_LoadBMP("images/Save.bmp");
	loadPicture(surfaces->save, surfaces);

	surfaces->load = SDL_LoadBMP("images/load.bmp");
	loadPicture(surfaces->load, surfaces);

	surfaces->ranking = SDL_LoadBMP("images/ranking.bmp");
	loadPicture(surfaces->ranking, surfaces);

	surfaces->powerUp = SDL_LoadBMP("images/power.bmp");
	loadPicture(surfaces->powerUp, surfaces);

	surfaces->pistol = SDL_LoadBMP("images/pistol.bmp");
	loadPicture(surfaces->pistol, surfaces);

	surfaces->rifle = SDL_LoadBMP("images/rifle.bmp");
	loadPicture(surfaces->rifle, surfaces);

	surfaces->sniper = SDL_LoadBMP("images/sniper.bmp");
	loadPicture(surfaces->sniper, surfaces);

	surfaces->enemy = SDL_LoadBMP("images/car_enemy.bmp");
	loadPicture(surfaces->enemy, surfaces);

	surfaces->civilian = SDL_LoadBMP("images/car_civilian.bmp");
	loadPicture(surfaces->civilian, surfaces);

	surfaces->bullet = SDL_LoadBMP("images/bullet.bmp");
	loadPicture(surfaces->bullet, surfaces);

	surfaces->controls = SDL_LoadBMP("images/controls.bmp");
	loadPicture(surfaces->controls, surfaces);
}

void UploadSavesList(const struct toFile* toFile, int sizeOfRanking) {
	FILE* file;
	if (fopen_s(&file, "saves/lista.txt", "w") == 0) {
		fprintf(file, "%s ", toFile->slot1);
		fprintf(file, "%s ", toFile->slot2);
		fprintf(file, "%s ", toFile->slot3);
		fprintf(file, "%d ", sizeOfRanking);
		fclose(file);
	}
}

int DownloadSavesList(char* text1, char* text2, char* text3, int* sizeOfRanking) {
	int day;
	int month;
	int year;
	int hour;
	int minute;
	int second;
	FILE* file;
	if (fopen_s(&file, "saves/lista.txt", "r") == 0) {
		fscanf_s(file, "%d/%d/%d-%d:%d:%d ", &day, &month, &year, &hour, &minute, &second);
		sprintf(text1, "%d/%d/%d-%d:%d:%d ", day, month, year, hour, minute, second);
		fscanf_s(file, "%d/%d/%d-%d:%d:%d ", &day, &month, &year, &hour, &minute, &second);
		sprintf(text2, "%d/%d/%d-%d:%d:%d ", day, month, year, hour, minute, second);
		fscanf_s(file, "%d/%d/%d-%d:%d:%d ", &day, &month, &year, &hour, &minute, &second);
		sprintf(text3, "%d/%d/%d-%d:%d:%d ", day, month, year, hour, minute, second);
		fscanf_s(file, "%d  ", sizeOfRanking);
		fclose(file);
		return 1;
	}
	return 0;
}

void moving(struct game* game, bool state[6], double* speed) {
	if (state[SpeedUp] && state[Right]) {
		game->posX++;
		*speed = 2.0;
	}
	else if (state[SpeedUp] && state[Left]) {
		game->posX--;
		*speed = 2.0;
	}
	else if (state[SlowDown] && state[Right]) {
		game->posX++;
		*speed = 0.5;
	}
	else if (state[SlowDown] && state[Left]) {
		game->posX--;
		*speed = 0.5;
	}
	else if (state[Right]) game->posX++;
	else if (state[Left]) game->posX--;
	else if (state[SpeedUp]) *speed = 2.0;
	else if (state[SlowDown]) *speed = 0.5;
	else *speed = 1.0;

	if (state[RoadSide]) *speed = 0.3;
}

void sortArray(double* lista, int sizeOfRanking, int option) {
	double temp, temp2;
	int j, k;
	if (option == 1) k = 0;
	else k = 1;
	for (int i = 1; i < sizeOfRanking; i++) {
		temp = *(lista + i * 2 + k);
		temp2 = *(lista + i * 2 + 1 - k);
		j = i - 1;
		while (j >= 0 && *(lista + j * 2 + k) < temp) {
			*(lista + (j + 1) * 2) = *(lista + j * 2);
			*(lista + (j + 1) * 2 + 1) = *(lista + j * 2 + 1);
			j--;
		}
		*(lista + (j + 1) * 2 + k) = temp;
		*(lista + (j + 1) * 2 + 1 - k) = temp2;
	}
}

void saveRankings(double* lista, int* sizeOfRanking) {
	FILE* file;
	if (fopen_s(&file, "saves/ranking.txt", "w") == 0) {
		for (int i = 0; i < *sizeOfRanking; i++) {
			for (int j = 0; j < 2; j++) {
				if (*(lista + i * 2 + j) != NULL) fprintf(file, "%lf\n", *(lista + i * 2 + j));
				else fclose(file);
			}
		}
		fclose(file);
	}
}

void loadRankings(double* lista, int* sizeOfRanking) {
	FILE* file;
	if (fopen_s(&file, "saves/ranking.txt", "r") == 0) {
		for (int i = 0; i < *sizeOfRanking; i++) {
			for (int j = 0; j < 2; j++) {
				fscanf_s(file, "%lf\n", (lista + i * 2 + j));
			}
		}
		fclose(file);
	}
}

void saveHRranking(double* lista, int* sizeOfRanking) {
	FILE* file;
	if (fopen_s(&file, "saves/ranking_hr.txt", "w") == 0) {
		fprintf(file, "\tLp.\t\t\tScore\t\t\tTime [s]\n");
		for (int i = 0; i < *sizeOfRanking; i++) {
			fprintf(file, "\t%d\t\t\t", i + 1);
			if (*(lista + i * 2 ) != NULL) fprintf(file, "%.0lf\t\t\t", *(lista + i * 2));
			if (*(lista + i * 2 + 1) != NULL) fprintf(file, "%.2lf\n", *(lista + i * 2 + 1));
		}
		fclose(file);
	}
}

void calculations(struct game* game, bool state[6], double* delta, double* fpsTimer, double* speed, int* fps, int* frames) {
	game->worldTime += *delta;
	game->distance += *speed * *delta;
	if (state[RoadSide] || state[Block]) game->penalty += *speed * *delta;
	*fpsTimer += *delta;
	if (*fpsTimer > 0.5) {
		*fps = *frames * 2;
		*frames = 0;
		*fpsTimer -= 0.5;
	};
}

void bullets(struct game* game, struct bullet *bullet) {
	if (bullet->launched) {
		switch (game->gun) {
		case 2:
			if (game->worldTime - game->temp >= 0.025) {
				if (bullet->y >= 120) bullet->y-=3;
				else {
					bullet->launched = false;
					bullet->y = SCREEN_HEIGHT * 2 / 3 - 20;
				}
			}
			break;
		case 3:
			if (game->worldTime - game->temp >= 0.025) {
				if (bullet->y >= 48) bullet->y -= 7;
				else {
					bullet->launched = false;
					bullet->y = SCREEN_HEIGHT * 2 / 3 - 20;
				}
			}
			break;
		default:
			if (game->worldTime - game->temp >= 0.025) {
				if (bullet->y >= 192) bullet->y--;
				else {
					bullet->launched = false;
					bullet->y = SCREEN_HEIGHT * 2 / 3 - 20;
				}
			}
			break;
		}
	}
}

void movementOnMap(struct game* game, struct powerup* power, struct enemy* enemy, struct civilian* civilian, bool state[6], double* speed){
	if (game->worldTime - game->temp >= 0.025) {
		moving(game, state, speed);
		pushOtherCars(game, enemy, civilian);
	}
	if (game->worldTime - game->temp >= 0.075 / *speed) {
		mapMovement(game, power, enemy, civilian);
	}
	if (game->worldTime - game->temp >= 0.065 / *speed) {
		enemyMovement(game, enemy);
		civilianMovement(game, civilian);
	}
}

int checkIfHit(double *extraPoints, struct bullet* bullet, struct enemy* enemy, struct civilian* civilian, bool state[6]) {
	if (bullet->launched && bullet->x > enemy->x - 14 && bullet->x < enemy->x + 14 && bullet->y > enemy->y && bullet->y < enemy->y + 44) {
		bullet->launched = false;
		enemy->lives--;
		if (!state[Block]) *extraPoints += 500.0;
		return 0;
	}
	if (bullet->launched && bullet->x > civilian->x - 14 && bullet->x < civilian->x + 14 && bullet->y > civilian->y && bullet->y < civilian->y + 44) {
		bullet->launched = false;
		civilian->lives--;
		state[Block] = true;
		return 1;
	}
	return 0;
}

int checkIfCrash(struct surfaces* surfaces, struct game* game, struct enemy* enemy, struct civilian* civilian, bool *pause) {
	int playerPosX = game->posX + SCREEN_WIDTH / 2;
	int playerPosY = SCREEN_HEIGHT / 3 * 2;
	if (abs(playerPosX - enemy->x) < 28 && enemy->y + 44 > playerPosY && enemy->y < playerPosY - 25) {
		carExplode(surfaces, game, pause);
		enemy->onmap = false;
		enemy->y = SCREEN_HEIGHT;
		return 1;
	}
	else if (abs(playerPosX - civilian->x) < 28 && civilian->y + 44 > playerPosY && civilian->y < playerPosY - 25) {
		carExplode(surfaces, game, pause);
		civilian->onmap = false;
		civilian->y = SCREEN_HEIGHT;
		return 1;
	}
	else {
		return 0;
	}
}

void pushOtherCars(struct game* game, struct enemy* enemy, struct civilian* civilian) {
	int playerPosX = game->posX + SCREEN_WIDTH / 2;
	int playerPosY = SCREEN_HEIGHT / 3 * 2;
	if (abs(playerPosX - enemy->x) > 28 && abs(playerPosX - enemy->x) < 30 && abs(playerPosY - enemy->y) < 44) {
		if (playerPosX - enemy->x > 0) enemy->x--;
		else enemy->x++;
	}
	else if (abs(playerPosX - civilian->x) > 28 && abs(playerPosX - civilian->x) < 30 && abs(playerPosY - civilian->y) < 44) {
		if (playerPosX - civilian->x > 0) civilian->x--;
		else civilian->x++;
	}
}

void checkNPCCollision(double* extraPoints, struct surfaces* surfaces, struct game* game, struct enemy* enemy, struct civilian* civilian, bool state[6], struct bullet* bullet) {
	const clock_t begin_time = clock();
	static clock_t diff = clock();
	if ((float)begin_time / CLOCKS_PER_SEC - (float)diff / CLOCKS_PER_SEC > 5) state[Block] = false;
	int mapPart = enemy->y / 24;
	if (checkIfHit(extraPoints, bullet, enemy, civilian, state)) diff = clock();
	for (int i = 0; i < 3; i++) {
		int grass = game->plansza[mapPart + i][1];
		if (enemy->x < grass - ROADSIDE || enemy->x > SCREEN_WIDTH - grass + ROADSIDE) {
			if (!state[Block] && enemy->onmap) {
				*extraPoints += 1000.0;
			}
			enemy->onmap = false;
		}
	}
	mapPart = civilian->y / 24;
	for (int i = 0; i < 3; i++) {
		int grass = game->plansza[mapPart + i][1];
		if (civilian->x < grass - ROADSIDE || civilian->x > SCREEN_WIDTH - grass + ROADSIDE) {
			if (civilian->onmap) {
				state[Block] = true;
				diff = clock();
			}
			civilian->onmap = false;
		}
	}
}

void enemyMovement(struct game* game, struct enemy* enemy) {
	int playerPosX = game->posX + SCREEN_WIDTH / 2;
	if (enemy->y < SCREEN_HEIGHT / 2 + 50) {
		if (playerPosX - enemy->x > 0) enemy->x++;
		else enemy->x--;
	}
}

void civilianMovement(struct game* game, struct civilian* civilian) {
	int playerPosX = game->posX + SCREEN_WIDTH / 2;
	int mapPart = civilian->y / 24;
	for (int i = 0; i < 3; i++) {
		int grass = game->plansza[mapPart + i][1];
		if (civilian->x )
		if (civilian->x < grass + ROADSIDE && civilian->y < SCREEN_HEIGHT / 2 + 50) {
			civilian->x++;
		}
		else if (civilian->x > SCREEN_WIDTH - grass - ROADSIDE && civilian->y < SCREEN_HEIGHT / 2 + 50) {
			civilian->x--;
		}
	}
}