#ifndef __SDL_TIMER_H__
#define __SDL_TIMER_H__	

#include "SDL/include/SDL.h"

enum estadoTimerP {
	ENCENDIDO,
	EJECUTANDO,
	FIN,
	APAGADO,
};


class SDL_Timer
{
public:
	void Start(float m_duration);
	int Test();

private:
	uint32_t startTime;
	uint32_t duration; //segundos de cron�metro

public:
	estadoTimerP estadoTimer = APAGADO;
};

#endif // !__SDL_TIMER_H__