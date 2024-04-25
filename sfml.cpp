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
	bool debug;
	std::string font;
};

sf::VideoMode GVS(512+256,512+256);

IniSettings IniDownloader(std::string filename){
	IniSettings settings;
	std::ifstream stngs_strm(filename);
	stngs_strm
	>> settings.debug
	>> settings.font
	;
	stngs_strm.close();
	return settings;
}

class Player{
	sf::Vector2i pos;
	short hp;
	char symb;
	short* look;
	std::vector<sf::RenderWindow> pers_winds;
public:
	Player(){
		symb = 'P';
		hp = 3;
		look = new short[4]{1,0,0,0};
	}

	~Player(){delete look;}

	sf::Vector2i get_pos(){
		return pos;
	}

	void set_pos(sf::Vector2i target){
		pos = target;
	}

	char get_symb(){
		return symb;
	}

	void rotate(bool right){
		if(right)for(short i=0; i<4; i++){
			if(look[i]){look[i]=0; if(i+1!=4)look[i+1]=1; else look[0]=1;}}
		else for(short i=3; i !=-1; i--){
			if(look[i]){look[i]=0; if(i-1!=-1)look[i-1]=1; else look[3]=1;}}
	}

	sf::Vector2i get_look(){
		return sf::Vector2i((look[0] - look[2]), (look[1] - look[3]));
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
	sf::Vector2i dims;
	std::vector<sf::Vector2i> exits;
	std::vector< std::vector<Thing> > things;
	Player* plr;
public:
	Room(sf::Vector2i dims, sf::Vector2i ntrpnt, Player* plr){
		//std::cout<<dims
		this->plr = plr;
		plr->set_pos(ntrpnt);
		this->dims = dims;
		for(int i=0; i<dims.y; i++){
			std::vector<Thing> vctr;
			for(int j=0; j<dims.x; j++){
				Thing toadd;
				vctr.push_back(toadd);
			}
			things.push_back(vctr);
		}
		int exits = rand()%4;
		for(int i=0; i<exits;i++){
			sf::Vector2i n_xt;
			n_xt.x = i;
			n_xt.y = i%2?rand()%(dims.x-2):rand()%(dims.y-2);
			this->exits.push_back(n_xt);
		}
	}

	void draw(std::vector<sf::Drawable>& out){
		sf::Vector2i center(GVS.width, GVS.height);



		out.clear();

	}

	std::string dbg_out(){
		std::string out;
		for(int i=0; i<dims.y; i++){
			for(int j=0; j<dims.x; j++){
				out +=+(i == plr->get_pos().y && j == plr->get_pos().x)?'P':static_cast<char>(things[j][i].str[0]);
			}
			out+="\n";
		}

		return out;
	}



public:



	void move(){
		sf::Vector2i desternation = plr->get_pos() + plr->get_look();
		if(desternation.x>0 && desternation.x<dims.x && desternation.y>0 && desternation.y<dims.y)
			plr->set_pos(desternation);
	}
};

void text_centerer(sf::Text* txt){
	sf::Vector2f crd;
	crd.x = GVS.width/2 - txt->getLocalBounds().width/2;
	crd.y = GVS.height/2 - txt->getLocalBounds().height/2;
	txt->setPosition(crd);
}

int main()
{

	sf::Clock ticker;
    sf::RenderWindow window(GVS, "Piterne te manna");
    window.setFramerateLimit(30);

    IniSettings STNGS = IniDownloader("stngs.ini");
    sf::Font monofont;
    monofont.loadFromFile(STNGS.font);


    Player plr;

    Room test_room(sf::Vector2i(4,4), sf::Vector2i(1,1), &plr);

    std::cout<<'!'<<'\n';

    std::cout<<test_room.dbg_out();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::K)){Screamer boo(1);
        //system("umb.exe");
        }


        //movement
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
			plr.rotate(0);
		}
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
        	plr.rotate(1);
		}
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
        	test_room.move();
        }



        window.clear();

        if(STNGS.debug){
        	sf::Text debugmap;
        	debugmap.setFont(monofont);
        	debugmap.setString(test_room.dbg_out());
        	//debugmap.setCharacterSize(16);
        	text_centerer(&debugmap);
        	window.draw(debugmap);
        }

        window.display();

    }


    return 0;
}
