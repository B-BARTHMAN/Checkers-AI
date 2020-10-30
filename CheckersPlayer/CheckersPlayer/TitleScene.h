#pragma once
#include "Scene.h"

class TitleScene :
	public Scene
{

public:

	TitleScene(Game* game);
	~TitleScene();
	
	void Update() override;
	void Render() override;

	virtual void PollEvent(SDL_Event* event) override;
};

