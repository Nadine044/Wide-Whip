#ifndef __COIN_H__
#define __COIN_H__

#include "Entity.h"
#include "Animation.h"

class Entity;
class Animation;

enum class COIN_STATE
{
	UNKNOWN = -1,
	IN_GAME,
	PICKED
};

class Coin : public Entity
{
public:
	Coin(SDL_Rect&);

	//Destructor
	~Coin();

	bool Awake(const pugi::xml_node& coin) override;

	bool Start();
	bool Update(float dt) override;
	bool CleanUp() override;

	bool Save(pugi::xml_node&) const override;
	bool Load(pugi::xml_node&) override;

	void OnTrigger(Collider* col2) override;

protected:
	Animation			rotating;

	p2String			text_path;

	COIN_STATE			state;

	//FX here
	
};


#endif // !__COIN_H__
