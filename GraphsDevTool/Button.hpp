#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SFML/Graphics.hpp>
#include "UIComponent.hpp"
#include "CommandHolder.hpp"
#include "RoundedRect.hpp"


class Button : public UIComponent
{
public:
    Button();
    Button(const sf::Vector2f& position, const sf::Vector2f& size);

public:
    virtual void	            update(sf::RenderWindow& window, const sf::Vector2f& mousePos, const UserInput& userInput) override;
    void 			            draw(sf::RenderWindow& window) const override;
    void			            reset() override;

    void                        updateStateLook(State::ID state);

public:
    void		        	    setPosition(const sf::Vector2f& position) override;
    void		        	    setSize(const sf::Vector2f& size) override;

    void                        setCommand(CommandHolder* commandHolder, Command::ID mCommand);

    void                        setColors(const sf::Color& idle, const sf::Color& hovered, const sf::Color& pressed);
    void                        setTextures(const sf::Texture& idle, const sf::Texture& hovered, const sf::Texture& pressed);

    void                        setText(const sf::Font& font, int size, const sf::Color& color, const sf::Text::Style style, const std::string& str);
    void                        centerText();

    bool                        wasClicked();

    void                        setBox(const RoundedRect& box);

protected:
    RoundedRect                 box;

    sf::Text                    text;
    sf::Sprite                  sprite;

    StateLook<sf::Color>        stateColor;
    StateLook<sf::Texture>      stateTexture;

    bool                        hasTextures;
    bool                        hasText;

    CommandHolder*              commandHolder;
    Command::ID                 mCommand;

    bool                        clicked;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif 