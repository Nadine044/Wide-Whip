#ifndef __ENEMYWALK_H__
#define __ENEMYWALK_H__

#include "Enemy.h"

class Collider;

class EnemyWalk : public Enemy
{
public:
	EnemyWalk(SDL_Rect& rect);
	void OnTrigger(Collider* col) override;

	bool Update(float dt) override;

	bool CleanUp() override;

	void Gravity();


private:
	void GoToNextPoint(float dt) override;

private:
	Collider* ground = nullptr;
}; 


#endif // !__ENEMYWALK_H__