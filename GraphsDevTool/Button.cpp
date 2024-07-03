#include "Button.hpp"

//////////////////////////////////////////////////////////////////////////////////////////////////////

Button::Button() : UIComponent(sf::Vector2f(0, 0), sf::Vector2f(64, 32)), hasText(false), hasTextures(false), clicked(false),
    stateColor(sf::Color::Magenta, sf::Color::Blue, sf::Color::Red), commandHolder(nullptr), mCommand(Command::Nothing)
{
    box.setPosition(UIComponent::position);
    box.setSize(UIComponent::size);
    box.setColor(stateColor.idle);
}

Button::Button(const sf::Vector2f& position, const sf::Vector2f& size) : 
    UIComponent(position, size), 
    hasText(false), hasTextures(false), clicked(false),
    commandHolder(nullptr), mCommand(Command::Nothing),
    stateColor(sf::Color::Magenta, sf::Color::Blue, sf::Color::Red)
{
    box.setPosition(UIComponent::position);
    box.setSize(UIComponent::size);
    box.setColor(stateColor.idle);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void Button::update(sf::RenderWindow& window, const sf::Vector2f& mousePos, const UserInput& userInput)
{ 
    State::ID newState;
    if (contains(mousePos))
    {
        active = true;
        if (userInput.event == Event::MouseLeftPressed)
        {
            newState = State::Pressed;
        }
        else if (lastState == State::Pressed)
        {
            if (userInput.event == Event::MouseLeftReleased)
            {
                commandHolder->push(mCommand);
                newState = State::Hovered;
                clicked = true;
            }
            else newState = State::Pressed;
        }
        else newState = State::Hovered;
    }
    else
    {
        active = false;
        newState = State::Idle;
    }

    if(lastState != newState)
    {
        updateStateLook(newState);
        lastState = newState;
    }
}

void Button::draw(sf::RenderWindow& window) const
{
    window.draw(box);
    if(hasTextures) window.draw(sprite);
    if(hasText) window.draw(text);
}

void Button::reset()
{
    lastState = State::Idle;
    box.setColor(stateColor.idle);
    if (hasTextures) sprite.setTexture(stateTexture.idle);
}

void Button::updateStateLook(State::ID state)
{
    switch (state)
    {
    case State::Idle:
        box.setColor(stateColor.idle);
        break;
    case State::Hovered:
        box.setColor(stateColor.hovered);
        break;
    case State::Pressed:
        box.setColor(stateColor.pressed);
        break;
    }

    if (hasTextures)
    {
        switch (state)
        {
        case State::Idle:
            sprite.setTexture(stateTexture.idle);
            break;
        case State::Hovered:
            sprite.setTexture(stateTexture.hovered);
            break;
        case State::Pressed:
            sprite.setTexture(stateTexture.pressed);
            break;
        }
        sprite.setScale(size.x / sprite.getLocalBounds().width, size.y / sprite.getLocalBounds().height);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void Button::setPosition(const sf::Vector2f& position)
{
    this->position = position;
    box.setPosition(position);

    if(hasTextures) sprite.setPosition(position);
    if(hasText) centerText();
}

void Button::setSize(const sf::Vector2f& size)
{
    this->size = size;
    box.setSize(size);

    if(hasTextures) sprite.setScale(size.x / sprite.getLocalBounds().width, size.y / sprite.getLocalBounds().height);
    if(hasText) centerText();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void Button::setCommand(CommandHolder* commandHolder, Command::ID mCommand)
{
    this->commandHolder = commandHolder;
    this->mCommand = mCommand;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void Button::setColors(const sf::Color& idle, const sf::Color& hovered, const sf::Color& pressed)
{
    stateColor.idle = idle;
    stateColor.hovered = hovered;
    stateColor.pressed = pressed;

    box.setColor(stateColor.idle);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////


void Button::setText(const sf::Font& font, int size, const sf::Color& color, const sf::Text::Style style, const std::string& str)
{
    text.setFont(font);
    text.setCharacterSize(size);
    text.setFillColor(color);
    text.setStyle(style);
    text.setString(str);
    centerText();

    hasText = true;
}

void Button::centerText()
{
    text.setOrigin(text.getGlobalBounds().getSize() / 2.f + text.getLocalBounds().getPosition());
    text.setPosition(position + (size / 2.f));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void Button::setTextures(const sf::Texture& idle, const sf::Texture& hovered, const sf::Texture& pressed)
{
    stateTexture.idle = idle;
    stateTexture.hovered = hovered;
    stateTexture.pressed = pressed;

    sprite.setTexture(idle);
    sprite.setPosition(position);
    sprite.setScale(size.x / sprite.getLocalBounds().width, size.y / sprite.getLocalBounds().height);

    hasTextures = true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

bool Button::wasClicked()
{
    if (clicked)
    {
        clicked = false;
        return true;
    }
    return false;
}

void Button::setBox(const RoundedRect& box)
{
    this->box = box;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////