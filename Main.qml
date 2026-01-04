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

    ColumnLayout {
        id: root
        anchors.fill: parent
        spacing: 0

        Rectangle {
            id: toolRec
            Layout.fillWidth: true
            Layout.preferredHeight: 30
            color: "grey"

            RowLayout {
                spacing: 1
                // anchors.fill: parent
                height: parent.height
                Layout.alignment: Qt.AlignLeft

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

        RowLayout {
            id: sideBar
            Layout.alignment: Qt.AlignLeft

            Rectangle {
                id: sideRec
                Layout.preferredWidth: 40
                Layout.fillHeight: true
                color: "green"
            }
        }
    }
}
