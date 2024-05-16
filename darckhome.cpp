#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <cstdlib>
#include "WindowTransparency.h"

// Global VideoMode variable
sf::VideoMode GVS(512 + 256, 512 + 256);

// Function to center text within a given VideoMode
void text_centerer(sf::Text* txt, sf::VideoMode align) {
    sf::Vector2f crd;
    crd.x = align.width / 2 - txt->getLocalBounds().width / 2;
    crd.y = align.height / 2 - txt->getLocalBounds().height / 2;
    txt->setPosition(crd);
}

// Function to download and parse INI settings from a file

struct IniSettings {
    bool debug;
    std::string font;
    int width;
    int height;
    int framerate_limit;
};

IniSettings IniDownloader(const std::string& filename) {
    IniSettings settings;
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open settings file");
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string comment, value;

        if (std::getline(ss, comment, ',') && std::getline(ss, value)) {
            if (comment == "debug") {
                settings.debug = (value == "true");
            } else if (comment == "font") {
                settings.font = value;
            } else if (comment == "width") {
                settings.width = std::stoi(value);
            } else if (comment == "height") {
                settings.height = std::stoi(value);
            } else if (comment == "framerate_limit") {
                settings.framerate_limit = std::stoi(value);
            }
        }
    }

    return settings;
}

// Screamer class definition
class Screamer {
    unsigned short times;
    std::string pic{"pics/conuct.png"};
    sf::Vector2i res;

public:
    Screamer(unsigned short times, const std::string& pic, bool flg = false)
        : times(times), pic(pic){
    	sf::Texture eye;
    	eye.loadFromFile(pic);
    	res = sf::Vector2i(eye.getSize());
        if (flg) boo();
    }

    Screamer(int flg) : times(10) {
    	sf::Texture eye;
		eye.loadFromFile(pic);
		res = sf::Vector2i(eye.getSize());
        if (flg >= 1) boo();
    }

    Screamer(unsigned short times, short flg = 0) : times(times) {
    	sf::Texture eye;
		eye.loadFromFile(pic);
		res = sf::Vector2i(eye.getSize());
		if (flg) boo();
        if (flg >= 1) boo();
    }

    void boo() {
        sf::Texture eye;
        eye.loadFromFile(pic);
        auto cwindw = sf::VideoMode::getDesktopMode();
        std::vector<std::unique_ptr<sf::RenderWindow>> wind_lst;

        for (unsigned int i = 0; i < times; ++i) {
            auto wnd = std::make_unique<sf::RenderWindow>(sf::VideoMode(eye.getSize().x, eye.getSize().y), "(0)", sf::Style::None);
			#ifdef _WIN32
            makeWindowBackgroundTransparent(*wnd);  // Make the window background transparent
            #endif
            wnd->setPosition(sf::Vector2i(std::rand() % (cwindw.width - res.x), std::rand() % (cwindw.height - res.y)));
            wnd->draw(sf::Sprite(eye));
            wnd->display();
            wind_lst.push_back(std::move(wnd));
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(512));
    }
};

// Peeking Window class

class PeekingWindow {
    std::unique_ptr<sf::RenderWindow> window;
    sf::Vector2i screenSize;
    sf::Vector2i windowSize;
    int speed;
    bool active;
    bool extending;
    sf::Texture texture;
    sf::Sprite sprite;

public:
    PeekingWindow(const sf::Vector2i& size, int speed, const std::string& imageFile)
        : windowSize(size), speed(speed), active(false), extending(true) {
        screenSize = sf::Vector2i(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);
        if (!texture.loadFromFile(imageFile)) {
            throw std::runtime_error("Failed to load image file");
        }
        sprite.setTexture(texture);
        windowSize = sf::Vector2i(texture.getSize());
    }

    void createWindow() {
        if (!active) {
            window = std::make_unique<sf::RenderWindow>(sf::VideoMode(windowSize.x, windowSize.y), "Peeking Window", sf::Style::None);
            window->setPosition(sf::Vector2i(screenSize.x, screenSize.y / 2 - windowSize.y / 2));
            active = true;
            extending = true;

            #ifdef _WIN32
            makeWindowBackgroundTransparentRed(*window);  // Make the window background transparent
            #endif
        }
    }

    void update() {
        if (active) {
            sf::Vector2i pos = window->getPosition();
            if (extending) {
                if (pos.x > screenSize.x - windowSize.x) {
                    pos.x -= speed;
                    window->setPosition(pos);
                } else {
                    extending = false;
                }
            } else {
                if (pos.x < screenSize.x) {
                    pos.x += speed;
                    window->setPosition(pos);
                } else {
                    active = false;
                    window->close();
                    window.reset();
                }
            }
        }
    }

    bool isActive() const {
        return active;
    }

    void render() {
        if (active) {
            window->clear(sf::Color::Black);  // Clear with the transparent color key
            window->draw(sprite);
            window->display();
        }
    }
};

// Player class definition
const std::string ASCII_ARROWS[] = {"->", "|\nV", "<-", "^\n|"};

class Player {
    sf::Vector2i pos;
    short hp;
    char symb;
    short look;
    std::vector<sf::Vector2i> look_lst;
    sf::Text facing_txt;
    sf::RenderWindow facing_window;

public:
    sf::Font font;

    Player(const std::string& fontname, const sf::Vector2i& facing_window_position)
        : pos{0, 0}, hp{3}, symb{'P'}, look{0}, facing_window(sf::VideoMode(256, 128), "I am facing", sf::Style::None) {
        font.loadFromFile(fontname);
        facing_txt.setFont(font);
        facing_window.setPosition(facing_window_position);

        look_lst = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
        facing_txt.setString(ASCII_ARROWS[look]);
    }

    sf::Vector2i get_pos() const {
        return pos;
    }

    void set_pos(const sf::Vector2i& target) {
        pos = target;
    }

    char get_symb() const {
        return symb;
    }

    void rotate(short dir) {
        look = ((look + dir) % 4 + 4) % 4; // Ensure positive result
        facing_txt.setString(ASCII_ARROWS[look]);
    }

    sf::Vector2i get_look() const {
        return look_lst[look];
    }

    bool alive() const {
        return hp > 0;
    }

    void die() {
        Screamer boo(3);
    }

    void render() {
        facing_window.clear();
        text_centerer(&facing_txt, sf::VideoMode(256, 128));
        facing_window.draw(facing_txt);
        facing_window.display();
    }

    void close_window() {
        facing_window.close();
    }

    bool is_window_open() const {
        return facing_window.isOpen();
    }
};

// Thing class definition
class Thing {
public:
    bool empt;
    bool dmg;
    std::string str;
    bool str_or_pic;
    std::string pic;
    int type;

    Thing() : empt(true), dmg(false), str("floor"), str_or_pic(0), type(0) {}

    Thing(bool e, bool d, const std::string& name, int t)
        : empt(e), dmg(d), str(name), str_or_pic(0), type(t) {}

    int get_type() const {
        return type;
    }
};

// Room class definition
class Room {
    sf::Vector2i dims;
    std::vector<std::vector<Thing>> things;
    Player* plr;
    sf::Text litera;
    sf::Font font;
    bool keyer;
    sf::RenderWindow standing_window;

public:
    Room(const sf::Vector2i& dims, const sf::Vector2i& ntrpnt, Player* plr, const sf::Vector2i& standing_window_position)
        : dims(dims), plr(plr), font(plr->font), keyer(false), standing_window(sf::VideoMode(256, 128), "Standing Over", sf::Style::None) {
        standing_window.setPosition(standing_window_position);
        initializeRoom(dims, ntrpnt);
    }

    void initializeRoom(const sf::Vector2i& new_dims, const sf::Vector2i& ntrpnt) {
        dims = new_dims;
        plr->set_pos(ntrpnt);

        things.clear();
        things.resize(dims.y, std::vector<Thing>(dims.x));

        int exit_count = 1 + rand() % 3;
        for (int i = 0; i < exit_count; ++i) {
            Thing ext(true, false, "Exit", 5);
            things[rand() % dims.y][rand() % dims.x] = ext;
        }

        litera.setFont(font);
    }

    void handleEvent(const sf::Event& event) {
        if (event.type == sf::Event::KeyReleased) {
            keyer = true;
        }
    }

    void update() {
        if (keyer) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                plr->rotate(1);
                keyer = false;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                plr->rotate(-1);
                keyer = false;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                move();
                keyer = false;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                if (act() == 5) {
                    Screamer boo(1);
                    sf::Vector2i place(2 + rand() % 8, 2 + rand() % 8);
                    initializeRoom(place, sf::Vector2i(rand() % place.x, rand() % place.y));
                }
            }
        }
    }

    void move() {
        sf::Vector2i destination = plr->get_pos() + plr->get_look();
        if (destination.x >= 0 && destination.x < dims.x && destination.y >= 0 && destination.y < dims.y) {
            plr->set_pos(destination);
        }
        render();
    }

    int act() const {
        return things[plr->get_pos().y][plr->get_pos().x].get_type();
    }

    std::string dbg_out() const {
        std::string out;
        for (int i = 0; i < dims.y; ++i) {
            for (int j = 0; j < dims.x; ++j) {
                out += (i == plr->get_pos().y && j == plr->get_pos().x) ? 'P' : static_cast<char>(things[i][j].str[0]);
            }
            out += "\n";
        }
        return out;
    }

    void render() {
        standing_window.clear();
        litera.setString(get_thing(plr->get_pos()).str);
        text_centerer(&litera, sf::VideoMode(256, 128));
        standing_window.draw(litera);
        standing_window.display();
    }

    void close_window() {
        standing_window.close();
    }

    bool is_window_open() const {
        return standing_window.isOpen();
    }

    Thing get_thing(const sf::Vector2i& vec) const {
        return things[vec.y][vec.x];
    }
};

// MainWindow class definition
class MainWindow {
    sf::RenderWindow window;
    IniSettings settings;
    sf::Font monofont;
    Player player;
    Room room;
    PeekingWindow peekingWindow;

public:
    MainWindow()
        : window(GVS, "Piterne te manna"),
          settings(IniDownloader("stngs.ini")),
          player(settings.font, sf::Vector2i(window.getPosition().x - 256 + 8, window.getPosition().y)),
          room(sf::Vector2i(8, 8), sf::Vector2i(1, 1), &player, sf::Vector2i(window.getPosition().x + window.getSize().x + 8, window.getPosition().y)),
          peekingWindow(sf::Vector2i(200, 400), 10, "pics/eegg.png") {
        window.setFramerateLimit(30);
        monofont.loadFromFile(settings.font);
    }

    void run() {
        while (window.isOpen()) {
            handleEvents();
            update();
            render();
        }
    }

    void loadSettings() {
            settings = IniDownloader("stngs.ini");
            window.create(sf::VideoMode(settings.width, settings.height), "Piterne te manna");
            window.setFramerateLimit(settings.framerate_limit);
            monofont.loadFromFile(settings.font);
        }

private:
    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                player.close_window();
                room.close_window();
                return;
            }
            room.handleEvent(event);
        }
    }

    void update() {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) {
            Screamer boo(1);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
        	peekingWindow.createWindow();
		}

        peekingWindow.update();
        room.update();
    }

    void render() {
        window.clear();

        if (settings.debug) {
            sf::Text debugmap;
            debugmap.setFont(monofont);
            debugmap.setString(room.dbg_out());
            text_centerer(&debugmap, GVS);
            window.draw(debugmap);
        }

        window.display();

        if (player.is_window_open()) {
            player.render();
        }

        if (room.is_window_open()) {
            room.render();
        }

        if (peekingWindow.isActive()) {
            peekingWindow.render();
        }
    }
};

// Main function
int main() {
    MainWindow mainWindow;
    mainWindow.run();
    return 0;
}
