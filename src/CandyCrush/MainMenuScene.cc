#include "MainMenuScene.hh"
#include "GameScene.hh"

void SelectDifficultyVoid() {
	menuSceneState = MenuSceneState::DIFFICULTY;
}

void StartEasyGameVoid() {
	SM.SetCurDifficulty(Difficulty::EASY);
	SM.SetCurScene<GameScene>();
}

void StartMediumGameVoid() {
	SM.SetCurDifficulty(Difficulty::MEDIUM);
	SM.SetCurScene<GameScene>();
}
void StartHardGameVoid() {
	SM.SetCurDifficulty(Difficulty::HARD);
	SM.SetCurScene<GameScene>();
}

void BackToDefaultVoid() {
	menuSceneState = MenuSceneState::DEFAULT;
}

void ShowRankingVoid() {
	menuSceneState = MenuSceneState::RANKING;
}

void ExitGameVoid() {
	SDL_Quit();
	exit(0);
}


MainMenuScene::MainMenuScene()
{
	//Call button constructor
	Button playButton, rankingButton, exitButton;
	playButton = Button("PLAY", Transform(W.GetWidth() / 2, (W.GetHeight() / 2) - 50, 1, 1), WHITE);
	rankingButton = Button("RANKING", Transform(W.GetWidth() / 2, (W.GetHeight() / 2) + 50, 1, 1), WHITE);
	exitButton = Button("EXIT", Transform(W.GetWidth() / 2, (W.GetHeight() / 2) + 150, 1, 1), WHITE);
	
	Button easyButton, mediumButton, hardButton, backButton;
	easyButton = Button("EASY", Transform(W.GetWidth() / 2 - 150, (W.GetHeight() / 2), 1, 1), GREEN);
	mediumButton = Button("MEDIUM", Transform(W.GetWidth() / 2, (W.GetHeight() / 2), 1, 1), ORANGE);
	hardButton = Button("HARD", Transform(W.GetWidth() / 2 + 150, (W.GetHeight() / 2), 1, 1), RED);
	backButton = Button("BACK", Transform(W.GetWidth() / 2, (W.GetHeight() / 2 + 150), 1, 1), WHITE);
	
	//Set each button behavior
	playButton.SetButtonBehavior(SelectDifficultyVoid);
	rankingButton.SetButtonBehavior(ShowRankingVoid);
	exitButton.SetButtonBehavior(ExitGameVoid);

	easyButton.SetButtonBehavior(StartEasyGameVoid);
	mediumButton.SetButtonBehavior(StartMediumGameVoid);
	hardButton.SetButtonBehavior(StartHardGameVoid);
	backButton.SetButtonBehavior(BackToDefaultVoid);


	//Allocate buttons on a Vectors Array
	buttons[(int)MenuSceneState::DEFAULT].push_back(playButton);
	buttons[(int)MenuSceneState::DEFAULT].push_back(rankingButton);
	buttons[(int)MenuSceneState::DEFAULT].push_back(exitButton);

	buttons[(int)MenuSceneState::DIFFICULTY].push_back(easyButton);
	buttons[(int)MenuSceneState::DIFFICULTY].push_back(mediumButton);
	buttons[(int)MenuSceneState::DIFFICULTY].push_back(hardButton);
	buttons[(int)MenuSceneState::DIFFICULTY].push_back(backButton);


	buttons[(int)MenuSceneState::RANKING].push_back(backButton);
}


MainMenuScene::~MainMenuScene()
{
}

void MainMenuScene::OnEntry(void)
{
	menuSceneState = MenuSceneState::DEFAULT;
}

void MainMenuScene::OnExit(void)
{
}

void MainMenuScene::Update(void)
{
	if (IM.IsMouseDown<MOUSE_BUTTON_LEFT>())
		for (auto it : buttons[(int)menuSceneState]) if (it.IsMoused()) it.ExecuteBehavior();
}

void MainMenuScene::Draw(void)
{
	//Black BackGround
	GUI::DrawRectangle(std::make_pair(W.GetWidth(), W.GetHeight()), BLACK);
	GUI::DrawTextShaded<FontID::ARIAL>("PD: Frogger", { W.GetWidth() / 2, int(W.GetHeight()*.1f), 1, 1 }, WHITE, BLACK);


	if(menuSceneState == MenuSceneState::RANKING)
			R.Push(TTF_RenderText_Blended(R.GetFont<FontID::ARIAL>(), "not implemented yet!", RED), Transform(W.GetWidth() / 2, W.GetHeight() / 2, 1, 1));

	for (auto it : buttons[(int)menuSceneState]) it.DrawButton();
}

void MainMenuScene::setMenuSceneState(MenuSceneState v)
{
	menuSceneState = v;
}

