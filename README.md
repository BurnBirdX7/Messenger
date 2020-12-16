# Messenger

## Участники
- Лазарев Артемий
- Тимощак Егор
- Шемета Кирилл
- Крыжановская Полина

*Группа 3530202/80202*.

## Описание

Messenger - клиент-серверный мессенджер, который позволяет обмениваться личными сообщениями и создавать групповые чаты.

Программа состоит из 4х компонентов:
- `Commons` - Общие компоненты
  - `Data` - Работа с данными
  - `Network` - Работа с сетью
  - `System` - Исключения и контекст
- `Client` - клиентское приложение
- `Server` - серверное приложение
- `Tests` - модуль тестирования

## Зависимости
- Boost
- PostgreSQL
- pqxx
- OpenSSL *-ish*
- Google.Test
- ~ Crypto++

## Подготовка к сборке

### CMake

Для сборки используется система сборки CMake. Система позволяет собирать проекты под различные платформы,  
но на данный момент тестирование проводилось только для **Windows 10** и компилятора **MSVC2019 x64**.

CMake есть в составе сред JetBrain CLion и Microsoft Visual Studio. Скачать его можно [здесь](https://cmake.org/download/).

### OpenSSl
Перед тем как собрать проект сначала нужно собрать библиотеку OpenSSL или его заголовочно совместимый аналог:
- [OpenSSL](https://www.openssl.org/source/gitrepo.html)
- [**LibreSSL**](https://github.com/libressl-portable/portable)
- [BoringSSL](https://github.com/google/boringssl)

В кэше CMake необходимо будет указать 3 переменные:
 - `OPENSSL_INC_DIR` - путь к директории include
 - `OPENSSL_LIBSLL` - путь к статическкой библиотеке libssl
 - `OPENSSL_LIBCRYPTO` - путь к статической библиотеке libcrypto.  

Удобно это можно сделать если собирать проект в *Visual Studio* или если генерировать проект для своей среды спомощью **cmake-gui**.
Неудобный способ ниже.

Такой способ подключения обусловлен тем, что он универсален и подходит для подключения всех 3х аналогов.  
Аналоги же предлагнаются по причине того, что сборка оригинального OpenSSL - ужасная боль.
Собрать, например, LibreSSL в разы проще.

### Boost
Также требуется установить библиотеку [Boost](https://www.boost.org/users/download/) версии 1.74.0
Достаточно скачать архив с Boost, распаковать куда удобно и указать путь к Boost в переменной среды BOOST_ROOT.

### PostgreSQL и pqxx

Для работы мессенджера нужна база данных (Проект использует PostgreSQL 13) и библиотека [pqxx](https://github.com/jtv/libpqxx).  
Библиотека **pqxx** в свою очередь требует libpq (Поставляется вместе с PostgreSQL).

В зависимости от платформы может потребоваться линковка дополнительных библиотек
(Например, в случае Windows это **wsock32**, **ws2_32**)

## Сборка

Есть несколько вариантов сборки

 - ### Генератор проектов
CMake может сгенерировать проект для удобной вам среды / тулсета.  
Иногда потребуется указать пути до компиляторов.
Список доступных генераторов можно получить так:

    cmake -G
Пример:  

    cmake -G"Visual Studio 16 2019"
То же самое только нагляднее и удобнее можно сделать с помощью инструмента **cmake-gui**, который поставляется вместе с CMake.

 - ### Сборка в терминале

Перейдите в директорию проета.

    mkdir build
    cd build
    cmake -DOPENSSL_INC_DIR='/path/to/openssl/include' -DOPENSSL_LIBSLL='/path/to/openssl/lib/ssl.lib' -DOPENSSL_LIBCRYPTO='/path/to/openssl/lib/crypto.lib' ..
    cmake --build .

 - ### IDE
Некоторые среды разработки поддерживают CMake "Из коробки" - вам не нужно генерировать проект для них.  
(например, MS Visual Studio и JB CLion)

## Шифорвание

Для работы необходимо создать закрытый ключ и сертификат для сервера и подписать сертификат коренным CA-сертификатом (можно собственным).  
Путь до коренного сертификата нужно указать в `client_config.xml` (тэг `<ca_cert>`).  
Путь до сертификата и закрытого ключа сервера нужно указать в `server_config.xml` (тэги `<cert>` и `<key>`).

В папке `example` расположены тестовые ключ и сертификаты и DH-параметры.

### Генерация

Генерация корневого сертификата и его закрытого ключа:

    openssl genrsa -out root_ca_key.pem 2048
    openssl req -x509 -new -nodes -key root_ca_key.pem -days 10000 -out root_ca.pem

Генерация запроса на подпись для сертификата сервера и закрытого ключа сервера:

    openssl genrsa -out server_key.pem 2048
    openssl req -new -key server_key.pem -out server_req.pem

Подписывание сертификата

    openssl x509 -req -in server_req.pem -CA root_ca.pem -CAkey root_ca_key.pem -CAcreateserial -out server.pem -days 5000

Генерация DH-параметров (может занять какое-то время)

    openssl dhparam -out dh2048.pem 2048

## Unit-Тестирование

Для тестирования необходим [Google Test](https://github.com/google/googletest).  
Его нужно клонировать в репозиторий проекта в папку `Google_tests/lib`