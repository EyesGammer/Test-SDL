typedef enum Bool Bool;
enum Bool{
	FALSE = 0, TRUE = 1
};
typedef struct Tile Tile;
struct Tile{
	int x;
	int y;
	char* name;
	SDL_Texture *texture;
	Bool click;
	Bool selected;
};
typedef struct TileList TileList;
struct TileList{
	Tile tile;
	SDL_Rect tileRect;
	SDL_Texture *texture;
};
SDL_Texture *LoadTexture(char* file, SDL_Renderer *renderer);
Bool InnerRect(int x, int y, SDL_Rect* rect);
