#pragma once
#include<SFML/Graphics.hpp>
struct Position
{
	int path_index{}, floor{};
};
struct positionOnWindow
{
	int row{}, col{};
};

positionOnWindow get_row_col_by_click(sf::RenderWindow&);