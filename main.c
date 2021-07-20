#include "Frogger.h"
#include "GUI.h"

#define CAR_TYPES	5
#define TURTLES		2
#define WOODEN_LOGS 3
#define HEARTS		3

void game(EnvironmentVars* env) {
	
	int quit = 0, restart = 0;
	int black = SDL_MapRGB(env->screen->format, 0x00, 0x00, 0x00);

	// Frogger
	Object* frogger = newObject(env->renderer, "textures/frog.bmp", MESH_SIZE, MESH_SIZE, 0, 0, 0);
	if (!frogger)
		quit = 1;

	// Hearts
	Object* heart_instance = newObject(env->renderer, "textures/heart.bmp", MESH_SIZE / 2, MESH_SIZE / 2, 0, 0, 0);
	Object** hearts_array = newArrayOfObjects(heart_instance, HEARTS, 0);


	int cars_count[CAR_TYPES] = { 7, 6, 7, 4, 5 },
		cars_spawnX[CAR_TYPES] = { 0, GRID_WIDTH / 2, GRID_WIDTH - GRID_WIDTH / 4, GRID_WIDTH / 2, GRID_WIDTH },
		cars_spawnY[CAR_TYPES] = { GRID_HEIGHT - MESH_SIZE, GRID_HEIGHT - 2 * MESH_SIZE, },
		cars_distanceBetweenObjects[CAR_TYPES] = { 3, 4, 3, 5, 3 },
		cars_speed[CAR_TYPES] = { 60, -100, -80, -90, -70 },

		woodenLogs_count[WOODEN_LOGS] = { 6, 3, 4 },
		woodenLogs_spawnX[WOODEN_LOGS] = { 0, 0, 0 },
		woodenLogs_spawnY[WOODEN_LOGS] = { 2 * MESH_SIZE, 5 * MESH_SIZE, 4 * MESH_SIZE },
		woodenLogs_distanceBetweenObjects[WOODEN_LOGS] = { 1, 1, 2 },
		woodenLogs_speed[WOODEN_LOGS] = { 70, -80, 80 },
		
		turtles_count[TURTLES] = { 8, 6 },
		turtles_spawnX[TURTLES] = { 0, 0 },
		turtles_spawnY[TURTLES] = { 3 * MESH_SIZE, 6 * MESH_SIZE },
		turtles_distanceBetweenObjects[TURTLES] = { 1, 1 },
		turtles_speed[TURTLES] = { -50, -90 };

	Object* car_instance[CAR_TYPES];
	Object* woodenLog_instance[WOODEN_LOGS];
	Object* turtle_instance[TURTLES];

	Object** car_array[CAR_TYPES];
	Object** woodenLog_array[WOODEN_LOGS];
	Object** turtle_array[TURTLES];


	/*
		GENERATING OBJECTS
	*/

	// cars
	for (int i = 0; i < CAR_TYPES; i++) {
		int objWidth = (i == 4) ? MESH_SIZE * 2 : MESH_SIZE;

		char tmp[20];
		sprintf(tmp, "textures/car%d.bmp", i + 1);
		car_instance[i] = newObject(env->renderer, tmp, objWidth, MESH_SIZE, cars_spawnX[i], GRID_HEIGHT - (i + 1) * MESH_SIZE, cars_speed[i]);
	}

	for(int i = 0; i < CAR_TYPES; i++)
		car_array[i] = newArrayOfObjects(car_instance[i], cars_count[i], cars_distanceBetweenObjects[i]);


	// woodenLogs
	woodenLog_instance[0] = newObject(env->renderer, "textures/woodenLogSmall.bmp" , MESH_SIZE * 3, MESH_SIZE, woodenLogs_spawnX[0], woodenLogs_spawnY[0], woodenLogs_speed[0]);
	woodenLog_instance[1] = newObject(env->renderer, "textures/woodenLogMedium.bmp", MESH_SIZE * 4, MESH_SIZE, woodenLogs_spawnX[1], woodenLogs_spawnY[1], woodenLogs_speed[1]);
	woodenLog_instance[2] = newObject(env->renderer, "textures/woodenLogBig.bmp"   , MESH_SIZE * 5, MESH_SIZE, woodenLogs_spawnX[2], woodenLogs_spawnY[2], woodenLogs_speed[2]);
	

	for(int i = 0; i < WOODEN_LOGS; i++)
		woodenLog_array[i] = newArrayOfObjects(woodenLog_instance[i], woodenLogs_count[i], woodenLogs_distanceBetweenObjects[i]);

	
	// turtles
	turtle_instance[0] = newObject(env->renderer, "textures/turtles2.bmp", MESH_SIZE * 2, MESH_SIZE, turtles_spawnX[0], turtles_spawnY[0], turtles_speed[0]);
	turtle_instance[1] = newObject(env->renderer, "textures/turtles3.bmp", MESH_SIZE * 3, MESH_SIZE, turtles_spawnX[1], turtles_spawnY[1], turtles_speed[1]);

	for(int i = 0; i < 2; i++)
		turtle_array[i] = newArrayOfObjects(turtle_instance[i], turtles_count[i], turtles_distanceBetweenObjects[i]);

	initDecks();
	

	// Set Frogger to spawn point
	frogger->instance.x = FROGGER_INIT_X;
	frogger->instance.y = FROGGER_INIT_Y;

	int angle = 0, lives = HEARTS, points = 0;
	int lastY = FROGGER_INIT_Y;
	SDL_Event event;

	while(!quit) {

		drawPoints(env, points);

		// Updating FPS
		env->delta = getDeltaTime(&env->tick);
		env->tick = SDL_GetTicks();
		
		SDL_FillRect(env->screen, NULL, black);
		drawTerrain(env->renderer);

		/*
			Rendering static objects
		*/
		renderArrayOfObjects_static(hearts_array, lives);

		/*
			Rendering dynamic objects & collision check
		*/
		for (int i = 0; i < CAR_TYPES; i++) {
			renderArrayOfObjects_dynamic(car_array[i], cars_count[i], env->delta, cars_speed[i]);

			if (isCollisionWithArrayOfObjects(car_array[i], cars_count[i], frogger))
				death(frogger, &lives, &angle);
		}

		for (int i = 0; i < WOODEN_LOGS; i++) {
			renderArrayOfObjects_dynamic(woodenLog_array[i], woodenLogs_count[i], env->delta, woodenLogs_speed[i]);

			if (isCollisionWithArrayOfObjects(woodenLog_array[i], woodenLogs_count[i], frogger))
				moveObject(frogger, env->delta, woodenLogs_speed[i]);
			else
				// If frogger not on wooden log and has the same Y -> then it's in water
				if (woodenLogs_spawnY[i] == frogger->instance.y)
					death(frogger, &lives, &angle);
		}

		for (int i = 0; i < TURTLES; i++) {
			renderArrayOfObjects_dynamic(turtle_array[i], turtles_count[i], env->delta, turtles_speed[i]);

			if (isCollisionWithArrayOfObjects(turtle_array[i], turtles_count[i], frogger))
				moveObject(frogger, env->delta, turtles_speed[i]);
			else
				// If frogger not on turtle and has the same Y -> then it's in water
				if (turtles_spawnY[i] == frogger->instance.y)
					death(frogger, &lives, &angle);
		}

		renderObjectWithAngle(frogger, angle);

		SDL_RenderPresent(env->renderer);

		if (isPlayerOutsideScreen(frogger))
			death(frogger, &lives, &angle);

		// handling of events (if there were any)
		while(SDL_PollEvent(&event)) {
			switch(event.type) {

				case SDL_KEYDOWN:

					// UP
					if ((event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
						&& frogger->instance.y > MESH_SIZE) {
						frogger->instance.y -= MESH_SIZE;
						angle = 0;
					}

					// RIGHT
					else if ((event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
						&& frogger->instance.x < GRID_WIDTH - frogger->instance.w) {
						frogger->instance.x += MESH_SIZE;
						angle = 90;
					}
					
					// DOWN
					else if ((event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
						&& frogger->instance.y < GRID_HEIGHT) {
						frogger->instance.y += MESH_SIZE;
						angle = 180;
					}

					// LEFT
					else if ((event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
						&& frogger->instance.x > 0) {
						frogger->instance.x -= MESH_SIZE;
						angle = 270;
					}

					// PAUSE
					else if (event.key.keysym.sym == SDLK_p) {
						pause(env);
					}

					// QUIT
					else if (event.key.keysym.sym == SDLK_q) {
						if (quitQuestion(env))
							quit = 1;
					}

					frogger->floatX = frogger->instance.x;
					break;

				case SDL_QUIT:
					quit = 1;
					break;
			};
		};

		if (frogger->instance.y < lastY && frogger->instance.y != MESH_SIZE) {
			points += 10;
			lastY = frogger->instance.y;
		}
			
		// In case if frogger is below the river -> align his position to fit the grid
		alignPlayerPosition(frogger);
		
		switch (checkFinish(frogger)) {
		case 1:
			points += 50;
			lastY = FROGGER_INIT_Y;
			break;
		case 2:
			death(frogger, &lives, &angle);
			break;
		case 3:
			if (win_askForAction(env, points))
				restart = 1;
			quit = 1;
			break;
		default:
			break;
		}
		

		// Game Over
		if (lives <= 0) {
			if (gameOver_askForAction(env))
				restart = 1;
			quit = 1;
		}

		SDL_RenderClear(env->renderer);
	};

	env->screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	
	// Destroy simple objects
	destroyObject(frogger);
	destroyObject(heart_instance);

	// Destroy objects
	destroyObjectsFromArray_1D(hearts_array, HEARTS);

	destroyObjectsFromArray_1D(car_instance, CAR_TYPES);
	destroyObjectsFromArray_2D(car_array, cars_count, CAR_TYPES);

	destroyObjectsFromArray_1D(woodenLog_instance, 3);
	destroyObjectsFromArray_2D(woodenLog_array, woodenLogs_count, 3);

	destroyObjectsFromArray_1D(turtle_instance, 2);
	destroyObjectsFromArray_2D(turtle_array, turtles_count, 2);
	
	if (restart)
		game(env);
};


int main(int argc, char** argv) {
	
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}

	EnvironmentVars sdlEnv;
	sdlEnv.tick = 0;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}

	if (SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &sdlEnv.window, &sdlEnv.renderer) != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return 1;
	};

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(sdlEnv.renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(sdlEnv.renderer, 0, 0, 0, 255);

	SDL_SetWindowTitle(sdlEnv.window, "Frogger");

	sdlEnv.screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	sdlEnv.scrtex = SDL_CreateTexture(sdlEnv.renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		SCREEN_WIDTH, SCREEN_HEIGHT);

	SDL_ShowCursor(SDL_DISABLE);

	sdlEnv.charset = SDL_LoadBMP("cs8x8.bmp");
	if (sdlEnv.charset == NULL) {
		printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(sdlEnv.screen);
		SDL_DestroyTexture(sdlEnv.scrtex);
		SDL_DestroyWindow(sdlEnv.window);
		SDL_DestroyRenderer(sdlEnv.renderer);
		SDL_Quit();
		return 1;
	};
	SDL_SetColorKey(sdlEnv.charset, 1, 0x000000);

	int quit = 0;
	while(!quit)
		switch (menu(&sdlEnv)) {
		case 1:
			game(&sdlEnv);
			break;
		default:
			quit = 1;
			break;
		}

	// freeing all surfaces
	SDL_FreeSurface(sdlEnv.charset);
	SDL_FreeSurface(sdlEnv.screen);

	SDL_DestroyTexture(sdlEnv.scrtex);
	SDL_DestroyRenderer(sdlEnv.renderer);
	SDL_DestroyWindow(sdlEnv.window);

	SDL_Quit();
	return 0;
}