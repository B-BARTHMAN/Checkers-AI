#include "MainScene.h"
#include "MoveGen.h"
#include "TitleScene.h"

#include "MCSearch.h"

#define HELP

void MainScene::PollEvent(SDL_Event* event)
{
	//It's not the players turn, so dont do anything
	if (!playersTurn)
		return;

	//It's not a mouse click
	if (event->type != SDL_MOUSEBUTTONUP)
		return;

	//Get windowsizze
	int windowX, windowY; SDL_GetRendererOutputSize(game->renderer, &windowX, &windowY);

	//get mousePosition
	int mouse_x, mouse_y;
	unsigned int mouse_down = SDL_GetMouseState(&mouse_x, &mouse_y); mouse_y = windowY - mouse_y;

	//test if mouseclick was inside board
	if (mouse_x <= game->xScale * 340 || mouse_x >= game->xScale * 1220)
		return;
	if (mouse_y <= game->yScale * 10 || mouse_y >= game->yScale * 890)
		return;

	//selected square
	int selectedX = (mouse_x - (game->xScale * 340)) / (110 * game->xScale);
	int selectedY = (mouse_y - (game->yScale * 10)) / (110 * game->yScale);

	//White squares
	if ((selectedX + selectedY) & 0b1)
		return;

	if (lastSelected == NOSQUARE) {
		lastSelected = lastSquare == NOSQUARE ? Square((selectedX + 8 * selectedY) / 2) : lastSquare;
		return;
	}

	Move move = makeMove();

	if (abs(((selectedX + 8 * selectedY) / 2) - int(lastSelected)) > 5) {
		move = makeCapture();
	}

	move = addSquare(move, lastSelected); move = addSquare(move, Square((selectedX + 8 * selectedY) / 2));

	MoveGen possibilities = MoveGen(pos, lastSquare);
	if (std::find(possibilities.moves.begin(), possibilities.moves.end(), move) != possibilities.moves.end()) {
#ifdef HELP
		MCSearchResult res = MCSearch(pos, evaluate);
		Move bestMove = res.mostPlayed();
		std::cout << "Best Move: ";
		enumerateMove(bestMove);
#endif
		pos.makeMove(move);
#ifdef HELP
		std::cout << "Network Analysis of Position: " << network.forward(pos.toVector(WHITE)) << "\n";
#endif
		playersTurn = false;
		lastSquare = NOSQUARE;
		if (!MoveGen(pos).moves.size())
			game->activeScene->SwitchScene(new TitleScene(game));
	}
	if (isCapture(move)) {
		pos.toMove = ~pos.toMove;
		if (MoveGen(pos, Square((selectedX + 8 * selectedY) / 2)).moves.size() == 0) {
			pos.toMove = ~pos.toMove;
			lastSelected = NOSQUARE;
			if (!MoveGen(pos).moves.size())
				game->activeScene->SwitchScene(new TitleScene(game));
			return;
		}
		playersTurn = true;
		lastSquare = Square((selectedX + 8 * selectedY) / 2);
		lastSelected = lastSquare;
		if (!MoveGen(pos).moves.size())
			game->activeScene->SwitchScene(new TitleScene(game));
		return;
	}

	lastSelected = lastSquare == NOSQUARE ? NOSQUARE : lastSquare;

}