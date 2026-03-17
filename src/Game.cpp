#include "../include/Game.hpp"
#include "../include/GameState.hpp"
#include <iostream>

// Инициализация статического члена
Game* Game::instance = nullptr;

Game::Game() 
    : window(sf::VideoMode(sf::Vector2u{800, 600}), "Snake Game", sf::Style::Close)
    , frameCount(0) {
    
    initialize();
}

Game* Game::getInstance() {
    if (instance == nullptr) {
        // Создаем экземпляр только один раз
        static Game singletonInstance;
        instance = &singletonInstance;
    }
    return instance;
}

void Game::initialize() {
    // Настройка окна
    window.setFramerateLimit(60);
    
    // Создание змейки
    snake = std::make_unique<Snake>(sf::Vector2f{400, 300}, 20.0f);
    
    // Создание еды
    food = std::make_unique<Food>(20.0f);
    food->setRandomPosition(window.getSize(), 20.0f);
    
    // Создание отображения счета
    scoreDisplay = std::make_unique<ScoreDisplay>();
    scoreDisplay->initialize();
    
    // Настройка обработчиков событий змейки
    snake->addFoodObserver([this](Food* food) {
        // Увеличиваем счет при съедании еды
        scoreDisplay->incrementScore();
        
        // Перемещаем еду в новую случайную позицию
        food->setRandomPosition(window.getSize(), 20.0f);
    });
    
    snake->addCollisionObserver([this]() {
        // При столкновении переключаемся в состояние Game Over
        setState(new GameOverState(this));
    });
    
    // Настройка команд управления
    commands[sf::Keyboard::Key::Up] = std::make_unique<MoveUpCommand>();
    commands[sf::Keyboard::Key::W] = std::make_unique<MoveUpCommand>();
    
    commands[sf::Keyboard::Key::Down] = std::make_unique<MoveDownCommand>();
    commands[sf::Keyboard::Key::S] = std::make_unique<MoveDownCommand>();
    
    commands[sf::Keyboard::Key::Left] = std::make_unique<MoveLeftCommand>();
    commands[sf::Keyboard::Key::A] = std::make_unique<MoveLeftCommand>();
    
    commands[sf::Keyboard::Key::Right] = std::make_unique<MoveRightCommand>();
    commands[sf::Keyboard::Key::D] = std::make_unique<MoveRightCommand>();
    
    // Устанавливаем начальное состояние игры
    setState(new PlayingState(this));
    
    // Сбрасываем часы
    clock.restart();
    fpsClock.restart();
}

void Game::run() {
    while (window.isOpen()) {
        handleInput();
        update();
        render();
    }
}

void Game::handleInput() {
    // Обрабатываем события
    while (auto event = window.pollEvent()) {
        // Проверяем, является ли событие событием закрытия окна
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        
        // Передаем событие текущему состоянию
        if (currentState) {
            currentState->handleInput(*event);
        }
        
        // Обработка команд движения
        if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
            auto it = commands.find(keyEvent->code);
            if (it != commands.end() && snake) {
                it->second->execute(snake.get());
            }
        }
    }
}

void Game::update() {
    float deltaTime = clock.restart().asSeconds();
    
    // Обновление текущего состояния
    if (currentState) {
        currentState->update(deltaTime);
    }
}

void Game::render() {
    // Отрисовка текущего состояния
    if (currentState) {
        currentState->render();
    }
    
    // Отображение отрисованного кадра
    window.display();
    
    // Подсчет FPS
    frameCount++;
    if (fpsClock.getElapsedTime().asSeconds() >= 1.0f) {
        float fps = static_cast<float>(frameCount) / fpsClock.getElapsedTime().asSeconds();
        window.setTitle("Snake Game - FPS: " + std::to_string(static_cast<int>(fps)));
        frameCount = 0;
        fpsClock.restart();
    }
}

void Game::setState(GameState* state) {
    currentState.reset(state);
}

void Game::restart() {
    // Сброс змейки
    if (snake) {
        snake->reset(sf::Vector2f{400, 300});
    }
    
    // Сброс счета
    if (scoreDisplay) {
        scoreDisplay->resetScore();
    }
    
    // Перемещение еды
    if (food) {
        food->setRandomPosition(window.getSize(), 20.0f);
    }
    
    // Переключение в состояние игры
    setState(new PlayingState(this));
}

Snake* Game::getSnake() const {
    return snake.get();
}

Food* Game::getFood() const {
    return food.get();
}

ScoreDisplay* Game::getScoreDisplay() const {
    return scoreDisplay.get();
}

sf::Vector2u Game::getWindowSize() const {
    return window.getSize();
}

sf::RenderWindow* Game::getWindow() {
    return &window;
}

Game::~Game() {
    // Экземпляр будет освобожден автоматически, так как теперь
    // используется статическая локальная переменная
    instance = nullptr;
} 