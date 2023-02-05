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
};
//cursor position
struct coords {
	int x;
	int y;
};
//view switches
struct switches {
	bool save;
	bool load;
	bool quit;
	bool finish;
	bool pause;
	bool ranking;
};

void DrawString(SDL_Surface* screen, int x, int y, const char* text, SDL_Surface* charset);

void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y);

void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color);

void DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color);

void DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k, Uint32 outlineColor, Uint32 fillColor);



void startBoard(int plansza[MAP_HEIGHT][MAP_WIDTH]);	//initialize the map

void mapMovement(struct game* game); //map scrolling

int loadPicture(SDL_Surface* any, SDL_Surface* screen, SDL_Texture* scrtex, SDL_Renderer* renderer, SDL_Window* window);

void endProgram(SDL_Surface* screen, SDL_Texture* scrtex, SDL_Renderer* renderer, SDL_Window* window);

int checkCollision(SDL_Surface* screen, SDL_Texture* scrtex, SDL_Renderer* renderer, SDL_Window* window, SDL_Surface* player, struct game* game, bool* pause, bool state[5]);

void refresh(SDL_Surface* screen, SDL_Texture* scrtex, SDL_Renderer* renderer);		//refresh the screen

int initialize(SDL_Surface** screen, SDL_Texture** scrtex, SDL_Renderer** renderer, SDL_Window** window, struct colors* colors);

void initColors(SDL_Surface* screen, struct colors* colors);

void setBMPs(SDL_Surface* screen, SDL_Texture* scrtex, SDL_Renderer* renderer, SDL_Window* window, SDL_Surface** charset, SDL_Surface** pauza, SDL_Surface** saveNload, SDL_Surface** ranking);

void moving(struct game* game, bool state[5], double* speed);		//car movement

void sortArray(double* lista, int sizeOfRanking, int option);

void calculations(struct game* game, bool state[5], double* delta, double* fpsTimer, double* speed, double* penalty, int* fps, int* frames);



void saveFile(struct game* game, int option, struct toFile* toFile);	//save game

void saveRankings(double* lista, int* sizeOfRanking);		//save ranking to file

void UploadSavesList(struct toFile* toFile, int sizeOfRanking);		//save description of saves and size of ranking

void saveHRranking(double* lista, int* sizeOfRanking);	//saves human-readable ranking



void loadFile(struct game* game, int option);		//load game

void loadRankings(double* lista, int* sizeOfRanking);		//load ranking from file

int DownloadSavesList(char* text1, char* text2, char* text3, int* sizeOfRanking);	//loads description of saves and size of ranking