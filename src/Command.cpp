#include "../include/Command.hpp"

// Реализация команды движения вверх
void MoveUpCommand::execute(Snake* snake) {
    if (snake) {
        snake->setDirection(Snake::Direction::Up);
    }
}

// Реализация команды движения вниз
void MoveDownCommand::execute(Snake* snake) {
    if (snake) {
        snake->setDirection(Snake::Direction::Down);
    }
}

// Реализация команды движения влево
void MoveLeftCommand::execute(Snake* snake) {
    if (snake) {
        snake->setDirection(Snake::Direction::Left);
    }
}

// Реализация команды движения вправо
void MoveRightCommand::execute(Snake* snake) {
    if (snake) {
        snake->setDirection(Snake::Direction::Right);
    }
} 