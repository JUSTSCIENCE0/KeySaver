// Copyright (c) 2025, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include "controller.hpp"

#include <QDebug>

namespace KeysaverDesktop {
    Controller::Controller(Application* app_callback, QObject* parent) :
            QObject(parent),
            m_app(app_callback) {}

    Q_INVOKABLE void Controller::onConfirmMasterPassword(const QString& password) {
        qDebug() << password;
    }
}
