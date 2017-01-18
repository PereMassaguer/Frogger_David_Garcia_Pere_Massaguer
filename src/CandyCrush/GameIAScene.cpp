#include "GameIAScene.h"



GameIAScene::GameIAScene(void)
{
	m_background = { { 0, 0, W.GetWidth(), W.GetHeight() }, ObjectID::BGFrogger };

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


GameIAScene::~GameIAScene(void)
{
}

void GameIAScene::OnEntry(void)
{
	levelN = 0;
	for (auto &it : finishPoints) it.second = false;
	carAmount = 10 + 2 * levelN;
	cars = new Car[carAmount];
	for (int i = 0; i < carAmount; i++) cars[i] = Car(rand() % 5 + 6);

	player = Player(-CELL_WIDTH * 4);
	bot = Player(CELL_WIDTH * 2);
}


void GameIAScene::OnExit(void)
{
}

void GameIAScene::Update(void)
{

	srand(TM.GetCurTime());

	DetectControls();

	Coord aux = Coord(bot.GetGridCoords());
	aux.second--;
	if (m_grid.GetBehavior(aux) == BehaviorID::SAFE) bot.MoveUp();



	m_grid.ResetGrid();

	float currentVelocity = 3;
	player.Update(currentVelocity);
	bot.Update(currentVelocity);

	for (int i = 0; i < logs.size(); i++) logs[i]->Update(currentVelocity);
	for (int i = 0; i < carAmount; i++) cars[i].Update(currentVelocity);
	
	std::vector<std::pair<Coord, BehaviorID>> behaviors;
	for (int i = 0; i < logs.size(); i++) {
		if (logs[i]->ExitedMap()) logs.erase(logs.begin() + i);
		else {
			behaviors = logs[i]->CoordBehavior();
			for (int j = 0; j < behaviors.size(); j++)  m_grid.SetBehavior(behaviors[j]);
		}
	}
	for (int i = 0; i < carAmount; i++) {
		behaviors = cars[i].CoordBehavior();
		for (int j = 0; j < behaviors.size(); j++)  m_grid.SetBehavior(behaviors[j]);//HEAP ERROR
	}
	ControlSpawn();

	switch (m_grid.GetBehavior(Coord(player.GetGridCoords()))) {
	case BehaviorID::SAFE:
		break;
	case BehaviorID::RIP:
		m_grid.GetBehavior(Coord(player.GetGridCoords()));
		player.Reset();
		break;
	}
	switch (m_grid.GetBehavior(Coord(bot.GetGridCoords()))) {
	case BehaviorID::SAFE:
		break;
	case BehaviorID::RIP:
		m_grid.GetBehavior(Coord(bot.GetGridCoords()));
		bot.Reset();
		break;
	}
}


void GameIAScene::Draw(void)
{
	m_background.Draw(); //Render background
	for (auto it : logs) it->Draw();
	for (int i = 0; i < carAmount; i++) cars[i].Draw();
	DrawHud();
	player.Draw();
	bot.Draw();
	for (auto it : finishPoints)  if (it.second) it.first.Draw();
}


void GameIAScene::ControlSpawn() {
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
						m_grid.SetBehavior(std::make_pair(Coord(GRID_WIDTH - 1, index), aux));
						break;
					}
				}
				else {
					if (m_grid.GetBehavior(Coord(0, index)) == BehaviorID::SAFE && m_grid.GetBehavior(Coord(1, index)) == BehaviorID::SAFE && m_grid.GetBehavior(Coord(2, index)) == BehaviorID::SAFE) {
						cars[j] = Car(index);
						BehaviorID aux = BehaviorID::RIP;
						m_grid.SetBehavior(std::make_pair(Coord(0, index), aux));
						break;
					}
				}
			}
		}
	}
}


void GameIAScene::DetectControls()
{
	if (IM.IsKeyDown<KEY_BUTTON_UP>()) {
		if (player.GetGridCoords().second == 0) CheckObjectives(player);
		else { player.MoveUp(); }
	}			
	if (IM.IsKeyDown<KEY_BUTTON_DOWN>()) player.MoveDown();
	if (IM.IsKeyDown<KEY_BUTTON_LEFT>()) player.MoveLeft();
	if (IM.IsKeyDown<KEY_BUTTON_RIGHT>()) player.MoveRight();
}


void GameIAScene::CheckObjectives(Player agent) {
	int xDifference;
	bool validJump = false;
	for (int i = 0; i < 5; i++) {
		xDifference = agent.GetTransformCoords().first - finishPoints[i].first.transform.x;
		if (xDifference < 0) xDifference = -xDifference;
		if (xDifference < CELL_WIDTH) {
			if (!finishPoints[i].second) {
				finishPoints[i].second = true;
				validJump = true;
			}
			else { break; }
		}
	}
	if (validJump) {
		int countAccomplished = 0;
		for (int i = 0; i < 5; i++)	if (finishPoints[i].second) countAccomplished++;
		if (countAccomplished == 5) NextLevel();
		agent.Reset();
	}
	else {
		agent.Reset();
	}
}

void GameIAScene::NextLevel() {
	levelN++;
	for (auto &it : finishPoints) it.second = false;
	player.Reset();
	bot.Reset();

	delete[] cars;
	carAmount = 10 + 2 * levelN;
	cars = new Car[carAmount];
	for (int i = 0; i < carAmount; i++) cars[i] = Car(rand() % 5 + 6);
}

void GameIAScene::DrawHud() {
	GUI::DrawTextBlended<FontID::ARIAL>("Level " + std::to_string(levelN + 1),
	{ W.GetWidth() - 75, int(W.GetHeight()*.045f), 1, 1 }, WHITE); // Render levelId
}