import QtQuick
import QtQuick.Controls
import "qrc:/UI/components"

Window {
    width: 500
    height: 1000

    minimumWidth: 450
    minimumHeight: 800

    visible: true
    title: "KeysaverGUI"

    Component.onCompleted: Controller.onAppStarted()

    Loader {
        id: loader
        anchors.fill: parent
        source: "qrc:/UI/layouts/EnterMasterPassword.qml"
    }

    function loadMainLayout() {
        loader.source = "qrc:/UI/layouts/GetPassword.qml";
    }
}

