#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include <iostream>
#include <fstream>
#include <windows.h>

using namespace sf;
using namespace std;
class Engine {
private:
    float x, y;
    int first, second;
    int sum = 0;
    string sign;
    int count = 0;
    int el_time;
    int delta_time;
    int all_time = 60000;
    char  bs;
    string best_score = "";
    ofstream out;
    ifstream in;
    RectangleShape platform1;
    RectangleShape platform2;
    Texture texture, background;
    Sprite player, sprite;
    Text problem, timer, name, score, bestScore, tip;
    Font font;
    SoundBuffer buffer1, buffer2;
    Sound jump, w_a;
public:
    
    RenderWindow window;
    string s = "";
    Clock clock;
    bool isPlaying = false;
    
    // движение
    void Move() {
        problem.setPosition(-100, -100);
        count++;
        jump.play();
        if (count % 2 != 0) {
            for (x = 87; x <= 287; x += 5) {
                y = -x * x / 100 + 187 * x / 50 - 24969 / 100;
                player.setPosition(x, 245 - y);
                if (platform1.getPosition().y != 280) {
                    platform1.move(0, 10);
                }
                if (platform2.getPosition().y != 50) {
                    platform2.move(0, 10);
                }
                if (platform2.getPosition().y == 400) {
                    platform2.setPosition(70, -60);
                }
                Display();
            }
        }
        else {
            for (x = 287; x >= 87; x -= 5) {
                y = -x * x / 100 + 187 * x / 50 - 24969 / 100;
                player.setPosition(x, 245 - y);
                if (platform2.getPosition().y != 280) {
                    platform2.move(0, 10);
                }
                if (platform1.getPosition().y != 50) {
                    platform1.move(0, 10);
                }
                if (platform1.getPosition().y == 400) {
                    platform1.setPosition(270, -60);
                }
                Display();
            }

        }
        problemCreator();
    }
    // вывод на экран
    void Display() {
        Time();
        window.draw(sprite);
        window.draw(player);
        window.draw(platform1);
        window.draw(platform2);
        window.draw(problem);
        window.draw(timer);
        window.display();
    }
    void LobbyDisp() {
        window.clear(Color(123, 247, 74));
        window.draw(name);
        window.draw(score);
        window.draw(bestScore);
        window.draw(tip);
        window.display();
    }
    // установка параметров объектов
    void Setup() {
        window.create(VideoMode(400, 400), L"MathJumping", Style::Default);
        window.setVerticalSyncEnabled(true);
        window.setFramerateLimit(60);
        platform1.setFillColor(Color(36, 25, 25));
        platform1.setPosition(270, 50);
        platform1.setSize(Vector2f(100, 20));
        platform2.setFillColor(Color(36, 25, 25));
        platform2.setPosition(70, 280);
        platform2.setSize(Vector2f(100, 20));
        texture.loadFromFile("pers_png.png");
        player.setTexture(texture);
        player.setScale(2, 2);
        player.setPosition(87, 244);
        background.loadFromFile("phon_upgr.png");
        sprite.setTexture(background);
        font.loadFromFile("fenton-black1.ttf");
        problem.setFont(font);
        problem.setFillColor(Color(150, 168, 255));
        problem.setCharacterSize(18);
        timer.setFont(font);
        timer.setFillColor(Color::Black);
        timer.setCharacterSize(24);
        timer.setPosition(175, 30);
        buffer1.loadFromFile("jump.wav");
        buffer2.loadFromFile("wrong.wav");
        jump.setBuffer(buffer1);
        w_a.setBuffer(buffer2);
        w_a.setVolume(7);
        jump.setVolume(10);
    }
    void LobbySetup() {
        name.setFont(font);
        name.setCharacterSize(30);
        name.setFillColor(Color::Black);
        name.setString("Math Jumping");
        name.setPosition(100, 50);
        score.setPosition(190, 100);
        score.setCharacterSize(26);
        score.setFont(font);
        score.setFillColor(Color(255, 255, 255));
        bestScore.setCharacterSize(20);
        bestScore.setFillColor(Color::Black);
        bestScore.setFont(font);
        bestScore.setPosition(130, 150);
        in.open("bs.txt");
        in >> best_score;
        in.close();
        bestScore.setString("Best score: " + best_score);
        tip.setCharacterSize(10);
        tip.setPosition(280, 380);
        tip.setFont(font);
        tip.setFillColor(Color::Black); 
        tip.setString("Press SPACE to start");
    }
    // создание примеров
    void problemCreator() {
        count % 2 == 0 ? problem.setPosition(281, 50) : problem.setPosition(81, 50);
        srand(time(0));
        first = rand() % 150;
        second = rand() % 150;
        if (rand() % 2 == 0) {
            sign = " + ";
            sum = first + second;
        }
        else {
            sign = " - ";
            sum = first - second;
        }
        problem.setString(to_string(first) + sign + to_string(second));

    }
    // проверка ответа
    void Checker() {
        if ((s.size() >= 1 && s != "-") && sum == stoi(s)) {
            all_time += 2500;
            Move();
        }
        else {
            w_a.play();
            all_time -= 3000;
        }
        s = "";     
    }
    // подсчет оставшегося времени
    void Time() {
        el_time = clock.getElapsedTime().asMilliseconds();
        delta_time = all_time - el_time;
        if (delta_time <= 0) {
            isPlaying = false;
            all_time = 60000;
            score.setString(to_string(count));
            if (count > stoi(best_score)) {
                best_score = to_string(count);
                out.open("bs.txt");
                out << best_score;
                out.close();
                bestScore.setString("Best score: " + best_score);
            }
            count = 0;
        }
        delta_time < 5000 ? timer.setFillColor(Color(212, 0, 0)) : timer.setFillColor(Color::Black);
        timer.setString(to_string(delta_time / 1000) + ':' + to_string(delta_time / 10 % 100));
    }
};
int main()
{
    Engine megapers;
    megapers.Setup();
    megapers.LobbySetup();
 
    megapers.problemCreator();
    while (megapers.window.isOpen())
    {
        Event event;
        while (megapers.window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                megapers.window.close();
            if (event.type == Event::KeyPressed
                && event.key.code == Keyboard::Enter) {
                if (megapers.isPlaying) megapers.Checker();
            }
            if (event.type == Event::KeyPressed) {
                if (event.key.code == 59 && megapers.s.size() != 0) { 
                    megapers.s.erase(megapers.s.size() - 1);
                }
                if (event.key.code == 56) megapers.s += '-';
                if (event.key.code >= 26 && event.key.code <= 35) {
                    megapers.s += to_string(event.key.code - 26);
                }
            }

        }
        if (Keyboard::isKeyPressed(Keyboard::Space)
            && !megapers.isPlaying) {
            megapers.isPlaying = true;
            megapers.clock.restart();
        }
        if (Keyboard::isKeyPressed(Keyboard::Escape)) megapers.window.close();
        if (megapers.isPlaying) megapers.Display();
        else megapers.LobbyDisp();
    }

    return 0;
}
