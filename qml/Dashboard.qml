import QtQuick 2.12
import QtQuick.Controls 2.12
import QtCharts 2.3
import QtWebEngine 1.8

Item {
    signal openImage(string imagePath)
    signal openImageInBrowser(string imagePath)
    signal copyImage(string imagePath)
    signal copyImageUrl(string imagePath)
    signal saveImage(string imagePath)
    signal openMobileUrl(string url)

    Rectangle{
        id: dashboard
        width: parent.width
        height: parent.height
        color: "#f2f3ff"

        function getOverview() {
            overview_loading_text.text = "数据加载中..."
            let xhr_overview = new XMLHttpRequest()

            xhr_overview.onreadystatechange = function() {
                if (xhr_overview.readyState === XMLHttpRequest.DONE) {
                    if (xhr_overview.status === 200) {
                        let response_overview = JSON.parse(xhr_overview.responseText)

                        year_sales_number.text = response_overview.salesCount
                        year_income_number.text = response_overview.income
                        year_orders_number.text = response_overview.ordersCount
                        year_products_number.text = response_overview.productsCount
                        nearly_week_webView.cdata = response_overview.nearlyWeek
                        if(!nearly_week_webView.sended){
                            nearly_week_webView.runJavaScript("setData(" + JSON.stringify(response_overview.nearlyWeek) + ");")
                        }
                        styr_tbody.model = response_overview.ms.product
                        splr_tbody.model = response_overview.ms.supplier

                        overview_loading.visible = false
                        tabbar.visible = true
                        tabbutton_a.clicked()
                    } else {
                        console.log("HTTP request failed with status: " + xhr_overview.status)
                    }
                }
            }

            let url = serve + "/analysis/annual-overview?store-id=" + store + "&brand=" + brand
            xhr_overview.open("GET", url, true)
            xhr_overview.send()
        }

        TabBar {
            id: tabbar
            x: 15
            y: 15
            width: dashboard.width - 30
            visible: false

            TabButton {
                id: tabbutton_a
                text: "总体数据"
                onClicked: {
                    overview_year.visible = true
                    nearly_week.visible = true
                    ranks.visible = false
                }
            }
            TabButton {
                id: tabbutton_b
                text: "排行数据（15天）"
                onClicked: {
                    overview_year.visible = false
                    nearly_week.visible = false
                    ranks.visible = true
                }
            }
        }

        Rectangle {
            anchors.top: tabbar.bottom
            anchors.topMargin: 12
            anchors.left: tabbar.left
            width: tabbar.width
            height: dashboard.height - 30
            id: overview_loading
            color: "transparent"

            AnimatedImage {
                anchors.centerIn: parent
                anchors.horizontalCenterOffset: -50
                width: 32
                height: 36
                source: "qrc:/images/loading.gif"
            }

            Label {
                anchors.centerIn: parent
                anchors.horizontalCenterOffset: 10
                id: overview_loading_text
                text: dashboard.getOverview()
                font.pointSize: 10
            }
        }

        Rectangle {
            anchors.top: tabbar.bottom
            anchors.topMargin: 12
            anchors.left: tabbar.left
            width: tabbar.width
            height: 180
            radius: 10
            id: overview_year
            visible: false

            Image {
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.topMargin: 20
                anchors.leftMargin: 24
                width: 120
                height: 20
                source: "https://cdn.fixeam.com/tw/TW.png?imageMogr2/thumbnail120x"
                visible: brand == "兔皇"
            }

            Image {
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.topMargin: 20
                anchors.leftMargin: 24
                width: 50
                height: 20
                source: "https://cdn.fixeam.com/tw/KCOR.png?imageMogr2/thumbnail50x"
                visible: brand == "KCOR"
            }

            Image {
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.topMargin: 20
                anchors.leftMargin: 24
                width: 20
                height: 20
                source: "https://cdn.fixeam.com/tw/NT.png?imageMogr2/thumbnail20x"
                visible: brand == "NT"
            }

            Grid {
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.topMargin: 50
                anchors.leftMargin: 24
                width: parent.width - 78
                height: parent.height - 70
                rows: 1
                columns: 4
                spacing: 10

                Rectangle {
                    width: (parent.width - 30) / 4
                    height: parent.height

                    TextArea {
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.top: parent.top
                        anchors.topMargin: 20
                        text: "年度销售数量"
                        font.pixelSize: 14
                        id: year_sales_title
                        readOnly: true
                        selectByMouse: true
                        wrapMode: Text.Wrap
                    }

                    TextArea {
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.horizontalCenterOffset: -10
                        anchors.top: parent.top
                        anchors.topMargin: 50
                        id: year_sales_number
                        text: "0"
                        font.pixelSize: 26
                        readOnly: true
                        selectByMouse: true
                    }

                    TextArea {
                        anchors.left: year_sales_number.right
                        anchors.bottom: year_sales_number.bottom
                        anchors.bottomMargin: 5
                        text: "件"
                        font.pixelSize: 14
                        readOnly: true
                        selectByMouse: true
                    }
                }

                Rectangle {
                    width: (parent.width - 30) / 4
                    height: parent.height

                    TextArea {
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.top: parent.top
                        anchors.topMargin: 20
                        text: "年度净收金额"
                        font.pixelSize: 14
                        id: year_income_title
                        readOnly: true
                        selectByMouse: true
                        wrapMode: Text.Wrap
                    }

                    TextArea {
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.horizontalCenterOffset: -10
                        anchors.top: parent.top
                        anchors.topMargin: 50
                        id: year_income_number
                        text: "0"
                        font.pixelSize: 26
                        readOnly: true
                        selectByMouse: true
                    }

                    TextArea {
                        anchors.left: year_income_number.right
                        anchors.bottom: year_income_number.bottom
                        anchors.bottomMargin: 5
                        text: "元"
                        font.pixelSize: 14
                        readOnly: true
                        selectByMouse: true
                    }
                }

                Rectangle {
                    width: (parent.width - 30) / 4
                    height: parent.height

                    TextArea {
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.top: parent.top
                        anchors.topMargin: 20
                        text: "年度订单数量"
                        font.pixelSize: 14
                        id: year_orders_title
                        readOnly: true
                        selectByMouse: true
                        wrapMode: Text.Wrap
                    }

                    TextArea {
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.horizontalCenterOffset: -10
                        anchors.top: parent.top
                        anchors.topMargin: 50
                        id: year_orders_number
                        text: "0"
                        font.pixelSize: 26
                        readOnly: true
                        selectByMouse: true
                    }

                    TextArea {
                        anchors.left: year_orders_number.right
                        anchors.bottom: year_orders_number.bottom
                        anchors.bottomMargin: 5
                        text: "单"
                        font.pixelSize: 14
                        readOnly: true
                        selectByMouse: true
                    }
                }

                Rectangle {
                    width: (parent.width - 30) / 4
                    height: parent.height

                    TextArea {
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.top: parent.top
                        anchors.topMargin: 20
                        text: "年度上新款数"
                        font.pixelSize: 14
                        id: year_products_title
                        readOnly: true
                        selectByMouse: true
                        wrapMode: Text.Wrap
                    }

                    TextArea {
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.horizontalCenterOffset: -10
                        anchors.top: parent.top
                        anchors.topMargin: 50
                        id: year_products_number
                        text: "0"
                        font.pixelSize: 26
                        readOnly: true
                        selectByMouse: true
                    }

                    TextArea {
                        anchors.left: year_products_number.right
                        anchors.bottom: year_products_number.bottom
                        anchors.bottomMargin: 5
                        text: "SPU"
                        font.pixelSize: 14
                        readOnly: true
                        selectByMouse: true
                    }
                }
            }
        }

        Rectangle {
            anchors.top: overview_year.bottom
            anchors.left: overview_year.left
            anchors.topMargin: 12
            width: parent.width - 30
            height: parent.height - 270
            radius: 10
            id: nearly_week
            visible: false

            Label {
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.topMargin: 20
                anchors.leftMargin: 24
                width: 120
                height: 20
                text: "近期销售"
                font.pointSize: 11
            }

            WebEngineView {
                id: nearly_week_webView
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.topMargin: 80
                anchors.leftMargin: 20
                width: parent.width - 40
                height: parent.height - 80
                url: "qrc:/files/qml/dashboard_nearly_week_charts.html"

                property var cdata: []
                property bool sended: false

                onLoadProgressChanged: {
                    if(nearly_week_webView.loadProgress == 100 && cdata.length > 0){
                        nearly_week_webView.runJavaScript("setData(" + JSON.stringify(cdata) + ");")
                        sended = true
                    }
                }
            }
        }

        Grid {
            anchors.top: tabbar.bottom
            anchors.topMargin: 12
            anchors.left: tabbar.left
            width: tabbar.width
            height: parent.height - 80
            id: ranks
            rows: 1
            columns: 2
            spacing: 12
            visible: false
            onWidthChanged: {
                if(visible){
                    if(width < 800){
                        columns = 1
                        rows = 2
                        styr.width = width
                        splr.width = width
                        styr.height = (height - 12) / 2
                        splr.height = (height - 12) / 2
                    } else {
                        columns = 2
                        rows = 1
                        styr.width = (width - 12) / 2
                        splr.width = (width - 12) / 2
                        styr.height = height
                        splr.height = height
                    }
                }
            }

            Rectangle {
                width: (parent.width - 12) / 2
                height: parent.height
                id: styr
                color: "#ffffff"
                radius: 10

                Label {
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.topMargin: 20
                    anchors.leftMargin: 24
                    width: 120
                    height: 20
                    id: styr_name
                    text: "款式排行"
                    font.pointSize: 11
                }

                Grid {
                    anchors.top: styr_name.bottom
                    anchors.left: styr_name.left
                    anchors.topMargin: 15
                    width: parent.width - 48
                    height: 36
                    id: styr_thead
                    rows: 1
                    columns: 5

                    Rectangle {
                        width: parent.width / 5
                        height: parent.height
                        color: "#f3f3f3"

                        TextArea {
                            anchors.centerIn: parent
                            text: "款号"
                            font.pointSize: 10
                            readOnly: true
                            selectByMouse: true
                            wrapMode: Text.Wrap
                        }
                    }
                    Rectangle {
                        width: parent.width / 5
                        height: parent.height
                        color: "#f3f3f3"

                        TextArea {
                            anchors.centerIn: parent
                            text: "销售数量"
                            font.pointSize: 10
                            readOnly: true
                            selectByMouse: true
                            wrapMode: Text.Wrap
                        }
                    }
                    Rectangle {
                        width: parent.width / 5
                        height: parent.height
                        color: "#f3f3f3"

                        TextArea {
                            anchors.centerIn: parent
                            text: "销售金额"
                            font.pointSize: 10
                            readOnly: true
                            selectByMouse: true
                            wrapMode: Text.Wrap
                        }
                    }
                    Rectangle {
                        width: parent.width / 5
                        height: parent.height
                        color: "#f3f3f3"

                        TextArea {
                            anchors.centerIn: parent
                            text: "退款数量"
                            font.pointSize: 10
                            readOnly: true
                            selectByMouse: true
                            wrapMode: Text.Wrap
                        }
                    }
                    Rectangle {
                        width: parent.width / 5
                        height: parent.height
                        color: "#f3f3f3"

                        TextArea {
                            anchors.centerIn: parent
                            text: "退款金额"
                            font.pointSize: 10
                            readOnly: true
                            selectByMouse: true
                            wrapMode: Text.Wrap
                        }
                    }
                }

                ListView {
                    anchors.top: styr_thead.bottom
                    anchors.left: styr_thead.left
                    id: styr_tbody
                    width: parent.width - 48
                    height: parent.height - 100
                    model: []
                    clip: true

                    ScrollBar.vertical: ScrollBar {
                        onActiveChanged: {
                            active = true;
                        }
                    }

                    delegate: Grid {
                        id: styr_tbody_tr
                        width: parent.width
                        height: 36
                        rows: 1
                        columns: 5

                        Rectangle {
                            width: parent.width / 5
                            height: parent.height

                            TextArea {
                                anchors.centerIn: parent
                                text: modelData.stylenumber
                                font.pointSize: 10
                                readOnly: true
                                selectByMouse: true
                                wrapMode: Text.Wrap
                            }

                            Rectangle {
                                width: parent.width
                                height: 1
                                color: "#f3f3f3"
                                anchors.bottom: parent.bottom
                            }
                        }
                        Rectangle {
                            width: parent.width / 5
                            height: parent.height

                            TextArea {
                                anchors.centerIn: parent
                                text: modelData.salesCount
                                font.pointSize: 10
                                readOnly: true
                                selectByMouse: true
                                wrapMode: Text.Wrap
                            }

                            Rectangle {
                                width: parent.width
                                height: 1
                                color: "#f3f3f3"
                                anchors.bottom: parent.bottom
                            }
                        }
                        Rectangle {
                            width: parent.width / 5
                            height: parent.height

                            TextArea {
                                anchors.centerIn: parent
                                text: modelData.salesAmount
                                font.pointSize: 10
                                readOnly: true
                                selectByMouse: true
                                wrapMode: Text.Wrap
                            }

                            Rectangle {
                                width: parent.width
                                height: 1
                                color: "#f3f3f3"
                                anchors.bottom: parent.bottom
                            }
                        }
                        Rectangle {
                            width: parent.width / 5
                            height: parent.height

                            TextArea {
                                anchors.centerIn: parent
                                text: modelData.refundsCount
                                font.pointSize: 10
                                readOnly: true
                                selectByMouse: true
                                wrapMode: Text.Wrap
                            }

                            Rectangle {
                                width: parent.width
                                height: 1
                                color: "#f3f3f3"
                                anchors.bottom: parent.bottom
                            }
                        }
                        Rectangle {
                            width: parent.width / 5
                            height: parent.height

                            TextArea {
                                anchors.centerIn: parent
                                text: modelData.refundsAmount
                                font.pointSize: 10
                                readOnly: true
                                selectByMouse: true
                                wrapMode: Text.Wrap
                            }

                            Rectangle {
                                width: parent.width
                                height: 1
                                color: "#f3f3f3"
                                anchors.bottom: parent.bottom
                            }
                        }
                    }
                }
            }

            Rectangle {
                id: splr
                width: (parent.width - 12) / 2
                height: parent.height
                color: "#ffffff"
                radius: 10

                Label {
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.topMargin: 20
                    anchors.leftMargin: 24
                    id: splr_name
                    width: 120
                    height: 20
                    text: "供应商排行"
                    font.pointSize: 11
                }

                Grid {
                    anchors.top: splr_name.bottom
                    anchors.left: splr_name.left
                    anchors.topMargin: 15
                    width: parent.width - 48
                    height: 36
                    id: splr_thead
                    rows: 1
                    columns: 5

                    Rectangle {
                        width: parent.width / 5
                        height: parent.height
                        color: "#f3f3f3"

                        TextArea {
                            anchors.centerIn: parent
                            text: "供应商"
                            font.pointSize: 10
                            readOnly: true
                            selectByMouse: true
                            wrapMode: Text.Wrap
                        }
                    }
                    Rectangle {
                        width: parent.width / 5
                        height: parent.height
                        color: "#f3f3f3"

                        TextArea {
                            anchors.centerIn: parent
                            text: "销售数量"
                            font.pointSize: 10
                            readOnly: true
                            selectByMouse: true
                            wrapMode: Text.Wrap
                        }
                    }
                    Rectangle {
                        width: parent.width / 5
                        height: parent.height
                        color: "#f3f3f3"

                        TextArea {
                            anchors.centerIn: parent
                            text: "销售金额"
                            font.pointSize: 10
                            readOnly: true
                            selectByMouse: true
                            wrapMode: Text.Wrap
                        }
                    }
                    Rectangle {
                        width: parent.width / 5
                        height: parent.height
                        color: "#f3f3f3"

                        TextArea {
                            anchors.centerIn: parent
                            text: "退款数量"
                            font.pointSize: 10
                            readOnly: true
                            selectByMouse: true
                            wrapMode: Text.Wrap
                        }
                    }
                    Rectangle {
                        width: parent.width / 5
                        height: parent.height
                        color: "#f3f3f3"

                        TextArea {
                            anchors.centerIn: parent
                            text: "退款金额"
                            font.pointSize: 10
                            readOnly: true
                            selectByMouse: true
                            wrapMode: Text.Wrap
                        }
                    }
                }

                ListView {
                    anchors.top: splr_thead.bottom
                    anchors.left: splr_thead.left
                    id: splr_tbody
                    width: parent.width - 48
                    height: parent.height - 100
                    model: []
                    clip: true

                    ScrollBar.vertical: ScrollBar {
                        onActiveChanged: {
                            active = true;
                        }
                    }

                    delegate: Grid {
                        id: splr_tbody_tr
                        width: parent.width
                        height: 36
                        rows: 1
                        columns: 5

                        Rectangle {
                            width: parent.width / 5
                            height: parent.height

                            TextArea {
                                anchors.centerIn: parent
                                text: modelData.supplier
                                font.pointSize: 10
                                readOnly: true
                                selectByMouse: true
                                wrapMode: Text.Wrap
                            }

                            Rectangle {
                                width: parent.width
                                height: 1
                                color: "#f3f3f3"
                                anchors.bottom: parent.bottom
                            }
                        }
                        Rectangle {
                            width: parent.width / 5
                            height: parent.height

                            TextArea {
                                anchors.centerIn: parent
                                text: modelData.salesCount
                                font.pointSize: 10
                                readOnly: true
                                selectByMouse: true
                                wrapMode: Text.Wrap
                            }

                            Rectangle {
                                width: parent.width
                                height: 1
                                color: "#f3f3f3"
                                anchors.bottom: parent.bottom
                            }
                        }
                        Rectangle {
                            width: parent.width / 5
                            height: parent.height

                            TextArea {
                                anchors.centerIn: parent
                                text: modelData.salesAmount
                                font.pointSize: 10
                                readOnly: true
                                selectByMouse: true
                                wrapMode: Text.Wrap
                            }

                            Rectangle {
                                width: parent.width
                                height: 1
                                color: "#f3f3f3"
                                anchors.bottom: parent.bottom
                            }
                        }
                        Rectangle {
                            width: parent.width / 5
                            height: parent.height

                            TextArea {
                                anchors.centerIn: parent
                                text: modelData.refundsCount
                                font.pointSize: 10
                                readOnly: true
                                selectByMouse: true
                                wrapMode: Text.Wrap
                            }

                            Rectangle {
                                width: parent.width
                                height: 1
                                color: "#f3f3f3"
                                anchors.bottom: parent.bottom
                            }
                        }
                        Rectangle {
                            width: parent.width / 5
                            height: parent.height

                            TextArea {
                                anchors.centerIn: parent
                                text: modelData.refundsAmount
                                font.pointSize: 10
                                readOnly: true
                                selectByMouse: true
                                wrapMode: Text.Wrap
                            }

                            Rectangle {
                                width: parent.width
                                height: 1
                                color: "#f3f3f3"
                                anchors.bottom: parent.bottom
                            }
                        }
                    }
                }
            }
        }
    }
}
