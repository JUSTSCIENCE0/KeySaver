// Copyright (c) 2025, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include "application.hpp"
#include <keysaver/interface.h>
#include <QObject>
#include <QDebug>

namespace KeysaverDesktop {
    class Controller final : public QObject {
        Q_OBJECT

    public:
        // ctor & dtor
        explicit Controller(Application* app_callback, QObject* parent = nullptr);
        ~Controller();

        // QT
        Q_INVOKABLE void onAppStarted();
        Q_INVOKABLE void onConfirmMasterPassword(const QString& password);

    private:
        // members
        Application* m_app = nullptr;
        bool m_is_first_usage = false;

        // methods
        bool ValidateMasterPassword(const QString& password);
    };
}