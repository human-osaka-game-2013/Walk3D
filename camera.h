#ifndef _CAMERA_H_
#define _CAMERA_H_
#include "..\LibGame\LibGame.h"
void CameraControl( D3DXVECTOR3 pos, const CInputState* is);
D3DXVECTOR3 GetCamPos();
void SetCamPos( D3DXVECTOR3 );
#endif