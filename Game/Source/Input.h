#ifndef __INPUT_H__
#define __INPUT_H__

#include "Module.h"

#include "SDL/include/SDL.h"


//#define NUM_KEYS 352
#define NUM_MOUSE_BUTTONS 5
//#define LAST_KEYS_PRESSED_BUFFER 50
#define MAX_KEYS 300
#define MAX_PADS 4
#define MAX_BUTTONS 15

struct SDL_Rect;

struct _SDL_GameController;
struct _SDL_Haptic;

enum EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class GamePad {
public:
	GamePad(); 
	virtual ~GamePad();

	inline KeyState GetButton(int id) const
	{
		if (this != nullptr) return stateButtons[id];
	}

	void DeviceConnection(int index);

	void DeviceRemoval(int index);

	void UpdateGamepadInput();
public:


	//Input data
	bool a, b, x, y, r1, l1, r2, l2;
	bool left, right, down, up, start;
	float left_x, left_y, right_x, right_y, left_dz, right_dz;

	//Controller data
	bool enabled;
	int index;
	_SDL_GameController* controller;
	_SDL_Haptic* haptic;

	KeyState* stateButtons; 
	SDL_GameControllerButton buttons[MAX_BUTTONS]; 
};

class Input : public Module
{

public:

	Input(bool isActive);

	// Destructor
	virtual ~Input();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	// Called before quitting
	bool CleanUp();

	// Check key states (includes mouse and joy buttons)
	KeyState GetKey(int id) const
	{
		return keyboard[id];
	}

	KeyState GetMouseButtonDown(int id) const
	{
		return mouseButtons[id - 1];
	}

	// Check if a certain window event happened
	bool GetWindowEvent(EventWindow ev);

	// Get mouse / axis position
	void GetMousePosition(int &x, int &y);
	void GetMouseMotion(int& x, int& y);

	int GetScreenMouseX() const
	{
		return mouseX;
	}

	int GetScreenMouseY() const
	{
		return mouseY;
	}

	int GetWorldMouseXRelativeToPlayer(int playerPosX) const;

	int GetWorldMouseYRelativeToPlayer(int playerPosY) const;

	void HandlePadMouse(int mouseX, int mouseY, float mouseSpeed);

private:
	bool windowEvents[WE_COUNT];
	KeyState*	keyboard;
	KeyState mouseButtons[NUM_MOUSE_BUTTONS];
	int	mouseMotionX;
	int mouseMotionY;
	int mouseX;
	int mouseY;
public: 
	GamePad* pad; 
};

#endif // __INPUT_H__