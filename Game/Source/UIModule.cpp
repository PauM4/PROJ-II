#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "UIModule.h"

#include "Defs.h"
#include "Log.h"

UIModule::UIModule(bool isActive) : Module(isActive)
{
	name.Create("UIModule");
}

// Destructor
UIModule::~UIModule()
{}

// Called before render is available
bool UIModule::Awake(pugi::xml_node& config)
{
	LOG("Loading UIModule");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool UIModule::Start()
{
	currentMenuType = DISABLED;

	uint w, h;
	app->win->GetWindowSize(w, h);
	button1_continue = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Continue", { (int)w - 1820, (int)h - 300, 100, 20 }, this);
	button1_continue->state = GuiControlState::NONE;

	return true;
}

// Called each loop iteration
bool UIModule::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool UIModule::Update(float dt)
{
	switch (currentMenuType)
	{
	case MAIN:
		break;
	case PAUSE:
		break;
	case DIALOG:
		break;
	case COMBAT:
		break;
	case DISABLED:
		break;
	}
	return true;
}

// Called each loop iteration
bool UIModule::PostUpdate()
{
	bool ret = true;

	return ret;
}


// Called before quitting
bool UIModule::CleanUp()
{
	LOG("Freeing UIModule");

	return true;
}

bool UIModule::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Event by %d ", control->id);

	switch (control->id)
	{
	case 1:
		break;
	}
	return true;
}
