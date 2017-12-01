cmake_minimum_required(VERSION 3.8)
project(Blowfish_2)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        blowfish.cpp
        blowfish.h
        constants.h
        main.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_PREFIX_PATH "D:\\Qt\\5.9.3\\mingw53_32")#указать путь к  Qt
set(CMAKE_AUTOMOC ON)#Q_OBJECT - для использования в проекте механизма сигналов и слотов.
set(CMAKE_AUTOUIC ON)#для поддержки форм созданных с помощью QtDesigner
find_package( Qt5Core REQUIRED )# по аналогии с *.pro QT += gui core
find_package( Qt5Widgets REQUIRED )#виджеты
find_package( Qt5Network REQUIRED )#Сеть
find_package( Qt5Sql REQUIRED )#поддержка СУБД
find_package( Qt5Xml REQUIRED )#поддержка Xml
find_package( Qt5PrintSupport REQUIRED )#
find_package( Qt5OpenGL REQUIRED)

add_executable(Blowfish_2 ${SOURCE_FILES})
