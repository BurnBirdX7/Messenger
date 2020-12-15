# Messenger

## Описание

Программа состоит из 4х компонентов:
- `Commons` - Общие компоненты
  - `Data` - Работа с данными
  - `Network` - Работа с сетью
  - `System` - Исключения и контекст
- `Client` - клиентское приложение
- `Server` - серверное приложение
- `Tests` - модуль тестирования

## Подготовка к сборке

### CMake

Для сборки используется система сборки CMake. Система позволяет собирать проекты под различные платформы,  
но на данный момент тестирование проводилось только для **Windows 10** и компилятора **MSVC2019 x64**.

CMake есть в составе сред JetBrain CLion и Microsoft Visual Studio. Скачать его можно [здесь](https://cmake.org/download/).

### OpenSSl
Перед тем как собрать проект сначала нужно собрать библиотеку OpenSSL или его заголовочно совместимый аналог:
- [OpenSSL](https://www.openssl.org/source/gitrepo.html)
- [LibreSSL](https://github.com/libressl-portable/portable)
- [BoringSSL](https://github.com/google/boringssl)

В кэше CMake необходимо будет указать 3 переменные:
 - `OPENSSL_INC_DIR` - путь к директории include
 - `OPENSSL_LIBSLL` - путь к статическкой библиотеке libssl
 - `OPENSSL_LIBCRYPTO` - путь к статической библиотеке libcrypto.  

Удобно это можно сделать если собирать проект в *Visual Studio* или если генерировать проект для своей среды спомощью **cmake-gui**.
Неудобный способ ниже

### Boost
Также требуется установить библиотеку [Boost](https://www.boost.org/users/download/) версии 1.74.0
Достаточно скачать архив с Boost, распаковать куда удобно и указать путь к Boost в переменной среды BOOST_ROOT.

## Сборка

Есть несколько вариантов сборки

 - ### Генератор проектов
CMake может сгенерировать проект для удобной вам среды / тулсета.  
Иногда потребуется указать пути до компиляторов.
Список доступных генераторов можно получить так:

    cmake -G
Пример:  

    cmake -G"Visual Studio 16 2019"
То же самое только нагляднее и удобнее можно сделать с помощью инструмента cmake-gui, который поставляется вместе с CMake.

 - ### Сборка в терминале

Перейдите в директорию проета.

    mkdir build
    cd build
    cmake -DOPENSSL_INC_DIR='/path/to/openssl/include' -DOPENSSL_LIBSLL='/path/to/openssl/lib/ssl.lib' -DOPENSSL_LIBCRYPTO='/path/to/openssl/lib/crypto.lib' ..
    cmake --build .

## IDE
Некоторые среды разработки поддерживают CMake "Из коробки" - вам не нужно генерировать проект для них.  
(например, MS Visual Studio и JB CLion)

