// Copyright (c) 2025, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include "application.hpp"

#include <QObject>
#include <QDebug>

namespace KeysaverDesktop {
    class Controller : public QObject {
        Q_OBJECT

    public:
        explicit Controller(Application* app_callback, QObject* parent = nullptr);

        Q_INVOKABLE void onConfirmMasterPassword(const QString& password);

        Application* m_app = nullptr;
    };
}