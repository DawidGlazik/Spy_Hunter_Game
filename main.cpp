#include "functions.h"
#include "events.h"
#include "views.h"

enum {
	SpeedUp,
	SlowDown,
	Left,
	Right,
	RoadSide
};

int main(int argc, char **argv) {
	struct colors colors;
	struct toFile toFile = { "Pusty1", "Pusty2", "Pusty3"};
	struct game game;
	struct coords coords = {0, 0};
	struct switches switches = {false, false, false, false, false, false};
	SDL_Event event;
	SDL_Surface* screen, * charset;
	SDL_Surface* player;
	SDL_Surface* pauza;
	SDL_Surface* saveNload;
	SDL_Surface* ranking;
	SDL_Texture* scrtex;
	SDL_Window* window;
	SDL_Renderer* renderer;
	int t1, t2, frames, fps, sort, sizeOfRanking, scroll;
	int gameCounter = 1;
	double delta, fpsTimer, speed, penalty;
	bool state[5] = {false, false, false, false, false};
	char text1[128];
	char text2[128];
	char text3[128];
	frames = 0;
	fpsTimer = 0;
	fps = 0;
	speed = 1;
	penalty = 0.0;
	sort = 1;
	scroll = 0;
	sizeOfRanking = 0;
	game.score = 0;
	game.posX = 0;
	game.worldTime = 0;
	game.distance = 0;
	game.temp = 0.0;
	game.lives = 1000;	
	initialize(&screen, &scrtex, &renderer, &window, &colors);
	setBMPs(screen, scrtex, renderer, window, &charset, &pauza, &saveNload, &ranking);
	startBoard(game.plansza);
	t1 = SDL_GetTicks();
	if (DownloadSavesList(text1, text2, text3, &sizeOfRanking)) {
		toFile.slot1 = text1;
		toFile.slot2 = text2;
		toFile.slot3 = text3;
	}
	double* lista = (double*)malloc((sizeOfRanking + 1) * 2 * sizeof(double));
	loadRankings(lista, &sizeOfRanking);
	while(!switches.quit) {
			player = SDL_LoadBMP("obrazy/car.bmp");
			loadPicture(player, screen, scrtex, renderer, window);
			t2 = SDL_GetTicks();
			delta = (t2 - t1) * 0.001;
			t1 = t2;
			DrawSurface(screen, pauza, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		if (!switches.pause && !switches.finish && !switches.save && !switches.load && !switches.ranking) {
			SDL_ShowCursor(SDL_DISABLE);
			calculations(&game, state, &delta, &fpsTimer, &speed, &penalty, &fps, &frames);
			drawView(screen, player, charset, game.plansza, fps, &game, &colors, gameCounter);
			if (checkCollision(screen, scrtex, renderer, window, player, &game, &switches.pause, state)) {
				game.lives--;
				t2 = SDL_GetTicks();
				delta = (t2 - t1) * 0.001;
				t1 = t2;
			}
			if (game.lives == 0) {
				switches.finish = true;
				gameCounter++;
			}
			refresh(screen, scrtex, renderer);
			frames++;
		}
		if (switches.finish) finishView(screen, charset, game.score, &colors, &coords); //koniec gry
		if (switches.save) saveLoadView(screen, charset, saveNload, &colors, &toFile, &coords, &event); //zapis
		if (switches.load) saveLoadView(screen, charset, saveNload, &colors, &toFile, &coords, &event); //odczyt
		if (switches.ranking) rankingView(screen, ranking, charset, &colors, &sort, lista, sizeOfRanking, &scroll); //ranking
		refresh(screen, scrtex, renderer);
		if (!switches.save && !switches.load && !switches.ranking && !switches.finish) driveEvents(&event, &game, &switches, state, &penalty, &gameCounter);
		else if (switches.ranking) rankingEvents(&event, &switches, &coords, &sort, &scroll, sizeOfRanking);
		else if (switches.finish) finishEvents(screen, &event, &switches, &coords, &game, lista, &sizeOfRanking);
		else saveNloadEvents(&event, &switches, &coords, &toFile, &game);
		if (!state[RoadSide]) game.score = (game.distance - penalty) * 22.5;
		if ((int)game.worldTime == 30) game.lives = 2;
		if (game.lives < 2 && game.score > 1000 && game.score % 500 == 0) game.lives = 2;
	};
	saveRankings(lista, &sizeOfRanking);
	saveHRranking(lista, &sizeOfRanking);
	endProgram(screen, scrtex, renderer, window);
	UploadSavesList(&toFile, sizeOfRanking);
	free(lista);
	return 0;
	};