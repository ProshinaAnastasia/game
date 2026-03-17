#include "../include/Food.hpp"
#include <cmath>
#include <random>

Food::Food(float size)
    : foodSize(size)
    , animationTime(0.0f)
    , pulsationSpeed(4.0f)
    , pulsationAmplitude(0.2f) {
    
    // Настройка формы еды
    shape.setRadius(size / 2.0f);
    shape.setFillColor(sf::Color::Red);
    shape.setOutlineThickness(1.0f);
    shape.setOutlineColor(sf::Color(150, 0, 0));
    shape.setOrigin({size / 2.0f, size / 2.0f});
    
    // Инициализация генератора случайных чисел
    std::random_device rd;
    rng = std::mt19937(rd());
}

void Food::update(float deltaTime) {
    // Обновление анимации пульсации
    animationTime += deltaTime;
    
    // Плавное изменение размера для эффекта пульсации
    float pulsation = 1.0f + pulsationAmplitude * std::sin(animationTime * pulsationSpeed);
    shape.setScale({pulsation, pulsation});
    
    // Плавное изменение цвета для визуального эффекта
    int colorFluctuation = static_cast<int>((std::sin(animationTime * pulsationSpeed) + 1.0f) * 40.0f);
    shape.setFillColor(sf::Color(200 + colorFluctuation, 0, 0));
}

sf::Vector2f Food::getPosition() const {
    return shape.getPosition();
}

float Food::getSize() const {
    return foodSize;
}

void Food::setRandomPosition(const sf::Vector2u& windowSize, float gridSize) {
    // Создание равномерного распределения для координат в пределах сетки
    std::uniform_int_distribution<int> xDist(0, static_cast<int>((windowSize.x - foodSize) / gridSize));
    std::uniform_int_distribution<int> yDist(0, static_cast<int>((windowSize.y - foodSize) / gridSize));
    
    // Генерация случайной позиции на сетке
    float x = xDist(rng) * gridSize + foodSize / 2.0f;
    float y = yDist(rng) * gridSize + foodSize / 2.0f;
    
    // Установка новой позиции
    shape.setPosition({x, y});
}

void Food::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(shape, states);
} 