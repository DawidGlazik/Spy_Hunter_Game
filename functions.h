#pragma once
#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <time.h>

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}
#ifdef __cplusplus
extern "C"
#endif

#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480
#define MAP_HEIGHT 20
#define MAP_WIDTH 2
#define GRASS_DIFF 10			//change of grass' width
#define ROADSIDE 20

//set of colors
struct colors {
	int czerwony;
	int niebieski;
	int jasny_niebieski;
	int ciemny_zielony;
	int szary;
};
//descriptions of saves
struct toFile {
	char* slot1;
	char* slot2;
	char* slot3;
};
//game variables
struct game {
	int plansza[MAP_HEIGHT][MAP_WIDTH];
	int score;
	int posX;
	double worldTime;
	double distance;
	double temp;
	int lives;
	int gun;	//1-pistol, 2-rifle, 3-sniper
};
//cursor position
struct coords {
	int x;
	int y;
};
//view switches
struct switches {
	bool start;
	bool save;
	bool load;
	bool quit;
	bool finish;
	bool pause;
	bool ranking;
	bool controls;
};

struct powerup {
	bool onmap;
	int x;
	int y;
};

struct surfaces {
	SDL_Surface* screen;
	SDL_Surface* starter;
	SDL_Surface* charset;
	SDL_Surface* player;
	SDL_Surface* pauza;
	SDL_Surface* saveNload;
	SDL_Surface* ranking;
	SDL_Surface* powerUp;
	SDL_Surface* pistol;
	SDL_Surface* rifle;
	SDL_Surface* sniper;
	SDL_Surface* enemy;
	SDL_Surface* civilian;
	SDL_Surface* bullet;
	SDL_Texture* scrtex;
	SDL_Window* window;
	SDL_Renderer* renderer;
};

struct bullet {
	int x;
	int y;
	bool launched;
};

struct enemy {
	int x;
	int y;
	int lives;
	bool onmap;
};

struct civilian {
	int x;
	int y;
	int lives;
	bool onmap;
};

void DrawString(SDL_Surface* screen, int x, int y, const char* text, SDL_Surface* charset);

void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y);

void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color);

void DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color);

void DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k, Uint32 outlineColor, Uint32 fillColor);



void startBoard(int plansza[MAP_HEIGHT][MAP_WIDTH]);	//initialize the map

void mapMovement(struct game* game, struct powerup *power, struct enemy* enemy, struct civilian* civilian); //map scrolling

int loadPicture(SDL_Surface* any, struct surfaces *surfaces);

void endProgram(struct surfaces *surfaces);

int checkCollision(struct surfaces *surfaces, struct game* game, bool* pause, bool state[6]);

int checkPowerUp(double *delay, struct game* game, struct powerup* power);

void refresh(struct surfaces *surfaces);		//refresh the screen

int initialize(struct surfaces *surfaces, struct colors* colors);

void initColors(SDL_Surface* screen, struct colors* colors);

void setBMPs(struct surfaces* surfaces);

void moving(struct game* game, bool state[6], double* speed);		//car movement

void sortArray(double* lista, int sizeOfRanking, int option);

void calculations(struct game* game, bool state[6], double* delta, double* fpsTimer, double* speed, double* penalty, int* fps, int* frames);

void bullets(struct game* game, struct bullet *bullet);

void movementOnMap(struct game* game, struct powerup* power, struct enemy *enemy, struct civilian *civilian, bool state[6], double* speed);

int checkIfHit(double* extraPoints, struct bullet* bullet, struct enemy* enemy, struct civilian* civilian, bool state[6]);

int checkIfCrash(struct surfaces* surfaces, struct game* game, struct enemy* enemy, struct civilian* civilian, bool* pause);

void carExplode(struct surfaces* surfaces, struct game* game, bool* pause);

void pushOtherCars(struct game* game, struct enemy* enemy, struct civilian* civilian);

void checkNPCCollision(double* extraPoints, struct surfaces* surfaces, struct game* game, struct enemy* enemy, struct civilian* civilian, bool state[6], struct bullet* bullet);

void enemyMovement(struct game* game, struct enemy* enemy);

void civilianMovement(struct game* game, struct civilian* civilian);



void saveFile(const struct game* game, int option, struct toFile* toFile, struct enemy* enemy, struct civilian* civilian);	//save game

void saveRankings(double* lista, int* sizeOfRanking);		//save ranking to file

void UploadSavesList(const struct toFile* toFile, int sizeOfRanking);		//save description of saves and size of ranking

void saveHRranking(double* lista, int* sizeOfRanking);	//saves human-readable ranking



void loadFile(struct game* game, int option, struct enemy* enemy, struct civilian* civilian);		//load game

void loadRankings(double* lista, int* sizeOfRanking);		//load ranking from file

int DownloadSavesList(char* text1, char* text2, char* text3, int* sizeOfRanking);	//loads description of saves and size of ranking