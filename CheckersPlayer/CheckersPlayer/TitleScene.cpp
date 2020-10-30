#include "TitleScene.h"
#include "MainScene.h"
#include "Singleton.h"

void doEasy(Scene* scene) {
	difficulty = 1.5f;
	scene->SwitchScene(new MainScene(scene->game));
}
void doMedium(Scene* scene) {
	difficulty = 0.70f;
	scene->SwitchScene(new MainScene(scene->game));
}
void doHard(Scene* scene) {
	difficulty = 0.05f;
	scene->SwitchScene(new MainScene(scene->game));
}

TitleScene::TitleScene(Game* game)
	:Scene(game)
{
	SDL_Rect pos; pos.x = 640; pos.y = 190; pos.w = 320; pos.h = 160;
	RegisterButton(game->textures.CreateImage("assets/Einfach.png"), pos, doEasy);

	pos.x = 640; pos.y = 370; pos.w = 320; pos.h = 160;
	RegisterButton(game->textures.CreateImage("assets/Mittel.png"), pos, doMedium);

	pos.x = 640; pos.y = 550; pos.w = 320; pos.h = 160;
	RegisterButton(game->textures.CreateImage("assets/Schwer.png"), pos, doHard);
}

TitleScene::~TitleScene()
{
}

void TitleScene::Update()
{
	Scene::Update();
}

void TitleScene::Render()
{
	/*
	SDL_Rect rect; rect.x = 20; rect.y = 20; rect.w = 860; rect.h = 860;
	DrawImage(game->textures.CreateImage("assets/CheckersLogo.png"), &rect);
	*/
	Scene::Render();
}

void TitleScene::PollEvent(SDL_Event* event)
{
}
