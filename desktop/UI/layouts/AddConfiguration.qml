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
        text: qsTr("configuration_name")

        anchors.left: parent.left
        anchors.bottom: configurationName.top
        anchors.leftMargin: 16
        anchors.bottomMargin: 8
    }

    StyledInput {
        id: configurationName
        placeholderText: "Name"

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: passwordLengthLabel.top
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.bottomMargin: 24
    }

    StyledLabel {
        id: passwordLengthLabel
        text: qsTr("password_length")

        anchors.left: parent.left
        anchors.bottom: passwordLength.top
        anchors.leftMargin: 16
        anchors.bottomMargin: 8
    }

    StyledInput {
        id: passwordLength
        placeholderText: "16"
        maxLength: 2
        validaotor: IntValidator { bottom: 8; top: 30 }

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: characterSet.top
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.bottomMargin: 24
    }

    StyledLabel {
        id: characterSet
        text: qsTr("character_set")

        anchors.left: parent.left
        anchors.bottom: useUpperCase.top
        anchors.leftMargin: 16
        anchors.bottomMargin: 8
    }

    StyledSwitch {
        id: useUpperCase
        text: qsTr("use_upper_case")

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: useLowerCase.top
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.bottomMargin: 8
    }

    StyledSwitch {
        id: useLowerCase
        text: qsTr("use_lower_case")

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: alphabet.top
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.bottomMargin: 8
    }

    StyledLabel {
        id: alphabet
        text: qsTr("alphabet")

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
        text: qsTr("use_special_characters")

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: specialCharactersCountLabel.top
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.bottomMargin: 8
    }

    StyledLabel {
        id: specialCharactersCountLabel
        text: qsTr("special_characters_count")

        anchors.left: parent.left
        anchors.bottom: specialCharactersCount.top
        anchors.leftMargin: 16
        anchors.bottomMargin: 8
    }

    StyledInput {
        id: specialCharactersCount
        placeholderText: "2"
        maxLength: 1
        validaotor: IntValidator { bottom: 1; top: 8 }

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: specialCharactersSetLabel.top
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.bottomMargin: 8
    }

    StyledLabel {
        id: specialCharactersSetLabel
        text: qsTr("special_characters_set")

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
        text: qsTr("use_digits")

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: digitsCountLabel.top
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.bottomMargin: 8
    }

    StyledLabel {
        id: digitsCountLabel
        text: qsTr("digits_count")

        anchors.left: parent.left
        anchors.bottom: digitsCount.top
        anchors.leftMargin: 16
        anchors.bottomMargin: 8
    }

    StyledInput {
        id: digitsCount
        placeholderText: "2"
        maxLength: 1
        validaotor: IntValidator { bottom: 1; top: 8 }

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
            text: qsTr("cancel")
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
            text: qsTr("confirm")

            anchors.right: parent.right
            anchors.rightMargin: 0
            width: parent.width / 2 - 4
        }
    }
}
