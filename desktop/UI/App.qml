import QtQuick
import QtQuick.Controls
import KeysaverGUI
import QtQuick.Studio.DesignEffects

Window {
    width: 500
    height: 1000

    minimumWidth: 450
    minimumHeight: 750

    visible: true
    title: "KeysaverGUI"

    Loader {
        id: loader
        anchors.fill: parent
        source: "EnterMasterPassword.qml"  // Загружаем стартовый экран
    }
}

