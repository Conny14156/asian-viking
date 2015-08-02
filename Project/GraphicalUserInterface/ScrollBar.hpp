#ifndef ScrollBar_HPP
#define ScrollBar_HPP
#include <SFML\Graphics\VertexArray.hpp>
#include "Button.hpp"
#include "../Text/Message.hpp"
class BaseWindowObject;
class BaseWindow;
class ScrollBar
{
public:

	ScrollBar(int x, int y, int sizeX, int sizeY, int iconSize, Vector2 iconOffset, BaseWindow* parent, bool swapItems);

	int selectedBag = 0;
	int swapBag = 0;

	bool movingItem = false;
	bool unequipBag = false;
	bool equipItem = false;
	bool requestSwap = false;

	//offset from main sprite
	Vector2 offset;
	sf::Sprite* selected;
	std::vector<BaseWindowObject*> sprites;
	std::pair<sf::Sprite*, ToolTip> tooltip;
	sf::VertexArray scrollLine;

	void draw();
	void setPosition(int x, int y);
	void setSizeY(int sizeY);
	void setScrollBarSize(int size);
	void setSizeX(int x);
	void setScroll(float deltaScroll);
	
	bool getIfMovingItem();
	bool mouseInside();
	bool checkCulling(sf::Sprite* sprite, Vector2& icons);
	bool mouseInsideIcon(sf::Sprite& sprite);

private:

	int iconSize;
	
	bool _draw = false;
	bool canSwapItems;
	bool scrolling;

	float scrolled = 0;
	float maxScrolls = 1;
	float deltaScrolls = 0;
	double mousePosY = 0;
	
	Vector2 size;
	Vector2 position;
	Vector2 iconOffset;
	Button top;
	Button bot;
	Button scroll;
	BaseWindow* parent;

	void scrollHandle();
	void setTiles(Vector2& off);
	void checkIfScrollBar(Vector2&off);
	void drawIcon(BaseWindowObject& obj, Vector2& off, int i);
};
#endif