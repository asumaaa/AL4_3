#pragma once
#include "dxgidebug.h"
#include <d3d12.h>

class Masage
{
public:
	static Masage* GetInstance();
	bool Update();
	//Xボタンで終了メッセ時が来たらゲームループを抜ける 
	bool ExitGameloop();
public:
	MSG msg{};
};

