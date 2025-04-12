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

    void Controller::ShowError(KeysaverStatus code) {
        switch (code)
        {
        case KeysaverStatus::E_INVALID_MASTER_PASSWORD:
            QMessageBox::information(nullptr, 
                tr("error"),
                tr("invalid_master_password"));
            break;
        case KeysaverStatus::E_TOO_SHORT_MASTER_PASSWORD:
            QMessageBox::information(nullptr, 
                tr("error"),
                tr("short_master_password"));
            break;
        case KeysaverStatus::E_SERVICE_ALREADY_EXISTS:
            QMessageBox::information(nullptr, 
                tr("error"),
                tr("service_already_exists"));
            break;
        case KeysaverStatus::E_SERVICE_URL_ALREADY_EXISTS:
            QMessageBox::information(nullptr, 
                tr("error"),
                tr("service_url_already_exists"));
            break;
        case KeysaverStatus::E_CONFIG_ALREADY_EXISTS:
            QMessageBox::information(nullptr, 
                tr("error"),
                tr("config_already_exists"));
            break;
        case KeysaverStatus::E_INVALID_PASSWORD_LENGTH:
            QMessageBox::information(nullptr, 
                tr("error"),
                tr("invalid_password_length"));
            break;
        case KeysaverStatus::E_INVALID_SPECIAL_CHAR_COUNT:
            QMessageBox::information(nullptr, 
                tr("error"),
                tr("invalid_special_chars_count"));
            break;
        case KeysaverStatus::E_INVALID_DIGITS_AMOUNT:
            QMessageBox::information(nullptr, 
                tr("error"),
                tr("invalid_digits_amount"));
            break;
        case KeysaverStatus::E_WITHOUT_ANY_CASE:
            QMessageBox::information(nullptr, 
                tr("error"),
                tr("without_any_case"));
            break;
        default:
            QMessageBox::information(nullptr, 
                tr("error"),
                tr("internal_error") + std::to_string(int(code)).c_str());
            break;
        }
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
            QMessageBox::information(nullptr, tr("error"),
                tr("master_password_uppercase_letter"));
            return false;
        }
        if (!has_lower) {
            QMessageBox::information(nullptr, tr("error"),
                tr("master_password_lowercase_letter"));
            return false;
        }
        if (!has_spec) {
            QMessageBox::information(nullptr, tr("error"),
                tr("master_password_special_char"));
            return false;
        }
        if (!has_digit) {
            QMessageBox::information(nullptr, tr("error"),
                tr("master_password_digit"));
            return false;
        }

        return true;
    }

    Q_INVOKABLE void Controller::onConfirmMasterPassword(const QString& password) {
        if (!ValidateMasterPassword(password))
            return;

        auto pwd_utf8_bytes = password.toUtf8();
        auto result = keysaverSetMasterPassword(pwd_utf8_bytes.constData());
        if (is_keysaver_error(result)) {
            ShowError(result);
            return;
        }

        LoadPasswordGenerator();
    }

    Q_INVOKABLE void Controller::onAppStarted() {
        if (m_is_first_usage) {
            QMessageBox::information(nullptr, 
                tr("welcome"),
                tr("welcome_message"));
        }
    }

    void Controller::LoadPasswordGenerator() {
        auto root = m_app->m_qml_app_engine.rootObjects().first();
        QMetaObject::invokeMethod(root, "loadMainLayout");
    }
}
