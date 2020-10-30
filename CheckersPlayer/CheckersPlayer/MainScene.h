#pragma once
#include "Scene.h"
#include "Position.h"
#include "NeuralNetInf.h"

extern NeuralNetInf network;
float evaluate(const Position& pos);

class MainScene :
	public Scene
{
	Position pos;

	SDL_Texture* board;
	SDL_Texture *whiteMan, *blackMan, *whiteKing, *blackKing;

	SDL_Texture* selection;

	bool playersTurn;
	Square lastSquare;

	Square lastSelected;

	void DrawBoard();

public:

	MainScene(Game* game);
	~MainScene();

	void Update() override;
	void Render() override;

	// Inherited via Scene
	virtual void PollEvent(SDL_Event* event) override;
};

