// Copyright (c) 2025, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

#include <keysaver/interface.h>

#include <QtQuick>
#include <QGuiApplication>

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
        static constexpr auto APP_RESOURCE_ID = "qrc:/UI/App.qml";

        // members
        QGuiApplication       m_gui_app;
        QQmlApplicationEngine m_qml_app_engine;
    };
}