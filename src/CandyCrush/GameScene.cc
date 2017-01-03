/******************************************************************
* Copyright (C) 2016 Jordi Serrano Berbel <jsberbel95@gmail.com> *
* This can not be copied, modified and/or distributed without    *
* express permission of the copyright owner.                     *
******************************************************************/

#include "GameScene.hh"
#include "MainMenuScene.hh"
#include <SDL_mixer.h>

// Our wave file
Mix_Chunk *wave = NULL;
void repeat(int a) {
	if (Mix_PlayChannel(a, wave, 0) == -1)
		exit(4);
	//Mix_ChannelFinished(&repeat);
}

using namespace Logger;


void continueGame() {
	isPaused = false;
}


void exitGame() {
	isPaused = true;
	SM.SetCurScene<MainMenuScene>();
	// clean up our resources
	Mix_FreeChunk(wave);
	// quit SDL_mixer
	Mix_CloseAudio();
}


GameScene::GameScene(void) : m_grid(){
	m_background = { { 0, 0, W.GetWidth(), W.GetHeight() }, ObjectID::BGFrogger };
	debugGrid = { { 0, 30, W.GetWidth(), W.GetHeight() }, ObjectID::GridDebug };

	Button continueButton, exitButton;
	continueButton = Button("CONTINUE", Transform(W.GetWidth() / 2, (int)(W.GetHeight() * 0.4f), 1, 1), WHITE);
	exitButton = Button("MENU", Transform(W.GetWidth() / 2, (int)(W.GetHeight() * 0.75f), 1, 1), WHITE);

	continueButton.SetButtonBehavior(continueGame);
	exitButton.SetButtonBehavior(exitGame);

	pauseButtons.push_back(continueButton);
	pauseButtons.push_back(exitButton);

	gameOverButtons.push_back(exitButton);


	finishPoints[0].first = Coord(38, 80);
	finishPoints[1].first = Coord(122, 80);
	finishPoints[2].first = Coord(211, 80);
	finishPoints[3].first = Coord(298, 80);
	finishPoints[4].first = Coord(382, 80);
	//for (auto it : finishPoints) it.second = false;

	carAmount = 10 + 2 * levelN;
	cars = new Car[carAmount];
	for (int i = 0; i < carAmount; i++) cars[i] = Car(rand() % 5 + 6);
}


GameScene::~GameScene(void){


}


void GameScene::OnEntry(void) {
	//Music
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
		exit(2);
	std::string music = "sfx/Frogger_music_wav.wav";

	wave = Mix_LoadWAV(RESOURCE_FILE(music));
	if (wave == NULL)
		exit(3);
	if (Mix_PlayChannel(-1, wave, 0) == -1)
		exit(4);
	Mix_ChannelFinished(&repeat);
	IOManager::LevelParameters("cfg/FroggerLevelSettings.xml", SM.GetCurDifficulty(), hpLeft, velocity, velocityMod);

	isPaused = false;
	gameOver = false;
	levelN = 0;
	m_score = 0;
	lifeCounter = new Sprite[hpLeft];
	for (auto it : finishPoints) it.second = false;
	for (int i = 0; i < hpLeft; i++) { 
		lifeCounter[i].transform = Transform(5 + (i % 5) * CELL_WIDTH / 1.25f, W.GetHeight() - ((i < 5) ? 50 : 30), CELL_WIDTH / 1.25f, CELL_HEIGHT / 1.25f);
		lifeCounter[i].objectID = ObjectID::FrogIUp;
	}
}


void GameScene::OnExit(void) {
	//for (auto it : finishPoints) it.second = false;
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
				for (int j = 0; j < behaviors.size(); j++)  m_grid.SetBehavior(behaviors[j].first, behaviors[j].second);
			}
		}
		for (int i = 0; i < carAmount; i++) {
			behaviors = cars[i].CoordBehavior();
			for (int j = 0; j < behaviors.size(); j++)  m_grid.SetBehavior(behaviors[j].first, behaviors[j].second);
		}
		ControlSpawn();

		switch (m_grid.GetBehavior(Coord(player.GetGridCoords()))) {
		case BehaviorID::SAFE:

			break;
		case BehaviorID::RIP:
			hpLeft--;
			player.Reset();
			break;
		default:
			std::cout << "Behavior not implemented" << std::endl;
			break;
		}
	}
	if (hpLeft <= 0) { gameOver = true; }
	m_grid.DebugGrid(50);//See colliders on console (int refresh frequency ms)
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
		GUI::DrawRectangle(std::make_pair(W.GetWidth(), W.GetHeight() * 0.8375f), BLACK);
		GUI::DrawTextBlended<FontID::ARIAL>("Game paused", Transform((int)(W.GetWidth() / 2), (int)(W.GetHeight()*0.25f), 1, 1), WHITE);
		for (auto it : pauseButtons) it.DrawButton();
	}
	if (gameOver) {
		GUI::DrawRectangle(std::make_pair(W.GetWidth(), W.GetHeight()), BLACK);
		GUI::DrawTextBlended<FontID::ARIAL>("Game over", Transform((int)(W.GetWidth() / 2), (int)(W.GetHeight()*0.25f), 1, 1), WHITE);
		for (auto it : gameOverButtons) it.DrawButton();
	}
}


void GameScene::DetectControls()
{
	if (IM.IsKeyDown<SDLK_ESCAPE>()) {
		isPaused = !isPaused;
		if (!isPaused) TM.FPSBegin();
	}


	if (!isPaused && !gameOver) {
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
		//TODO Arrows menu
		if (isPaused) {
			if (IM.IsMouseDown<MOUSE_BUTTON_LEFT>())
				for (auto it : pauseButtons) if (it.IsMoused()) it.ExecuteBehavior();
		}
		else if (gameOver) {
			if (IM.IsMouseDown<MOUSE_BUTTON_LEFT>())
				for (auto it : gameOverButtons) if (it.IsMoused()) it.ExecuteBehavior();
		}
	}
}


void GameScene::CheckObjectives() {
	int xDifference;
	bool validJump = false;
	for (int i = 0; i < 5; i++) {
		xDifference = player.GetTransformCoords().first - finishPoints[i].first.first;
		if (xDifference < 0) xDifference = -xDifference;
		if (xDifference < CELL_WIDTH) {
			if (!finishPoints[i].second) {
				finishPoints[i].second = true;
				validJump = true;
				break;
			}
			else { break; }
		}			
	}
	if (validJump) {
		int countAccomplished = 0;
		for (int i = 0; i < 5; i++)	if (finishPoints[i].second) countAccomplished++;
		if (countAccomplished == 5) nextLevel = true;
		player.Reset();
	}
	else {
		hpLeft--;
		player.Reset();
	}
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
					if (m_grid.GetBehavior(Coord(GRID_WIDTH - 1, i)) == BehaviorID::SAFE && m_grid.GetBehavior(Coord(GRID_WIDTH - 2, i)) == BehaviorID::SAFE && m_grid.GetBehavior(Coord(GRID_WIDTH - 3, i)) == BehaviorID::SAFE) {
						cars[j] = Car(i);
						BehaviorID aux = BehaviorID::RIP;
						m_grid.SetBehavior(Coord(GRID_WIDTH - 1, i), aux);
						break;
					}
				}
				else {
					if (m_grid.GetBehavior(Coord(0, i)) == BehaviorID::SAFE && m_grid.GetBehavior(Coord(1, i)) == BehaviorID::SAFE && m_grid.GetBehavior(Coord(2, i)) == BehaviorID::SAFE) {
						cars[j] = Car(i);
						BehaviorID aux = BehaviorID::RIP;
						m_grid.SetBehavior(Coord(0, i), aux);
						break;
					}
				}
			}
		}
	}
}


void GameScene::DrawHud() {
	debugGrid.Draw(); //Debug grid to easily locate grid cells

	GUI::DrawTextBlended<FontID::ARIAL>("Score: " + std::to_string(m_score),
	{ 60, int(W.GetHeight()*.045f), 1, 1 }, WHITE); // Render score that will be different when updated

	GUI::DrawTextBlended<FontID::ARIAL>("Level " + std::to_string(levelN + 1),
	{W.GetWidth() - 75, int(W.GetHeight()*.045f), 1, 1 }, WHITE); // Render levelId

	for (int i = 0; i < hpLeft; i++) lifeCounter[i].Draw();
}