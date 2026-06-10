import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

ApplicationWindow {
    visible: true
    width: 900
    height: 600
    title: qsTr("PlayArchive")

    color: "#121212"

    FileDialog {
        id: openDialog
        title: "Open Archive"
        onAccepted: archiveModel.openArchive(selectedFile.toString().replace("file:///", ""))
    }

    FolderDialog {
        id: extractDialog
        title: "Select Extraction Folder"
        onAccepted: archiveModel.extractArchive(currentArchivePath, selectedFolder.toString().replace("file:///", ""))
    }

    property string currentArchivePath: ""

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // Toolbar
        Rectangle {
            Layout.fillWidth: true
            height: 50
            color: "#1f1f1f"
            
            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 10
                anchors.rightMargin: 10
                spacing: 15

                Button {
                    text: "Open"
                    onClicked: openDialog.open()
                }

                Button {
                    text: "Extract"
                    enabled: archiveModel.rowCount() > 0
                    onClicked: extractDialog.open()
                }

                Button {
                    text: "Settings"
                }

                Item { Layout.fillWidth: true }

                Label {
                    text: "PlayArchive .gcf"
                    color: "#00ff88"
                    font.bold: true
                    font.pixelSize: 18
                }
            }
        }

        // Main Content
        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            model: archiveModel
            delegate: Rectangle {
                width: parent.width
                height: 40
                color: index % 2 == 0 ? "#1a1a1a" : "#121212"
                
                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 15
                    anchors.rightMargin: 15
                    
                    Label {
                        text: model.path
                        color: "white"
                        Layout.fillWidth: true
                    }
                    Label {
                        text: (model.size / 1024).toFixed(2) + " KB"
                        color: "#888"
                        Layout.preferredWidth: 100
                    }
                }
            }
        }

        // Status Bar
        Rectangle {
            Layout.fillWidth: true
            height: 30
            color: "#1f1f1f"
            
            Label {
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                text: archiveModel.status
                color: "#ccc"
                font.pixelSize: 12
            }

            ProgressBar {
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                width: 200
                value: archiveModel.progress
            }
        }
    }

    DropArea {
        anchors.fill: parent
        onDropped: (drop) => {
            if (drop.hasUrls) {
                let url = drop.urls[0].toString().replace("file:///", "")
                archiveModel.openArchive(url)
            }
        }
    }
}
