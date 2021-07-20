#pragma once
#include "SDL_enhanced.h"

#define MESH_SIZE		64
#define DIMENSIONS		14
#define DECKS			5

#define GRID_WIDTH		DIMENSIONS * MESH_SIZE
#define GRID_HEIGHT		(DIMENSIONS - 1) * MESH_SIZE
#define SCREEN_WIDTH	GRID_WIDTH
#define SCREEN_HEIGHT	GRID_HEIGHT + 2 * MESH_SIZE

#define FROGGER_INIT_X	MESH_SIZE * 7
#define FROGGER_INIT_Y	GRID_WIDTH - MESH_SIZE

typedef struct {
	SDL_Renderer* renderer;
	int spawnX, spawnY;
	int movementSpeed;
} RawData;

typedef struct {
	SDL_Rect instance;
	SDL_Texture* texture;
	double floatX;
	RawData rData;
} Object;

typedef struct {
	int count;
	int distanceBetweenObjects;
} ObjectsInArrayData;

int decks[DECKS];

// Initializes winning decks for Frogger
void initDecks();

Object* newObject(SDL_Renderer* renderer, char* textureSRC, int width, int height, int posX, int posY, int movementSpeed);

// Create new object WITHOUT info about PATH to texture
// Use it if textures are already included
Object* newObjectWTX(SDL_Renderer* renderer, SDL_Texture* texture, int width, int height, int posX, int posY, int movementSpeed);

void setRawData(SDL_Renderer* renderer, Object* object, int spawnX, int spawnY, int movementSpeed);

Object** newArrayOfObjects(Object* object, int arrSize, int distanceBetweenObjects);

void renderObject(Object* object);
void renderObjectWithAngle(Object* object, int angle);

void renderArrayOfObjects_static(Object** arrayOfObjects, int arrSize);
void renderArrayOfObjects_dynamic(Object** arrayOfObjects, int arrSize, double delta, int deltaMultiplier);

// 'multiplier' can be interpreted as speed
void moveObject(Object* obj, double delta, double multiplier);

int isCollision(Object* obj1, Object* obj2);
int isCollisionWithArrayOfObjects(Object** arrayOfObjects, int arrSize, Object* objForColisionCheck);
int isPlayerOutsideScreen(Object* player);
void death(Object* player, int* lives, int* playerAngle);

void destroyObject(Object* object);
void destroyObjectsFromArray_1D(Object** arrOfObjects, int arrSize);
void destroyObjectsFromArray_2D(Object*** arrOfObjects, int* objectsInRow, int arrSize);

void drawTerrain(SDL_Renderer* renderer);

void alignPlayerPosition(Object* obj);

// Returns 0 if frogger isn't even near the end
// Returns 1 if frogger has locked his position
// Returns 2 if frogger crashed on the end
// Returns 3 if frogger has locked ALL decks
int checkFinish(Object* obj);