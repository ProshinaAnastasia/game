#include "../include/GameState.hpp"
#include "../include/Game.hpp"
#include <iostream>
#include <filesystem>
#include <vector>

// Создаем статический шрифт для временной инициализации
static sf::Font defaultFont;

// Вспомогательная функция для загрузки шрифта из различных путей
bool loadFontFromPaths(sf::Font& font) {
    std::vector<std::string> fontPaths = {
        "assets/fonts/arial.ttf",
        "assets/fonts/Roboto-Regular.ttf",
        "../assets/fonts/arial.ttf",
        "../assets/fonts/Roboto-Regular.ttf",
        "/System/Library/Fonts/Supplemental/Arial.ttf",
        "/System/Library/Fonts/Supplemental/Arial Unicode.ttf",
        "/System/Library/Fonts/Supplemental/Comic Sans MS.ttf"
    };
    
    for (const auto& path : fontPaths) {
        if (std::filesystem::exists(path) && font.openFromFile(path)) {
            std::cout << "Шрифт загружен из " << path << std::endl;
            return true;
        }
    }
    
    // Если не удалось загрузить шрифт из файлов, попробуем системный шрифт
    // (зависит от платформы)
    std::cout << "Не удалось загрузить шрифт из файлов, использую системный." << std::endl;
    return false;
}

// ------ GameState ------
GameState::GameState(Game* game) : game(game) {}

GameState::~GameState() {}

// ------ PlayingState ------
PlayingState::PlayingState(Game* game) : GameState(game) {}

void PlayingState::handleInput(sf::Event event) {
    // Обработка нажатия клавиши Escape для паузы
    if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
        if (keyEvent->code == sf::Keyboard::Key::Escape) {
            game->setState(new PausedState(game));
        }
    }
}

void PlayingState::update(float deltaTime) {
    // Обновление змейки и еды
    game->getSnake()->update(deltaTime);
    game->getFood()->update(deltaTime);
    
    // Проверка столкновения с едой
    if (game->getSnake()->checkFoodCollision(game->getFood())) {
        // Еда была съедена, обновляем позицию еды
        game->getFood()->setRandomPosition(
            game->getWindow()->getSize(),
            game->getSnake()->getSegmentSize()
        );
    }
    
    // Проверка столкновения со стенами
    if (game->getSnake()->checkWallCollision(game->getWindow()->getSize())) {
        // Столкновение со стеной, игра окончена
        game->setState(new GameOverState(game));
    }
    
    // Проверка столкновения с собой
    if (game->getSnake()->checkSelfCollision()) {
        // Столкновение с собой, игра окончена
        game->setState(new GameOverState(game));
    }
}

void PlayingState::render() {
    // Отрисовка текущего состояния игры в игровом окне
    sf::RenderWindow* window = game->getWindow();
    
    // Очистка окна (черный фон)
    window->clear(sf::Color(0, 0, 0));
    
    // Отрисовка сетки (если включено)
    // TODO: добавить отрисовку сетки
    
    // Отрисовка змейки и еды
    window->draw(*game->getSnake());
    window->draw(*game->getFood());
    window->draw(*game->getScoreDisplay());
}

// ------ PausedState ------
PausedState::PausedState(Game* game) 
    : GameState(game)
    , pauseText(defaultFont, "", 72) {
    if (!loadFontFromPaths(font)) {
        // Если не удалось загрузить шрифт, используем запасной вариант
        std::cerr << "Не удалось загрузить шрифт для меню паузы." << std::endl;
    }
    
    // Настройка текста паузы после загрузки шрифта
    pauseText = sf::Text(font, "ПАУЗА", 72);
    pauseText.setFillColor(sf::Color::Yellow);
    
    // Центрирование текста на экране
    auto textBounds = pauseText.getLocalBounds();
    pauseText.setOrigin({textBounds.size.x / 2.0f, textBounds.size.y / 2.0f});
    pauseText.setPosition({
        game->getWindow()->getSize().x / 2.0f,
        game->getWindow()->getSize().y / 2.0f
    });
}

void PausedState::handleInput(sf::Event event) {
    // Обработка нажатия клавиши Escape или P для возобновления игры
    if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
        if (keyEvent->code == sf::Keyboard::Key::Escape || keyEvent->code == sf::Keyboard::Key::P) {
            game->setState(new PlayingState(game));
        }
    }
}

void PausedState::update(float) {
    // В режиме паузы ничего не обновляется
}

void PausedState::render() {
    // Отрисовка текущего состояния игры в игровом окне
    sf::RenderWindow* window = game->getWindow();
    
    // Очистка окна (темно-серый фон для режима паузы)
    window->clear(sf::Color(50, 50, 50));
    
    // Отрисовка змейки, еды и счета (как в обычном режиме)
    window->draw(*game->getSnake());
    window->draw(*game->getFood());
    window->draw(*game->getScoreDisplay());
    
    // Отрисовка текста паузы поверх игры
    window->draw(pauseText);
}

// ------ GameOverState ------
GameOverState::GameOverState(Game* game) 
    : GameState(game)
    , gameOverText(defaultFont, "", 72)
    , scoreText(defaultFont, "", 36)
    , restartText(defaultFont, "", 24) {
    if (!loadFontFromPaths(font)) {
        // Если не удалось загрузить шрифт, используем запасной вариант
        std::cerr << "Не удалось загрузить шрифт для экрана окончания игры." << std::endl;
    }
    
    // Настройка текста окончания игры после загрузки шрифта
    gameOverText = sf::Text(font, "ИГРА ОКОНЧЕНА", 72);
    gameOverText.setFillColor(sf::Color::Red);
    
    // Центрирование текста на экране
    auto gameOverBounds = gameOverText.getLocalBounds();
    gameOverText.setOrigin({gameOverBounds.size.x / 2.0f, gameOverBounds.size.y / 2.0f});
    gameOverText.setPosition({
        game->getWindow()->getSize().x / 2.0f,
        game->getWindow()->getSize().y / 2.0f - 100.0f
    });
    
    // Настройка текста счета
    std::string scoreString = "Ваш счет: " + std::to_string(game->getScoreDisplay()->getScore());
    scoreText = sf::Text(font, scoreString, 36);
    auto scoreBounds = scoreText.getLocalBounds();
    scoreText.setOrigin({scoreBounds.size.x / 2.0f, scoreBounds.size.y / 2.0f});
    scoreText.setPosition({
        game->getWindow()->getSize().x / 2.0f,
        game->getWindow()->getSize().y / 2.0f
    });
    
    // Текст с инструкцией для перезапуска
    restartText = sf::Text(font, "Нажмите R для перезапуска", 24);
    auto restartBounds = restartText.getLocalBounds();
    restartText.setOrigin({restartBounds.size.x / 2.0f, restartBounds.size.y / 2.0f});
    restartText.setPosition({
        game->getWindow()->getSize().x / 2.0f,
        game->getWindow()->getSize().y / 2.0f + 100.0f
    });
}

void GameOverState::handleInput(sf::Event event) {
    // Обработка нажатия клавиши R для перезапуска игры
    if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
        if (keyEvent->code == sf::Keyboard::Key::R) {
            game->restart();
        }
    }
}

void GameOverState::update(float) {
    // В режиме окончания игры ничего не обновляется
}

void GameOverState::render() {
    // Отрисовка текущего состояния игры в игровом окне
    sf::RenderWindow* window = game->getWindow();
    
    // Очистка окна (темный фон для режима окончания игры)
    window->clear(sf::Color(20, 20, 20));
    
    // Отрисовка текстов
    window->draw(gameOverText);
    window->draw(scoreText);
    window->draw(restartText);
} 