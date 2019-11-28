#ifndef __ENEMYFLY_H__
#define __ENEMYFLY_H__

#include "Enemy.h"


class EnemyFly : public Enemy
{
public:
	EnemyFly(SDL_Rect& rect);

private:
	void GoToNextPoint() override;
};

#endif // !__ENEMYFLY_H__
