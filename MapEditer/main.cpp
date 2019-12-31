#include "dtest.h"
#include <time.h>

#include "../main.h"
#include "mapmode.h"

CMapMode	MapMode;

void StartUp()
{
	DTest.SetWindowSize(720,720);
	DTest.FullScreen(false);
	DTest.SetWindowTitle("MapEditer");
}
void AfterStartUp()
{
	MapMode.AfterStartUp();
	DTest.CreateFont(0,15,NULL,1000);
}
void OnTimer()
{
	MapMode.OnTimer();
}
void OnDraw()
{
	MapMode.OnDraw();
}
void End()
{
} 