#include "/workspaces/game/include/Snake.hpp"
#include "/workspaces/game/include/Food.hpp"
#include <cmath>

// Реализация метода-конструктора сегмента
Snake::Segment::Segment(const sf::Vector2f& position, float size) : sf::RectangleShape(sf::Vector2f{size, size}) {
    setPosition({position.x, position.y});
    setFillColor(sf::Color::Green);
    setOutlineThickness(1.0f);
    setOutlineColor(sf::Color(0, 100, 0));
}

// Конструктор класса Snake
Snake::Snake(const sf::Vector2f& initialPosition, float segmentSize)
    : direction(Direction::Right)
    , nextDirection(Direction::Right)
    , segmentSize(segmentSize)
    , speed(5.0f)
    , moveTimer(0.0f)
    , shouldGrow(false) {
    
    // Инициализация начального сегмента (головы)
    body.push_front(Segment(initialPosition, segmentSize));
    
    // Добавляем еще два сегмента для начальной змейки
    body.push_front(Segment(sf::Vector2f{initialPosition.x - segmentSize, initialPosition.y}, segmentSize));
    body.push_front(Segment(sf::Vector2f{initialPosition.x - 2 * segmentSize, initialPosition.y}, segmentSize));
}

void Snake::update(float deltaTime) {
    moveTimer += deltaTime;
    
    // Движение с фиксированной скоростью
    if (moveTimer >= 1.0f / speed) {
        // Обновляем направление
        direction = nextDirection;
        
        // Перемещаем змейку
        move();
        moveTimer = 0.0f;
    }
}

void Snake::setDirection(Direction newDirection) {
    // Проверка на противоположное направление (нельзя развернуться на 180 градусов)
    if ((newDirection == Direction::Up && direction == Direction::Down) ||
        (newDirection == Direction::Down && direction == Direction::Up) ||
        (newDirection == Direction::Left && direction == Direction::Right) ||
        (newDirection == Direction::Right && direction == Direction::Left)) {
        return;
    }
    
    nextDirection = newDirection;
}

bool Snake::checkFoodCollision(Food* food) {
    if (!food) return false;
    
    // Получение позиции и размера головы змейки
    sf::Vector2f headPos = body.back().getPosition();
    float headSize = body.back().getSize().x;
    
    // Получение позиции и размера еды
    sf::Vector2f foodPos = food->getPosition();
    float foodSize = food->getSize();
    
    // Проверка пересечения прямоугольников (головы змейки и еды)
    if (headPos.x < foodPos.x + foodSize &&
        headPos.x + headSize > foodPos.x &&
        headPos.y < foodPos.y + foodSize &&
        headPos.y + headSize > foodPos.y) {
        
        // Увеличиваем змейку
        grow();
        
        // Уведомляем наблюдателей о столкновении с едой
        notifyFoodObservers(food);
        
        return true;
    }
    
    return false;
}

bool Snake::checkSelfCollision() {
    // Проверка на самопересечение работает только если у змейки минимум 4 сегмента
    if (body.size() < 4) return false;
    
    sf::Vector2f headPos = body.back().getPosition();
    float headSize = body.back().getSize().x;
    
    // Проверяем пересечение головы с любым другим сегментом (кроме следующего за головой)
    auto it = body.begin();
    auto end = body.end();
    --end; // Пропускаем голову
    --end; // Пропускаем сегмент после головы (из-за особенностей реализации)
    
    for (; it != end; ++it) {
        sf::Vector2f segPos = it->getPosition();
        
        if (std::abs(headPos.x - segPos.x) < headSize * 0.5f &&
            std::abs(headPos.y - segPos.y) < headSize * 0.5f) {
            
            // Уведомляем наблюдателей о столкновении
            notifyCollisionObservers();
            return true;
        }
    }
    
    return false;
}

bool Snake::checkWallCollision(const sf::Vector2u& windowSize) {
    sf::Vector2f headPos = body.back().getPosition();
    
    // Проверка выхода за границы окна
    if (headPos.x < 0 || headPos.y < 0 ||
        headPos.x + segmentSize > windowSize.x ||
        headPos.y + segmentSize > windowSize.y) {
        
        // Уведомляем наблюдателей о столкновении
        notifyCollisionObservers();
        return true;
    }
    
    return false;
}

void Snake::grow() {
    shouldGrow = true;
}

void Snake::reset(const sf::Vector2f& initialPosition) {
    // Очищаем текущее тело змейки
    body.clear();
    
    // Инициализация начального сегмента (головы)
    body.push_front(Segment(initialPosition, segmentSize));
    
    // Добавляем еще два сегмента для начальной змейки
    body.push_front(Segment(sf::Vector2f{initialPosition.x - segmentSize, initialPosition.y}, segmentSize));
    body.push_front(Segment(sf::Vector2f{initialPosition.x - 2 * segmentSize, initialPosition.y}, segmentSize));
    
    // Сброс направления
    direction = Direction::Right;
    nextDirection = Direction::Right;
    
    // Сброс параметров
    moveTimer = 0.0f;
    shouldGrow = false;
}

size_t Snake::getSize() const {
    return body.size();
}

sf::Vector2f Snake::getHeadPosition() const {
    if (body.empty()) return sf::Vector2f(0, 0);
    return body.back().getPosition();
}

float Snake::getSegmentSize() const {
    return segmentSize;
}

void Snake::addCollisionObserver(CollisionObserver observer) {
    collisionObservers.push_back(observer);
}

void Snake::addFoodObserver(FoodObserver observer) {
    foodObservers.push_back(observer);
}

void Snake::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // Отрисовка всех сегментов змейки
    for (const auto& segment : body) {
        target.draw(segment, states);
    }
    
    // Отрисовка глаз на голове (только если змейка имеет хотя бы один сегмент)
    if (!body.empty()) {
        // Получаем позицию головы
        sf::Vector2f headPos = body.back().getPosition();
        
        // Размер глаз
        float eyeRadius = segmentSize * 0.15f;
        
        // Создаем и настраиваем глаза
        sf::CircleShape leftEye(eyeRadius);
        sf::CircleShape rightEye(eyeRadius);
        leftEye.setFillColor(sf::Color::White);
        rightEye.setFillColor(sf::Color::White);
        
        // Позиционируем глаза в зависимости от направления движения
        if (direction == Direction::Right) {
            leftEye.setPosition({headPos.x + segmentSize * 0.25f - eyeRadius, headPos.y + segmentSize * 0.25f - eyeRadius});
            rightEye.setPosition({headPos.x + segmentSize * 0.75f - eyeRadius, headPos.y + segmentSize * 0.25f - eyeRadius});
        } else if (direction == Direction::Down) {
            leftEye.setPosition({headPos.x + segmentSize * 0.25f - eyeRadius, headPos.y + segmentSize * 0.75f - eyeRadius});
            rightEye.setPosition({headPos.x + segmentSize * 0.75f - eyeRadius, headPos.y + segmentSize * 0.75f - eyeRadius});
        } else if (direction == Direction::Left) {
            leftEye.setPosition({headPos.x + segmentSize * 0.25f - eyeRadius, headPos.y + segmentSize * 0.25f - eyeRadius});
            rightEye.setPosition({headPos.x + segmentSize * 0.25f - eyeRadius, headPos.y + segmentSize * 0.75f - eyeRadius});
        } else { // Up
            leftEye.setPosition({headPos.x + segmentSize * 0.75f - eyeRadius, headPos.y + segmentSize * 0.25f - eyeRadius});
            rightEye.setPosition({headPos.x + segmentSize * 0.75f - eyeRadius, headPos.y + segmentSize * 0.75f - eyeRadius});
        }
        
        // Отрисовка глаз
        target.draw(leftEye, states);
        target.draw(rightEye, states);
        
        // Создаем и отрисовываем зрачки
        float pupilRadius = eyeRadius * 0.5f;
        sf::CircleShape leftPupil(pupilRadius);
        sf::CircleShape rightPupil(pupilRadius);
        leftPupil.setFillColor(sf::Color::Black);
        rightPupil.setFillColor(sf::Color::Black);
        
        // Позиционируем зрачки
        leftPupil.setPosition({leftEye.getPosition().x + eyeRadius * 0.5f, leftEye.getPosition().y + eyeRadius * 0.5f});
        rightPupil.setPosition({rightEye.getPosition().x + eyeRadius * 0.5f, rightEye.getPosition().y + eyeRadius * 0.5f});
        
        // Отрисовка зрачков
        target.draw(leftPupil, states);
        target.draw(rightPupil, states);
    }
}

void Snake::notifyCollisionObservers() {
    for (const auto& observer : collisionObservers) {
        observer();
    }
}

void Snake::notifyFoodObservers(Food* food) {
    for (const auto& observer : foodObservers) {
        observer(food);
    }
}

void Snake::move() {
    // Получаем позицию головы
    sf::Vector2f headPos = body.back().getPosition();
    
    // Вычисляем новую позицию головы
    sf::Vector2f newPos = headPos;
    
    switch (direction) {
        case Direction::Up:
            newPos.y -= segmentSize;
            break;
        case Direction::Down:
            newPos.y += segmentSize;
            break;
        case Direction::Left:
            newPos.x -= segmentSize;
            break;
        case Direction::Right:
            newPos.x += segmentSize;
            break;
    }
    
    // Добавляем новый сегмент (новую голову) в конец очереди
    body.push_back(Segment(newPos, segmentSize));
    
    // Если не нужно расти, удаляем хвост
    if (!shouldGrow) {
        body.pop_front();
    } else {
        shouldGrow = false; // Сбрасываем флаг роста
    }
    
    // Делаем голову чуть ярче для визуального отличия
    body.back().setFillColor(sf::Color(0, 255, 0));
}