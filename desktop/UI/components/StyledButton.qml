import QtQuick
import QtQuick.Controls

Button {
    id: styledButton
    property color backgroundColor : "black"
    property color borderColor : "black"
    property color textColor : "white"

    background: Rectangle {
        color: styledButton.backgroundColor
        radius: 10
        border.color: styledButton.borderColor
        border.width: 4
    }

    contentItem: Text {
        id: styledButtonText
        font.bold: true
        color: styledButton.textColor
        text: styledButton.text
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        anchors.fill: parent
        font.weight: Font.Bold
        font.pointSize: 18
        font.family: "Arial"
        font.capitalization: Font.AllUppercase
    }

    spacing: 7
    display: AbstractButton.TextOnly

    SequentialAnimation {
        id: onPressedAnimation
        PropertyAnimation { target: styledButton; property: "opacity"; to: 0.7; duration: 0 }
    }

    SequentialAnimation {
        id: onReleasedAnimation
        PropertyAnimation { target: styledButton; property: "opacity"; to: 1.0; duration: 0 }
    }

    onPressed: onPressedAnimation.start()
    onReleased: onReleasedAnimation.start()
    onCanceled: onReleasedAnimation.start()
}
