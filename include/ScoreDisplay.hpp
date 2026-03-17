#ifndef SCORE_DISPLAY_HPP
#define SCORE_DISPLAY_HPP

#include <SFML/Graphics.hpp>

/**
 * @brief Класс ScoreDisplay отвечает за отображение счета игры
 */
class ScoreDisplay : public sf::Drawable {
public:
    // Конструктор по умолчанию
    ScoreDisplay();
    
    // Инициализация (загрузка шрифта, создание текста)
    void initialize();
    
    // Увеличение счета на заданное количество очков
    void incrementScore(int points = 10);
    
    // Сброс счета до нуля
    void resetScore();
    
    // Получение текущего счета
    int getScore() const;
    
    // Переопределение метода draw для отрисовки счета
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    // Текущий счет
    int score;
    
    // Шрифт и текст для отображения счета
    sf::Font font;
    sf::Text scoreText;
    
    // Обновление текста счета
    void updateText();
};

#endif // SCORE_DISPLAY_HPP 