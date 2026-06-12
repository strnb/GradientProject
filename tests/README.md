# Юнит-тесты GradientProject

Тесты написаны с использованием **Google Test** и покрывают весь публичный
API модулей из папки `shared/` (Qt не требуется).

## Структура

```
tests/
├── CMakeLists.txt      ← система сборки, автоматически скачивает GTest
├── test_gradient.cpp   ← computeGradient / gradientDescent / gradientPath
├── test_sha256.cpp     ← sha256 + hashPassword (crypto_utils)
├── test_vigenere.cpp   ← vigenereEncrypt / vigenereDecrypt
└── test_spline.cpp     ← splineInterpolate
```

## Быстрый старт

```bash
# Из корня репозитория
cd tests
cmake -B build -S .
cmake --build build -j$(nproc)

# Запустить все тесты
cd build && ctest --output-on-failure
```

Или запустить отдельный исполняемый файл с подробным выводом:

```bash
./build/test_all --gtest_color=yes
./build/test_gradient
./build/test_sha256
./build/test_vigenere
./build/test_spline
```

## Что тестируется

### `test_gradient.cpp` (17 тестов)
| Функция | Кейсы |
|---|---|
| `computeGradient` | x², sin, неизвестная функция, граничные значения |
| `gradientDescent` | сходимость к нулю, ноль итераций, одна итерация, неизвестная функция |
| `gradientPath` | размер вектора = N+1, первый элемент = start, монотонное убывание, совпадение с `gradientDescent` |

### `test_sha256.cpp` (13 тестов)
| Функция | Кейсы |
|---|---|
| `sha256` | длина 64 символа, hex-lowercase, NIST-векторы ("", "abc"), детерминированность, регистрозависимость, длинный ввод, нулевые байты |
| `hashPassword` | делегирует в sha256, пустой пароль, детерминированность |

### `test_vigenere.cpp` (16 тестов)
| Функция | Кейсы |
|---|---|
| `vigenereEncrypt` | классический вектор "ATTACKATDAWN/LEMON", пустой ключ, пустой текст, не-буквы, lower/upper, обёртка алфавита |
| `vigenereDecrypt` | обратный вектор, пустой ключ/текст, не-буквы, обёртка |
| round-trip | длинный текст, пробелы и пунктуация, однобуквенный ключ |

### `test_spline.cpp` (14 тестов)
| Функция | Кейсы |
|---|---|
| `splineInterpolate` | точные узлы, середина сегмента, линейная функция, вне диапазона (→ 0), отрицательные координаты, дробный шаг |

## Требования

- CMake ≥ 3.20
- GCC / Clang / MSVC с поддержкой C++17
- Доступ в интернет для первой сборки (FetchContent скачивает GTest)
