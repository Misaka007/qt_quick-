import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.12
import an.OpenGLItem 1.0
Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")
    OpenGLItem{
        id:op
        visible: true
        anchors.fill: parent

    }
//    Button{}

}
