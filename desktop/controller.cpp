// Copyright (c) 2025, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include "controller.hpp"

#include <QDebug>

namespace KeysaverDesktop {
    Controller::Controller(QObject* parent) :
            QObject(parent) {}

    Q_INVOKABLE void Controller::onConfirmMasterPassword() {
        qDebug() << "Not implemented";
    }
}
