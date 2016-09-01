#include "BaseParseArgument.hpp"
#include "Comparer/CompareResultTypes.hpp"
#include "Reference/ReferenceArgumentTypes.hpp"
#include "Color/ColorArgument.hpp"
#include "Icon\IconArgument.hpp"
namespace GUI
{
	namespace Parser
	{
		namespace ParseArgument
		{
			BaseParseArgument::BaseParseArgument(ArgumentType type):type(type)
			{
			}
			BaseParseArgument* BaseParseArgument::startList()
			{
				return new BaseParseArgument(ArgumentType::BeginList);
			}
			BaseParseArgument* BaseParseArgument::newListLine()
			{
				return new BaseParseArgument(ArgumentType::NewListLine);
			}
			BaseParseArgument* BaseParseArgument::endList()
			{
				return new BaseParseArgument(ArgumentType::EndList);
			}
			BaseParseArgument* BaseParseArgument::colorArgument(sf::Color color)
			{
				return new ColorArgument(color);
			}
			BaseParseArgument* BaseParseArgument::colorArgument(short r, short g, short b, short a)
			{
				return new ColorArgument(sf::Color(r,g,b,a));
			}
			BaseParseArgument* BaseParseArgument::IntCompareArgument(int & first, int & second)
			{
				return new IntCompareResult(first, second);
			}

			BaseParseArgument * BaseParseArgument::IntReferenceArgument(int & reference)
			{
				return new IntReference(reference);
			}

			BaseParseArgument * BaseParseArgument::newIconArgument(sf::Texture& texture, Vector2i size)
			{
				return new IconArgument(texture,Vector2(0,0),size);
			}
		}
	}
}