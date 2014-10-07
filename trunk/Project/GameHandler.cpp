#include "Game.h"
#include <SFML\Window\Mouse.hpp>
#include <vector>
#include "Component\GameObject.h"
#include "Component\IBaseComponent.hpp"
#include "CustomWindow.h"
#include "Component\HitboxComponent.hpp"
#include "Component\TransformComponent.hpp"
#include "Component\RenderComponent.h"
#include "Gfx.h"
#include <SFML\Window\Keyboard.hpp>
#include <SFML\Window\Event.hpp>
#include "Window.h"
#include "Engine.hpp"
void Game::Update()
{
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		SetGfx()->camera.move(0,10);

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		SetGfx()->camera.move(-10,0);

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		SetGfx()->camera.move(0,-10);

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		SetGfx()->camera.move(10,0);

	SetWindow()->setView(SetGfx()->camera);
}

void synchronizeComponents()
{

}
void mouseClick()
{
	
}
