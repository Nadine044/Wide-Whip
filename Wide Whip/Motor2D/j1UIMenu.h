#ifndef __UIMENU_H__
#define __UIMENU_H__

#include "j1Module.h"

class UIImage;
class UIInputText;
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
	UIImage*			newGameParent = nullptr;
	UIImage*			settingsParent = nullptr;
	UIImage*			creditsParent = nullptr;

	UIInputText*		name_input_text = nullptr;

	int					musicScrollValue = 0;
	int					sfxScrollValue = 0;

};
#endif // !__UIMENU_H__
