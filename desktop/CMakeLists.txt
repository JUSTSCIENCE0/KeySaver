# Copyright (c) 2025, Yakov Usoltsev
# Email: yakovmen62@gmail.com
#
# License: MIT

cmake_minimum_required(VERSION 3.22.1)

project(keysaver-desktop)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(IMPLEMENTATION
    main.cpp
    application.hpp
    application.cpp
    controller.hpp
    controller.cpp
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Quick REQUIRED)

add_executable(keysaver-desktop)

# compile options
if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(keysaver-desktop PRIVATE
        # warnings options
        /W4 /WX
        /DUNICODE /D_UNICODE  # use unicode
        /MP                   # multiple processes build
    )
elseif ((CMAKE_CXX_COMPILER_ID MATCHES "GNU") OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
    target_compile_options(keysaver-desktop PRIVATE
        # warnings options
        -Wall -Wextra -Wpedantic -Wcast-align -Wcast-qual
        -Wdisabled-optimization -Wformat=2 -Winit-self 
        -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual
        -Wredundant-decls -Wshadow -Wsign-promo 
        -Wstrict-overflow=5 -Wundef -Wunused
        -fdiagnostics-show-option
        -Werror
    )
endif() # GCC & Clang

# sources
target_sources(keysaver-desktop
    PRIVATE
        ${IMPLEMENTATION}
)

# qt resources
qt_add_resources(keysaver-desktop "qml_resources"
    PREFIX "/"
    FILES
        UI/App.qml
        UI/components/HelpButton.qml
        UI/components/ImageButton.qml
        UI/components/StyledButton.qml
        UI/components/StyledComboBox.qml
        UI/components/StyledInput.qml
        UI/components/StyledLabel.qml
        UI/components/StyledSwitch.qml
        UI/layouts/AddConfiguration.qml
        UI/layouts/AddService.qml
        UI/layouts/EnterMasterPassword.qml
        UI/layouts/GetPassword.qml
        UI/layouts/SetupService.qml
        UI/pictures/bot.jpg
        UI/pictures/car.jpg
        UI/pictures/cat.jpg
        UI/pictures/city.jpg
        UI/pictures/egg.jpg
        UI/pictures/girl.jpg
        UI/pictures/ic_enable_backup.png
        UI/pictures/ic_help.png
        UI/pictures/ic_setup.png
        UI/pictures/ic_share_backup.png
        UI/pictures/phone.jpg
        UI/pictures/plane.jpg
        UI/pictures/trees.jpg
)

target_link_libraries(keysaver-desktop
    PRIVATE
        keysaver-core
        Qt6::Quick
)
