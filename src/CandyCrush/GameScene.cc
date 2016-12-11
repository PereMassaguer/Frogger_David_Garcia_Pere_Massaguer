/******************************************************************
* Copyright (C) 2016 Jordi Serrano Berbel <jsberbel95@gmail.com> *
* This can not be copied, modified and/or distributed without    *
* express permission of the copyright owner.                     *
******************************************************************/

#include "GameScene.hh"
#include "MainMenuScene.hh"

using namespace Logger;

#define CELL_WIDTH 80
#define CELL_HEIGHT 80

void continueGame() {
	isPaused = false;
}

void exitGame() {
	isPaused = true;
	SM.SetCurScene<MainMenuScene>();
}

GameScene::GameScene(void)/* : m_grid("lvl/testLvl.dat", CELL_WIDTH, CELL_HEIGHT)*/{
	m_background = { { 0, 0, W.GetWidth(), W.GetHeight() }, ObjectID::BGFrogger };
	isPaused = false;

	Button continueButton, exitButton;
	continueButton = Button("CONTINUE", Transform(W.GetWidth() / 2, (int)(W.GetHeight() * 0.4f), 1, 1), WHITE);
	exitButton = Button("EXIT", Transform(W.GetWidth() / 2, (int)(W.GetHeight() * 0.75f), 1, 1), WHITE);

	continueButton.SetButtonBehavior(continueGame);
	exitButton.SetButtonBehavior(exitGame);

	pauseButtons.push_back(continueButton);
	pauseButtons.push_back(exitButton);
}

GameScene::~GameScene(void){
}

void GameScene::OnEntry(void) {
	IOManager::LevelParameters("cfg/FroggerLevelSettings.xml", SM.GetCurDifficulty(), hpLeft, velocity, velocityMod);
}

void GameScene::OnExit(void) {
}

void GameScene::Update(void) {
	if (IM.IsKeyDown<SDLK_ESCAPE>()) isPaused = !isPaused;
	if (!isPaused) {//doBehaviors
		
	}
	if (IM.IsMouseDown<MOUSE_BUTTON_LEFT>()) {
		for (auto it : pauseButtons)
			if (it.IsMoused()) it.ExecuteBehavior();
	}
}

void GameScene::Draw(void) {
	m_background.Draw(); // Render background

	if (isPaused) {
		GUI::DrawRectangle(std::make_pair(W.GetWidth() * 0.75f, W.GetHeight() * 0.75f), BLACK);
		GUI::DrawTextBlended<FontID::ARIAL>("Game paused", Transform((int)(W.GetWidth() / 2), (int)(W.GetHeight()*0.25f), 1, 1), WHITE);
		for (auto it : pauseButtons) it.DrawButton();
	}

	//m_grid.Draw(); // Render grid
	GUI::DrawTextBlended<FontID::ARIAL>("Score: " + std::to_string(m_score), 
	{ 60, int(W.GetHeight()*.95f), 1, 1 }, WHITE); // Render score that will be different when updated
}
