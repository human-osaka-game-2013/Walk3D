#ifndef ENEMY_H_
#define ENEMY_H_

#include"chara.h"
#include"player.h"
#include"XAudio.h"
#include "Billboard.h"

class CEnemy : public CChara
{
private:

public:
	enum
	{
		ST_WAIT,
		ST_RUN,

	};
	
	void Animation( XAudio* audio );
	void Control( );
};

#endif