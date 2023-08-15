#pragma once
#include "WindowContainer.h"
#include "Timer.h"

class Engine : WindowContainer
{
public:
	bool Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height);
	bool ProcessMessages();
	void Update(float dt);
	void RenderFrame(float dt);
	
private:
	//Timer timer;
	bool moveswitch =true;
	float at = .0f;
};