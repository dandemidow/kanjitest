import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Rectangle {
        id: main
        anchors.fill: parent
        property string background: kanji.state===0?"white":"red";
        opacity: 0.9
        color: background
        border.width: 5
        border.color: "black"
        Column
        {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 5

            Rectangle
            {
                color: "transparent"
                height: 100
                width: main.width
                Text {
                    anchors.centerIn: parent
                    font.pixelSize: 50
                    text: kanji.item
                }

            }

            Rectangle
            {
                id: meaning
                height: 25
                width: main.width
                color: "transparent"
                Text {
                    anchors.centerIn: parent
                    font.pixelSize: 10
                    text: kanji.mean
                }
            }

            Rectangle
            {
                id: levels
                width: main.width
                height: 15
                color: "transparent"
                ListView
                {
                    anchors.centerIn: parent
                    id: levd
                    width: parent.height*model.length
                    height: parent.height
                    model: kanji.groups
                    orientation: ListView.Horizontal
                    delegate:
                        Component{
                        Item{
                            height: levels.height
                            width: levels.height
                            property bool selected: kanji.isEnable(index)
                            Rectangle
                            {
                                border.color: selected?"black":"transparent"
                                anchors.fill: parent
                                radius: 5
                                color: "transparent"
                                Text { id: butText; anchors.centerIn: parent; text: modelData }
                                MouseArea
                                {
                                    hoverEnabled: true
                                    anchors.fill: parent
                                    onClicked:
                                    {
                                        selected=selected?false:true;
                                        kanji.setEnable(index, selected);
                                    }
                                }
                            }
                        }
                    }
                    interactive: false
                    highlightFollowsCurrentItem: false
                    focus: true
                    clip: false
                }
            }

            Rectangle
            {
                color: "transparent"
                width: main.width
                height: 150
                ListView
                {
                    anchors.fill: parent
                    id: con
                    width: 500
                    height: 150
                    anchors.bottomMargin: 20
                    model: kanji.readingVariant
                    delegate:
                        Component{
                        Item{
                            anchors.horizontalCenter: parent.horizontalCenter
                            height: 25
                            width: 250
                            Rectangle
                            {
                                border.color: "black"
                                anchors.fill: parent
                                color: "transparent"
                                Text { id: butText; anchors.centerIn: parent; text: modelData }
                                MouseArea
                                {
                                    hoverEnabled: true
                                    anchors.fill: parent
                                    onClicked:
                                    {
                                        kanji.choose(index);
                                    }
                                    //onEntered: ;//butText.scale = 1.5
                                    //onExited: ;//butText.scale = 1
                                }
                            }
                        }
                    }
                    interactive: false
                    highlightFollowsCurrentItem: false
                    focus: true
                    clip: false
                }
            }
        }
    }
}
