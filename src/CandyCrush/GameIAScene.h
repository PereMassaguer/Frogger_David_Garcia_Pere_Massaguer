#pragma once

#include "Car.hh"
#include "Grid.hh"
#include "GUI.hh"
#include "Insect.hh"
#include "Log.hh"
#include "Player.hh"
#include "Scene.hh"
#include "Sprite.hh"

class GameIAScene : public Scene
{
	Grid m_grid;
	Sprite m_background;

	Player player;
	Player bot;

	int carAmount;
	Element *cars;
	std::vector<Element*> logs;
	std::pair<Sprite, bool> finishPoints[5];

	int levelN;

public:
	explicit GameIAScene(void);
	~GameIAScene() override;
	void OnEntry(void) override;
	void OnExit(void) override;
	void Update(void) override;
	void Draw(void) override;



private:
	void ControlSpawn();
	void DetectControls();
	void CheckObjectives(Player agent);
	void DrawHud();
	void NextLevel();
};