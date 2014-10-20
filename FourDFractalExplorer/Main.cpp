

#include "CONSTANTS.H"

#include "Leap.cpp"
#include "Effect.hpp"
#include <SFML/Graphics.hpp>
#include <Vec3.hpp>
#include <iostream>
#include <vector>
FracListener *fracListener;
#include "Quaternion.hpp"
#include "Main.h"

FracListener * getListener(){
	return fracListener;
}

int main(int argc, char **argv){

	Leap::Listener *leaplistener = new FracListener();
	fracListener = ((FracListener *) (leaplistener));
	Leap::Controller controller;
	controller.addListener(*leaplistener);



	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML works!", sf::Style::Fullscreen);

	std::vector<Effect*> effects;

	Quaternion quaternion;
	effects.push_back(&quaternion);

	for (std::size_t i = 0; i < effects.size(); ++i)
		effects[i]->load();

	window.setVerticalSyncEnabled(true);

	while (window.isOpen()){
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if(event.type == sf::Event::KeyPressed)
			{
				if(event.key.code == sf::Keyboard::Escape)
					window.close();
			}

		}

		float mult = 1;
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
			mult = -1;
		else
			mult = 1;

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			fracListener->a -= mult * 0.05;
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			fracListener->b += mult * 0.05;
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			fracListener->c += mult * 0.05;
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::F))
			fracListener->d += mult * 0.05;

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::R)){
			fracListener->a = .5f;
			fracListener->b = .5f;
			fracListener->c = .5f;
			fracListener->d = .5f;
		}

		window.clear();

		for (int i = 0; i < effects.size(); ++i){
			effects[i]->update();
		}



		for (int i = 0; i < effects.size(); ++i){
			window.draw(*(effects[i]));
		}

		window.display();
	}

	return 0;
}