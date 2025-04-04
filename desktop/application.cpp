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
        Application app(argc, argv);

        Controller controller(&app);
        app.m_qml_app_engine.rootContext()->setContextProperty("Controller", &controller);

        const QUrl url(QString::fromUtf8(app.APP_RESOURCE_ID));
        QObject::connect(&app.m_qml_app_engine, &QQmlApplicationEngine::objectCreationFailed,
                         &app.m_gui_app, []() { QCoreApplication::exit(-1); },
                         Qt::QueuedConnection);
        app.m_qml_app_engine.load(url);

        if (app.m_qml_app_engine.rootObjects().isEmpty())
            return -1;

        return app.m_gui_app.exec();
    }
}