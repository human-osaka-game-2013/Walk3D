#include"camera.h"
#define SPEEDCAM 1.f
D3DXVECTOR3 CamPos;
float a = 0.f;
void CameraControl( D3DXVECTOR3 pos, const CInputState* is)
{
	float kakudo = atan2(pos.x-CamPos.x,pos.z-CamPos.z);
	if(is->Button[13])
	{
		CamPos.z+=SPEEDCAM;
	}
	if(is->Button[11])
	{
		
		CamPos.z -= SPEEDCAM;
	}
	if(is->Button[10])
	{
		CamPos.x -= SPEEDCAM;
	}
	if(is->Button[12])
	{
		CamPos.x += SPEEDCAM;
	}
	//a+= 0.05f;
	//if(fabs(pos.x-CamPos.x) + fabs(pos.z-CamPos.z) >=25)
	//{
		
	//	CamPos.x += 0.1f * sin(kakudo);
	//	CamPos.z += 0.1f * cos(kakudo);
	//}

	//if(fabs(pos.x-CamPos.x) + fabs(pos.z-CamPos.z) <=15)
	//{
	//	float kakudo = atan2(pos.x-CamPos.x,pos.z-CamPos.z);
	//	CamPos.x -= 0.1f * sin(kakudo);
	//	CamPos.z -= 0.1f * cos(kakudo);
	//}
	
}

D3DXVECTOR3 GetCamPos()
{
	return CamPos;
}

void SetCamPos( D3DXVECTOR3 _pos )
{
	CamPos = _pos;
}