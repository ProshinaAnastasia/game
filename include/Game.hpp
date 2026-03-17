#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <map>
#include "Snake.hpp"
#include "Food.hpp"
#include "GameState.hpp"
#include "ScoreDisplay.hpp"
#include "Command.hpp"

/**
 * @brief Класс Game реализует паттерн Singleton и управляет игровым процессом
 */
class Game {
private:
    // Singleton instance
    static Game* instance;
    
    // Приватный конструктор (для паттерна Singleton)
    Game();
    
    // Окно игры
    sf::RenderWindow window;
    
    // Змейка
    std::unique_ptr<Snake> snake;
    
    // Еда
    std::unique_ptr<Food> food;
    
    // Текущее состояние игры
    std::unique_ptr<GameState> currentState;
    
    // Отображение счета
    std::unique_ptr<ScoreDisplay> scoreDisplay;
    
    // Время с последнего обновления
    sf::Clock clock;
    
    // Счетчик FPS
    sf::Clock fpsClock;
    int frameCount;
    
    // Карта команд
    std::map<sf::Keyboard::Key, std::unique_ptr<Command>> commands;
    
    // Инициализация игры
    void initialize();
    
    // Обработка ввода
    void handleInput();
    
    // Обновление состояния игры
    void update();
    
    // Отрисовка игры
    void render();

public:
    // Удаляем конструктор копирования и оператор присваивания
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    
    // Получение экземпляра Singleton
    static Game* getInstance();
    
    // Запуск игры
    void run();
    
    // Перезапуск игры
    void restart();
    
    // Переключение состояния игры
    void setState(GameState* state);
    
    // Получение змейки
    Snake* getSnake() const;
    
    // Получение еды
    Food* getFood() const;
    
    // Получение отображения счета
    ScoreDisplay* getScoreDisplay() const;
    
    // Получение размеров окна
    sf::Vector2u getWindowSize() const;
    
    // Получение окна
    sf::RenderWindow* getWindow();
    
    // Деструктор
    ~Game();
};

#endif // GAME_HPP 