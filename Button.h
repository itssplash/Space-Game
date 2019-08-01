#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

class Button
{
public :
	Button(sf::Vector2f btnsize, sf::String textonbutton, sf::Color btncolor,sf::Color textcolor,sf::String fontfile)
	{
		btn.setSize(btnsize);
		btnwidth = btnsize.x;
		btnheight = btnsize.y;

		btn.setFillColor(btncolor);
		if (!f.loadFromFile(fontfile))
		{
			std::cout << "error" << std::endl;
		}
		t.setString(textonbutton);
		t.setFont(f);
		t.setFillColor(textcolor);
	}
	void draw(sf::RenderWindow& window)
	{
		window.draw(btn);
		window.draw(t);
	}
	void setPos(sf::Vector2f pos)
	{
		btn.setPosition(pos);

		float x = (btn.getPosition().x + btnwidth / 2) - t.getLocalBounds().width / 2;
		float y = (btn.getPosition().y + btnheight / 2) - t.getLocalBounds().height / 2;
		t.setPosition(x,y);
	}

	bool ismoseover(sf::RenderWindow& window)
	{
		int mousex = sf::Mouse::getPosition(window).x;
		int mousey = sf::Mouse::getPosition(window).y;

		int btnposx = btn.getPosition().x + btnwidth;
		int btnposy = btn.getPosition().y + btnheight;

		if (mousex > btn.getPosition().x && mousex < btnposx && mousey > btn.getPosition().y && mousey < btnposy)
		{
			return true;
		}
		return false;
	}
private :
	sf::RectangleShape btn;
	sf::Text t;
	sf::Font f;

	int btnwidth;
	int btnheight;
};