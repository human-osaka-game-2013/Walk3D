#ifndef PLAYER_H_
#define PLAYER_H_

#include"chara.h"
#include"XAudio.h"


class CPlayer : public CChara
{
private:

public:
	enum
	{
		ST_WAIT,
		ST_RUN,
	};
	int shader_num;
	void Control();
	void Animation( XAudio* audio );

};
#endif