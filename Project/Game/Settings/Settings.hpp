#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <string>
#include <SFML\Graphics.hpp>
struct Settings
{
	enum Difficulty
	{
		easy,
		normal,
		hard,
	};

	std::string continueGameFile = "";

	sf::VideoMode res;

	Difficulty diff;

	bool showOverhead;
	bool showTooltips;

	__int8 volume;
	__int8 volumeEffects;

	template <class archive>
	void load(archive & ar, Settings& settings);
	template <class archive>
	void save(archive & ar, const Settings& settings);
};

#endif