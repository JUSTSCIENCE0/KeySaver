import QtQuick
import QtQuick.Controls

Rectangle {
    id: styledInput
    width: 400
    height: 40
    radius: 10
    border.color: "#e0e0e0"
    border.width: 4

    property int mode: TextInput.Normal
    property int maxLength: 30
    property bool readOnly: false
    property alias text: styledTextInput.text

    TextInput {
        id: styledTextInput
        text: ""
        anchors.fill: parent
        font.letterSpacing: 0.9
        font.pixelSize: 24
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        font.italic: false
        font.weight: Font.Normal
        rightPadding: 8
        leftPadding: 8
        font.family: "Arial"
        maximumLength: styledInput.maxLength
        echoMode: styledInput.mode
        readOnly: styledInput.readOnly
    }
}
