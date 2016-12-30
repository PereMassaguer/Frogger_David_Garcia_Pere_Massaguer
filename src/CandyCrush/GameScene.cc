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

	finishPoints[0].first = Coord(38, 80);
	finishPoints[1].first = Coord(122, 80);
	finishPoints[2].first = Coord(211, 80);
	finishPoints[3].first = Coord(298, 80);
	finishPoints[4].first = Coord(382, 80);
	for (auto it : finishPoints) it.second = false;

	carAmount = 10;
	cars = new Element[carAmount];
	for (int i = 0; i < carAmount; i++) cars[i] = Car(rand() % 5 + 6);
}


GameScene::~GameScene(void){


}


void GameScene::OnEntry(void) {
	IOManager::LevelParameters("cfg/FroggerLevelSettings.xml", SM.GetCurDifficulty(), hpLeft, velocity, velocityMod);
	
	lifeCounter = new Sprite[hpLeft];
	for (int i = 0; i < hpLeft; i++) { 
		lifeCounter[i].transform = Transform(5 + i * CELL_WIDTH / 1.25f, W.GetHeight() - 50, CELL_WIDTH / 1.25f, CELL_HEIGHT / 1.25f);
		lifeCounter[i].objectID = ObjectID::FrogIUp;
	}
}


void GameScene::OnExit(void) {
}

void GameScene::Update(void) {
	srand(TM.GetCurTime());
	DetectControls();

	if (!isPaused) {
		std::vector<std::pair<Coord, BehaviorID>> behaviors;
		m_grid.ResetGrid();
		player.Update();



		for (int i = 0; i < carAmount; i++) cars[i].Update();
		for (int i = 0; i < logs.size(); i++) {			
			logs[i]->Update();
			if (logs[i]->ExitedMap())	logs.erase(logs.begin() + i);
			else { 
				behaviors = logs[i]->CoordBehavior();
				for (int i = 0; i < behaviors.size(); i++)  m_grid.SetBehavior(behaviors[i].first, behaviors[i].second);
			}
		}
		for (int i = 0; i < carAmount; i++) {
			behaviors = cars[i].CoordBehavior();
			for (int i = 0; i < behaviors.size(); i++)  m_grid.SetBehavior(behaviors[i].first, behaviors[i].second);
		}
		ControlSpawn();

		switch (m_grid.GetBehavior(Coord(player.GetGridCoords()))) {
		case BehaviorID::SAFE:

			break;
		case BehaviorID::RIP:
			hpLeft--;
			player.Reset();
			//if(hpLeft <= 0) std::cout << "ur det fegg" << std::endl; 
			//TODO call game over scene
			break;
		default:
			std::cout << "Behavior not implemented" << std::endl;
			break;
		}
	}

	
	m_grid.DebugGrid(500);
	//if (m_grid.GetBehavior(player.GetCoords()) == BehaviorID::RIP) std::cout << "water detected" << std::endl;
}


void GameScene::Draw(void) {

	m_background.Draw(); //Render background
	for (auto it : logs) it->Draw();
	for (int i = 0; i < carAmount; i++) cars[i].Draw();
	DrawHud();

	player.Draw();

	for (auto it : finishPoints) {
		if (it.second) {
			Sprite temp;
			temp.transform = { it.first.first - CELL_WIDTH / 2, it.first.second - CELL_HEIGHT / 2, CELL_WIDTH, CELL_HEIGHT };
			temp.objectID = ObjectID::ObjectiveDone;
			temp.Draw();
		}
	}

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
		if (IM.IsKeyDown<KEY_BUTTON_UP>()) {
			if (player.GetGridCoords().second == 0) {
				CheckObjectives();
			}				
			player.MoveUp();
		}
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


void GameScene::CheckObjectives() {
	int xDifference;
	for (int i = 0; i < 5; i++) {
		xDifference = player.GetTransformCoords().first - finishPoints[i].first.first;
		if (xDifference < 0) xDifference = -xDifference;
		std::cout << xDifference << std::endl;
		if (xDifference < CELL_WIDTH) finishPoints[i].second = true;
	}
	int countAccomplished = 0;
	for (int i = 0; i < 5; i++)	if (finishPoints[i].second) countAccomplished++;
	if (countAccomplished == 5) nextLevel = true;
}



void GameScene::ControlSpawn() {
	for (int i = 0; i < 5; i++) {
		if (i % 2 == 0) {
			if (m_grid.GetBehavior(Coord(0, i)) != BehaviorID::SAFE && m_grid.GetBehavior(Coord(1, i)) != BehaviorID::SAFE && m_grid.GetBehavior(Coord(2, i)) != BehaviorID::SAFE) {
				Element *auxElement = new Log(i);
				logs.push_back(auxElement);
			}
		}
		else {
			if (m_grid.GetBehavior(Coord(GRID_WIDTH - 1, i)) != BehaviorID::SAFE && m_grid.GetBehavior(Coord(GRID_WIDTH - 2, i)) != BehaviorID::SAFE && m_grid.GetBehavior(Coord(GRID_WIDTH - 3, i)) != BehaviorID::SAFE) {
				Element *auxElement = new Log(i);
				logs.push_back(auxElement);
			}
		}
	}


	for (int j = 0; j < carAmount; j++) {
		if (cars[j].ExitedMap()) {
			for (int i = 6; i < GRID_HEIGHT - 1; i++) {
				if (i % 2 == 0) {
					if (m_grid.GetBehavior(Coord(0, i)) == BehaviorID::SAFE) {
						cars[j] = Car(i);
						BehaviorID aux = BehaviorID::RIP;
						m_grid.SetBehavior(Coord(0, i), aux);
						break;
					}
				}
				else {
					if (m_grid.GetBehavior(Coord(GRID_WIDTH - 1, i)) == BehaviorID::SAFE) {
						cars[j] = Car(i);
						BehaviorID aux = BehaviorID::RIP;
						m_grid.SetBehavior(Coord(GRID_WIDTH - 1, i), aux);
						break;
					}
				}
			}
		}
	}
}


void GameScene::DrawHud() {
	//debugGrid.Draw(); //Debug Grid


	GUI::DrawTextBlended<FontID::ARIAL>("Score: " + std::to_string(m_score),
	{ 60, int(W.GetHeight()*.045f), 1, 1 }, WHITE); // Render score that will be different when updated
	
	for (int i = 0; i < hpLeft; i++) lifeCounter[i].Draw();
}