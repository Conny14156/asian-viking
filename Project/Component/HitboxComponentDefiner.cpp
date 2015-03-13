#include "HitboxComponent.hpp"
#include "../Engine.hpp"
#include "GameObject.h"
#include "RenderComponent.h"
#include "TransformComponent.hpp"
const unsigned int IBaseComponent<HitboxComponent>::typeID = 1003;
std::string IBaseComponent<HitboxComponent>::componentName = "HitBoxComponent";
HitboxComponent::HitboxComponent():size(128,128),position(0,0)
{


}
HitboxComponent::HitboxComponent(const HitboxComponent &hc) :size(hc.size), position(hc.position)
{


}
HitboxComponent::HitboxComponent(int width,int height):size(width,height),position(0,0)
{
	

}
void HitboxComponent::attachOn(GameObject* go)
{
	//Dont put this in constructors, Cause rigidcomponent will call this twice from constructor hierarchy calls
	REGISTER_EDITOR_VARIABLE(Vector2,position,Position);
	REGISTER_EDITOR_VARIABLE(Vector2,size,Size);
	
	BaseComponent::attachOn(go);
	Engine::Physics.addPhysics(this);
}