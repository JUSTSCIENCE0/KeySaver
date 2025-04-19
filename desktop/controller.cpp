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

    static inline bool confirm_action(const QObject* context) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(context->tr("warning"));
        msgBox.setText(context->tr("want_continue"));
        msgBox.addButton(context->tr("yes"), QMessageBox::YesRole);
        msgBox.addButton(context->tr("no"), QMessageBox::NoRole);

        return 0 == msgBox.exec();
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

    Q_INVOKABLE void Controller::onSelectedServiceChanged(const QString& service) {
        if (tr("add_smth") == service) {
            auto root = m_app->m_qml_app_engine.rootObjects().first();
            QMetaObject::invokeMethod(root, "loadAddService");
        }
    }

    Q_INVOKABLE void Controller::onSelectedConfigChanged(const QString& config) {
        if (tr("add_smth") == config) {
            auto root = m_app->m_qml_app_engine.rootObjects().first();
            QMetaObject::invokeMethod(root, "loadAddConfig");
        }
    }

    Q_INVOKABLE void Controller::onShowSetupService(const QString& service) {
        if (service.isEmpty()) {
            QMessageBox::information(nullptr, 
                tr("error"),
                tr("required_field_empty"));
            return;
        }

        m_setup_service = service.toUtf8().constData();

        auto root = m_app->m_qml_app_engine.rootObjects().first();
        QMetaObject::invokeMethod(root, "loadSetupService");
    }

    Q_INVOKABLE void Controller::onAddService(const QString& service_url,
                                              const QString& service_name,
                                              const QString& config) {
        if (service_url.isEmpty() ||
            service_name.isEmpty() ||
            config.isEmpty()) {
            QMessageBox::information(nullptr, 
                tr("error"),
                tr("required_field_empty"));
            return;
        }

        auto service_url_bytes = service_url.toUtf8();
        auto service_name_bytes = service_name.toUtf8();
        auto config_bytes = config.toUtf8();

        KeysaverService new_service = {
            .url = service_url_bytes.constData(),
            .name = service_name_bytes.constData(),
            .conf_id = config_bytes.constData()
        };

        auto code = keysaverAddService(new_service);
        if (is_keysaver_error(code)) {
            ShowError(code);
            return;
        }

        servicesListUpdated();

        auto root = m_app->m_qml_app_engine.rootObjects().first();
        QMetaObject::invokeMethod(root, "closeLayout");
    }

    Q_INVOKABLE void Controller::onDeleteService() {
        assert(m_setup_service.length());

        if (!confirm_action(this))
            return;

        auto code = keysaverDeleteService(m_setup_service.c_str());
        if (is_keysaver_error(code)) {
            ShowError(code);
            return;
        }

        QMessageBox::information(nullptr, 
            tr("success"),
            tr("service_deleted_successfull"));

        m_setup_service = "";

        servicesListUpdated();

        auto root = m_app->m_qml_app_engine.rootObjects().first();
        QMetaObject::invokeMethod(root, "closeLayout");
    }

    Q_INVOKABLE void Controller::onEditService(const QString& service_name,
                                                const QString& config) {
        assert(m_setup_service.length());
        if (service_name.isEmpty() ||
            config.isEmpty()) {
            QMessageBox::information(nullptr, 
                tr("error"),
                tr("required_field_empty"));
            return;
        }

        auto service_name_bytes = service_name.toUtf8();
        auto config_bytes = config.toUtf8();

        KeysaverService edit_service = {
            .url = "stub",
            .name = service_name_bytes.constData(),
            .conf_id = config_bytes.constData()
        };

        if (!confirm_action(this))
            return;

        auto code = keysaverEditService(m_setup_service.c_str(), edit_service);
        if (is_keysaver_error(code)) {
            ShowError(code);
            return;
        }

        QMessageBox::information(nullptr, 
            tr("success"),
            tr("service_updated_successfull"));

        m_setup_service = "";

        servicesListUpdated();

        auto root = m_app->m_qml_app_engine.rootObjects().first();
        QMetaObject::invokeMethod(root, "closeLayout");
    }

    Q_INVOKABLE void Controller::onAddConfig(const QString& config_name,
                                             int pwd_length,
                                             bool use_upper,
                                             bool use_lower,
                                             int alphabet,
                                             bool use_spec,
                                             int spec_count,
                                             const QString& spec_set,
                                             bool use_digits,
                                             int digits_count) {
        assert(pwd_length >= 0);
        assert(spec_count >= 0);
        assert(digits_count >= 0);
        assert(alphabet >= 0);

        if (config_name.isEmpty() ||
            spec_set.isEmpty()) {
            QMessageBox::information(nullptr, 
                tr("error"),
                tr("required_field_empty"));
            return;
        }

        if (!use_upper && !use_lower) {
            QMessageBox::information(nullptr, 
                tr("error"),
                tr("without_any_case"));
            return;
        }

        auto config_name_bytes = config_name.toUtf8();
        auto spec_set_bytes = spec_set.toUtf8();

        KeysaverConfiguration config = {
            .id_name = config_name_bytes.constData(),
            .length = uint(pwd_length),
            .use_upper = use_upper,
            .use_lower = use_lower,
            .alphabet = alphabet,
            .use_special_chars = use_spec,
            .special_chars_count = uint(spec_count),
            .special_charset = spec_set_bytes.constData(),
            .use_digits = use_digits,
            .digits_amount = uint(digits_count)
        };

        auto code = keysaverAddConfiguration(config);
        if (is_keysaver_error(code)) {
            ShowError(code);
            return;
        }

        configsListUpdated();

        auto root = m_app->m_qml_app_engine.rootObjects().first();
        QMetaObject::invokeMethod(root, "closeLayout");
    }

    Q_INVOKABLE QString Controller::generatePassword(const QString& service_name, int hash_id) {
        if (service_name.isEmpty()) {
            QMessageBox::information(nullptr, 
                tr("error"),
                tr("required_field_empty"));
            return tr("error");
        }

        auto service_name_bytes = service_name.toUtf8();
        char result[KEYSAVER_STRING_MAX_SIZE] = "";
        auto code = keysaverGeneratePassword(service_name_bytes.constData(), hash_id, result);
        if (is_keysaver_error(code)) {
            ShowError(code);
            return tr("error");
        }

        return QString::fromUtf8(result);
    }

    void Controller::LoadPasswordGenerator() {
        auto root = m_app->m_qml_app_engine.rootObjects().first();
        QMetaObject::invokeMethod(root, "loadMainLayout");
    }

    QStringList Controller::servicesList() const {
        int serviceCount = 0;
        auto code = keysaverGetServicesCount(&serviceCount);
        if (is_keysaver_error(code)) {
            ShowError(code);
            return { tr("add_smth") };
        }

        if (!serviceCount) {
            return { tr("add_smth") };
        }

        std::vector<char> cStrBuffer(KEYSAVER_STRING_MAX_SIZE * serviceCount);
        std::vector<char*> serviceNamePtrs(serviceCount);
        for (int i = 0; i < serviceCount; ++i) {
            serviceNamePtrs[i] = cStrBuffer.data() + (i * KEYSAVER_STRING_MAX_SIZE);
        }
        code = keysaverGetServicesList(serviceNamePtrs.data());
        if (is_keysaver_error(code)) {
            ShowError(code);
            return { tr("add_smth") };
        }

        QStringList result;
        for (auto service: serviceNamePtrs) {
            result << QString::fromUtf8(service);
        }
        result << tr("add_smth");

        return result;
    }

    QStringList Controller::configsList() const {
        int configsCount = 0;
        auto code = keysaverGetConfigurationsCount(&configsCount);
        if (is_keysaver_error(code)) {
            ShowError(code);
            return { tr("add_smth") };
        }

        if (!configsCount) {
            return { tr("add_smth") };
        }

        std::vector<char> cStrBuffer(KEYSAVER_STRING_MAX_SIZE * configsCount);
        std::vector<char*> configsNamePtrs(configsCount);
        for (int i = 0; i < configsCount; ++i) {
            configsNamePtrs[i] = cStrBuffer.data() + (i * KEYSAVER_STRING_MAX_SIZE);
        }
        code = keysaverGetConfigurationsList(configsNamePtrs.data());
        if (is_keysaver_error(code)) {
            ShowError(code);
            return { tr("add_smth") };
        }

        QStringList result;
        for (auto service: configsNamePtrs) {
            result << QString::fromUtf8(service);
        }
        result << tr("add_smth");

        return result;
    }
}
