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
    property alias placeholderText: placeholder.text

    TextInput {
        id: styledTextInput
        text: ""
        anchors.fill: parent
        font.letterSpacing: 0.9
        font.pixelSize: 18
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

        onTextChanged: placeholder.visible = text.length === 0
    }

    Text {
        id: placeholder
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 8
        color: "#999999"
        font.pixelSize: 18
        font.family: "Arial"
        visible: input.text.length === 0
    }
}
