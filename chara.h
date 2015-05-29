#ifndef CHARA_H_
#define CHARA_H_
#define BOMB_NUM 5

#define PL_CAMERA_LENGTH 20.f
#define PL_CAMERA_HIGH 37.f
#define BOMB_INITLENGTH 20.f
#define HUDA_EFFECT_SPEED 0.0015f
#include "..\LibGame\LibGame.h"
class CChara
{
protected:
	BOOL m_DamageFlag;	//ダメージフラグ
	int m_DamageAfterTime;	//鋼体時間
public:
	int status;
	D3DXVECTOR3 Pos;
	float angle;
	float size;
	float speed;
	int HP;
	void SetDamageFlag( BOOL _flag ){ m_DamageFlag = _flag; }
	BOOL GetDamageFlag(){ return m_DamageFlag; }
};

#endif