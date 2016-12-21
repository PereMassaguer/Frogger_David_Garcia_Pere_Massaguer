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
	debugGrid = { { 0, 30, W.GetWidth(), W.GetHeight() }, ObjectID::GridDebug };
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
	DetectControls();

	if (!isPaused) {//doBehaviors
		std::vector<std::pair<Coord, BehaviorID>> behaviors;
		m_grid.ResetGrid();
		for (int i = 0; i < spawnables.size(); i++) {			
			spawnables[i]->Update();
			if (spawnables[i]->ExitedMap())	spawnables.erase(spawnables.begin() + i);
			else { 
				behaviors = spawnables[i]->CoordBehavior();
				for (int i = 0; i < behaviors.size(); i++)  m_grid.SetBehavior(behaviors[i].first, behaviors[i].second);
			}
		}
	}

	switch (m_grid.GetBehavior(Coord(player.GetCoords()))) {
		case BehaviorID::SAFE:

			break;
		case BehaviorID::RIP:
			hpLeft--;
			if(hpLeft <= 0) std::cout << "ur det fegg" << std::endl; 
			//TODO call end over scene
			break;
		default:
			std::cout << "Behavior not implemented" << std::endl;
			break;
	}
	
	m_grid.DebugGrid(500);
	//if (m_grid.GetBehavior(player.GetCoords()) == BehaviorID::RIP) std::cout << "water detected" << std::endl;
}


void GameScene::Draw(void) {

	m_background.Draw(); //Render background
	debugGrid.Draw(); //Debug Grid
	GUI::DrawTextBlended<FontID::ARIAL>("Score: " + std::to_string(m_score),
	{ 60, int(W.GetHeight()*.95f), 1, 1 }, WHITE); // Render score that will be different when updated

	player.Draw();
	for (auto it : spawnables) it->Draw();

	if (isPaused) {
		GUI::DrawRectangle(std::make_pair(W.GetWidth() * 0.75f, W.GetHeight() * 0.75f), BLACK);
		GUI::DrawTextBlended<FontID::ARIAL>("Game paused", Transform((int)(W.GetWidth() / 2), (int)(W.GetHeight()*0.25f), 1, 1), WHITE);
		for (auto it : pauseButtons) it.DrawButton();
	}
}


void GameScene::DetectControls()
{
	if (IM.IsKeyDown<SDLK_ESCAPE>()) {
		isPaused = !isPaused;
		if (!isPaused) TM.FPSBegin();
	}


	if (!isPaused) {
		if (IM.IsKeyDown<KEY_BUTTON_UP>()) player.MoveUp();
		if (IM.IsKeyDown<KEY_BUTTON_DOWN>()) player.MoveDown();
		if (IM.IsKeyDown<KEY_BUTTON_LEFT>()) player.MoveLeft();
		if (IM.IsKeyDown<KEY_BUTTON_RIGHT>()) player.MoveRight();
	}
	else {
		if (IM.IsMouseDown<MOUSE_BUTTON_LEFT>())
			for (auto it : pauseButtons) if (it.IsMoused()) it.ExecuteBehavior();
		//TODO Arrows menu
	}
}
