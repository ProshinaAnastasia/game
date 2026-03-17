#include "../include/Game.hpp"
#include <iostream>

/**
 * @brief Основная функция программы
 * @return Код завершения программы
 */
int main() {
    try {
        // Получаем экземпляр игры через Singleton
        Game* game = Game::getInstance();
        
        // Запускаем игру
        game->run();
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Неизвестная ошибка!" << std::endl;
        return 1;
    }
} 