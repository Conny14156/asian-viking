#ifndef ToolTip_HPP
#define ToolTip_HPP
#include "Text\FloatingText.hpp"
#include <SFML\Graphics\Sprite.hpp>
class ToolTip
{
public:
	ToolTip();
	ToolTip(std::string text);
	ToolTip(std::string text,std::string icon);
	ToolTip(std::string text,std::string body,std::string icon);
	ToolTip(const ToolTip& toolTip);

	void setPosition(double x,double y);
	void draw();

	void draw(std::string text);
	void draw(std::string title,std::string body);

	void setToolTipText(std::string text,std::string body = "");

	const std::string getToolTipTitle();
	const std::string getToolTipBody();
	ToolTip& operator=(const ToolTip&copy);
private:
	FloatingText toolTipText;
	FloatingText toolTipBody;

	sf::Sprite toolTipIcon;
};

#endif