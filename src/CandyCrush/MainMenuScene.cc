#include "MainMenuScene.hh"



MainMenuScene::MainMenuScene()
{
	Button aux = Button("PLAY", Transform(W.GetWidth()*0.5,  (W.GetHeight() / 2) + 60, 1, 1), { 190, 0, 160 }, { 50, 200, 230 });
	buttonList.push_back(aux);
}


MainMenuScene::~MainMenuScene()
{
}

void MainMenuScene::OnEntry(void)
{
	//Draw();
}

void MainMenuScene::OnExit(void)
{
}

void MainMenuScene::Update(void)
{
	if (IM.IsMouseDown<MOUSE_BUTTON_LEFT>()) {
		SM.SetCurScene<GameScene>();
	}
}

void MainMenuScene::Draw(void)
{
	for (auto it : buttonList) it.DrawButton();
}