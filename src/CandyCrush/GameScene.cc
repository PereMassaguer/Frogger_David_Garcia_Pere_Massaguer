/******************************************************************
* Copyright (C) 2016 Jordi Serrano Berbel <jsberbel95@gmail.com> *
* This can not be copied, modified and/or distributed without    *
* express permission of the copyright owner.                     *
******************************************************************/

#include "GUI.hh"
#include "System.hh"
#include "Logger.hh"
#include "GameScene.hh"
using namespace Logger;

#define CELL_WIDTH 80
#define CELL_HEIGHT 80

GameScene::GameScene(void) : m_grid("lvl/testLvl.dat", CELL_WIDTH, CELL_HEIGHT) {
	m_background = { { 0, 0, W.GetWidth(), W.GetHeight() }, ObjectID::BGFrogger };
}

GameScene::~GameScene(void){
}

void GameScene::OnEntry(void) {
}

void GameScene::OnExit(void) {
}

void GameScene::Update(void) {
	
}

void GameScene::Draw(void) {
	m_background.Draw(); // Render background
	m_grid.Draw(); // Render grid
	GUI::DrawTextBlended<FontID::ARIAL>("Score: " + std::to_string(m_score), 
										{ W.GetWidth() >> 1, int(W.GetHeight()*.9f), 1, 1 }, 
										{ 115, 0, 180 }); // Render score that will be different when updated
}
