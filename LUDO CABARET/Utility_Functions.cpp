#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
//...Header file to be defined here...//
#include"Utility_Functions.h"
positionOnWindow get_row_col_by_click(sf::RenderWindow& window)
{
	sf::Event evnt;
	while (1)
	{
		while (window.pollEvent(evnt))
		{
			if (evnt.type == evnt.Closed)
				window.close();
			if (evnt.type == evnt.MouseButtonPressed && sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				auto temp = sf::Mouse::getPosition(window);
				return { temp.y,temp.x };
			}
		}
	}
}
