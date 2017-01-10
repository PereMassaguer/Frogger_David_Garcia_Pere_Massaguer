/******************************************************************
* Copyright (C) 2016 Jordi Serrano Berbel <jsberbel95@gmail.com> *
* This can not be copied, modified and/or distributed without    *
* express permission of the copyright owner.                     *
******************************************************************/

#include "GameEngine.hh"

// Game constants
#define SCREEN_WIDTH 420
#define SCREEN_HEIGHT 560

int main(int argc, char* args[]) {
	GameEngine::Run<SCREEN_WIDTH, SCREEN_HEIGHT>("Open Frogger 2017 Pere Massaguer David Garcia");
	
	return 0;
}