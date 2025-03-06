import QtQuick
import QtQuick.Controls
import KeysaverGUI
import QtQuick.Studio.DesignEffects

Rectangle {
    id: rectangle
    width: Window.width
    height: Window.height
    opacity: 1

    GridView {
        id: hash_images
        height: 399
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        model: ListModel {
            ListElement { imagePath: "cat.jpg" }
            ListElement { imagePath: "car.jpg" }
            ListElement { imagePath: "egg.jpg" }
            ListElement { imagePath: "city.jpg" }
            ListElement { imagePath: "bot.jpg" }
            ListElement { imagePath: "girl.jpg" }
            ListElement { imagePath: "phone.jpg" }
            ListElement { imagePath: "trees.jpg" }
            ListElement { imagePath: "plane.jpg" }
        }
        delegate: Item {
            width: hash_images.cellWidth
            height: hash_images.cellHeight

            Image {
                width: 125
                height: 125
                sourceSize.width: 200
                sourceSize.height: 200
                source: imagePath
                fillMode: Image.PreserveAspectFit
                anchors.centerIn: parent
            }
        }
        cellWidth: width / 3
        cellHeight: 133
    }
}
