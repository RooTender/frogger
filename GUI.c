#include "GUI.h"

#define YES 2
#define NO 1

void drawTextOnCenter(EnvironmentVars* env, const char* text) {
	DrawString(env->screen, env->screen->w / 2 - strlen(text) * 8 / 2, env->screen->h / 2, text, env->charset);
	SDL_UpdateTexture(env->scrtex, NULL, env->screen->pixels, env->screen->pitch);

	SDL_RenderCopy(env->renderer, env->scrtex, NULL, NULL);
	SDL_RenderPresent(env->renderer);
}
int choice_YesNo(EnvironmentVars* env) {
	SDL_Event event;
	int choice = 0;

	while (!choice) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_y) // YES
					choice = YES;
				if (event.key.keysym.sym == SDLK_n) // NO
					choice = NO;
				break;
			}
		}
		env->tick = SDL_GetTicks();
	}

	return --choice;
}

int menu(EnvironmentVars* env) {
	SDL_Event event;
	
	int blue = SDL_MapRGB(env->screen->format, 0x11, 0x11, 0xCC);
	int choice = 0, choosen = 0;

	while (!choosen) {
		DrawRectangle(env->screen, 8, 8, SCREEN_WIDTH - 16, SCREEN_HEIGHT - 16, blue, blue);

		char* text = "New Game";
		DrawString(env->screen, env->screen->w / 2 - strlen(text) * 8 / 2, env->screen->h / 3, text, env->charset);

		text = "Quit";
		DrawString(env->screen, env->screen->w / 2 - strlen(text) * 8 / 2, env->screen->h / 3 + 8 * 3, text, env->charset);

		text = ">            <";
		DrawString(env->screen, env->screen->w / 2 - strlen(text) * 8 / 2, env->screen->h / 3 + 8 * 3 * choice, text, env->charset);

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				if ((event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w) && choice > 0)
					choice--;

				if ((event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s) && choice < 1)
					choice++;

				if (event.key.keysym.sym == SDLK_RETURN)
					choosen = ++choice;

				break;

			default:
				break;
			}
		}
		
		SDL_UpdateTexture(env->scrtex, NULL, env->screen->pixels, env->screen->pitch);
		SDL_RenderClear(env->renderer);
		SDL_RenderCopy(env->renderer, env->scrtex, NULL, NULL);
		SDL_RenderPresent(env->renderer);
	}

	return choosen;
}

void pause(EnvironmentVars* env) {
	SDL_Event event;

	int pause = 1;
	drawTextOnCenter(env, "Paused");

	while (pause) {

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_p)
					pause = 0;
				break;
			}
		}
		env->tick = SDL_GetTicks();
	}
}

int quitQuestion(EnvironmentVars* env) {
	drawTextOnCenter(env, "Quit Game? Y / N");
	return choice_YesNo(env);
}

int gameOver_askForAction(EnvironmentVars* env) {
	drawTextOnCenter(env, "GAME OVER! Restart? Y / N");
	return choice_YesNo(env);
}

int win_askForAction(EnvironmentVars* env, int points) {
	char text[50] = "";
	sprintf(text, "You WON with %d score! Restart? Y / N\0", points);
	drawTextOnCenter(env, text);
	return choice_YesNo(env);
}

void drawPointsOnLockedPosition(EnvironmentVars* env, Object* player, int points) {

	// Max value "text" can handle is 9999
	char text[5] = "";
	sprintf(text, "%d\0", points);

	DrawString(env->screen, player->instance.x + player->instance.w / 2 - strlen(text) * 8 / 2, MESH_SIZE / 2, text, env->charset);
	SDL_UpdateTexture(env->scrtex, NULL, env->screen->pixels, env->screen->pitch);

	SDL_RenderCopy(env->renderer, env->scrtex, NULL, NULL);
}

void drawPoints(EnvironmentVars* env, int points) {
	
	// Max value "text" can handle is 9999
	char text[14] = "";
	sprintf(text, "Points: %d\0", points);

	DrawString(env->screen, env->screen->w / 2 - strlen(text) * 8 / 2, SCREEN_HEIGHT - MESH_SIZE / 2 - 8, text, env->charset);
	SDL_UpdateTexture(env->scrtex, NULL, env->screen->pixels, env->screen->pitch);

	SDL_RenderCopy(env->renderer, env->scrtex, NULL, NULL);
}