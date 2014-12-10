#include <Windows.h>
#include <list>
#include <math.h>
#include "CustomWindow.h"
#include "Gfx.h"

#include "Game.h"
#include "Component\GameObject.h"
#include "Component\RenderComponent.h"
#include <SFML\Graphics\Sprite.hpp>
#include "Component\TransformComponent.hpp"
#include "Component\DialogComponent.hpp"
Gfx gfx;
Gfx::Gfx()
{
	camera.reset(sf::FloatRect(0,0,800,600));
/*	if(!rex.create(camera.getSize().x,camera.getSize().y))
	{
		std::cout << "Error creating Rex";
	}
	/*
	for(int i = 0; i < 5; i++)
	{
		std::vector<Tile> temp;

		for(int j = 0; j < 5; j++)
		{
			Tile tTemp(i,j);
			tTemp.sprite.setPosition(i*tTemp.sprite.getTextureRect().width,j*tTemp.sprite.getTextureRect().height);
			temp.push_back(tTemp);
		}
		tileList.push_back(temp);
	}
	//*/
}
bool Gfx::loadTexture(std::string name) 
{
	sf::Texture tempTexture;
	//Remove this one day
	std::cout << "Testing";


	if (!tempTexture.loadFromFile(name))
	{
		MessageBox(0, "Error loading this file", name.c_str(), MB_OK);
		return false;
	}	
	loadedTextureMap.insert(loadedTextureMap.end(),std::make_pair(name, tempTexture));
		
	return true;

}
sf::Texture* Gfx::requestTexture(std::string name) 
{
	std::map<std::string, sf::Texture>::iterator it;
	it = loadedTextureMap.find(name);
	if (it != loadedTextureMap.end())
		return &it->second;
	if(loadTexture(name))
		return &loadedTextureMap.find(name)->second;
	return false;
}
void Gfx::insertDrawableObject(GameObject* entityToDraw)
{
	std::map<int, std::vector<GameObject*>>::iterator it;
	std::map < int,std::vector<GameObject*>>::iterator endIt;
	int renderID = entityToDraw->GetComponent<RenderComponent>()->renderlayer;

	it = this->gameObjectDrawList.find(renderID);
	endIt = gameObjectDrawList.end();

	if (it == this->gameObjectDrawList.end())
		gameObjectDrawList.insert(gameObjectDrawList.end(), std::pair<int, std::vector<GameObject*>>(renderID, {entityToDraw}));							//it->second.insert(it->second.end(),entityToDraw);
	else if (it != gameObjectDrawList.end())
	{
		bool temp = false; //Rework this, Somehow: check .end()->first < renderID
		#pragma region EntityToDrawInsertionBetween layers
		for (it = gameObjectDrawList.begin(); it != gameObjectDrawList.end(); it++)
		{
			if (renderID > it->first)
			{
				if (renderID < 1 + it->first)
				{
					gameObjectDrawList.insert
											(	gameObjectDrawList.begin(),
												std::pair<int, std::vector<GameObject*>>(renderID, { entityToDraw })
											);
					temp = true;
					break;
				}
			}
		}
		#pragma endregion
		if(!temp)
			gameObjectDrawList.at(renderID).push_back(entityToDraw);
		
	}
}
void Gfx::removeFromDrawList(GameObject* entityToRemove)
{
	int renderID = entityToRemove->GetComponent<RenderComponent>()->renderlayer;
	for (int i = 0; gameObjectDrawList[renderID].size(); i++)
	{
		if (gameObjectDrawList[renderID][i] == entityToRemove)
		{
			// removes the entity pointer from the draw list, gfx does NOT delete the object
			gameObjectDrawList[renderID].erase(gameObjectDrawList[renderID].begin() + i);

			break;
		}
	}
}
void Gfx::Draw()
{
	DrawBG();
	{
		RenderComponent* rc;
		TransformComponent* tc;
		DialogComponent* dc;
		for(std::map<int,std::vector<GameObject*>>::iterator it = gameObjectDrawList.begin(); it != gameObjectDrawList.end(); it++)
		{
			for(int j = 0; j < it->second.size(); j++)
			{
				rc = it->second[j]->GetComponent<RenderComponent>();
				tc = it->second[j]->GetComponent<TransformComponent>();
				dc = it->second[j]->GetComponent<DialogComponent>();
				// not all gameobjects have dialogs
				if (dc)
				{
					if (dc->msg.timer.getElapsedTime().asSeconds() > dc->duration && dc->duration > 0)
						dc->close();
				}
				rc->sprite.setPosition(tc->position.x,tc->position.y);
				Engine::Window.draw(rc->sprite); 
				
			}
		}
	}
	DrawTxt();
	//rex.display();
	//Engine::Window.draw(sf::Sprite(rex.getTexture()));
	//*/
}
void Gfx::DrawBG()
{
	
	SetWindow()->draw(backgroundSprite.sprite);
	for (std::vector<Tile>::iterator it = foregroundSpriteList.begin(); it != foregroundSpriteList.end(); it++)
		SetWindow()->draw(it->sprite);
}
void Gfx::DrawTxt()
{
	for (int i = 0; i < msg.size(); i++)
	{
		msg[i]->drawMessage(SetWindow());
		if (msg[i]->timer.getElapsedTime().asSeconds() > msg[i]->duration && msg[i]->duration > 0)
		{
			removeFromMessageList(msg[i]);
		}
	}
}

void Gfx::insertDrawableMessage(Message* messageToDraw) 
{
	messageToDraw->timer.restart();
	msg.push_back(messageToDraw);
}
void Gfx::removeFromMessageList(Message* messageToRemove)
{
	for (int i = 0; i < msg.size(); i++)
	{
		if (msg[i] == messageToRemove)
		{
			msg.erase(msg.begin()+i);
			delete messageToRemove;
			messageToRemove = 0;
			break;
		}
	}
}
void Gfx::requestBackground(Tile bg)
{
	backgroundSprite = bg;
	backgroundSprite.setRepeated(true);
	backgroundSprite.sprite.setPosition(backgroundSprite.position.x, backgroundSprite.position.y);

	backgroundSprite.sprite.setTextureRect(sf::IntRect(0, 0,
	backgroundSprite.sprite.getTexture()->getSize().x+backgroundSprite.sizeX, 
	backgroundSprite.sprite.getTexture()->getSize().y+backgroundSprite.sizeY));
	backgroundSprite.sprite.setOrigin(backgroundSprite.sprite.getTextureRect().width / 2, backgroundSprite.sprite.getTextureRect().height / 2);
	
}

void Gfx::insertDrawableForeground(Tile fg)
{
	fg.sprite.setPosition(fg.position.x, fg.position.y);

	fg.sprite.setTextureRect(
	sf::IntRect(0, 0,
	fg.sprite.getTexture()->getSize().x,
	fg.sprite.getTexture()->getSize().y)
	);

	fg.sprite.setOrigin(fg.sprite.getTextureRect().width / 2, fg.sprite.getTextureRect().height / 2);
	foregroundSpriteList.push_back(fg);
}
void Gfx::removeFromForegroundList(Tile fgToRemove)
{
	for (int i = 0; i < foregroundSpriteList.size(); i++)
	{
		if (foregroundSpriteList[i].name == fgToRemove.name)
		{
			foregroundSpriteList.erase(foregroundSpriteList.begin() + i);
			break;
		}
	}
}

void Gfx::moveBackground(int x, int y,float panSpeed)
{
	backgroundSprite.sprite.setPosition(backgroundSprite.sprite.getPosition().x + (x*panSpeed), backgroundSprite.sprite.getPosition().y + (y*panSpeed));
}







//ReadOnly, Returns a const value pointer towards gfx
const Gfx* RequestGfx()
{
	return &gfx;
}
//Returns a value pointer toward gfx
Gfx* SetGfx()
{
	return &gfx;
}