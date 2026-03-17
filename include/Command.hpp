#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Snake.hpp"

/**
 * @brief Абстрактный класс Command представляет команду в игре
 * Реализует паттерн Command для управления змейкой
 */
class Command {
public:
    // Виртуальный деструктор
    virtual ~Command() = default;
    
    // Выполнение команды
    virtual void execute(Snake* snake) = 0;
};

/**
 * @brief Класс MoveUpCommand представляет команду движения вверх
 */
class MoveUpCommand : public Command {
public:
    // Выполнение команды движения вверх
    virtual void execute(Snake* snake) override;
};

/**
 * @brief Класс MoveDownCommand представляет команду движения вниз
 */
class MoveDownCommand : public Command {
public:
    // Выполнение команды движения вниз
    virtual void execute(Snake* snake) override;
};

/**
 * @brief Класс MoveLeftCommand представляет команду движения влево
 */
class MoveLeftCommand : public Command {
public:
    // Выполнение команды движения влево
    virtual void execute(Snake* snake) override;
};

/**
 * @brief Класс MoveRightCommand представляет команду движения вправо
 */
class MoveRightCommand : public Command {
public:
    // Выполнение команды движения вправо
    virtual void execute(Snake* snake) override;
};

#endif // COMMAND_HPP 