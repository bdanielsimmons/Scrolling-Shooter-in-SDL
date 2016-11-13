// Faux Galaga.cpp : Defines the entry point for the console application.
#include <string>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <stdlib.h>
#include <ctime>
#include <vector>
#include "Constants.h"
#include "Player.h"
#include "Enemy.h"
#include "Projectile.h"

bool running = true;
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren);
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int, int);
SDL_Event e;

int f, j, randow, randoh;

int main(int, char**) {
	SDL_Init(SDL_INIT_EVERYTHING);
	Mix_Chunk* Laser;
	Mix_Music* Song;
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
	Laser = Mix_LoadWAV("laser.wav");
	Song = Mix_LoadMUS("sunnings.mp3");
	if (Song == NULL) {
		std::cout << Mix_GetError();
	}

	Player test(950,530);
	int bW, bH;
	SDL_Window *window = SDL_CreateWindow("ScrollMaster", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_Texture *background = loadTexture("background.bmp", renderer);
	SDL_Texture *background2 = loadTexture("space12.bmp", renderer);
	SDL_Texture *image = loadTexture("spaceship.bmp", renderer);
	SDL_QueryTexture(background, NULL, NULL, &bW, &bH);
	SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	/*for (i = 0; i < NUM_TILES; ++i) tile_list[i].draw()
	SDL_Texture* target_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 10, 10);
	SDL_SetRenderTarget(renderer, target_texture);*/

	Mix_PlayMusic(Song, -1);

	SDL_Event e;
	while (running) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				running = false;
			}
		}
		auto keys = SDL_GetKeyboardState(NULL);
		test.Update(keys, Laser, Song);
		camera.x = (test.getX() + PLAYER_WIDTH / 2) - SCREEN_WIDTH / 2;
		camera.y = (test.getY() + PLAYER_HEIGHT / 2) - SCREEN_HEIGHT / 2;

		if (camera.x < 0) camera.x = 0;
		if (camera.y < 0) camera.y = 0;
		if (camera.x > LEVEL_WIDTH - camera.w) camera.x = LEVEL_WIDTH - camera.w;
		if (camera.y > LEVEL_HEIGHT - camera.h) camera.y = LEVEL_HEIGHT - camera.h;
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, background, &camera, NULL);
		f = SDL_GetTicks();
		if (f > j + ((rand() % 51)) + 15) {
			//randow = (rand() % ((LEVEL_WIDTH + 1)));
			randoh = (rand() % ((LEVEL_HEIGHT + 1)));
			Enemy::createEnemy(LEVEL_WIDTH/*randow*/, randoh, 0, 0, ENEMY_W, ENEMY_H);
		}
		j = f;
		Projectile::Update();
		Enemy::Update(test.getX(), test.getY(), test.getW(), test.getH());
		Projectile::Draw(renderer, camera.x, camera.y);
		Enemy::Draw(renderer, camera.x, camera.y);
		test.Draw(renderer, camera.x, camera.y);
		SDL_RenderPresent(renderer);
		SDL_Delay(1);
	}
	Mix_FreeChunk(Laser);
	Mix_FreeMusic(Song);
	Mix_CloseAudio();
	SDL_Quit();
	return 0;
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y) {
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	//Query the texture to get its width and height to use
	SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(ren, tex, NULL, &dst);
}


SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren) {
	//Initialize to nullptr to avoid dangling pointer issues
	SDL_Texture *texture = nullptr;
	SDL_Surface *loadedImage = SDL_LoadBMP(file.c_str());
	texture = SDL_CreateTextureFromSurface(ren, loadedImage);
	SDL_FreeSurface(loadedImage);
	return texture;
}