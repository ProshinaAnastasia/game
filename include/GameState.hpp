#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <SFML/Graphics.hpp>

// Предварительное объявление класса Game
class Game;

/**
 * @brief Абстрактный класс GameState представляет состояние игры
 * Реализует паттерн State для управления состояниями игры
 */
class GameState {
public:
    // Конструктор
    GameState(Game* game);
    
    // Виртуальный деструктор
    virtual ~GameState();
    
    // Обработка ввода
    virtual void handleInput(sf::Event event) = 0;
    
    // Обновление состояния
    virtual void update(float deltaTime) = 0;
    
    // Отрисовка состояния
    virtual void render() = 0;

protected:
    // Указатель на игру
    Game* game;
};

/**
 * @brief Класс PlayingState представляет состояние активной игры
 */
class PlayingState : public GameState {
public:
    // Конструктор
    PlayingState(Game* game);
    
    // Обработка ввода
    virtual void handleInput(sf::Event event) override;
    
    // Обновление состояния
    virtual void update(float deltaTime) override;
    
    // Отрисовка состояния
    virtual void render() override;
};

/**
 * @brief Класс PausedState представляет состояние паузы
 */
class PausedState : public GameState {
public:
    // Конструктор
    PausedState(Game* game);
    
    // Обработка ввода
    virtual void handleInput(sf::Event event) override;
    
    // Обновление состояния
    virtual void update(float deltaTime) override;
    
    // Отрисовка состояния
    virtual void render() override;

private:
    // Текст для отображения паузы
    sf::Font font;
    sf::Text pauseText;
};

/**
 * @brief Класс GameOverState представляет состояние окончания игры
 */
class GameOverState : public GameState {
public:
    // Конструктор
    GameOverState(Game* game);
    
    // Обработка ввода
    virtual void handleInput(sf::Event event) override;
    
    // Обновление состояния
    virtual void update(float deltaTime) override;
    
    // Отрисовка состояния
    virtual void render() override;

private:
    // Текст для отображения Game Over
    sf::Font font;
    sf::Text gameOverText;
    
    // Текст для отображения счета
    sf::Text scoreText;
    
    // Текст для подсказки перезапуска
    sf::Text restartText;
};

#endif // GAMESTATE_HPP 