#ifndef __UIMENU_H__
#define __UIMENU_H__

#include "j1Module.h"

class UIImage;

class UIMenu : public j1Module
{
public:
	UIMenu();

	bool Awake(pugi::xml_node& config) override;
	bool Start() override;
	bool PreUpdate() override;
	bool Update(float dt) override;
	bool PostUpdate() override;
	bool CleanUp() override;

	bool ButtonEvent(const UIButtonType type) override;
	bool SliderEvent(const UIScrollBarType type, const float ratio) override;

private:
	UIImage*			settingsParent = nullptr;
	UIImage*			creditsParent = nullptr;

};
#endif // !__UIMENU_H__
