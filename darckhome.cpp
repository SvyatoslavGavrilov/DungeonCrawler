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

// Custom struct definitions
struct IniSettings {
    bool debug;
    std::string font;
};

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
IniSettings IniDownloader(const std::string& filename) {
    IniSettings settings;
    std::ifstream settings_stream(filename);
    settings_stream >> settings.debug >> settings.font;
    return settings;
}

// Screamer class definition
class Screamer {
    unsigned short times;
    std::string pic{"lol.png"};
    sf::Vector2i res{512, 512};

public:
    Screamer(unsigned short times, const std::string& pic, sf::Vector2i res, bool flg = false)
        : times(times), pic(pic), res(res) {
        if (flg) boo();
    }

    Screamer(int flg) : times(10) {
        if (flg >= 1) boo();
    }

    Screamer(unsigned short times, short flg = 0) : times(times) {
        if (flg >= 1) boo();
    }

    void boo() {
        sf::Texture eye;
        eye.loadFromFile(pic);
        auto cwindw = sf::VideoMode::getDesktopMode();
        std::vector<std::unique_ptr<sf::RenderWindow>> wind_lst;

        for (unsigned int i = 0; i < times; ++i) {
            auto wnd = std::make_unique<sf::RenderWindow>(sf::VideoMode(res.x, res.y), "(0)");
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

public:
    PeekingWindow(const sf::Vector2i& size, int speed)
        : windowSize(size), speed(speed), active(false) {
        screenSize = sf::Vector2i(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);
    }

    void createWindow() {
        if (!active) {
            window = std::make_unique<sf::RenderWindow>(sf::VideoMode(windowSize.x, windowSize.y), "Peeking Window");
            window->setPosition(sf::Vector2i(screenSize.x, screenSize.y / 2 - windowSize.y / 2));
            active = true;
        }
    }

    void update() {
        if (active) {
            sf::Vector2i pos = window->getPosition();
            if (pos.x > screenSize.x - windowSize.x) {
                pos.x -= speed;
                window->setPosition(pos);
            } else {
                active = false;
                window->close();
                window.reset();
            }
        }
    }

    bool isActive() const {
        return active;
    }

    void render() {
        if (active) {
            window->clear(sf::Color::Red);  // Example content
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
    sf::RenderWindow* main_window;

public:
    sf::Font font;

    Player(const std::string& fontname, sf::RenderWindow* window)
        : pos{0, 0}, hp{3}, symb{'P'}, look{0}, main_window(window) {
        font.loadFromFile(fontname);
        facing_txt.setFont(font);

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
        text_centerer(&facing_txt, sf::VideoMode(main_window->getSize().x / 4, main_window->getSize().y));
        facing_txt.setPosition(0, main_window->getSize().y / 2 - facing_txt.getGlobalBounds().height / 2);
        main_window->draw(facing_txt);
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
    sf::RenderWindow* main_window;

public:
    Room(const sf::Vector2i& dims, const sf::Vector2i& ntrpnt, Player* plr, sf::RenderWindow* window)
        : dims(dims), plr(plr), font(plr->font), keyer(false), main_window(window) {
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
        text_centerer(&litera, sf::VideoMode(main_window->getSize().x / 4, main_window->getSize().y));
        litera.setString(get_thing(plr->get_pos()).str);
        litera.setPosition(3 * main_window->getSize().x / 4, main_window->getSize().y / 2 - litera.getGlobalBounds().height / 2);
        main_window->draw(litera);
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
          player(settings.font, &window),
          room(sf::Vector2i(8, 8), sf::Vector2i(1, 1), &player, &window),
          peekingWindow(sf::Vector2i(200, 400), 10) {
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

private:
    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }
            room.handleEvent(event);
        }
    }

    void update() {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) {
            Screamer boo(1);
        }

        peekingWindow.update();
        room.update();
        player.render();
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

        room.render();
        player.render();

        window.display();

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
