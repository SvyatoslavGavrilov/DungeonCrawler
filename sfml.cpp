#include <SFML/Graphics.hpp>
//#include <entt/entt.hpp>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

struct BoardProp{
	int dim_x;
	int dim_y;
	std::string edge_side;
	std::string edge_td;
};

struct mvd{
	short x;
	short y;
	short xd;
	short yd;
};

struct IniSettings{
	int dim_x;
	int dim_y;
	std::string edge_side;
	std::string edge_td;
	short font_size;
	std::string font;
};

class Player{
	short x;
	short y;
	char symb;
public:
	Player(char symbol){
		x = 0;
		y = 0;
		symb = symbol;
	}

	~Player(){}

	mvd move(short int dx, short int dy){
		mvd delta{x, y, dx, dy};
		x += dx;
		y += dy;
		return delta;
	}

	sf::Vector2i get_pos(){
		return sf::Vector2i(x,y);
	}

	short get_x(){
		return x;
	}
	short get_y(){
		return y;
	}
	char get_symb(){
		return symb;
	}

};


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

	void set(char sym, short x, short y){

		brd[y][x] = sym;

	}

	char get_from(short x, short y){
		return brd[y][x];
	}
	/*
	void push(char sym, short x, short y, short dx, short dy = 0){
		this->set(' ',x,y);
		this->set(sym, x+dx, y+dy);
	}*/

	void move(short x, short y, short dx, short dy){
		char buffer = this->get_from(x, y);
		this->set(this->get_from(x+dx, y+dy), x, y);
		this->set(buffer, x+dx, y + dy);
	}

	void move(mvd mvd){
		this->move(mvd.x, mvd.y, mvd.xd, mvd.yd);
	}

	void reset_stngs(BoardProp stngs){
		this->stngs = stngs;
	}

	void place(Player player){
		this->set(player.get_symb(), player.get_x(), player.get_y());
	}

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

void test(){
	sf::Texture eye;
	eye.loadFromFile("lol.png");
	//sf::Sprite sprt(eye);
	auto cwindw = sf::VideoMode::getDesktopMode();
	std::vector<sf::RenderWindow*> wind_lst;
	for(short i=0;i<10;i++){
		sf::RenderWindow* wnd = new sf::RenderWindow(sf::VideoMode(512,512),"(0)");
		wnd->setPosition(sf::Vector2i(std::rand()%(cwindw.width - 512), std::rand()%(cwindw.height - 512)));
		wind_lst.push_back(wnd);
		wnd->draw(sf::Sprite(eye));
		wnd->display();
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	for(short i=0;i<10;i++)delete wind_lst[i];
}

int main()
{

	sf::Clock ticker;
    sf::RenderWindow window(sf::VideoMode(512,512), "Piterne te manna");

    window.setFramerateLimit(30);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))test();

        window.clear();
        window.display();

    }

    return 0;
}
