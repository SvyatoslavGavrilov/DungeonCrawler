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


class Screamer{
	unsigned short times;
	std::string pic{"lol.png"};
	sf::Vector2i res{512,512};

public:
	Screamer(unsigned short times, std::string pic, sf::Vector2i res, bool flg=0){
		this->times = times;
		this->pic = pic;
		this->res = res;
		if(flg)boo();
	}

	Screamer(bool flg=0){if(flg)boo();
	times = 10;}
	Screamer(unsigned short times, bool flg=0){if(flg)boo();this->times = times;}

	void boo(){
		sf::Texture eye;
		eye.loadFromFile(pic);
		auto cwindw = sf::VideoMode::getDesktopMode();
		std::vector<sf::RenderWindow*> wind_lst;
		for(unsigned int i=0;i<times;i++){
			sf::RenderWindow* wnd = new sf::RenderWindow(sf::VideoMode(res.x, res.y),"(0)");
			wnd->setPosition(sf::Vector2i(std::rand()%(cwindw.width - res.x), std::rand()%(cwindw.height - res.y)));
			wind_lst.push_back(wnd);
			//wnd->setView(sf::View());
			wnd->draw(sf::Sprite(eye));
			wnd->display();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(512));
		for(unsigned int i=0;i<times;i++)delete wind_lst[i];
	}
};

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

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){Screamer boo(16, 1);
        system("umb.exe");
        }

        window.clear();
        window.display();

    }

    return 0;
}
