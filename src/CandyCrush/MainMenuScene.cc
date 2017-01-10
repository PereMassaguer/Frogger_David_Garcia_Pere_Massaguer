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

void ShowCreditsVoid() {
	menuSceneState = MenuSceneState::CREDITS;
}

void ExitGameVoid() {
	SDL_Quit();
	exit(0);
}


MainMenuScene::MainMenuScene()
{
	//Call button constructor
	Button playButton, rankingButton, creditsButton, exitButton;
	playButton = Button("PLAY", Transform(W.GetWidth() / 2, (W.GetHeight() / 2) - 100, 1, 1), WHITE);
	rankingButton = Button("RANKING", Transform(W.GetWidth() / 2, (W.GetHeight() / 2) - 25, 1, 1), WHITE);
	creditsButton = Button("CREDITS", Transform(W.GetWidth() / 2, (W.GetHeight() / 2) + 50, 1, 1), WHITE);
	exitButton = Button("EXIT", Transform(W.GetWidth() / 2, (int)(W.GetHeight() * 0.85f), 1, 1), WHITE);
	
	Button easyButton, mediumButton, hardButton, backButton;
	easyButton = Button("EASY", Transform(W.GetWidth() / 2 - 150, (W.GetHeight() / 2), 1, 1), GREEN);
	mediumButton = Button("MEDIUM", Transform(W.GetWidth() / 2, (W.GetHeight() / 2), 1, 1), ORANGE);
	hardButton = Button("HARD", Transform(W.GetWidth() / 2 + 150, (W.GetHeight() / 2), 1, 1), RED);
	backButton = Button("BACK", Transform(W.GetWidth() / 2, (int)(W.GetHeight() * 0.85f), 1, 1), WHITE);
	
	//Set each button behavior
	playButton.SetButtonBehavior(SelectDifficultyVoid);
	rankingButton.SetButtonBehavior(ShowRankingVoid);
	creditsButton.SetButtonBehavior(ShowCreditsVoid);
	exitButton.SetButtonBehavior(ExitGameVoid);

	easyButton.SetButtonBehavior(StartEasyGameVoid);
	mediumButton.SetButtonBehavior(StartMediumGameVoid);
	hardButton.SetButtonBehavior(StartHardGameVoid);
	backButton.SetButtonBehavior(BackToDefaultVoid);


	//Allocate buttons on a Vectors Array
	buttons[(int)MenuSceneState::DEFAULT].push_back(playButton);
	buttons[(int)MenuSceneState::DEFAULT].push_back(rankingButton);
	buttons[(int)MenuSceneState::DEFAULT].push_back(creditsButton);
	buttons[(int)MenuSceneState::DEFAULT].push_back(exitButton);

	buttons[(int)MenuSceneState::DIFFICULTY].push_back(easyButton);
	buttons[(int)MenuSceneState::DIFFICULTY].push_back(mediumButton);
	buttons[(int)MenuSceneState::DIFFICULTY].push_back(hardButton);
	buttons[(int)MenuSceneState::DIFFICULTY].push_back(backButton);
	
	buttons[(int)MenuSceneState::RANKING].push_back(backButton);

	buttons[(int)MenuSceneState::CREDITS].push_back(backButton);
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
	GUI::DrawRectangle(std::make_pair(W.GetWidth(), W.GetHeight()), BLACK, Transform(W.GetWidth() / 2, W.GetHeight() / 2, 1, 1));
	GUI::DrawTextShaded<FontID::ARIAL>("PD: Frogger", { W.GetWidth() / 2, int(W.GetHeight()*.1f), 1, 1 }, WHITE, BLACK);

	
	if (menuSceneState == MenuSceneState::RANKING) {
		std::vector<PersonData> rankInfo = Rank.GetRankingInfo();
		int i;
		for (i = 0; i < rankInfo.size(); i++) {
			GUI::DrawTextBlended<FontID::ARIAL>(rankInfo[i].name , { (int)(W.GetWidth() * 0.35f), (int)(W.GetHeight() * (0.25f + 0.055f * i)), 1, 1 }, WHITE);
			GUI::DrawTextBlended<FontID::ARIAL>(std::to_string(rankInfo[i].score), { (int)(W.GetWidth() * 0.65f), (int)(W.GetHeight() * (0.25f + 0.055f * i)), 1, 1 }, WHITE);
		}
		for (i; i < 10; i++) {
			GUI::DrawTextBlended<FontID::ARIAL>("--------", { (int)(W.GetWidth() * 0.35f), (int)(W.GetHeight() * (0.25f + 0.055f * i)), 1, 1 }, WHITE);
			GUI::DrawTextBlended<FontID::ARIAL>("0", { (int)(W.GetWidth() * 0.65f), (int)(W.GetHeight() * (0.25f + 0.055f * i)), 1, 1 }, WHITE);
		}
	}
	else if (menuSceneState == MenuSceneState::CREDITS) {
		R.Push(TTF_RenderText_Blended(R.GetFont<FontID::ARIAL>(), "David Garcia Poyo", WHITE), Transform(W.GetWidth() / 2, W.GetHeight() / 2 - 75, 1, 1));
		R.Push(TTF_RenderText_Blended(R.GetFont<FontID::ARIAL>(), "Pere Massaguer Vila", WHITE), Transform(W.GetWidth() / 2, W.GetHeight() / 2 - 25, 1, 1));
	}
	else if(menuSceneState == MenuSceneState::DIFFICULTY)
		R.Push(TTF_RenderText_Blended(R.GetFont<FontID::ARIAL>(), "Select difficulty", WHITE), Transform(W.GetWidth() / 2, (int)(W.GetHeight() *0.375f), 1, 1));

	for (auto it : buttons[(int)menuSceneState]) it.DrawButton();
}