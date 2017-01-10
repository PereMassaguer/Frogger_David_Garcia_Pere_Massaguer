/******************************************************************
* Copyright (C) 2016 Jordi Serrano Berbel <jsberbel95@gmail.com> *
* This can not be copied, modified and/or distributed without    *
* express permission of the copyright owner.                     *
******************************************************************/

#include "GameScene.hh"
#include "MainMenuScene.hh"

using namespace Logger;

// Our wave file
Mix_Chunk *wave = NULL;
void repeat(int a) {
	if (Mix_PlayChannel(a, wave, 0) == -1)
		exit(4);
	//Mix_ChannelFinished(&repeat);
}


void continueGame() {
	gameState = GameSceneState::PLAYING; 
	TM.FPSBegin();
}


void exitGame() {
	SM.SetCurScene<MainMenuScene>();
	// clean up our resources
	Mix_FreeChunk(wave);
	// quit SDL_mixer
	Mix_CloseAudio();
}


void submitScore() {
	Rank.AddRankingInfo(playerData);
	exitGame();
}


GameScene::GameScene(void) : m_grid(){
	m_background = { { 0, 0, W.GetWidth(), W.GetHeight() }, ObjectID::BGFrogger };
	debugGrid = { { 0, 30, W.GetWidth(), W.GetHeight() }, ObjectID::GridDebug };

	Button continueButton, submitButton, exitButton;
	continueButton = Button("CONTINUE", Transform(W.GetWidth() / 2, (int)(W.GetHeight() * 0.4f), 1, 1), WHITE);
	submitButton = Button("SUBMIT & EXIT", Transform(W.GetWidth() / 2, (int)(W.GetHeight() * 0.75f), 1, 1), WHITE);
	exitButton = Button("EXIT", Transform(W.GetWidth() / 2, (int)(W.GetHeight() * 0.75f), 1, 1), WHITE);

	continueButton.SetButtonBehavior(continueGame);
	submitButton.SetButtonBehavior(submitScore);
	exitButton.SetButtonBehavior(exitGame);

	pauseButtons.push_back(continueButton);
	pauseButtons.push_back(exitButton);

	gameOverButtons.push_back(submitButton);


	finishPoints[0].first.transform = { 38 - CELL_WIDTH / 2, 80 - CELL_HEIGHT / 2, CELL_WIDTH, CELL_HEIGHT };
	finishPoints[1].first.transform = { 122 - CELL_WIDTH / 2, 80 - CELL_HEIGHT / 2, CELL_WIDTH, CELL_HEIGHT };
	finishPoints[2].first.transform = { 211 - CELL_WIDTH / 2, 80 - CELL_HEIGHT / 2, CELL_WIDTH, CELL_HEIGHT };
	finishPoints[3].first.transform = { 298 - CELL_WIDTH / 2, 80 - CELL_HEIGHT / 2, CELL_WIDTH, CELL_HEIGHT };
	finishPoints[4].first.transform = { 382 - CELL_WIDTH / 2, 80 - CELL_HEIGHT / 2, CELL_WIDTH, CELL_HEIGHT };
	for (auto &it : finishPoints) {
		it.second = false;
		it.first.objectID = ObjectID::ObjectiveDone;
	}
}


GameScene::~GameScene(void){
	delete[] cars;
	delete[] lifeCounter;
}


void GameScene::OnEntry(void) {
	//Music
	std::string music = "sfx/Frogger_music_wav.wav";
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) exit(2);
	wave = Mix_LoadWAV(RESOURCE_FILE(music));
	if (wave == NULL) exit(3);
	if (Mix_PlayChannel(-1, wave, 0) == -1)	exit(4);
	Mix_ChannelFinished(&repeat);

	IOManager::LevelParameters("cfg/FroggerLevelSettings.xml", SM.GetCurDifficulty(), hpLeft, velocity, velocityMod);

	gameState = GameSceneState::PLAYING;
	playerData = PersonData{"",0};
	levelN = 0;
	lifeCounter = new Sprite[hpLeft];
	for (auto &it : finishPoints) it.second = false;
	for (int i = 0; i < hpLeft; i++) { 
		lifeCounter[i].transform = Transform((int)(5 + (i % 5) * CELL_WIDTH / 1.25f), W.GetHeight() - ((i < 5) ? 50 : 30), (int)(CELL_WIDTH / 1.25f), (int)(CELL_HEIGHT / 1.25f));
		lifeCounter[i].objectID = ObjectID::FrogIUp;
	}
	carAmount = 10 + 2 * levelN;
	cars = new Car[carAmount];
	for (int i = 0; i < carAmount; i++) cars[i] = Car(rand() % 5 + 6);
	insect = Insect();
	maxYCoord = 0;
	startingTime = 120 / velocityMod;
	timeLeft = startingTime;
}


void GameScene::OnExit(void) {

}

void GameScene::Update(void) {
	srand(TM.GetCurTime());

	DetectControls();

	if (gameState == GameSceneState::PLAYING) {
		timeLeft < 0.0f ? gameState = GameSceneState::GAMEOVER : timeLeft -= TM.GetDeltaTime()/1000;
		m_grid.ResetGrid();

		float currentVelocity = velocity + playerData.score / 5000;
		player.Update(currentVelocity);
		for (int i = 0; i < logs.size(); i++) logs[i]->Update(currentVelocity);
		for (int i = 0; i < carAmount; i++) cars[i].Update(currentVelocity);
		insect.Update();

		std::vector<std::pair<Coord, BehaviorID>> behaviors;
		for (int i = 0; i < logs.size(); i++) {
			if (logs[i]->ExitedMap()) logs.erase(logs.begin() + i);
			else {
				behaviors = logs[i]->CoordBehavior();
				for (int j = 0; j < behaviors.size(); j++)  m_grid.SetBehavior(behaviors[j].first, behaviors[j].second);
			}
		}
		for (int i = 0; i < carAmount; i++) {
			behaviors = cars[i].CoordBehavior();
			for (int j = 0; j < behaviors.size(); j++)  m_grid.SetBehavior(behaviors[j].first, behaviors[j].second);//HEAP ERROR
		}
		ControlSpawn();

		switch (m_grid.GetBehavior(Coord(player.GetGridCoords()))) {
		case BehaviorID::SAFE:
			break;
		case BehaviorID::RIP:
			hpLeft--; 
			m_grid.GetBehavior(Coord(player.GetGridCoords()));
			player.Reset();
			maxYCoord = 0;
			break;
		}
		if (hpLeft <= 0) { gameState = GameSceneState::GAMEOVER; }
	}
	if (gameState == GameSceneState::GAMEOVER) {
		std::string t = playerData.name;
		if (t.size() < 8) {
			//Deteccion entrada nombre, no conseguimos hacrlo con 2 for, la intencion era recorrer (SDLK_0..SDLK_9),(a..z)
			if (IM.IsKeyDown<SDLK_0>()) playerData.name[t.size()] = '0';
			else if (IM.IsKeyDown<SDLK_1>()) playerData.name[t.size()] = '1';
			else if (IM.IsKeyDown<SDLK_2>()) playerData.name[t.size()] = '2';
			else if (IM.IsKeyDown<SDLK_3>()) playerData.name[t.size()] = '3';
			else if (IM.IsKeyDown<SDLK_4>()) playerData.name[t.size()] = '4';
			else if (IM.IsKeyDown<SDLK_5>()) playerData.name[t.size()] = '5';
			else if (IM.IsKeyDown<SDLK_6>()) playerData.name[t.size()] = '6';
			else if (IM.IsKeyDown<SDLK_7>()) playerData.name[t.size()] = '7';
			else if (IM.IsKeyDown<SDLK_8>()) playerData.name[t.size()] = '8';
			else if (IM.IsKeyDown<SDLK_9>()) playerData.name[t.size()] = '9';
			else if (IM.IsKeyDown<'a'>()) playerData.name[t.size()] = 'A';
			else if (IM.IsKeyDown<'b'>()) playerData.name[t.size()] = 'B';
			else if (IM.IsKeyDown<'c'>()) playerData.name[t.size()] = 'C';
			else if (IM.IsKeyDown<'d'>()) playerData.name[t.size()] = 'D';
			else if (IM.IsKeyDown<'e'>()) playerData.name[t.size()] = 'E';
			else if (IM.IsKeyDown<'f'>()) playerData.name[t.size()] = 'F';
			else if (IM.IsKeyDown<'g'>()) playerData.name[t.size()] = 'G';
			else if (IM.IsKeyDown<'h'>()) playerData.name[t.size()] = 'H';
			else if (IM.IsKeyDown<'i'>()) playerData.name[t.size()] = 'I';
			else if (IM.IsKeyDown<'j'>()) playerData.name[t.size()] = 'J';
			else if (IM.IsKeyDown<'k'>()) playerData.name[t.size()] = 'K';
			else if (IM.IsKeyDown<'l'>()) playerData.name[t.size()] = 'L';
			else if (IM.IsKeyDown<'m'>()) playerData.name[t.size()] = 'M';
			else if (IM.IsKeyDown<'n'>()) playerData.name[t.size()] = 'N';
			else if (IM.IsKeyDown<'o'>()) playerData.name[t.size()] = 'O';
			else if (IM.IsKeyDown<'p'>()) playerData.name[t.size()] = 'P';
			else if (IM.IsKeyDown<'q'>()) playerData.name[t.size()] = 'Q';
			else if (IM.IsKeyDown<'r'>()) playerData.name[t.size()] = 'R';
			else if (IM.IsKeyDown<'s'>()) playerData.name[t.size()] = 'S';
			else if (IM.IsKeyDown<'t'>()) playerData.name[t.size()] = 'T';
			else if (IM.IsKeyDown<'u'>()) playerData.name[t.size()] = 'U';
			else if (IM.IsKeyDown<'v'>()) playerData.name[t.size()] = 'V';
			else if (IM.IsKeyDown<'w'>()) playerData.name[t.size()] = 'W';
			else if (IM.IsKeyDown<'x'>()) playerData.name[t.size()] = 'X';
			else if (IM.IsKeyDown<'y'>()) playerData.name[t.size()] = 'Y';
			else if (IM.IsKeyDown<'z'>()) playerData.name[t.size()] = 'Z';
		}
		//erase last character
		if (IM.IsKeyDown<SDLK_BACKSPACE>() && t.size() > 0) playerData.name[t.size() - 1] = NULL;
	}
}


void GameScene::Draw(void) {

	switch (gameState) {
	case GameSceneState::PLAYING:
		m_background.Draw(); //Render background
		for (auto it : logs) it->Draw();
		for (int i = 0; i < carAmount; i++) cars[i].Draw();
		DrawHud();
		player.Draw();
		for (auto it : finishPoints)  if (it.second) it.first.Draw();
		insect.Draw();
		break;

	case GameSceneState::PAUSED:
		m_background.Draw(); //Render background
		DrawHud();
		GUI::DrawRectangle(std::make_pair(W.GetWidth(), W.GetHeight() * 0.8375f), BLACK, Transform(W.GetWidth() / 2, W.GetHeight() / 2, 1, 1));
		GUI::DrawTextBlended<FontID::ARIAL>("Game paused", Transform((int)(W.GetWidth() / 2), (int)(W.GetHeight()*0.15f), 1, 1), WHITE);
		for (auto it : pauseButtons) it.DrawButton();
		break;

	case GameSceneState::GAMEOVER:
		GUI::DrawRectangle(std::make_pair(W.GetWidth(), W.GetHeight()), BLACK, Transform(W.GetWidth() / 2, W.GetHeight() / 2, 1, 1));
		GUI::DrawTextBlended<FontID::ARIAL>("Game over", Transform((int)(W.GetWidth() / 2), (int)(W.GetHeight()*0.15f), 1, 1), WHITE);
		
		std::string t = playerData.name;
		if (t.length() > 2) for (auto it : gameOverButtons) it.DrawButton();

		std::string tempName = playerData.name;
		for (int i = (int)(t.length()); i < 8; i++) tempName += " _";
		GUI::DrawTextBlended<FontID::ARIAL>("Enter your name(3-8 char)", Transform((int)(W.GetWidth() / 2), (int)(W.GetHeight()*0.45f), 1, 1), WHITE);
		GUI::DrawTextBlended<FontID::ARIAL>(tempName.c_str(), Transform((int)(W.GetWidth() / 2), (int)(W.GetHeight()*0.525f), 1, 1), WHITE);
	}
}


void GameScene::DetectControls()
{
	switch (gameState) {
		case GameSceneState::PLAYING:
			if (IM.IsKeyDown<SDLK_ESCAPE>() || IM.IsKeyDown<'p'>()) gameState = GameSceneState::PAUSED;
			if (gameState == GameSceneState::PLAYING) {
				if (IM.IsKeyDown<KEY_BUTTON_UP>())
					if (player.GetGridCoords().second == 0) CheckObjectives();
					else {
						player.MoveUp();
						if (player.GetGridCoords().second < GRID_HEIGHT - maxYCoord - 1) {
							maxYCoord++;
							playerData.score += 10;
						}
					}	 
				if (IM.IsKeyDown<KEY_BUTTON_DOWN>()) player.MoveDown();
				if (IM.IsKeyDown<KEY_BUTTON_LEFT>()) player.MoveLeft();
				if (IM.IsKeyDown<KEY_BUTTON_RIGHT>()) player.MoveRight();
			}
			break;
		case GameSceneState::PAUSED:
			if (IM.IsKeyDown<SDLK_ESCAPE>() || IM.IsKeyDown<'p'>()) gameState = GameSceneState::PLAYING;
			if (IM.IsMouseDown<MOUSE_BUTTON_LEFT>())
				for (auto it : pauseButtons) if (it.IsMoused()) it.ExecuteBehavior();
			break;
		case GameSceneState::GAMEOVER:
			std::string t = playerData.name;
			if (IM.IsMouseDown<MOUSE_BUTTON_LEFT>() && t.length() > 2)
				for (auto it : gameOverButtons) if (it.IsMoused()) it.ExecuteBehavior();
			break;
	}
}


void GameScene::CheckObjectives() {
	int xDifference;
	bool validJump = false;
	for (int i = 0; i < 5; i++) {
		xDifference = player.GetTransformCoords().first - finishPoints[i].first.transform.x;
		if (xDifference < 0) xDifference = -xDifference;
		if (xDifference < CELL_WIDTH) {
			if (!finishPoints[i].second) {
				finishPoints[i].second = true;
				validJump = true;
				if (insect.CheckGrabbed(i)) {
					playerData.score += 200;
					insect.Reset();
				}
				break;
			}
			else { break; }
		}
	}
	if (validJump) {
		int countAccomplished = 0;
		for (int i = 0; i < 5; i++)	if (finishPoints[i].second) countAccomplished++;
		if (countAccomplished == 5) NextLevel();
		player.Reset();
		maxYCoord = 0;
		playerData.score += (int)(50 + 10 * timeLeft);
	}
	else {
		hpLeft--;
		player.Reset();
		maxYCoord = 0;
	}
}



void GameScene::ControlSpawn() {
	for (int i = 0; i < 5; i++) {
		Element *auxElement = new Log(i);
		if (i % 2 == 0) {
			if (m_grid.GetBehavior(Coord(0, i)) != BehaviorID::SAFE && m_grid.GetBehavior(Coord(1, i)) != BehaviorID::SAFE && m_grid.GetBehavior(Coord(2, i)) != BehaviorID::SAFE) {
				logs.push_back(auxElement);
			}
		}
		else if (i % 2 != 0) {
			if (m_grid.GetBehavior(Coord(GRID_WIDTH - 1, i)) != BehaviorID::SAFE && m_grid.GetBehavior(Coord(GRID_WIDTH - 2, i)) != BehaviorID::SAFE && m_grid.GetBehavior(Coord(GRID_WIDTH - 3, i)) != BehaviorID::SAFE) {
				Element *auxElement = new Log(i);
				logs.push_back(auxElement);//HEAP ERROR
			}
		}
	}

	for (int j = 0; j < carAmount; j++) {
		if (cars[j].ExitedMap()) {
			int randAux = rand() % 5;
			for (int i = 6; i < GRID_HEIGHT - 1; i++) {
				int index = 6 + (i + randAux) % 5;
				if (index % 2 == 0) {
					if (m_grid.GetBehavior(Coord(GRID_WIDTH - 1, index)) == BehaviorID::SAFE && m_grid.GetBehavior(Coord(GRID_WIDTH - 2, index)) == BehaviorID::SAFE && m_grid.GetBehavior(Coord(GRID_WIDTH - 3, index)) == BehaviorID::SAFE) {
						cars[j] = Car(index);
						BehaviorID aux = BehaviorID::RIP;
						m_grid.SetBehavior(Coord(GRID_WIDTH - 1, index), aux);
						break;
					}
				}
				else {
					if (m_grid.GetBehavior(Coord(0, index)) == BehaviorID::SAFE && m_grid.GetBehavior(Coord(1, index)) == BehaviorID::SAFE && m_grid.GetBehavior(Coord(2, index)) == BehaviorID::SAFE) {
						cars[j] = Car(index);
						BehaviorID aux = BehaviorID::RIP;
						m_grid.SetBehavior(Coord(0, index), aux);
						break;
					}
				}
			}
		}
	}
}


void GameScene::DrawHud() {
	//debugGrid.Draw(); //Debug grid to easily locate grid cells
	std::string text = "Score: " + std::to_string(playerData.score);
	std::pair<int, int> textSize;
	TTF_SizeText(R.GetFont<FontID::ARIAL>(), text.c_str(), &textSize.first, &textSize.second);
	GUI::DrawTextBlended<FontID::ARIAL>("Score: " + std::to_string(playerData.score), { 15 + textSize.first / 2, int(W.GetHeight()*.045f), 1, 1 }, WHITE); // Render score that will be different when updated

	GUI::DrawTextBlended<FontID::ARIAL>("Level " + std::to_string(levelN + 1),
	{W.GetWidth() - 75, int(W.GetHeight()*.045f), 1, 1 }, WHITE); // Render levelId

	for (int i = 0; i < hpLeft; i++) lifeCounter[i].Draw();

	GUI::DrawTextBlended<FontID::ARIAL>(std::to_string((int)timeLeft),
	{ int(W.GetWidth() * 0.85f), int(W.GetHeight() * .95f), 1, 1 }, {(int)(255 * (1 - timeLeft / startingTime)), (int)(127 * (timeLeft / startingTime)), 0});
}


void GameScene::NextLevel() {
	levelN++;
	playerData.score += 1000;
	for (auto &it : finishPoints) it.second = false;
	player.Reset();
	insect = Insect();
	insect.Reset();

	delete[] cars;
	carAmount = 10 + 2 * levelN;
	cars = new Car[carAmount];
	for (int i = 0; i < carAmount; i++) cars[i] = Car(rand() % 5 + 6);
}