import QtQuick
import QtQuick.Controls

Rectangle {
    id: addConfiguration
    width: Window.width
    height: Window.height
    opacity: 1

    HelpButton {
        id: help
    }

    StyledLabel {
        id: configurationNameLabel
        text: qsTr("Configuration name")

        anchors.left: parent.left
        anchors.bottom: configurationName.top
        anchors.leftMargin: 16
        anchors.bottomMargin: 8
    }

    StyledInput {
        id: configurationName

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: passwordLengthLabel.top
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.bottomMargin: 24
    }

    StyledLabel {
        id: passwordLengthLabel
        text: qsTr("Password length")

        anchors.left: parent.left
        anchors.bottom: passwordLength.top
        anchors.leftMargin: 16
        anchors.bottomMargin: 8
    }

    StyledInput {
        id: passwordLength
        maxLength: 2

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: characterSet.top
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.bottomMargin: 24
    }

    StyledLabel {
        id: characterSet
        text: qsTr("Character set")

        anchors.left: parent.left
        anchors.bottom: useUpperCase.top
        anchors.leftMargin: 16
        anchors.bottomMargin: 8
    }

    StyledSwitch {
        id: useUpperCase
        text: qsTr("Use upper case")

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: useLowerCase.top
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.bottomMargin: 8
    }

    StyledSwitch {
        id: useLowerCase
        text: qsTr("Use lower case")

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: alphabet.top
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.bottomMargin: 8
    }

    StyledLabel {
        id: alphabet
        text: qsTr("Alphabet")

        anchors.left: parent.left
        anchors.bottom: selectedAlphabet.top
        anchors.leftMargin: 16
        anchors.bottomMargin: 8
    }

    StyledComboBox {
        id: selectedAlphabet
        model: ["Latin (English, US)", "Кириллица (Русский, Россия)"]

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: addSpecialCharacters.top
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.bottomMargin: 8
    }

    StyledSwitch {
        id: addSpecialCharacters
        text: qsTr("Add special characters")

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: specialCharactersCountLabel.top
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.bottomMargin: 8
    }

    StyledLabel {
        id: specialCharactersCountLabel
        text: qsTr("Special characters count")

        anchors.left: parent.left
        anchors.bottom: specialCharactersCount.top
        anchors.leftMargin: 16
        anchors.bottomMargin: 8
    }

    StyledInput {
        id: specialCharactersCount
        maxLength: 1

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: specialCharactersSetLabel.top
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.bottomMargin: 8
    }

    StyledLabel {
        id: specialCharactersSetLabel
        text: qsTr("Special characters set")

        anchors.left: parent.left
        anchors.bottom: specialCharactersSet.top
        anchors.leftMargin: 16
        anchors.bottomMargin: 8
    }

    StyledInput {
        id: specialCharactersSet
        text: "!@#$%^&*()_-+=/?.,<>'\";:[]{}"

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: addDigits.top
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.bottomMargin: 8
    }

    StyledSwitch {
        id: addDigits
        text: qsTr("Add digits")

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: digitsCountLabel.top
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.bottomMargin: 8
    }

    StyledLabel {
        id: digitsCountLabel
        text: qsTr("Digits count")

        anchors.left: parent.left
        anchors.bottom: digitsCount.top
        anchors.leftMargin: 16
        anchors.bottomMargin: 8
    }

    StyledInput {
        id: digitsCount
        maxLength: 1

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: cancelConfirm.top
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.bottomMargin: 24
    }

    Row {
        id: cancelConfirm
        height: 40
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.bottomMargin: 8

        StyledButton {
            id: cancelSetup
            text: qsTr("Cancel")
            backgroundColor: "white"
            borderColor: "#e0e0e0"
            textColor: "black"

            anchors.left: parent.left
            anchors.leftMargin: 0
            width: parent.width / 2 - 4

            onClicked: loader.source = "GetPassword.qml"
        }

        StyledButton {
            id: confirmSetup
            text: qsTr("Confirm")

            anchors.right: parent.right
            anchors.rightMargin: 0
            width: parent.width / 2 - 4
        }
    }
}
