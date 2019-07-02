#pragma once
#include <d3dcompiler.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
class Graphics
{
public:
	Graphics();
	virtual ~Graphics();
	
	bool initialize(HINSTANCE hInstance, HWND hw, bool fullscreen);
	void releaseAll();
	virtual bool loadContent();
	virtual void unLoadContent();

	virtual void update(float dt);
	virtual void render();

private:
	bool fullscreen;
	unsigned int width;
	unsigned int height;
	
protected:
	HINSTANCE hInstance;
	HWND hwnd;
	class D3D* mD3D;
	class Model* triangle;
	class Shader* shader;
	class Camera* camera;
	std::unique_ptr<DirectX::SpriteBatch> mSpriteBatch;
	std::unique_ptr<DirectX::SpriteFont> mSpriteFont;
	
};