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

namespace otdt{

struct BoardProp{
	int dim_x;
	int dim_y;
	std::string edge_side;
	std::string edge_td;
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

}