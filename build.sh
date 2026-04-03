#!/bin/bash
set -e

PROJECT_DIR="$(cd "$(dirname "$0")" && pwd)"
DEPS_DIR="$PROJECT_DIR/deps"
SFML_DIR="$DEPS_DIR/SFML"
SFML_INSTALL_DIR="$DEPS_DIR/sfml_install"
SFML_CONFIG="$SFML_INSTALL_DIR/lib/cmake/SFML3/SFMLConfig.cmake"

echo "=== Проект в: $PROJECT_DIR ==="

# Установка системных зависимостей (требуют sudo, но это только apt)
# Если sudo недоступен, придётся пропустить, но тогда сборка SFML не пройдёт.
if command -v sudo &> /dev/null && command -v apt &> /dev/null; then
    echo "Устанавливаю зависимости сборки через apt..."
    sudo apt update
    sudo apt install -y git cmake build-essential \
        libudev-dev libopenal-dev libvorbis-dev libflac-dev \
        libfreetype-dev libx11-dev libxrandr-dev
else
    echo "Предупреждение: не могу установить зависимости через apt. Убедитесь, что они уже есть."
fi

# Сборка SFML, если отсутствует
if [ ! -f "$SFML_CONFIG" ]; then
    echo "SFML 3.0 не найдена в $SFML_INSTALL_DIR. Сборка локально..."
    mkdir -p "$DEPS_DIR"
    
    if [ ! -d "$SFML_DIR" ]; then
        git clone --depth 1 --branch 3.0.0 https://github.com/SFML/SFML.git "$SFML_DIR"
    fi
    
    cd "$SFML_DIR"
    cmake -B build -DCMAKE_INSTALL_PREFIX="$SFML_INSTALL_DIR" -DCMAKE_BUILD_TYPE=Release
    cmake --build build -j$(nproc)
    cmake --install build
    cd "$PROJECT_DIR"
    
    echo "SFML установлена локально в $SFML_INSTALL_DIR"
else
    echo "SFML уже установлена в $SFML_INSTALL_DIR"
fi

# Подчистка CMakeLists.txt от старых путей macOS (если нужно)
if grep -q "/opt/homebrew" CMakeLists.txt; then
    echo "Удаляю устаревшие пути macOS из CMakeLists.txt..."
    sed -i.bak '/\/opt\/homebrew/d' CMakeLists.txt
fi

# Убедимся, что CMakeLists.txt использует правильную SFML
# Добавляем подсказку для find_package, если ещё нет
if ! grep -q "SFML_DIR" CMakeLists.txt; then
    echo "Добавляю в CMakeLists.txt путь к локальной SFML..."
    # Вставляем после project(...) строку с указанием SFML_DIR
    sed -i.bak '/^project(/a \
# Указываем путь к локально установленной SFML (если используется build.sh)\
set(SFML_DIR "'"$SFML_INSTALL_DIR/lib/cmake/SFML3"'" CACHE PATH "Path to SFML3")\
' CMakeLists.txt
fi

# Очистка и сборка проекта
echo "=== Сборка проекта ==="
rm -rf build
mkdir build
cd build

# Передаём CMake путь к SFML (на всякий случай)
cmake .. -DSFML_DIR="$SFML_INSTALL_DIR/lib/cmake/SFML3"
make -j$(nproc)

echo "Сборка успешно завершена!"
chmod +x snake
echo "Запустите игру: LD_LIBRARY_PATH=/usr/local/lib ./snake"