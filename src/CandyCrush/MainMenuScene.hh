#pragma once
#include "Button.hh"
#include "GameScene.hh"
#include "GUI.hh"
#include "Scene.hh"
#include "System.hh"

class MainMenuScene : public Scene
{
	std::vector<Button> buttonList;
	int k;
public:
	explicit MainMenuScene();
	~MainMenuScene() override;

	void OnEntry(void) override;
	void OnExit(void) override;
	void Update(void) override;
	void Draw(void) override;
};

