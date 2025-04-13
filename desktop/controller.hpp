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
        Q_PROPERTY(QStringList servicesList READ servicesList NOTIFY servicesListUpdated)

    public:
        // ctor & dtor
        explicit Controller(Application* app_callback, QObject* parent = nullptr);
        ~Controller();

        // QT calls
        Q_INVOKABLE void onAppStarted();
        Q_INVOKABLE void onConfirmMasterPassword(const QString& password);
        Q_INVOKABLE void onSelectedServiceChanged(const QString& service);

        // QT options
        QStringList servicesList() const;

    signals:
        void servicesListUpdated();

    private:
        // members
        Application* m_app = nullptr;
        bool m_is_first_usage = false;

        // methods
        static void ShowError(KeysaverStatus code);
        bool ValidateMasterPassword(const QString& password);
        void LoadPasswordGenerator();
    };
}