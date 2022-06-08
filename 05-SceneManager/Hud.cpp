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
	
	if (CMario::GetInstance()->IsOnPowerMode())
		powerMeterAniId = ID_ANI_POWER_START_TIER_0;
	else if (playerSpeedX < SPEED_X_TIER_1)
		powerMeterAniId = ID_ANI_POWER_START_TIER_0;
	else if (playerSpeedX < SPEED_X_TIER_2)
		powerMeterAniId = ID_ANI_POWER_START_TIER_1;
	else if (playerSpeedX < SPEED_X_TIER_3)
		powerMeterAniId = ID_ANI_POWER_START_TIER_2;
	else if (playerSpeedX < SPEED_X_TIER_4)
		powerMeterAniId = ID_ANI_POWER_START_TIER_3;
	else if (playerSpeedX < SPEED_X_TIER_5)
		powerMeterAniId = ID_ANI_POWER_START_TIER_4;
	else if (playerSpeedX < SPEED_X_TIER_6)
		powerMeterAniId = ID_ANI_POWER_START_TIER_5;
	else if (playerSpeedX < SPEED_X_TIER_7)
		powerMeterAniId = ID_ANI_POWER_START_TIER_6;
	else
		powerMeterAniId = ID_ANI_POWER_START_TIER_7;

	CSprites::GetInstance()->Get(ID_SPRITE_BLACK_BG)->Draw(camX + 136, hudPosY - BLACK_BG_POS_Y + 2);
	CSprites::GetInstance()->Get(ID_SPRITE_HUD_BAR)->Draw(hudPosX, hudPosY);
	CAnimations::GetInstance()->Get(powerMeterAniId)->Render(hudPosX - POWER_START_POS_X, hudPosY - POWER_START_POS_Y);
}