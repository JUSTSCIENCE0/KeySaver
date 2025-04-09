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

    bool Controller::ValidateMasterPassword(const QString& password) {
        bool has_upper = false, has_lower = false,
             has_spec = false, has_digit = false;

        static const auto SPEC_SET = QString::fromUtf8(Application::DEFAULT_SPEC_SYMBOLS_SET);

        for (auto symbol : password) {
            if (symbol.isUpper()) {
                has_upper = true;
                continue;
            }
            if (symbol.isLower()) {
                has_lower = true;
                continue;
            }
            if (symbol.isDigit()) {
                has_digit = true;
                continue;
            }
            if (SPEC_SET.contains(symbol)) {
                has_spec = true;
                continue;
            }
        }

        if (!has_upper) {
            QMessageBox::information(nullptr, "Error!", "No upper");
            return false;
        }
        if (!has_lower) {
            QMessageBox::information(nullptr, "Error!", "No lower");
            return false;
        }
        if (!has_spec) {
            QMessageBox::information(nullptr, "Error!", "No special");
            return false;
        }
        if (!has_digit) {
            QMessageBox::information(nullptr, "Error!", "No digit");
            return false;
        }

        return true;
    }

    Q_INVOKABLE void Controller::onConfirmMasterPassword(const QString& password) {
        if (!ValidateMasterPassword(password))
            return;

        qDebug() << password;
    }

    Q_INVOKABLE void Controller::onAppStarted() {
        if (m_is_first_usage) {
            QMessageBox::information(nullptr, "Attention!", "First usage");
        }
    }
}
