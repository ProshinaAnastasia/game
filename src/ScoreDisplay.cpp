#include "../include/ScoreDisplay.hpp"
#include <iostream>
#include <filesystem>

// Статический шрифт для временной инициализации
static sf::Font defaultFont;

ScoreDisplay::ScoreDisplay() 
    : score(0)
    , scoreText(defaultFont, "", 24) {
    // Создаем текст для отображения счета после загрузки шрифта
    updateText();
}

void ScoreDisplay::initialize() {
    // Загрузка шрифта из различных возможных путей
    std::string fontPaths[] = {
        "assets/fonts/arial.ttf",
        "../assets/fonts/arial.ttf",
        "/System/Library/Fonts/Supplemental/Arial.ttf",
        "/System/Library/Fonts/Supplemental/Arial Unicode.ttf"
    };
    
    bool fontLoaded = false;
    for (const auto& path : fontPaths) {
        if (std::filesystem::exists(path) && font.openFromFile(path)) {
            std::cout << "Шрифт загружен из " << path << std::endl;
            fontLoaded = true;
            break;
        }
    }
    
    if (!fontLoaded) {
        std::cerr << "Не удалось загрузить шрифт для отображения счета." << std::endl;
    }
    
    // Создаем текст для отображения счета с загруженным шрифтом
    scoreText = sf::Text(font, "Score: 0", 24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition({10.f, 10.f});
    updateText();
}

void ScoreDisplay::updateText() {
    // Обновление текста счета
    scoreText.setString("Score: " + std::to_string(score));
}

void ScoreDisplay::incrementScore(int points) {
    score += points;
    updateText();
}

void ScoreDisplay::resetScore() {
    score = 0;
    updateText();
}

int ScoreDisplay::getScore() const {
    return score;
}

void ScoreDisplay::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // Отрисовка текста счета
    target.draw(scoreText, states);
} 