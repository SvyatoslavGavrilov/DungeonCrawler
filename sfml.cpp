#include <SFML/Graphics.hpp>
//#include <entt/entt.hpp>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>


struct BoardProp{
	int dim_x;
	int dim_y;
	std::string edge_side;
	std::string edge_td;
};

struct IniSettings{
	int dim_x;
	int dim_y;
	std::string edge_side;
	std::string edge_td;
	short font_size;
};

IniSettings IniDownloader(std::string filename){
	IniSettings settings;
	std::ifstream stngs_strm(filename);
	stngs_strm
	>> settings.dim_x
	>> settings.dim_y
	>> settings.edge_side
	>> settings.edge_td
	>> settings.font_size
	;
	stngs_strm.close();
	return settings;
}


std::string Board_Drawer(BoardProp boad, bool count=0){
	std::string map;
	map += boad.edge_side;
	for(int i=0; i<boad.dim_x; i++)map += boad.edge_td;
	map += boad.edge_side + "\n";
	for(int y=0; y<boad.dim_y; y++){
		map += boad.edge_side;
		map += [](const char str, int times)
					{std::stringstream stream; for (int i = 0; i < times; i++) stream << str; return stream.str();}
		('.', boad.dim_x);

		std::string line_num;
		line_num += [](bool c, int n){
			std::stringstream conv;
			if(c)conv<<n+1;
			return conv.str();}(count, y);

		map += boad.edge_side + line_num + "\n";
	}

	map += boad.edge_side;
		for(int i=0; i<boad.dim_x; i++)map += boad.edge_td;
		map += boad.edge_side + '\n';

	return map;
};




int main()
{
    sf::RenderWindow window(sf::VideoMode(1080,760), "Hello World");

    BoardProp board;

    // Load settings
    IniSettings settings = IniDownloader("stngs.ini");

    board.dim_x = settings.dim_x;
    board.dim_y = settings.dim_y;
    board.edge_side = settings.edge_side;
    board.edge_td = settings.edge_td;


    // Load font from fonts/
    sf::Font font;
    font.loadFromFile("fonts/1942.ttf");
    sf::Text text;
    text.setFont(font);


    //setting text

    text.setString(Board_Drawer(board, 1));
    text.setCharacterSize(settings.font_size);

    text.setFillColor(sf::Color::Red);

    //window.display();

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
