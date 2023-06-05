#include "GuiCheckBox.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Log.h"
#include "Fonts.h"

GuiCheckBox::GuiCheckBox(uint32 id, SDL_Rect bounds, SDL_Texture* tex, const char* text) : GuiControl(GuiControlType::CHECKBOX, id)
{
	this->bounds = bounds;
	this->text = text;
	this->tex = tex;

	canClick = true;
	drawBasic = false;
	crossed = false;
	debug = false;

	/*buttonHovering = app->audio->LoadFx("Assets/Audio/Fx/HoveringButton.wav");*/
	hoverOnce = false;

	/*buttonPressed = app->audio->LoadFx("Assets/Audio/Fx/PressingButton.wav");*/
	pressedOnce = false;

}

GuiCheckBox::~GuiCheckBox()
{

}

bool GuiCheckBox::Update(float dt)
{
	if (state != GuiControlState::DISABLED)
	{
		// Update the state of the GUiButton according to the mouse position
		app->input->GetMousePosition(mouseX, mouseY);

		GuiControlState previousState = state;

		// I'm inside the limitis of the button
		if (mouseX >= bounds.x && mouseX <= bounds.x + bounds.w &&
			mouseY >= bounds.y && mouseY <= bounds.y + bounds.h) {

			state = GuiControlState::FOCUSED;
			if (previousState != state) {
				LOG("Change state from %d to %d", previousState, state);
			}

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN) {
				state = GuiControlState::PRESSED;
			}

			//
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP) {
				NotifyObserver();
			}
		}
		else {
			state = GuiControlState::NORMAL;
		}
	}

	if (state == GuiControlState::PRESSED) {
		if (crossed) crossed = false;
		else crossed = true;
	}

	return false;
}

bool GuiCheckBox::Draw(Render* render)
{
	// Draw the button according the GuiControl State

	rect.x = 0;
	rect.y = 0;
	rect.w = bounds.w;
	rect.h = bounds.h;

	switch (state)
	{
	case GuiControlState::DISABLED:

		//render->DrawRectangle(bounds, 200, 200, 200, 255, true, false);
		break;
	case GuiControlState::NORMAL:

		//render->DrawRectangle(bounds, 0, 0, 255, 255, true, false);
		app->render->DrawTexture(tex, -app->render->camera.x + bounds.x, -app->render->camera.y + bounds.y, &rect);

		break;
	case GuiControlState::FOCUSED:

		rect.y = bounds.h;
		app->render->DrawTexture(tex, -app->render->camera.x + bounds.x, -app->render->camera.y + bounds.y, &rect);

		break;
	case GuiControlState::PRESSED:

		rect.y = bounds.h * 2;
		app->render->DrawTexture(tex, -app->render->camera.x + bounds.x, -app->render->camera.y + bounds.y, &rect);
		break;
	}

	if (crossed && state != GuiControlState::DISABLED) {
		SDL_Rect cross = { 0,bounds.h * 3,bounds.w,bounds.h };
		app->render->DrawTexture(tex, -app->render->camera.x + bounds.x, -app->render->camera.y + bounds.y, &cross);
	}

	app->fonts->DrawText(text.GetString(), bounds.x + bounds.w / 4, bounds.y, bounds.w, bounds.h, { 255,255,255 }, app->fonts->gameFont, true);

	return false;
}