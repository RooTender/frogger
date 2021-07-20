#include "Frogger.h"

void initDecks()
{
	for (int i = 0; i < DECKS; i++)
		decks[i] = 0;
}

Object* newObjectBase(SDL_Renderer* renderer, int width, int height, int posX, int posY, int movementSpeed) {
	Object* obj = malloc(sizeof(Object));

	if (obj) {
		obj->instance = CreateRect(posX, posY, width, height);
		obj->floatX = posX;
		setRawData(renderer, obj, posX, posY, movementSpeed);

		return obj;
	}

	return NULL;
}
Object* newObject(SDL_Renderer* renderer, char* textureSRC, int width, int height, int posX, int posY, int movementSpeed)
{
	Object* obj = newObjectBase(renderer, width, height, posX, posY, movementSpeed);

	if (obj) {
		obj->texture = GetTexture(renderer, textureSRC);
		if (!obj->texture)
			return NULL;

		return obj;
	}

	return NULL;
}
Object* newObjectWTX(SDL_Renderer* renderer, SDL_Texture* texture, int width, int height, int posX, int posY, int movementSpeed)
{
	Object* obj = newObjectBase(renderer, width, height, posX, posY, movementSpeed);

	if (obj) {
		obj->texture = texture;
		return obj;
	}

	return NULL;
}
void setRawData(SDL_Renderer* renderer, Object* object, int spawnX, int spawnY, int movementSpeed)
{
	RawData* data = &object->rData;
	data->renderer = renderer;

	data->spawnX = spawnX;
	data->spawnY = spawnY;
	data->movementSpeed = movementSpeed;
}
Object** newArrayOfObjects(Object* object, int arrSize, int distanceBetweenObjects)
{
	Object** arr = malloc(arrSize * sizeof(Object*));

	for (int i = 0; i < arrSize; i++) {
		if (arr) {
			arr[i] = newObjectWTX(object->rData.renderer, object->texture, object->instance.w, object->instance.h,
								  object->rData.spawnX + i * (distanceBetweenObjects + 1) * object->instance.w,
								  object->rData.spawnY, object->rData.movementSpeed);
		}
	}
	return arr;
}

// rendering
void renderObject(Object* object)
{
	SDL_RenderCopy(object->rData.renderer, object->texture, NULL, &object->instance);
}
void renderObjectWithAngle(Object* object, int angle)
{
	SDL_RenderCopyEx(object->rData.renderer, object->texture, NULL, &object->instance, angle, NULL, SDL_FLIP_NONE);
}

void renderArrayOfObjects_static(Object** arrayOfObjects, int arrSize)
{
	for (int i = 0; i < arrSize; i++) {
		renderObject(arrayOfObjects[i]);
	}
}
void renderArrayOfObjects_dynamic(Object** arrayOfObjects, int arrSize, double delta, int deltaMultiplier) {

	for (int i = 0; i < arrSize; i++) {
		moveObject(arrayOfObjects[i], delta, deltaMultiplier);
	}
	renderArrayOfObjects_static(arrayOfObjects, arrSize);
}


// movement
void moveObject(Object* obj, double delta, double multiplier)
{
	obj->floatX += delta * multiplier;
	obj->instance.x = (int)obj->floatX;

	int beyondNextScreenValue = (GRID_WIDTH * 2 + obj->instance.w);
	if (obj->instance.x > beyondNextScreenValue) {
		obj->floatX -= beyondNextScreenValue + (double)(obj->instance.w);
		obj->instance.x -= beyondNextScreenValue + obj->instance.w;
	}

	if (obj->instance.x < -obj->instance.w) {
		obj->floatX += (double)(beyondNextScreenValue);
		obj->instance.x += beyondNextScreenValue;
	}
}
int isCollision(Object* obj1, Object* obj2)
{
	if (obj1 == NULL || obj2 == NULL)
		return 0;

	if (obj1->instance.y == obj2->instance.y &&
		(obj1->instance.x + obj1->instance.w) > obj2->instance.x &&
		((obj1->instance.x) < (obj2->instance.x + obj2->instance.w)))
		return 1;
	return 0;
}
int isCollisionWithArrayOfObjects(Object** arrayOfObjects, int arrSize, Object* objForColisionCheck) {
	for (int i = 0; i < arrSize; i++) {
		if (isCollision(objForColisionCheck, arrayOfObjects[i]))
			return 1;
	}
	return 0;
}

int isPlayerOutsideScreen(Object* player) {
	if (player->instance.x < 0 ||
		player->instance.x > GRID_WIDTH - player->instance.w ||

		player->instance.y < MESH_SIZE ||
		player->instance.y > GRID_HEIGHT)
		
		return 1;
	return 0;
}

void death(Object* player, int* lives, int* playerAngle)
{
	player->instance.x = FROGGER_INIT_X;
	player->instance.y = FROGGER_INIT_Y;
	*playerAngle = 0;
	(*lives)--;
}

// destroy
void destroyObject(Object* object)
{
	if (object->texture)
		SDL_DestroyTexture(object->texture);
	
	object->rData.renderer = 0;
	object->rData.spawnX = 0;
	object->rData.spawnY = 0;
	object->rData.movementSpeed = 0;

	free(object);
}
void destroyObjectsFromArray_1D(Object** arrOfObjects, int arrSize)
{
	for (int i = 0; i < arrSize; i++)
		destroyObject(arrOfObjects[i]);

}
void destroyObjectsFromArray_2D(Object*** arrOfObjects, int* objectsInRow, int arrSize)
{
	for (int i = 0; i < arrSize; i++)
		destroyObjectsFromArray_1D(arrOfObjects[i], objectsInRow[i]);

	for (int i = 0; i < arrSize; i++)
		free(arrOfObjects[i]);

}

// draw functions
void drawTiles(SDL_Renderer* renderer, char* textureSRC, SDL_Rect objectRect) {
	SDL_Texture* texture = GetTexture(renderer, textureSRC);

	SDL_RenderCopy(renderer, texture, NULL, &objectRect);
	SDL_DestroyTexture(texture);
}
void drawTerrain(SDL_Renderer* renderer) {
	SDL_Rect objRect;
	objRect.h = MESH_SIZE;
	objRect.w = MESH_SIZE * (DIMENSIONS);
	objRect.x = 0;

	// Generate purpleTiles
	objRect.y = GRID_HEIGHT;
	drawTiles(renderer, "textures/purpleTile.bmp", objRect);
	
	objRect.y -= 6 * MESH_SIZE;
	drawTiles(renderer, "textures/purpleTile.bmp", objRect);

	// Generate endGrass
	objRect.y = MESH_SIZE;
	drawTiles(renderer, "textures/end.bmp", objRect);

	// Generate waterTiles
	objRect.y += MESH_SIZE;
	objRect.h = MESH_SIZE * 5;
	drawTiles(renderer, "textures/water.bmp", objRect);

	// Draw froggers in decks
	objRect.y = MESH_SIZE;
	objRect.h = MESH_SIZE;
	objRect.w = MESH_SIZE;
	for (int i = 0; i < 5; i++)
		if (decks[i]) {
			objRect.x = MESH_SIZE / 2 + i * 3 * MESH_SIZE;
			drawTiles(renderer, "textures/frog.bmp", objRect);
		}
}

void alignPlayerPosition(Object* obj) {
	if (obj->instance.y > 6 * MESH_SIZE) {
		obj->instance.x -= obj->instance.x % MESH_SIZE;
		obj->floatX = obj->instance.x;
	}
}

int checkFinish(Object* obj)
{
	int win = 1;
	for(int i = 0; i < 5; i++)
		if(!decks[i])
			win = 0;

	if (win)
		return 3;

	if (obj->instance.y == MESH_SIZE) {
		for (int i = 0; i < 5; i++) {
			if ((abs(obj->instance.x - (MESH_SIZE / 2 + i * 3 * MESH_SIZE)) < MESH_SIZE / 2) && decks[i] == 0) {
				decks[i] = 1;

				// Win situation. Next round -> teleport frogger to init XY 
				obj->instance.x = FROGGER_INIT_X;
				obj->instance.y = FROGGER_INIT_Y;


				return 1;
			}
		}
		return 2;
	}
	return 0;
}
