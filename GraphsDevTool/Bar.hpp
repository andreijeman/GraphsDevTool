#ifndef BAR_HPP
#define BAR_HPP

#include <vector>

#include "Button.hpp"
#include "RoundedRect.hpp"
#include "UIComponent.hpp"
#include "UIEntity.hpp"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Bar : public UIEntity
{
public:
    Bar();
    Bar(const sf::Vector2f& position, const sf::Vector2f& size);

public:
    void                                            update(sf::RenderWindow& window, const sf::Vector2f& mousePos, const UserInput& userInput) override;
    void 	                                        draw(sf::RenderWindow& window) const override;
    void	                                		reset() override;

public:
    void                                            addObject(std::shared_ptr<UIObject> object);
    void                                            setBackground(const RoundedRect& background);

protected:
    std::vector<std::shared_ptr<UIObject>>          objects;
    std::shared_ptr<UIObject>                       focusedObject;

    RoundedRect                                     background;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif