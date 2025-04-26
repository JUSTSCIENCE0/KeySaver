// Copyright (c) 2025, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include "application.hpp"
#include "controller.hpp"

#include <QQuickStyle>

namespace KeysaverDesktop {
    static const QString QT_SERVER_NAME = "KeysaverDesktopQtInstance";

    Application::Application(int argc, char *argv[]) :
        m_gui_app(argc, argv) {}

    std::string Application::GetPrefferedLocale() {
        auto current_locale = QLocale::system().name();

        const std::string RUSSIAN_LOCALE = "ru_RU";
        if (current_locale == RUSSIAN_LOCALE.c_str()) {
            return RUSSIAN_LOCALE;
        }

        return DEFAULT_LOCALE;
    }

    bool Application::IsAlreadyRunning() {
        QLocalSocket socket;
        socket.connectToServer(QT_SERVER_NAME);
        if (socket.waitForConnected(100)) {
            return true;
        }
    
        QLocalServer::removeServer(QT_SERVER_NAME);
        return false;
    }

    int Application::Run(int argc, char *argv[]) {
        std::filesystem::path app_path{argv[0]};

        try
        {
            if (IsAlreadyRunning()) {
                qDebug() << "Instance already exists";
                return 0;
            }

            QLocalServer server;
            if (!server.listen(QT_SERVER_NAME)) {
                qDebug() << "Failed to create QLocalServer";
                return 1;
            }

            QQuickStyle::setStyle("Basic");
            Application app(argc, argv);

            Controller controller(&app);
            app.m_qml_app_engine.rootContext()->setContextProperty("Controller", &controller);

            auto locale_file = QCoreApplication::applicationDirPath();
            locale_file.append("/");
            locale_file.append(GetPrefferedLocale() + ".qm");
            QTranslator translator;
            if (!translator.load(locale_file)) {
                qDebug() << "Can't load translation file";
            }
            app.m_gui_app.installTranslator(&translator);

            const QUrl url(QString::fromUtf8(app.APP_RESOURCE_ID));
            QObject::connect(&app.m_qml_app_engine, &QQmlApplicationEngine::objectCreationFailed,
                             &app.m_gui_app, []() { QCoreApplication::exit(-1); },
                             Qt::QueuedConnection);
            app.m_qml_app_engine.load(url);
    
            if (app.m_qml_app_engine.rootObjects().isEmpty())
                return -1;

            return app.m_gui_app.exec();
        }
        catch(KeysaverStatus code) {
            qDebug() << "Error code: " << int(code);
            return -1;
        }
        catch(...) {
            qDebug() << "Unexpected error";
            return -1;
        }

        return 0;
    }
}