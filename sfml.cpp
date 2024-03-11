#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>
#include<vector>
#include<string>
#include <sstream>
#include<iostream>


struct BoardProp{
	int dim_x;
	int dim_y;
	std::string edge_side;
	std::string edge_td;
};





std::string Board_Drawer(BoardProp boad){
	std::string map;
	map += boad.edge_side;
	for(int i=0; i<boad.dim_x; i++)map += boad.edge_td;
	/*
	map += [](const char str, int times)
			{std::stringstream stream; for (int i = 0; i < times; i++) stream << str; return stream.str();}
	('#', dims.dim_x);
	*/
	map += boad.edge_side + "\n";
	for(int y=0; y<boad.dim_y; y++){
		map += boad.edge_side;
		map += [](const char str, int times)
					{std::stringstream stream; for (int i = 0; i < times; i++) stream << str; return stream.str();}
		('.', boad.dim_x);
		map += boad.edge_side + "\n";
	}

	map += boad.edge_side;
		for(int i=0; i<boad.dim_x; i++)map += boad.edge_td;
		/*
		map += [](const char str, int times)
				{std::stringstream stream; for (int i = 0; i < times; i++) stream << str; return stream.str();}
		('#', dims.dim_x);
		*/
		map += boad.edge_side + '\n';

	return map;
};

class BoardDrawer{

};

int main()
{

    sf::RenderWindow window(sf::VideoMode(1080,760), "Hello World");

    BoardProp board;

    board.dim_x = 40;
    board.dim_y = 16;
    board.edge_side = "%";
    board.edge_td = '%';

    std::cout<<Board_Drawer(board);


    sf::Font font;
    font.loadFromFile("fonts/1942.ttf");
    sf::Text text;
    text.setFont(font);

    text.setString(Board_Drawer(board));

    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Red);
    //text.setStyle(sf::Text::Bold | sf::Text::Underlined);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(text);
        window.display();
    }

    return 0;
}
