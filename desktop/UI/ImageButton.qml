import QtQuick
import QtQuick.Controls

Image {
    id: imageButton
    width: 40
    height: 40
    fillMode: Image.PreserveAspectFit

    property var onButtonClick: function() {}

    SequentialAnimation {
        id: onPressedAnimation
        PropertyAnimation { target: imageButton; property: "opacity"; to: 0.7; duration: 0 }
    }

    SequentialAnimation {
        id: onReleasedAnimation
        PropertyAnimation { target: imageButton; property: "opacity"; to: 1.0; duration: 0 }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: imageButton.onButtonClick()

        onPressed: onPressedAnimation.start()
        onReleased: onReleasedAnimation.start()
        onExited: onReleasedAnimation.start()
    }
}
