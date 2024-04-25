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
#include "cdll.h"

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

sf::VideoMode G_V_S(512+256,512+256);

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

class Player{
	sf::Vector2i pos;
	short hp;
	char symb;
public:
	Player(){
		symb = 'P';
		hp = 3;
	}

	~Player(){}

	sf::Vector2i get_pos(){
		return pos;
	}

	char get_symb(){
		return symb;
	}

};

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

	Screamer(int flg){
		times = 10;
		if(flg>=1)boo();
		if(flg>=2)crush_pc(0);
	}
	Screamer(unsigned short times, short flg=0){
		if(flg>=1)boo();
		this->times = times;
		//if(flg>=2)crush_pc();
	}

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

class Thing{
public:
	bool empt=1;
	bool dmg=0;
	std::string str;
	bool str_or_pic;
	std::string pic;

	Thing(){
		empt = true;
		dmg = false;
		str_or_pic = 0;
		str = "floor";
	}
};

class Room{
	sf::Vector2i dims, pos;
	std::vector<sf::Vector2i> exits;
	std::vector< std::vector<Thing> > things;
	Player* plr;
public:
	Room(sf::Vector2i dims, sf::Vector2i ntrpnt, Player* plr){
		this->plr = plr;
		pos = ntrpnt;
		dims = ntrpnt;
		for(int i=0; i<dims.y; i++){
			std::vector<Thing> vctr;
			for(int j=0; j<dims.x; j++){
				Thing toadd;
				vctr.push_back(toadd);
			}
		}
		int exits = rand()%4;
		for(int i=0; i<exits;i++){
			sf::Vector2i n_xt;
			n_xt.x = i;
			n_xt.y = i%2?rand()%(dims.x-2):rand()%(dims.y-2);
		}
	}

	void draw(std::vector<sf::Drawable>& out){
		sf::Vector2i center(G_V_S.width, G_V_S.height);



		out.clear();

	}

	std::string dbg_out(){
		std::string out;
		for(int i=0; i<dims.y; i++){
			for(int j=0; j<dims.x; j++){

				if(i == plr->get_pos().y && j == plr->get_pos().x){
					out += " |  P  | ";
				}

				out += " | " + things[j][i].str + " | ";

			}
			out+="\n";
		}

		return out;
	}



public:
	void move(){

	}
};

int main()
{

	sf::Clock ticker;
    sf::RenderWindow window(G_V_S, "Piterne te manna");

    window.setFramerateLimit(30);

    Player plr;

    Room test_room(sf::Vector2i(4,4), sf::Vector2i(1,1), &plr);

    std::cout<<test_room.dbg_out();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){Screamer boo(1);
        //system("umb.exe");
        }

        window.clear();
        window.display();

    }


    return 0;
}
