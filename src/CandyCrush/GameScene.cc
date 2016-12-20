/******************************************************************
* Copyright (C) 2016 Jordi Serrano Berbel <jsberbel95@gmail.com> *
* This can not be copied, modified and/or distributed without    *
* express permission of the copyright owner.                     *
******************************************************************/

#include "GameScene.hh"
#include "MainMenuScene.hh"

using namespace Logger;


void continueGame() {
	isPaused = false;
}


void exitGame() {
	isPaused = true;
	SM.SetCurScene<MainMenuScene>();
}


GameScene::GameScene(void) : m_grid(){
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
	Element *auxElement = new Element(1, 2);
	spawnables.push_back(auxElement);
}


void GameScene::OnExit(void) {
}

void GameScene::Update(void) {
	detectControls();

	if (!isPaused) {//doBehaviors
		for (int i = 0; i < spawnables.size(); i++) {			
			spawnables[i]->Update();
			if (spawnables[i]->ExitedMap())	spawnables.erase(spawnables.begin() + i);			
		}
	}

	//if (m_grid.GetBehavior(player.GetCoords()) == BehaviorID::RIP) 
		//std::cout << "water detected" << std::endl;
}


void GameScene::Draw(void) {
	m_background.Draw(); // Render background

	player.Draw();
	for (auto it : spawnables) it->Draw();



	if (isPaused) {
		GUI::DrawRectangle(std::make_pair(W.GetWidth() * 0.75f, W.GetHeight() * 0.75f), BLACK);
		GUI::DrawTextBlended<FontID::ARIAL>("Game paused", Transform((int)(W.GetWidth() / 2), (int)(W.GetHeight()*0.25f), 1, 1), WHITE);
		for (auto it : pauseButtons) it.DrawButton();
	}

	//m_grid.Draw(); // Render grid
	GUI::DrawTextBlended<FontID::ARIAL>("Score: " + std::to_string(m_score), 
	{ 60, int(W.GetHeight()*.95f), 1, 1 }, WHITE); // Render score that will be different when updated
}


void GameScene::detectControls()
{
	if (IM.IsKeyDown<SDLK_ESCAPE>()) isPaused = !isPaused;

	if (IM.IsMouseDown<MOUSE_BUTTON_LEFT>())
		for (auto it : pauseButtons) if (it.IsMoused()) it.ExecuteBehavior();

	if (IM.IsKeyDown<KEY_BUTTON_UP>()) player.MoveUp();

	if (IM.IsKeyDown<KEY_BUTTON_DOWN>()) player.MoveDown();

	if (IM.IsKeyDown<KEY_BUTTON_LEFT>()) player.MoveLeft();

	if (IM.IsKeyDown<KEY_BUTTON_RIGHT>()) player.MoveRight();
}
