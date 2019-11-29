#ifndef __ENEMYFLY_H__
#define __ENEMYFLY_H__

#include "Enemy.h"
#include "p2DynArray.h"
#include "p2Point.h"

class EnemyFly : public Enemy
{
public:
	EnemyFly(SDL_Rect& rect);

private:
	void GoToNextPoint() override;
	void MoveNormal(const iPoint * next_point);
	void MoveInCollision(const iPoint * next_point);
};

#endif // !__ENEMYFLY_H__
