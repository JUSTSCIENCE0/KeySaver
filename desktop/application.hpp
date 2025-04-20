// Copyright (c) 2025, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

#include <QtQuick>
#include <QApplication>
#include <QLocalServer>
#include <QLocalSocket>

namespace KeysaverDesktop {
    class Application final {
        friend class Controller;

    public:
        // ctors & dtor
        Application(const Application&) = delete;
        Application(Application&&) = delete;
        ~Application() = default;
        Application& operator=(const Application&) = delete;
        Application& operator=(Application&&) = delete;

        static int Run(int argc, char *argv[]);

    private:
        Application(int argc, char *argv[]);

        // consts
        static constexpr auto DEFAULT_LOCALE = "en_US";
        static constexpr auto APP_RESOURCE_ID = "qrc:/UI/App.qml";
        static constexpr auto MAIN_LAYOUT_RESOURCE_ID = "qrc:/UI/layouts/GetPassword.qml";
        static constexpr auto DEFAULT_SPEC_SYMBOLS_SET = "!@#$%^&*()_-+=/?.,<>'\";:[]{}";

        // members
        QApplication       m_gui_app;
        QQmlApplicationEngine m_qml_app_engine;

        // methods
        static bool IsAlreadyRunning();
        static std::string GetPrefferedLocale();
    };
}