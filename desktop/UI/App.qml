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

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: "qrc:/UI/layouts/EnterMasterPassword.qml"
    }

    function loadMainLayout() {
        stackView.replace("qrc:/UI/layouts/GetPassword.qml");
    }

    function loadAddService() {
        Qt.callLater(() => {
            stackView.push("qrc:/UI/layouts/AddService.qml");
        })
    }

    function loadAddConfig() {
        stackView.push("qrc:/UI/layouts/AddConfiguration.qml");
    }

    function loadSetupService() {
        stackView.push("qrc:/UI/layouts/SetupService.qml");
    }

    function closeLayout() {
        stackView.pop()
    }
}

