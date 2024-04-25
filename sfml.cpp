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

void text_centerer(sf::Text* txt, sf::VideoMode alighn){
	sf::Vector2f crd;
	crd.x = alighn.width/2 - txt->getLocalBounds().width/2;
	crd.y = alighn.height/2 - txt->getLocalBounds().height/2;
	txt->setPosition(crd);
}

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

class Player{
	sf::Vector2i pos;
	short hp;
	char symb;
	short look;
	std::vector<sf::Vector2i> look_lst;
	sf::RenderWindow* facing_w;
	sf::Text facing_txt;
	sf::Font font;
	std::vector<std::string> directions;
public:
	Player(std::string fontname){
		font.loadFromFile(fontname);
		facing_txt.setFont(font);
		facing_w = new sf::RenderWindow(sf::VideoMode(256, 256), "I am facing");

		symb = 'P';
		hp = 3;

		look = 0;

		sf::Vector2i toadd(1,0);
		look_lst.push_back(toadd);
		toadd.y = 1;
		toadd.x = 0;
		look_lst.push_back(toadd);
		toadd.x = -1;
		toadd.y = 0;
		look_lst.push_back(toadd);
		toadd.y = -1;
		toadd.x=0;
		look_lst.push_back(toadd);

		directions.push_back("East");
		directions.push_back("South");
		directions.push_back("West");
		directions.push_back("North");

		facing_txt.setString(directions[look]);

	}

	~Player(){
		delete facing_w;
	}

	sf::Vector2i get_pos(){
		return pos;
	}

	void set_pos(sf::Vector2i target){
		pos = target;
	}

	char get_symb(){
		return symb;
	}

	void rotate(short dir){
		look = ((look+dir)%4)>=0?((look+dir)%4):3;
		facing_txt.setString(directions[look]);
	}

	sf::Vector2i get_look(){
		return look_lst[look];
	}

	bool alive(){
		return hp;
	}

	void die(){
		Screamer boo(3);
	}

	void render(){
		text_centerer(&facing_txt, sf::VideoMode(256, 256));
		facing_w->clear();
		facing_w->draw(facing_txt);
		facing_w->display();
	}

};

class Thing{
public:
	bool empt;
	bool dmg;
	std::string str;
	bool str_or_pic;
	std::string pic;
	int type;

	Thing(){
		empt = true;
		dmg = false;
		str_or_pic = 0;
		str = "floor";
		type = 0;
	}
	Thing(bool e, bool d, std::string name, int t){
			empt = true;
			dmg = false;
			str_or_pic = 0;
			str = name;
			type = t;
		}
	int get_type(){
		return type;
	}
};

class Room{
	sf::Vector2i dims;
	std::vector<sf::Vector2i> exits;
	std::vector< std::vector<Thing> > things;
	Player* plr;
public:
	Room(sf::Vector2i dims, sf::Vector2i ntrpnt, Player* plr){
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
		int exits = 1 + rand()%3;
		for(int i=0; i<exits;i++){
			//this->exits.push_back(n_xt);
			Thing ext(1, 0, "Exit", 5);
			things[rand()%dims.y][rand()%dims.x] = ext;
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
				out +=+(i == plr->get_pos().y && j == plr->get_pos().x)?'P':static_cast<char>(things[i][j].str[0]);
			}
			out+="\n";
		}

		return out;
	}

	Thing get_thing(sf::Vector2i vec){
		return things[vec.x][vec.y];
	}


public:
	void move(){
		sf::Vector2i desternation = plr->get_pos() + plr->get_look();
		//if(get_thing(desternation).empt)
		//std::cout<<"Moving"<<"\n"<<plr->get_look().x<<' '<<plr->get_look().y<<"\n";
		if(desternation.x>=0 && desternation.x<dims.x && desternation.y>=0 && desternation.y<dims.y)
			plr->set_pos(desternation);
	}

	int act(){
		return things[plr->get_pos().y][plr->get_pos().x].get_type();
	}
};

int main()
{

	sf::Clock ticker;
    sf::RenderWindow window(GVS, "Piterne te manna");
    window.setFramerateLimit(30);

    IniSettings STNGS = IniDownloader("stngs.ini");
    sf::Font monofont;
    monofont.loadFromFile(STNGS.font);


    Player plr(STNGS.font);

    Room main_room(sf::Vector2i(8,8), sf::Vector2i(1,1), &plr);

    bool keyer=0;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if(event.type == sf::Event::KeyReleased && keyer==0){keyer = 1;}

            //movement
            if(keyer){
    			if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
    				plr.rotate(1);
    				keyer = 0;
    			}
    			else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
    				plr.rotate(-1);
    				keyer = 0;
    			}
    			else if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
    				main_room.move();
    				keyer = 0;
    			}
    			else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)){
    				if(main_room.act() == 5){
    					Screamer boo(1);
    					sf::Vector2i place(2+rand()%8, 2+rand()%8);
    					Room n_r(place, sf::Vector2i(rand()%place.x, rand()%place.y), &plr);
    					main_room = n_r;
    				}
    			}

            }

        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::K)){
        	Screamer boo(1);
        //system("umb.exe");
        }

        plr.render();

        window.clear();

        if(STNGS.debug){
        	sf::Text debugmap;
        	debugmap.setFont(monofont);
        	debugmap.setString(main_room.dbg_out());
        	//debugmap.setCharacterSize(16);
        	text_centerer(&debugmap, GVS);
        	window.draw(debugmap);
        }
        plr.render();
        window.display();

    }


    return 0;
}
