#include "functions.h"
#include "events.h"
#include "views.h"

enum {
	SpeedUp,
	SlowDown,
	Left,
	Right,
	RoadSide,
	Block
};

int main(int argc, char **argv) {
	struct colors colors;
	struct toFile toFile = { "Empty1", "Empty2", "Empty3"};
	struct game game;
	struct powerup power;
	struct coords coords = {0, 0};
	struct switches switches = {true, false, false, false, false, false, false, false};
	struct surfaces surfaces;
	struct bullet bullet = {0, SCREEN_HEIGHT * 2 / 3 - 20, false};
	struct enemy enemy = { 0,0,2,false };
	struct civilian civilian = { 0,0,2,false };
	SDL_Event event;
	int t1, t2, frames, fps, sort, sizeOfRanking, scroll;
	double delta, fpsTimer, speed, penalty, delay = 0, extraPoints = 0;
	bool state[6] = {false, false, false, false, false, false};
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
	game.gun = 1;
	power.onmap = false;
	power.x = 0;
	power.y = 0;
	initialize(&surfaces, &colors);
	setBMPs(&surfaces);
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
			surfaces.player = SDL_LoadBMP("images/car.bmp");
			loadPicture(surfaces.player, &surfaces);
			t2 = SDL_GetTicks();
			delta = (t2 - t1) * 0.001;
			t1 = t2;
			if (!switches.start && switches.pause) DrawSurface(surfaces.screen, surfaces.pauza, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		if (!switches.start && !switches.pause && !switches.finish && !switches.save && !switches.load && !switches.ranking && !switches.controls) {
			SDL_ShowCursor(SDL_DISABLE);
			calculations(&game, state, &delta, &fpsTimer, &speed, &penalty, &fps, &frames);
			movementOnMap(&game, &power, &enemy, &civilian, state, &speed);
			drawView(&surfaces, game.plansza, fps, delay, &game, &colors, &power, &bullet, &enemy, &civilian);
			bullets(&game, &bullet);
			checkNPCCollision(&extraPoints, &surfaces, &game, &enemy, &civilian, state, &bullet);
			if (checkPowerUp(&delay, &game, &power)) delay = game.worldTime;
			if (checkCollision(&surfaces, &game, &switches.pause, state) || checkIfCrash(&surfaces, &game, &enemy, &civilian, &switches.pause)) {
				game.lives--;
				t2 = SDL_GetTicks();
				delta = (t2 - t1) * 0.001;
				t1 = t2;
			}
			if (game.lives == 0) {
				switches.finish = true;
			}
			refresh(&surfaces);
			frames++;
		}
		if (switches.start) startView(&surfaces, &colors, &coords);
		if (switches.controls) controlsView(&surfaces, &colors);
		if (switches.finish) finishView(&surfaces, game.score, &colors, &coords);
		if (switches.save) saveLoadView(&surfaces, &colors, &toFile, &coords, &event, 1);
		if (switches.load) saveLoadView(&surfaces, &colors, &toFile, &coords, &event, 2);
		if (switches.ranking) rankingView(&surfaces, &colors, &sort, lista, sizeOfRanking, &scroll);
		refresh(&surfaces);
		if (!switches.save && !switches.load && !switches.ranking && !switches.finish && !switches.start && !switches.controls) driveEvents(&event, &game, &switches, &bullet, state, &penalty);
		else if (switches.start) starterEvents(surfaces.screen ,&event, &switches, &coords);
		else if (switches.controls) controlsEvents(&event, &switches, &coords);
		else if (switches.ranking) rankingEvents(&event, &switches, &coords, &sort, &scroll, sizeOfRanking);
		else if (switches.finish) finishEvents(surfaces.screen, &event, &switches, &coords, &game, lista, &sizeOfRanking);
		else saveNloadEvents(&event, &switches, &coords, &toFile, &game, &enemy, &civilian);
		if (!state[RoadSide] && !state[Block]) game.score = (game.distance - penalty) * 22.5 + extraPoints;
		if ((int)game.worldTime == 30) game.lives = 2;
		if (game.lives < 2 && game.score > 1000 && (int)game.worldTime % 30 == 0) game.lives = 2;
	};
	saveRankings(lista, &sizeOfRanking);
	saveHRranking(lista, &sizeOfRanking);
	endProgram(&surfaces);
	UploadSavesList(&toFile, sizeOfRanking);
	free(lista);
	return 0;
	};