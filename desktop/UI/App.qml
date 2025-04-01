import QtQuick
import QtQuick.Controls

Window {
    width: 500
    height: 1000

    minimumWidth: 450
    minimumHeight: 800

    visible: true
    title: "KeysaverGUI"

    Loader {
        id: loader
        anchors.fill: parent
        source: "qrc:/UI/layouts/EnterMasterPassword.qml"
    }
}

