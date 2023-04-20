#include "App.h"
#include "Input.h"
#include "Window.h"

#include "Defs.h"
#include "Log.h"

#include "SDL/include/SDL.h"


GamePad::GamePad() {
	stateButtons = new KeyState[MAX_BUTTONS]; 
	memset(stateButtons, KEY_IDLE, sizeof(KeyState) * MAX_BUTTONS);

	memset(buttons, 0, sizeof(SDL_GameControllerButton) * MAX_BUTTONS);

	buttons[0] = SDL_CONTROLLER_BUTTON_A;
	buttons[1] = SDL_CONTROLLER_BUTTON_B;
	buttons[2] = SDL_CONTROLLER_BUTTON_X;
	buttons[3] = SDL_CONTROLLER_BUTTON_Y;
	buttons[4] = SDL_CONTROLLER_BUTTON_BACK;
	buttons[5] = SDL_CONTROLLER_BUTTON_GUIDE;
	buttons[6] = SDL_CONTROLLER_BUTTON_START;
	buttons[7] = SDL_CONTROLLER_BUTTON_LEFTSTICK;
	buttons[8] = SDL_CONTROLLER_BUTTON_RIGHTSTICK;
	buttons[9] = SDL_CONTROLLER_BUTTON_LEFTSHOULDER;
	buttons[10] = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER;
	buttons[11] = SDL_CONTROLLER_BUTTON_DPAD_UP;
	buttons[12] = SDL_CONTROLLER_BUTTON_DPAD_DOWN;
	buttons[13] = SDL_CONTROLLER_BUTTON_DPAD_LEFT;
	buttons[14] = SDL_CONTROLLER_BUTTON_DPAD_RIGHT;

	enabled = false;
}

GamePad::~GamePad()
{
	RELEASE(stateButtons);
}

void GamePad::DeviceConnection(int index)
{
	if (this != nullptr)
	{
		if (SDL_IsGameController(index))
		{
			if (enabled == false)
			{
				if (controller = SDL_GameControllerOpen(index))
				{
					LOG("Found a gamepad with id %i named %s", 0, SDL_GameControllerName(controller));
					enabled = true;
					left_dz = right_dz = 0.1f;
					haptic = SDL_HapticOpen(index);
					if (haptic != nullptr)
						LOG("... gamepad has force feedback capabilities");
					index = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controller));
				}
			}
		}
	}
}

void GamePad::DeviceRemoval(int index)
{
	// If an existing gamepad has the given index, deactivate all SDL device functionallity

	if (enabled)
	{
		SDL_HapticClose(haptic);
		SDL_GameControllerClose(controller);
		RELEASE(stateButtons);
	}

}

void GamePad::UpdateGamepadInput()
{
	// Update all input data

	if (enabled == true)
	{
		//int b = SDL_GameControllerEventState(-1);

		for (int i = 0; i < MAX_BUTTONS; ++i)
		{
			if (SDL_GameControllerGetButton(controller, buttons[i]) == 0)
			{
				// KEY IDLE

				if (stateButtons[i] == KEY_REPEAT || stateButtons[i] == KEY_DOWN)
					stateButtons[i] = KEY_UP;
				else
					stateButtons[i] = KEY_IDLE;
			}
			else if (SDL_GameControllerGetButton(controller, buttons[i]) == 1)
			{
				if (stateButtons[i] == KEY_IDLE)
					stateButtons[i] = KEY_DOWN;
				else
					stateButtons[i] = KEY_REPEAT;
				// KEY_DOWN
			}

			l2 = float(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT)) / 32767.0f;
			r2 = float(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT)) / 32767.0f;

			left_x = float(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX)) / 32767.0f;
			left_y = float(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY)) / 32767.0f;
			right_x = float(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX)) / 32767.0f;
			right_y = float(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY)) / 32767.0f;

			// Apply deadzone. All values below the deadzone will be discarded
			left_x = (fabsf(left_x) > left_dz) ? left_x : 0.0f;
			left_y = (fabsf(left_y) > left_dz) ? left_y : 0.0f;
			right_x = (fabsf(right_x) > right_dz) ? right_x : 0.0f;
			right_y = (fabsf(right_y) > right_dz) ? right_y : 0.0f;


		}
	}

}

Input::Input(bool isActive) : Module(isActive)
{
	name.Create("input");

	keyboard = new KeyState[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KeyState) * MAX_KEYS);
	memset(mouseButtons, KEY_IDLE, sizeof(KeyState) * NUM_MOUSE_BUTTONS);

	pad = new GamePad(); 
}

// Destructor
Input::~Input()
{
	delete[] keyboard;
	RELEASE(pad); 
}

// Called before render is available
bool Input::Awake(pugi::xml_node& config)
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0)
	{
		LOG("SDL_INIT_GAMECONTROLLER could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	if (SDL_InitSubSystem(SDL_INIT_HAPTIC) < 0)
	{
		LOG("SDL_INIT_HAPTIC could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called before the first frame
bool Input::Start()
{
	SDL_StopTextInput();
	return true;
}

// Called each loop iteration
bool Input::PreUpdate()
{
	static SDL_Event event;

	const Uint8* keys = SDL_GetKeyboardState(NULL);

	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if(keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if(keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	for(int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if(mouseButtons[i] == KEY_DOWN)
			mouseButtons[i] = KEY_REPEAT;

		if(mouseButtons[i] == KEY_UP)
			mouseButtons[i] = KEY_IDLE;
	}

	while(SDL_PollEvent(&event) != 0)
	{
		switch(event.type)
		{
			case SDL_QUIT:
				windowEvents[WE_QUIT] = true;
			break;

			case SDL_WINDOWEVENT:
				switch(event.window.event)
				{
					//case SDL_WINDOWEVENT_LEAVE:
					case SDL_WINDOWEVENT_HIDDEN:
					case SDL_WINDOWEVENT_MINIMIZED:
					case SDL_WINDOWEVENT_FOCUS_LOST:
					windowEvents[WE_HIDE] = true;
					break;

					//case SDL_WINDOWEVENT_ENTER:
					case SDL_WINDOWEVENT_SHOWN:
					case SDL_WINDOWEVENT_FOCUS_GAINED:
					case SDL_WINDOWEVENT_MAXIMIZED:
					case SDL_WINDOWEVENT_RESTORED:
					windowEvents[WE_SHOW] = true;
					break;
				}
			break;

			case SDL_CONTROLLERDEVICEADDED:
				pad->DeviceConnection(event.cdevice.which);
			break;
			case SDL_CONTROLLERDEVICEREMOVED:
				pad->DeviceRemoval(event.cdevice.which);
				RELEASE(pad); 
			break;

			case SDL_MOUSEBUTTONDOWN:
				mouseButtons[event.button.button - 1] = KEY_DOWN;
				//LOG("Mouse button %d down", event.button.button-1);
			break;

			case SDL_MOUSEBUTTONUP:
				mouseButtons[event.button.button - 1] = KEY_UP;
				//LOG("Mouse button %d up", event.button.button-1);
			break;

			case SDL_MOUSEMOTION:
				int scale = app->win->GetScale();
				mouseMotionX = event.motion.xrel / scale;
				mouseMotionY = event.motion.yrel / scale;
				mouseX = event.motion.x / scale;
				mouseY = event.motion.y / scale;
				//LOG("Mouse motion x %d y %d", mouse_motion_x, mouse_motion_y);
			break;
			
		}
	}
	if (pad) pad->UpdateGamepadInput(); 

	return true;
}

// Called before quitting
bool Input::CleanUp()
{
	LOG("Quitting SDL event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);

	if (pad->haptic != nullptr)
	{
		SDL_HapticStopAll(pad->haptic);
		SDL_HapticClose(pad->haptic);
	}

	RELEASE(pad);
	SDL_QuitSubSystem(SDL_INIT_HAPTIC);
	SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
	SDL_QuitSubSystem(SDL_INIT_EVENTS);

	return true;
}


bool Input::GetWindowEvent(EventWindow ev)
{
	return windowEvents[ev];
}

void Input::GetMousePosition(int& x, int& y)
{
	x = mouseX;
	y = mouseY;
}

void Input::GetMouseMotion(int& x, int& y)
{
	x = mouseMotionX;
	y = mouseMotionY;
}

int Input::GetWorldMouseXRelativeToPlayer(int playerPosX) const
{
	int mousePosX = GetScreenMouseX();
	int halfWindowWidth = 1920 / 2;

	if (mousePosX < halfWindowWidth)
	{
		mousePosX = playerPosX - (halfWindowWidth - mousePosX);
	}
	else
	{
		mousePosX = playerPosX + (mousePosX - halfWindowWidth);
	}

	return mousePosX;
}

int Input::GetWorldMouseYRelativeToPlayer(int playerPosY) const
{
	int mousePosY = GetScreenMouseY();
	int halfWindowHeigth = 1080 / 2;

	if (mousePosY < halfWindowHeigth)
	{
		mousePosY = playerPosY - (halfWindowHeigth - mousePosY);
	}
	else
	{
		mousePosY = playerPosY + (mousePosY - halfWindowHeigth);
	}

	return mousePosY;
}