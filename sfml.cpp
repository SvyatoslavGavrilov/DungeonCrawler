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
	std::string font;
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
	>> settings.font
	;
	stngs_strm.close();
	return settings;
}

class Board{

	BoardProp stngs;
	std::vector<std::string> brd;

	public:

	Board (BoardProp boad){
		stngs = boad;
		for(int i = 0; i<stngs.dim_y; i++){
			std::string str="";
			for(int j = 0; j<stngs.dim_x; j++)str+=" ";
			brd.push_back(str);
		}
	}

	std::string Board_Drawer(bool count=0){
		std::string map;
		map += stngs.edge_side;
		for(int i=0; i<stngs.dim_x; i++)map += stngs.edge_td;
		map += stngs.edge_side + "\n";
		for(int y=0; y<stngs.dim_y; y++){
			map += stngs.edge_side;

			/*map += [](const char str, int times)
						{std::stringstream stream; for (int i = 0; i < times; i++) stream << str; return stream.str();}
			('.', stngs.dim_x);
			*/

			map += brd[y];

			std::string line_num;
			line_num += [](bool c, int n){
				std::stringstream conv;
				if(c)conv<<n+1;
				return conv.str();}(count, y);

			map += stngs.edge_side + line_num + "\n";
		}

		map += stngs.edge_side;
			for(int i=0; i<stngs.dim_x; i++)map += stngs.edge_td;
			map += stngs.edge_side + '\n';

		return map;
	};

	void set(char sym, int x, int y){

		brd[y][x] = sym;

	}

	void reset_stngs(BoardProp stngs){
		this->stngs = stngs;
	}

};




int main()
{
    sf::RenderWindow window(sf::VideoMode(1080,760), "Hello World");

    BoardProp brd_stngs;

    // Load settings
    IniSettings settings = IniDownloader("stngs.ini");

    brd_stngs.dim_x = settings.dim_x;
    brd_stngs.dim_y = settings.dim_y;
    brd_stngs.edge_side = settings.edge_side;
    brd_stngs.edge_td = settings.edge_td;


    // Load font from fonts/
    sf::Font font;
    font.loadFromFile(settings.font);
    sf::Text text;
    text.setFont(font);

    Board brd(brd_stngs); //create Board class (class containing what is displayed);

    brd.set('A', 1, 1);
    brd.set('B', 2, 1);
    brd.set('C', 4, 4);

    //get things drawn
    text.setString(brd.Board_Drawer());
    text.setCharacterSize(settings.font_size);

    text.setFillColor(sf::Color::Red);




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
