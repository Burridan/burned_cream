#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <assert.h>
#include "main.h"

int main(int argc, char** argv)
{
	window_initAll();
	Character* tigre = character_Init("./data/spritesheet.bmp");
	Background* decors = background_Load("./data/decors.bmp");

	//recuperation des dimensions de la texture
	//texture_info BGInfo={0,0,0,0};

	//SDL_QueryTexture(pTexture, &BGInfo.fmt, &BGInfo.acc, &BGInfo.w, &BGInfo.h);
	//source du sprite

	//destination de depart du sprite
	decors->dstRect = background_setCentered(decors);
	tigre->fg.dstRect  = (SDL_Rect){0,0,128,48};

	//Copie de la texture dans le rendu
	renderBackground(decors);
	character_copyToRender(tigre);
	//rafraichissement du rendu dans la fenetre
	SDL_RenderPresent(getRenderer());

	mainEventLoop();
	//destruction de tout les assets
	character_DestroyAll();
	background_DestroyAll();
	//destruction du moteur de rendu
	SDL_DestroyRenderer(getRenderer());

	SDL_Quit();

	return 0;
}

int mainEventLoop()
{
	int end = 0;
	Character* tgr = getCh(0);
	tgr->bhv = standing;
	tgr->dir = E;
	unsigned char dirMask=0;
	SDL_Event evenement;
	printf("Starts main loop\n");
	while(!end)
	{
		SDL_PollEvent(&evenement);
		character_setDirection(getCh(0),dirMask);
		if(tgr->bhv == moving)
		{
			refreshAnimation(ANIM_FPS);
		}
		switch(evenement.type)
		{
			case SDL_KEYDOWN:
				switch(evenement.key.keysym.sym)
				{
					case SDLK_q:
						end=1;
					break;
					case SDLK_k: case SDLK_UP: dirMask 	 |= 8; break;
					case SDLK_j: case SDLK_DOWN: dirMask  |= 4; break;
					case SDLK_h: case SDLK_LEFT: dirMask  |= 2; break;
					case SDLK_l: case SDLK_RIGHT: dirMask |= 1; break;

				}
			break;
			case SDL_KEYUP:
				switch(evenement.key.keysym.sym)
				{
					case SDLK_r:
						printf("r released\n");
					break;
					case SDLK_k: case SDLK_UP: dirMask 	 &= 7 ; break;
					case SDLK_j: case SDLK_DOWN: dirMask  &= 11; break;
					case SDLK_h: case SDLK_LEFT: dirMask  &= 13; break;
					case SDLK_l: case SDLK_RIGHT: dirMask &= 14; break;
				}
			break;
			default:
			break;
		}
	}
return 0;
}
void renderBackground(Background* bg)
{
	if(SDL_RenderCopy(getRenderer(),bg->sprite.pTexture,
		&(bg->srcRect),
		&(bg->dstRect)))
		fprintf(stderr,"Echec de copie : %s\n",SDL_GetError());
}
void refreshAnimation(unsigned char fps)
{
	static unsigned int cTime, lTime;
	cTime = SDL_GetTicks();
	//printf("last time : %d\n current time : %d\n",lTime,cTime);
	if(cTime > (lTime + (1000/fps)))
	{
		Character* pTgr = getCh(0);
		Background* pBg = getBg(0);
		moveCharacter(pTgr);

		renderBackground(pBg);
		character_copyToRender(pTgr);
		SDL_RenderPresent(getRenderer());
		lTime = cTime;
	}
}
void moveCharacter(Character* pCh)
{
	const float qPi = 0.7853; //un pas diagonal vaut un quart de pi sur x et y
	const int pas = 10; //10 pixels par pas sinon les diagonales sont trop  carrés
	const int pasDiag = qPi * pas;
	SDL_Rect* pPos = &(pCh->fg.dstRect);
	switch(pCh->dir)
	{
		case E: pPos->x+=pas; break;
		case W: pPos->x-=pas; break;
		case S: pPos->y+=pas; break;
		case N: pPos->y-=pas; break;
		case SE:
		pPos->x += pasDiag;
		pPos->y += pasDiag;
		break;
		case SW:
		pPos->x -= pasDiag;
		pPos->y += pasDiag;
		break;
		case NE:
		pPos->x += pasDiag;
		pPos->y -= pasDiag;
		break;
		case NW:
		pPos->x -= pasDiag;
		pPos->y -= pasDiag;
		break;
	}
	//printf("x:%d y:%d\n",pPos->x,pPos->y); 
}
