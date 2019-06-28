#pragma once

#include "Graphics.h"

class BlankDemo : public Graphics
{
public:
	BlankDemo();
	virtual ~BlankDemo();

	bool loadContent()override;
	void unLoadContent()override;

	void update(float dt);
	void render();
};