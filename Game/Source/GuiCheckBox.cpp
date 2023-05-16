#include "GuiCheckBox.h"
#include "App.h"
#include "Scene.h"
#include "GuiManager.h"
#include "Audio.h"
#include "Window.h"

GuiCheckBox::GuiCheckBox(uint32 id, SDL_Rect bounds) : GuiControl(GuiControlType::CHECKBOX, id)
{
    this->bounds = bounds;
}

GuiCheckBox::~GuiCheckBox()
{
}

bool GuiCheckBox::Update(Input* input, float dt)
{
    if (state != GuiControlState::DISABLED)
    {
        int mouseX, mouseY;
        input->GetMousePosition(mouseX, mouseY);

        // Check collision between mouse and button bounds
        if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) &&
            (mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
        {
            state = GuiControlState::FOCUSED;

            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
            {
                state = GuiControlState::PRESSED;
            }

            // If mouse button pressed -> Generate event!
            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
            {
                state = GuiControlState::SELECTED;
            }
        }
        else state = GuiControlState::NORMAL;
    }

    return false;
}

bool GuiCheckBox::Draw(Render* render)
{

    // Draw the right button depending on state
    switch (state)
    {
    case GuiControlState::DISABLED:
        break;

    case GuiControlState::NORMAL:
        if (!checked) render->DrawTexture(texture, bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, &SDL_Rect({ 0,0,20,20 }), 0, 0, 0, 0);
        else render->DrawTexture(texture, bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, &SDL_Rect({ 60,0,20,20 }), 0, 0, 0, 0);
        break;

    case GuiControlState::FOCUSED:
        if (!checked) render->DrawTexture(texture, bounds.x, bounds.y, &SDL_Rect({ 20,0,20,20 }), 0, 0, 0, 0);
        else render->DrawTexture(texture, bounds.x, bounds.y, &SDL_Rect({ 80,0,20,20 }), 0, 0, 0, 0);
        break;

    case GuiControlState::PRESSED:
        if (!checked) render->DrawTexture(texture, bounds.x, bounds.y, &SDL_Rect({ 40,0,20,20 }), 0, 0, 0, 0);
        else render->DrawTexture(texture, bounds.x, bounds.y, &SDL_Rect({ 100,0,20,20 }), 0, 0, 0, 0);
        break;

    case GuiControlState::SELECTED:
        NotifyObserver();
        break;

    default:
        break;
    }
    return false;
}

bool GuiCheckBox::DrawDebug(Render* render)
{
    int scale = app->win->GetScale();

    SDL_Rect drawBounds = SDL_Rect({ bounds.x * scale - app->render->camera.x, bounds.y * scale - app->render->camera.y, bounds.w * scale, bounds.h * scale });

    switch (state)
    {
    case GuiControlState::DISABLED:
        render->DrawRectangle(drawBounds, 255, 0, 0, 128, true, false);
        break;
    case GuiControlState::FOCUSED:
        render->DrawRectangle(drawBounds, 0, 255, 0, 128, true, false);
        break;
    case GuiControlState::NORMAL:
        render->DrawRectangle(drawBounds, 0, 0, 255, 128, true, false);
        break;
    case GuiControlState::PRESSED:
        render->DrawRectangle(drawBounds, 255, 255, 0, 128, true, false);
        break;
    case GuiControlState::SELECTED:
        render->DrawRectangle(drawBounds, 0, 255, 255, 128, true, false);
        break;
    }

    return true;
}