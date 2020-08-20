#include <stdio.h>
#include <SDL2/SDL.h>
#include "headers.h"

int main(){
	// Initialisation
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Init(SDL_INIT_VIDEO);
	Bool isRunning = TRUE;
	const int SCREEN_WIDTH = 704, SCREEN_HEIGHT = 660;
	window = SDL_CreateWindow("Tower Defense", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
	if(window == NULL){
		printf("[ERREUR] La fenêtre ne peut pas être crée: %s\n", SDL_GetError());
		return 1;
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(renderer == NULL){
		printf("[ERREUR] Le renderer n'a pas pu être créer\n");
		return 1;
	}
	// Initialisation de la couleur de fond et affichage
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
	// Initialisation pour les FPS
	const int FPS = 60;
	int frameTime = 0;
	unsigned int lastTime = 0, currTime;
	float deltaTime = 0;
	// Initialisation propre au jeu
	TileList tiles[400];
	TileList tilesChoose[10];
	int x, y = 0, count_x = 0, count_y = 0, count = 0;
	int selectedID = 0, oldSelectedID = 0, selectedIDC = 0, oldSelectedIDC = 0;
	for(count_x = 0; count_x < 400; count_x++){
		Tile tile = {x, y, "ground", LoadTexture("textures/ground.bmp", renderer), FALSE, FALSE};
		SDL_Rect tileRect = {tile.x, tile.y, 32, 32};
		tiles[count_x].tile = tile;
		tiles[count_x].tileRect = tileRect;
		tiles[count_x].texture = tile.texture;
		if(count == 19){
			y += 32;
			x = 0;
			count = 0;
		}else{
			x += 32;
			count++;
		}
	}
	tiles[0].texture = tiles[0].tile.texture = LoadTexture("textures/place.bmp", renderer);
	tiles[0].tile.name = "place";
	tiles[0].tile.click =  TRUE;
	tiles[131].texture = tiles[131].tile.texture = LoadTexture("textures/place.bmp", renderer);
	tiles[131].tile.name = "place";
	tiles[131].tile.click =  TRUE;
	y = 0;
	char texturePlaceholder[] = "textures/X.bmp";
	for(count_y = 0; count_y < 10; count_y++){
		texturePlaceholder[9] = count_y + '0';
		Tile tile = {640, y, "choose", LoadTexture(texturePlaceholder, renderer), TRUE, FALSE};
		SDL_Rect tileRect = {tile.x, tile.y, 64, 64};
		tilesChoose[count_y].tile = tile;
		tilesChoose[count_y].tileRect = tileRect;
		tilesChoose[count_y].texture = tile.texture;
		y += 64;
	}
	tilesChoose[selectedIDC].tile.selected = TRUE;
	Tile testTile = {0, 0, "bullet", LoadTexture("textures/bullet.bmp", renderer), FALSE, FALSE};
	SDL_Rect test = {testTile.x, testTile.y, 8, 8};
	SDL_RenderPresent(renderer);
	// Boucle principale
	while(isRunning){
		// Gérer les FPS
		//prevTime = currTime;
		//currTime = SDL_GetTicks();
		//deltaTime = (float)((currTime - prevTime) / 1000.0);
		// Gérer les événements SDL
		SDL_Event event;
		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_WINDOWEVENT:
					// Evénements de fenêtre
					switch(event.window.event){
						case SDL_WINDOWEVENT_RESIZED:
							SDL_RenderClear(renderer);
							SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
							SDL_RenderPresent(renderer);
							break;
					}
					break;
				case SDL_QUIT:
					isRunning = FALSE;
					break;
				case SDL_KEYDOWN:
					// Evénements du clavier
					switch(event.key.keysym.sym){
						case SDLK_a:
							if(tilesChoose[selectedIDC].tile.selected && tiles[selectedID].tile.selected && tiles[selectedID].texture != tilesChoose[selectedIDC].texture){
								tiles[selectedID].texture = tilesChoose[selectedIDC].texture;
								tiles[selectedID].tile.selected = FALSE;
							}
							break;
						case SDLK_e:
							if(tilesChoose[selectedIDC].tile.selected && tiles[selectedID].tile.selected && tiles[selectedID].texture != tiles[selectedID].tile.texture){
								tiles[selectedID].texture = tiles[selectedID].tile.texture;
								tiles[selectedID].tile.selected = FALSE;
							}
							break;
					}
					break;
				case SDL_MOUSEBUTTONUP:
					// Evénements de la souris
					if(event.button.button == SDL_BUTTON_LEFT){
						for(x = 0; x < 400; x++){
							if(tiles[x].tile.click && InnerRect(event.button.x, event.button.y, &tiles[x].tileRect)){
								oldSelectedID = selectedID;
								tiles[x].tile.selected = TRUE;
								selectedID = x;
								if(selectedID != oldSelectedID)
									tiles[oldSelectedID].tile.selected = FALSE;
							}
						}
						for(x = 0; x < 10; x++){
							if(tilesChoose[x].tile.click && InnerRect(event.button.x, event.button.y, &tilesChoose[x].tileRect)){
								oldSelectedIDC = selectedIDC;
								tilesChoose[x].tile.selected = TRUE;
								selectedIDC = x;
								if(selectedIDC != oldSelectedIDC)
									tilesChoose[oldSelectedIDC].tile.selected = FALSE;
							}
						}
					}else if(event.button.button == SDL_BUTTON_RIGHT){
						tiles[selectedID].tile.selected = FALSE;
					}
					break;
			}
		}
		// Pré-mise à jour de l'écran
		SDL_RenderClear(renderer);
		// Mise à jour de l'écran
		int temp = -1;
		for(x = 0; x < 400; x++){
			SDL_RenderCopy(renderer, tiles[x].texture, NULL, &tiles[x].tileRect);
			if(selectedID == x && tiles[x].tile.selected){
				temp = x;
				SDL_RenderDrawRect(renderer, &tiles[x].tileRect);
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			}
		}
		if(temp != -1 && tiles[temp].tile.name == "place"){
			SDL_Rect placeHolder = {tiles[temp].tileRect.x - 16, tiles[temp].tileRect.y - 16, 64, 64};
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 70);
			SDL_RenderFillRect(renderer, &placeHolder);
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		}
		for(x = 0; x < 10; x++){
			SDL_RenderCopy(renderer, tilesChoose[x].texture, NULL, &tilesChoose[x].tileRect);
			if(selectedIDC == x && tilesChoose[x].tile.selected){
				SDL_RenderDrawRect(renderer, &tilesChoose[x].tileRect);
			}
		}
		// Gèrer les FPS
		/*frameTime++;
		if(FPS / frameTime == 15){
			frameTime = 0;
			//test.x += (float)400 * deltaTime;
			//test.y += (float)400 * deltaTime;
		}*/
		//SDL_RenderCopy(renderer, testTile.texture, NULL, &test);
		SDL_RenderPresent(renderer);
		currTime = SDL_GetTicks();
		/*if(currTime > lastTime + 1000)
			lastTime = currTime;*/
	}
	// Fermeture de la fenêtre
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

Bool InnerRect(int x, int y, SDL_Rect* rect){
	if(x >= rect->x && x < rect->x + rect->w && y >= rect->y && y < rect->y + rect->h){
		return TRUE;
	}else{
		return FALSE;
	}
}

// Fonction de création des texture à partir de fichiers .bmp
SDL_Texture *LoadTexture(char* file, SDL_Renderer *renderer){
	SDL_Texture *texture = NULL;
	SDL_Surface *surface = SDL_LoadBMP(file);
	if(surface == NULL){
		printf("[ERREUR] Le fichier %s ne peut pas être chargé\n", file);
	}else{
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		if(texture == NULL)
			printf("[ERREUR] La texture ne peut pas être crée\n");
	}
	SDL_FreeSurface(surface);
	return texture;
}
