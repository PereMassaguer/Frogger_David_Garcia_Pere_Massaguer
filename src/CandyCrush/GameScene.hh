/******************************************************************
* Copyright (C) 2016 Jordi Serrano Berbel <jsberbel95@gmail.com> *
* This can not be copied, modified and/or distributed without    *
* express permission of the copyright owner.                     *
******************************************************************/

#pragma once
#include "Button.hh"
#include "Grid.hh"
#include "GUI.hh"
#include "ID.hh"
#include "IOManager.hh"
#include "InputManager.hh"
#include "Logger.hh"
#include "Player.hh"
#include "Scene.hh"
#include "System.hh"

// GameScene class with the main gameplay mode
class GameScene : public Scene {
	Grid m_grid;
	Sprite m_background;
	int m_score{ 0 };
	int hpLeft;
	float velocity;
	float velocityMod;

	Player player;
	std::vector<Button> pauseButtons;
	std::vector<Element*> spawnables;

public:
	explicit GameScene(void);
	~GameScene() override;
	void OnEntry(void) override;
	void OnExit(void) override;
	void Update(void) override;
	void Draw(void) override;

private:
	void detectControls();

};
static bool isPaused;