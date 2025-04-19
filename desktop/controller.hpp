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
        Q_PROPERTY(QStringList configsList READ configsList NOTIFY configsListUpdated)

    public:
        // ctor & dtor
        explicit Controller(Application* app_callback, QObject* parent = nullptr);
        ~Controller();

        // QT calls
        Q_INVOKABLE void onAppStarted();
        Q_INVOKABLE void onConfirmMasterPassword(const QString& password);
        Q_INVOKABLE void onSelectedServiceChanged(const QString& service);
        Q_INVOKABLE void onSelectedConfigChanged(const QString& config);
        Q_INVOKABLE void onShowSetupService(const QString& service);
        Q_INVOKABLE void onAddService(const QString& service_url,
                                      const QString& service_name,
                                      const QString& config);
        Q_INVOKABLE void onDeleteService();
        Q_INVOKABLE QString generatePassword(const QString& service_name, int hash_id);
        Q_INVOKABLE QString getSetupService() const {
            assert(m_setup_service.length());
            return QString::fromUtf8(m_setup_service.c_str());
        }

        // QT properties
        QStringList servicesList() const;
        QStringList configsList() const;

    signals:
        void servicesListUpdated();
        void configsListUpdated();

    private:
        // members
        Application* m_app = nullptr;
        bool m_is_first_usage = false;
        std::string m_setup_service = "";

        // methods
        static void ShowError(KeysaverStatus code);
        bool ValidateMasterPassword(const QString& password);
        void LoadPasswordGenerator();
    };
}