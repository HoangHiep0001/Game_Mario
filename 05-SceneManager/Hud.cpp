#include "Hud.h"
#include "Mario.h"

void CHud::Render()
{
	float camX, camY;
	CGame::GetInstance()->GetCamPos(camX, camY);

	float hudPosX = camX + HUD_POS_X;
	float hudPosY = camY + GAME_SCREEN_HEIGHT - HUD_POS_Y;

	float playerSpeedX = abs(CMario::GetInstance()->GetSpeedX());

	int powerMeterAniId = -1;
	powerMeterAniId = ID_ANI_POWER_START_TIER_0;


	CSprites::GetInstance()->Get(ID_SPRITE_BLACK_BG)->Draw(camX + 136, hudPosY - BLACK_BG_POS_Y + 2);
	CSprites::GetInstance()->Get(ID_SPRITE_HUD_BAR)->Draw(hudPosX, hudPosY);
	CAnimations::GetInstance()->Get(powerMeterAniId)->Render(hudPosX - POWER_START_POS_X, hudPosY - POWER_START_POS_Y);
}