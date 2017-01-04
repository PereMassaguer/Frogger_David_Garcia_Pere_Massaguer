/******************************************************************
* Copyright (C) 2016 Jordi Serrano Berbel <jsberbel95@gmail.com> *
* This can not be copied, modified and/or distributed without    *
* express permission of the copyright owner.                     *
******************************************************************/

#pragma once
#include "Button.hh"
#include "Car.hh"
#include "Grid.hh"
#include "GUI.hh"
#include "ID.hh"
#include "IOManager.hh"
#include "InputManager.hh"
#include "Insect.hh"
#include "Logger.hh"
#include "Player.hh"
#include "Scene.hh"
#include "System.hh"
#include "Log.hh"

// GameScene class with the main gameplay mode
class GameScene : public Scene {
	Grid m_grid;
	Sprite m_background;
	int m_score;
	int hpLeft;
	float velocity;
	float velocityMod;

	Player player;
	std::string playerName;
	std::vector<Button> pauseButtons;
	std::vector<Button> gameOverButtons;

	int carAmount;
	Element *cars;
	std::vector<Element*> logs; 
	Insect insect;
	std::pair<Sprite, bool> finishPoints[5];
	Sprite *lifeCounter;


	int levelN;
	Sprite debugGrid;
public:
	explicit GameScene(void);
	~GameScene() override;
	void OnEntry(void) override;
	void OnExit(void) override;
	void Update(void) override;
	void Draw(void) override;

private:
	void DetectControls();
	void CheckObjectives();
	void ControlSpawn();
	void DrawHud();
	void NextLevel();
};
static GameSceneState gameState;