import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// ============================================================
// MetricCard — reusable gauge card component
//
// Demonstrates:
//   - Custom QML component (reusable)
//   - Canvas API for drawing arcs
//   - Behavior { NumberAnimation } for smooth value changes
//   - Property binding with color logic
// ============================================================
Rectangle {
    id: card

    // Public API — set by parent
    property string title:    "Metric"
    property double value:    0.0
    property string unit:     ""
    property double maxValue: 100.0
    property color  accentColor:    "#1565c0"
    property string icon:     "📊"

    // Internal
    property double _displayValue: value   // animated copy

    radius: 12
    color:  "white"
    border.color: Qt.lighter(card.accentColor, 1.8)
    border.width: 2

    // Drop shadow effect

    // Smooth value animation — key QML concept
    Behavior on _displayValue {
        NumberAnimation {
            duration: 400
            easing.type: Easing.OutCubic
        }
    }

    // Update display value when real value changes
    onValueChanged: _displayValue = value

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 8

        // Title row
        RowLayout {
            Layout.fillWidth: true

            Text {
                text: card.icon
                font.pixelSize: 20
            }

            Label {
                text: card.title
                font.pixelSize: 14
                font.bold: true
                color: "#333"
                Layout.fillWidth: true
            }
        }

        // Canvas gauge arc
        Canvas {
            id: gauge
            Layout.fillWidth: true
            Layout.preferredHeight: width * 0.55
            Layout.alignment: Qt.AlignHCenter

            // Redraw when value or color changes
            onPaint: {
                var ctx = getContext("2d");
                ctx.clearRect(0, 0, width, height);

                var cx = width / 2;
                var cy = height * 0.9;
                var r  = Math.min(width, height * 1.8) * 0.42;

                var startAngle = Math.PI;
                var endAngle   = 2 * Math.PI;
                var progress   = card._displayValue / card.maxValue;
                var fillAngle  = startAngle + progress * Math.PI;

                // Background arc
                ctx.beginPath();
                ctx.arc(cx, cy, r, startAngle, endAngle);
                ctx.strokeStyle = "#e0e0e0";
                ctx.lineWidth   = 14;
                ctx.lineCap     = "round";
                ctx.stroke();

                // Value arc
                if (progress > 0) {
                    ctx.beginPath();
                    ctx.arc(cx, cy, r, startAngle, fillAngle);
                    ctx.strokeStyle = card.accentColor;
                    ctx.lineWidth   = 14;
                    ctx.lineCap     = "round";
                    ctx.stroke();
                }
            }

            // Repaint when these change
            Connections {
                target: card
                function onColorChanged()         { gauge.requestPaint(); }
                function on_DisplayValueChanged() { gauge.requestPaint(); }
            }
        }

        // Value text
        Text {
            Layout.alignment: Qt.AlignHCenter
            text: card._displayValue.toFixed(1) + " " + card.unit
            font.pixelSize: 26
            font.bold: true
            color: card.accentColor

            // Color transition animation
            Behavior on color {
                ColorAnimation { duration: 500 }
            }
        }

        // Mini progress bar
        Rectangle {
            Layout.fillWidth: true
            height: 6
            radius: 3
            color: "#e0e0e0"

            Rectangle {
                width: parent.width * Math.min(card._displayValue / card.maxValue, 1.0)
                height: parent.height
                radius: parent.radius
                color: card.accentColor

                Behavior on width {
                    NumberAnimation { duration: 400; easing.type: Easing.OutCubic }
                }

                Behavior on color {
                    ColorAnimation { duration: 500 }
                }
            }
        }

        // Min / Max labels
        RowLayout {
            Layout.fillWidth: true
            Label { text: "0"; font.pixelSize: 10; color: "#aaa" }
            Item  { Layout.fillWidth: true }
            Label {
                text: card.maxValue.toFixed(0) + " " + card.unit
                font.pixelSize: 10
                color: "#aaa"
            }
        }
    }
}
