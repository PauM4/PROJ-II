#include "GuiSlider.h"
#include "App.h"
#include "Scene.h"
#include "GuiManager.h"
#include "Audio.h"
#include "Window.h"
#include "UIModule.h"

GuiSlider::GuiSlider(uint32 id, SDL_Rect bounds) : GuiControl(GuiControlType::SLIDER, id)
{
    this->bounds = bounds;
    this->unit = bounds.w / 100.0f;

    value = round(value);
    sliderPosx = ((value * unit) + bounds.x) - unit;
}

GuiSlider::~GuiSlider()
{
}

bool GuiSlider::Update(Input* input, float dt)
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

            unit = bounds.w / 100.0f;
            value = (mouseX - bounds.x) / unit;
            value = round(value);

            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
            {
                state = GuiControlState::PRESSED;
                sliderPosx = ((value * unit) + bounds.x) - unit - 5;
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

bool GuiSlider::Draw(Render* render)
{
    // Draw the right button depending on state
    switch (state)
    {
    case GuiControlState::DISABLED:
        break;

    case GuiControlState::NORMAL:
        render->DrawRectangle({ bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, 100, 100 }, 255, 0, 0);
        break;

    case GuiControlState::FOCUSED:
        render->DrawRectangle({ bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, 100, 100 }, 100, 0, 0);
        break;

    case GuiControlState::PRESSED:
        render->DrawRectangle({ bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, 100, 100 }, 200, 0, 0);
        break;

    case GuiControlState::SELECTED:
        render->DrawRectangle({ bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, 100, 100 }, 0, 0, 0);
        break;

    default:
        break;
    }
    return false;
}

bool GuiSlider::DrawDebug(Render* render)
{
    int scale = app->win->GetScale();

    SDL_Rect drawBounds = SDL_Rect({ bounds.x * scale, bounds.y * scale, bounds.w * scale, bounds.h * scale });

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