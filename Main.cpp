//#include <winsock2.h>
#include "Game.h"

int main()
{
    srand(time(NULL));
	GameInit();

	while(Juego.Estado != FIN)// mientras el estado no sea fin se haran las sgts instrucciones.
	{
		GameUpdate();
		GameRender();
		rest(40);// El FPS
	}
	textout_ex(Juego.buffer, font, "El juego ha terminado!", 450,H/2, makecol(255,0,0),-1);
	//GameRender();
	rest(1000);
	GameDeinit();
	return 0;
}

END_OF_MAIN();
