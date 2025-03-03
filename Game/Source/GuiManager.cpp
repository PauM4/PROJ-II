#include "GuiManager.h"
#include "App.h"
#include "Textures.h"

#include "GuiButton.h"
#include "Audio.h"

GuiManager::GuiManager(bool isActive) :Module(isActive)
{
	name.Create("guiManager");
}

GuiManager::~GuiManager() {}

bool GuiManager::Start()
{
	hoverFxId = app->audio->LoadFx("Assets/Sounds/FX/ui_hover_button.wav");
	pressedFxId = app->audio->LoadFx("Assets/Sounds/FX/ui_pressed_button.wav");

	buttonNormalTexture = app->tex->Load("Assets/UI/buttonNormal.png");
	buttonHoverTexture = app->tex->Load("Assets/UI/buttonHover.png");
	buttonPressedTexture = app->tex->Load("Assets/UI/buttonPressed.png");

	return true;
}

GuiControl* GuiManager::CreateGuiControl(GuiControlType type, int id, const char* text, SDL_Rect bounds, Module* observer, SDL_Rect sliderBounds)
{
	// L15: DONE1: Create a GUI control and add it to the list of controls

	GuiControl* guiControl = nullptr;

	//Call the constructor according to the GuiControlType
	switch (type)
	{
	case GuiControlType::BUTTON:
		guiControl = new GuiButton(id, bounds, text);
		break;
		/*
		case GuiControlType::TOGGLE:
			break;
		case GuiControlType::CHECKBOX:
			break;
		case GuiControlType::SLIDER:
			break;
		case GuiControlType::SLIDERBAR:
			break;
		case GuiControlType::COMBOBOX:
			break;
		case GuiControlType::DROPDOWNBOX:
			break;
		case GuiControlType::INPUTBOX:
			break;
		case GuiControlType::VALUEBOX:
			break;
		case GuiControlType::SPINNER:
			break;
		default:
			break;
			*/
	}

	//Set the observer
	guiControl->SetObserver(observer);

	// Created GuiControls are add it to the list of controls
	guiControlsList.Add(guiControl);

	return guiControl;
}

bool GuiManager::Update(float dt)
{
	accumulatedTime += dt;
	if (accumulatedTime >= updateMsCycle) doLogic = true;

	// We control how often the GUI is updated to optimize the performance
	if (doLogic == true)
	{
		ListItem<GuiControl*>* control = guiControlsList.start;

		while (control != nullptr)
		{
			if (control->data->state != GuiControlState::NONE)
			{
				control->data->Update(dt);
			}
			control = control->next;
		}

		accumulatedTime = 0.0f;
		doLogic = false;
	}

	return true;
}

bool GuiManager::Draw() {

	ListItem<GuiControl*>* control = guiControlsList.start;

	while (control != nullptr)
	{
		if (control->data->state != GuiControlState::NONE)
		{
			control->data->Draw(app->render);
		}
		control = control->next;
	}

	return true;

}

bool GuiManager::CleanUp()
{
	ListItem<GuiControl*>* control = guiControlsList.start;

	while (control != nullptr)
	{
		RELEASE(control);
	}

	app->tex->UnLoad(buttonNormalTexture);
	app->tex->UnLoad(buttonHoverTexture);
	app->tex->UnLoad(buttonPressedTexture);

	return true;

	return false;
}



