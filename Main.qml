import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    width: 640
    height: 480
    minimumWidth: width / 2
    minimumHeight: height / 2
    visible: true
    title: qsTr("TestTool")

    // Window -> Layout -> Rectangle -> Layout
    ColumnLayout {
        id: root
        anchors.fill: parent
        spacing: 0

        RowLayout {
            id: toolBar

            Rectangle {
                id: toolRec
                Layout.fillWidth: true
                Layout.preferredHeight: 20
                color: "grey"

                RowLayout {
                    spacing: 1
                    height: parent.height

                    Button {
                        id: fileBtn
                        text: qsTr("file")
                        Layout.fillHeight: true
                    }

                    Button {
                        id: toolBtn
                        text: qsTr("tool")
                        Layout.fillHeight: true
                    }

                    Button {
                        id: viewBtn
                        text: qsTr("view")
                        Layout.fillHeight: true
                    }

                    Button {
                        id: helpBtn
                        text: qsTr("help")
                        Layout.fillHeight: true
                    }
                }
            }
        }

        RowLayout {
            id: sideBar
            spacing: 1

            Rectangle {
                id: sideRec
                color: "green"
                Layout.fillHeight: true
                Layout.preferredWidth: 70
                Layout.maximumWidth: 100

                ColumnLayout {
                    id: toolLayout
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.margins: 6

                    Button {
                        text: qsTr("数据分析")
                        Layout.preferredHeight: 50
                        Layout.preferredWidth: sideRec.width - 12

                        onClicked: pageLoader.source = "page/dataPage.qml"
                    }

                    Button {
                        text: qsTr("波形")
                        Layout.preferredHeight: 50
                        Layout.preferredWidth: sideRec.width - 12

                        onClicked: pageLoader.source = "page/wavePage.qml"
                    }

                    Button {
                        text: qsTr("接受数据")
                        Layout.preferredHeight: 50
                        Layout.preferredWidth: sideRec.width - 12

                        onClicked: pageLoader.source = "page/serialPort.qml"
                    }
                }
            }

            Rectangle {
                id: mainRec
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "lightgrey"

                Loader {
                    id: pageLoader
                    anchors.fill: parent
                }
            }
        }
    }
}
