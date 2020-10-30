#include "MainScene.h"

#include "MCSearch.h"
#include "NeuralNetInf.h"
#include "Singleton.h"
#include "TitleScene.h"

NeuralNetInf network;

float evaluate(const Position& pos) {
	return network.forward(pos.toVector(pos.toMove));
}

MainScene::MainScene(Game* game)
	:Scene(game)
{
	//Load the network
	network.load("assets/Model");

	board = game->textures.CreateImage("assets/Board.png");

	whiteMan = game->textures.CreateImage("assets/WhiteMan.png");
	blackMan = game->textures.CreateImage("assets/BlackMan.png");
	whiteKing = game->textures.CreateImage("assets/WhiteKing.png");
	blackKing = game->textures.CreateImage("assets/BlackKing.png");

	selection = game->textures.CreateImage("assets/Selection.png");

	playersTurn = true; lastSquare = NOSQUARE; lastSelected = NOSQUARE;

	pos = Position();
}

MainScene::~MainScene()
{
}

void MainScene::Update()
{
	if (!playersTurn) {
		MCSearchResult res = MCSearch(pos, evaluate);
		std::cout << res.outcome << "\n";
		pos.makeMove(res.chooseUniform(difficulty));
		playersTurn = true;
		if (!MoveGen(pos).moves.size())
			game->activeScene->SwitchScene(new TitleScene(game));
	}
	Scene::Update();
}

void MainScene::DrawBoard() {

	SDL_Rect boardPos; boardPos.x = 340; boardPos.y = 10; boardPos.w = 880; boardPos.h = 880;
	DrawImage(board, &boardPos);

	if (lastSelected != NOSQUARE) {

		int y = int(lastSelected) / 4;
		int x = 2 * (int(lastSelected) - 4 * y); x += (y & 0b1) ? 1 : 0;

		SDL_Rect selectionPos{ (340 + 110 * x), (10 + 110 * (7 - y)), 110, 110 };
		DrawImage(selection, &selectionPos);
	}

	for (int x = 0; x < 8; ++x) {
		for (int y = 0; y < 8; ++y) {

			if ((x + y) & 0b1)
				continue;

			unsigned int square = 1u << (x + 8 * y) / 2;

			SDL_Rect squarePos{ (340 + 110 * x), (10 + 110 * (7 - y)), 110, 110 };

			if (pos.mans(WHITE) & square)
				DrawImage(whiteMan, &squarePos);
			else if (pos.mans(BLACK) & square)
				DrawImage(blackMan, &squarePos);
			else if (pos.kings(WHITE) & square)
				DrawImage(whiteKing, &squarePos);
			else if (pos.kings(BLACK) & square)
				DrawImage(blackKing, &squarePos);

		}
	}
}

void MainScene::Render()
{
	DrawBoard();

	Scene::Render();
}
