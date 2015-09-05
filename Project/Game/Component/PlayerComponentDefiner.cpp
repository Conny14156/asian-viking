#include "PlayerComponent.hpp"
#include "GameObject.h"
#include "MovementComponent.hpp"
#include "../../Engine.hpp"

const unsigned int IBaseComponent<PlayerComponent>::typeID = 1252352;
std::string IBaseComponent<PlayerComponent>::componentName = "PlayerComponent";

void PlayerComponent::attachOn(GameObject* toAttach)
{
	BaseComponent::attachOn(toAttach);
	toAttach->AddComponent<MovementComponent>();

	//Engine::Input.registerCallback(std::bind(&PlayerComponent::manoeuvre,this,Vector2(0,0)),sf::Keyboard::D,Input::Action::Release);
	Engine::Input.registerCallback(std::bind(&PlayerComponent::manoeuvre,this,Vector2(1,0)),sf::Keyboard::D,Input::Action::Hold);
	Engine::Input.registerCallback(std::bind(&PlayerComponent::manoeuvre,this,Vector2(-1,0)),sf::Keyboard::A,Input::Action::Hold);
	Engine::Input.registerCallback(std::bind(&PlayerComponent::manoeuvre,this,Vector2(0,-1)),sf::Keyboard::W,Input::Action::Hold);
	Engine::Input.registerCallback(std::bind(&PlayerComponent::manoeuvre,this,Vector2(0,1)),sf::Keyboard::S,Input::Action::Hold);

}

void PlayerComponent::manoeuvre(Vector2 Direction)
{
	attachedOn->GetComponent<MovementComponent>()->addForce(Direction,50);
}