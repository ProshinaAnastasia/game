#ifndef SNAKE_HPP
#define SNAKE_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <deque>
#include <memory>
#include <functional>

class Food;

/**
 * @brief Класс Snake реализует модель змейки в игре
 * Использует наблюдателей для уведомления о событиях
 */
class Snake : public sf::Drawable {
public:
    // Типы наблюдателей событий змейки
    using CollisionObserver = std::function<void()>;
    using FoodObserver = std::function<void(Food*)>;
    
    // Перечисление направлений движения
    enum class Direction {
        Up,     // Соответствует Up в Command.cpp
        Down,   // Соответствует Down в Command.cpp
        Left,   // Соответствует Left в Command.cpp
        Right   // Соответствует Right в Command.cpp
    };
    
    // Конструктор
    Snake(const sf::Vector2f& initialPosition, float segmentSize);
    
    // Обновление позиции змейки
    void update(float deltaTime);
    
    // Изменение направления движения
    void setDirection(Direction newDirection);
    
    // Проверка столкновения с едой
    bool checkFoodCollision(Food* food);
    
    // Проверка самостолкновения
    bool checkSelfCollision();
    
    // Проверка столкновения со стеной
    bool checkWallCollision(const sf::Vector2u& windowSize);
    
    // Рост змейки
    void grow();
    
    // Сброс змейки к начальному состоянию
    void reset(const sf::Vector2f& initialPosition);
    
    // Получение текущего размера (длины) змейки
    size_t getSize() const;
    
    // Получение позиции головы
    sf::Vector2f getHeadPosition() const;
    
    // Получение размера сегмента змейки
    float getSegmentSize() const;
    
    // Добавление наблюдателя столкновения со стеной или самой собой
    void addCollisionObserver(CollisionObserver observer);
    
    // Добавление наблюдателя столкновения с едой
    void addFoodObserver(FoodObserver observer);
    
    // Переопределение метода draw для рисования змейки
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    // Структура сегмента змейки
    struct Segment : public sf::RectangleShape {
        Segment(const sf::Vector2f& position, float size);
    };
    
    // Тело змейки (двусторонняя очередь сегментов)
    std::deque<Segment> body;
    
    // Текущее направление движения
    Direction direction;
    
    // Следующее направление движения (для обработки быстрых нажатий клавиш)
    Direction nextDirection;
    
    // Размер сегмента
    float segmentSize;
    
    // Скорость змейки
    float speed;
    
    // Время с последнего обновления
    float moveTimer;
    
    // Флаг роста змейки
    bool shouldGrow;
    
    // Наблюдатели столкновения
    std::vector<CollisionObserver> collisionObservers;
    
    // Наблюдатели для еды
    std::vector<FoodObserver> foodObservers;
    
    // Уведомление наблюдателей о столкновении
    void notifyCollisionObservers();
    
    // Уведомление наблюдателей о еде
    void notifyFoodObservers(Food* food);
    
    // Перемещение змейки
    void move();
};

#endif // SNAKE_HPP 