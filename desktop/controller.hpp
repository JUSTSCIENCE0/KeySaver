// Copyright (c) 2025, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include <QObject>
#include <QDebug>

namespace KeysaverDesktop {
    class Controller : public QObject {
        Q_OBJECT

    public:
        explicit Controller(QObject* parent = nullptr);

        Q_INVOKABLE void onConfirmMasterPassword();
    };
}