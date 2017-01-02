#pragma once
#include <cstdlib>
#include "Button.hh"
#include "GUI.hh"
#include "Scene.hh"
#include "System.hh"

class MainMenuScene : public Scene
{
	std::vector<Button> buttons[(int)MenuSceneState::COUNT];
public:

	explicit MainMenuScene();
	~MainMenuScene() override;

	void OnEntry(void) override;
	void OnExit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void setMenuSceneState(MenuSceneState v);
};
static MenuSceneState menuSceneState;