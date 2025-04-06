// Copyright (c) 2025, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include "controller.hpp"

#include <QDebug>
#include <QMessageBox>

#include <filesystem>

#ifdef __linux__
#include  <unistd.h>
#include  <pwd.h>
#endif

namespace KeysaverDesktop {
    static inline std::filesystem::path get_config_path() {
        std::filesystem::path result;
#ifdef __linux__
        struct passwd* pwd = getpwuid(getuid());
        if (pwd && pwd->pw_dir) {
            result = std::filesystem::path(pwd->pw_dir);
        }
#endif
        if (result.empty()) {
            result = std::filesystem::current_path();
        }

        result.append(".config/keysaver");
        if (!std::filesystem::exists(result))
            std::filesystem::create_directories(result);

        return result;
    }

    Controller::Controller(Application* app_callback, QObject* parent) :
            QObject(parent),
            m_app(app_callback) {
        auto config_path = get_config_path();
        auto result = keysaverInit(config_path.c_str());
        if (is_keysaver_error(result))
            throw result;

        if (KeysaverStatus::M_DATABASE_NOT_FOUND == result) {
            m_is_first_usage = true;
        }
    }

    Controller::~Controller() {
        keysaverSyncDatabase();
        keysaverClose();
    }

    Q_INVOKABLE void Controller::onConfirmMasterPassword(const QString& password) {
        qDebug() << password;
    }

    Q_INVOKABLE void Controller::onAppStarted() {
        if (m_is_first_usage) {
            QMessageBox::information(nullptr, "Attention!", "First usage");
        }
    }
}
