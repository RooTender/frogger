#pragma once
#include "Frogger.h"

int menu(EnvironmentVars* env);

void pause(EnvironmentVars* env);

int quitQuestion(EnvironmentVars* env);

int gameOver_askForAction(EnvironmentVars* env);

int win_askForAction(EnvironmentVars* env, int points);

void drawPoints(EnvironmentVars* env, int points);

void drawPointsOnLockedPosition(EnvironmentVars* env, Object* player, int points);