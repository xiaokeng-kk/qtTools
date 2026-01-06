import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import qtTools 1.0

Item {

    RowLayout {

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#ffffff"

            RowLayout {
                anchors.fill: parent
                spacing: 10

                // ===== 左侧：串口控制区 =====
                ColumnLayout {
                    Layout.preferredWidth: 180
                    spacing: 8

                    ComboBox {
                        id: portCombo
                        model: serial.availablePorts()
                    }

                    Button {
                        id: connectBtn
                        text: serial.isOpen ? "断开串口" : "连接串口"

                        onClicked: {
                            if (serial.isOpen) {
                                serial.close()
                            } else {
                                serial.open("COM3", 1500000)
                            }
                        }
                    }

                    Label {
                        text: serial.isOpen ? "状态：已连接" : "状态：未连接"
                        color: serial.isOpen ? "green" : "red"
                    }
                }

                // ===== 右侧：接收数据显示 =====
                TextArea {
                    id: rxText
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    readOnly: true
                    wrapMode: TextArea.Wrap
                    placeholderText: "串口接收数据..."
                }

                // ===== 接收数据 =====
                Connections {
                    target: serial
                    function onDataReceived(data) {
                        rxText.text += data
                        rxText.cursorPosition = rxText.length
                    }
                }
            }
        }
    }
}
