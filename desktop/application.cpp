// Copyright (c) 2025, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include "application.hpp"
#include "controller.hpp"

namespace KeysaverDesktop {
    Application::Application(int argc, char *argv[]) :
        m_gui_app(argc, argv) {}

    int Application::Run(int argc, char *argv[]) {
        std::filesystem::path app_path{argv[0]};
        std::string app_dir = app_path.parent_path();

        try
        {
            Application app(argc, argv);

            Controller controller(&app);
            app.m_qml_app_engine.rootContext()->setContextProperty("Controller", &controller);

            std::string locale_file = app_dir + "/en_US.qm";
            QTranslator translator;
            if (!translator.load(locale_file.c_str())) {
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