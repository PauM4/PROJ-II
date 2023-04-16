#include "PerformanceBar2.h"
#include "SDL/include/SDL_pixels.h"
#include "Window.h"
#include "App.h"
#include "Render.h"
PerformanceBar::PerformanceBar()
{

}

PerformanceBar::~PerformanceBar()
{

}


void PerformanceBar::Render()
{
	int barHeight = app->win->GetHeight();
	const int numSections = 5; //(prepareUpdate, preUpdate, etc.)

	
	SDL_Color colors[] = {
		{ 255, 255, 255, 255 }, // prepareUpdate - white 
		{ 255, 0, 0, 255 }, // preUpdate - red
		{ 255, 255, 0, 255 }, // doUpdate - yellow
		{ 0, 255, 0, 255 }, // postUpdate - green
		{ 0, 255, 255, 255 }, // finishUpdate - blue
	};

	// Calcula la altura de cada sección de la barra de rendimiento
	double sectionHeights[numSections] = {
		app->prepareUpdate / app->update,
		app->preUpdate / app->update,
		app->doUpdate / app->update,
		app->postUpdate / app->update,
		app->finishUpdate / app->update,
	};

	//std::cout << "prepareUpdate: " << sectionHeights[0] << std::endl;
	//std::cout << "preUpdate" << sectionHeights[1] << std::endl;
	//std::cout << "doUpdate" << sectionHeights[2] << std::endl;
	//std::cout << "postUpdate" << sectionHeights[3] << std::endl;
	//std::cout << "finishUpdate" << sectionHeights[4] << std::endl;

	const int barWidth = 10;
	const int rectHeight = barHeight / numSections;

	// Drawin the rectangle sections
	int y = -app->render->camera.y;
	for (int i = 0; i < numSections; i++)
	{
		int sectionHeight = (int)(sectionHeights[i] * barHeight);
		app->render->DrawRectangle({ -app->render->camera.x, y, barWidth, sectionHeight }, colors[i].r, colors[i].g, colors[i].b, colors[i].a, true);
		y += sectionHeight;
	}
	

}

