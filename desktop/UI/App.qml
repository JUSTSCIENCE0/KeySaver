import QtQuick
import QtQuick.Controls
import KeysaverGUI
import QtQuick.Studio.DesignEffects

Window {
    width: 500
    height: 1000

    minimumWidth: 450
    minimumHeight: 300

    visible: true
    title: "KeysaverGUI"

    Loader {
        id: loader
        anchors.fill: parent
        source: "EnterMasterPassword.qml"  // Загружаем стартовый экран
    }
}

