// Game.cpp: 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include "myheader.h"

//! [main]
int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
)
{
	MyTestApp app;
	app.initApp();
	app.getRoot()->startRendering();
	app.closeApp();
	return 0;
}
//! [main]