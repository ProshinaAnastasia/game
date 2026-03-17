#ifndef FOOD_HPP
#define FOOD_HPP

#include <SFML/Graphics.hpp>
#include <random>

/**
 * @brief Класс Food представляет еду в игре
 */
class Food : public sf::Drawable {
public:
    // Конструктор
    Food(float size);
    
    // Обновление позиции еды
    void update(float deltaTime);
    
    // Получение позиции еды
    sf::Vector2f getPosition() const;
    
    // Получение размера еды
    float getSize() const;
    
    // Перемещение еды в случайную позицию в пределах окна
    void setRandomPosition(const sf::Vector2u& windowSize, float gridSize);
    
    // Переопределение метода draw для отрисовки еды
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    // Форма еды
    sf::CircleShape shape;
    
    // Размер еды
    float foodSize;
    
    // Генератор случайных чисел
    std::mt19937 rng;
    
    // Анимация еды
    float animationTime;
    float pulsationSpeed;
    float pulsationAmplitude;
};

#endif // FOOD_HPP 