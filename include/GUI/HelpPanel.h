#include <SFML/Graphics.hpp>

class HelpPanel {
public:
    HelpPanel(const sf::Font& font,sf::RenderWindow& window);
    void toggle();
    void draw(sf::RenderWindow& window);
    bool isVisible() const;
    bool contains(sf::Vector2f point) const;

private:
    sf::RectangleShape background;
    sf::Text helpText;
    bool visible = false;
};