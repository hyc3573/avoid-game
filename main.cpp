#include <SFML/Graphics.hpp>
#include <iostream>
#include <deque>
#include <cstdlib>
#include <Windows.h>
#include <algorithm>
#include "texture.hpp"

#define SWIDTH 800
#define SHEIGHT 900

#define CWIDTH 25
#define CHEIGHT 25

#define PWIDTH 25
#define PHEIGHT 25

#define POOPCNT 5
#define PREGENSPD 1

using namespace std;
using namespace sf;

class Poop : public Sprite
{
public:
    Poop(const Texture& texture)
    {
        this->setTexture(texture);
        this->setTextureRect(IntRect(25, 0, 25, 25));

        this->setPosition(rand() % SWIDTH, 0);
    }
};

int main()
{
    unsigned int t = GetTickCount64();
    srand(t);

    RenderWindow window(VideoMode(SWIDTH, SHEIGHT), L"¶ËÇÇÇÏ±â");

    Event event;

    Clock clock;
    Clock pClock;

    int health;

    int charSpeed;
    int poopSpeed;

    int movement;

    deque<Poop> poops;
    Sprite charactor;

    Texture texture;
    if (!texture.loadFromMemory(textures_png, textures_png_len))
    {
        cout << "Error!" << endl;
    }

    float dt;

    auto reset = [&]()
    {
        clock.restart();
        pClock.restart();

        health = 3;

        charSpeed = 300;
        poopSpeed = 200;

        movement = 0;

        poops.push_back(Poop(texture));

        charactor = Sprite(texture);
        charactor.setTextureRect(IntRect(0, 0, 25, 25));
        charactor.setPosition(SWIDTH / 2, SHEIGHT - CHEIGHT);
    };

    auto newPoop = [&]()
    {
        poops.push_back(Poop(texture));
    };

    reset();

    while (window.isOpen()) 
    {
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case Event::Closed:
                window.close();
                break;

            case Event::KeyPressed:
                switch (event.key.code)
                {
                case Keyboard::Left:
                    movement = -1;
                    break;
                case Keyboard::Right:
                    movement = 1;
                    break;
                }
                break;
            case Event::KeyReleased:
                switch (event.key.code)
                {
                case Keyboard::Left:
                    if (movement == -1) movement = 0;
                    break;
                case Keyboard::Right:
                    if (movement == 1) movement = 0;
                    break;
                }
                break;
            }
        }

        dt = clock.getElapsedTime().asSeconds();
        clock.restart();

        for (auto& elem : poops)
        {
            elem.move(0, poopSpeed * dt);

            if (elem.getGlobalBounds().intersects(charactor.getGlobalBounds()))
            {
                cout << "dead" << endl;
            }
        }
        if (poops[0].getPosition().y > SHEIGHT + PHEIGHT)
        {
            poops.pop_front();
        }
        charactor.move(charSpeed * movement * dt, 0);
        charactor.setPosition(min(max(charactor.getPosition().x, 0.f), (float)(SWIDTH - CWIDTH)), SHEIGHT - CHEIGHT);

        if (pClock.getElapsedTime().asSeconds() > PREGENSPD && poops.size() < POOPCNT)
        {
            poops.push_back(Poop(texture));
            pClock.restart();
        }

        window.clear(Color::White);

        window.draw(charactor);

        for (auto elem : poops)
        {
            window.draw(elem);
        }

        window.display();
    }
    return 0;
}