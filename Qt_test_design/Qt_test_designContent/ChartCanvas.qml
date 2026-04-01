import QtQuick
import QtQuick.Controls

Rectangle {
    id: chart

    property string      title:     "Chart"
    property var         data:      []
    property double      maxValue:  100.0
    property color       lineColor: "#1565c0"
    property color       fillColor: "#e3f2fd"

    radius: 8
    color:  "white"
    border.color: "#e0e0e0"
    border.width: 1

    // Title label
    Text {
        id: titleLabel
        x: 10; y: 8
        text: chart.title
        font.pixelSize: 13
        font.bold: true
        color: "#333"
    }

    // Current value — top right
    Text {
        x: chart.width - width - 10
        y: 8
        text: chart.data.length > 0
            ? chart.data[chart.data.length - 1].toFixed(1)
            : "--"
        font.pixelSize: 13
        font.bold: true
        color: chart.lineColor
    }

    // Canvas fills the rest below the title
    Canvas {
        id: canvas
        x: 8
        y: titleLabel.height + 12
        width:  chart.width - 16
        height: chart.height - titleLabel.height - 20

        onPaint: {
            var ctx = getContext("2d");
            ctx.clearRect(0, 0, width, height);

            var data = chart.data;
            if (!data || data.length < 2) return;

            var W   = width;
            var H   = height;
            var pad = 4;

            // Grid lines
            ctx.strokeStyle = "#f0f0f0";
            ctx.lineWidth   = 1;
            for (var g = 0; g <= 4; g++) {
                var gy = pad + (H - 2*pad) * g / 4;
                ctx.beginPath();
                ctx.moveTo(0, gy);
                ctx.lineTo(W, gy);
                ctx.stroke();
            }

            // Calculate points
            var pts = [];
            for (var i = 0; i < data.length; i++) {
                var x = pad + (W - 2*pad) * i / (data.length - 1);
                var v = Math.min(Math.max(data[i] / chart.maxValue, 0), 1);
                var y = H - pad - (H - 2*pad) * v;
                pts.push({ x: x, y: y });
            }

            // Fill area
            ctx.beginPath();
            ctx.moveTo(pts[0].x, H - pad);
            for (var j = 0; j < pts.length; j++)
                ctx.lineTo(pts[j].x, pts[j].y);
            ctx.lineTo(pts[pts.length-1].x, H - pad);
            ctx.closePath();
            ctx.fillStyle = chart.fillColor;
            ctx.fill();

            // Line
            ctx.beginPath();
            ctx.moveTo(pts[0].x, pts[0].y);
            for (var k = 1; k < pts.length; k++)
                ctx.lineTo(pts[k].x, pts[k].y);
            ctx.strokeStyle = chart.lineColor;
            ctx.lineWidth   = 2.5;
            ctx.lineJoin    = "round";
            ctx.lineCap     = "round";
            ctx.stroke();

            // Last point dot
            var last = pts[pts.length - 1];
            ctx.beginPath();
            ctx.arc(last.x, last.y, 4, 0, Math.PI * 2);
            ctx.fillStyle = chart.lineColor;
            ctx.fill();
        }

        Connections {
            target: dataProvider
            function onDataChanged() { canvas.requestPaint(); }
        }

        Connections {
            target: chart
            function onLineColorChanged() { canvas.requestPaint(); }
        }
    }
}
