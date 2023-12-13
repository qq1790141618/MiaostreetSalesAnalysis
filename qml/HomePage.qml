import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtGraphicalEffects 1.0
import QtQuick.Controls.Styles 1.4

Item {
    signal openImage(string imagePath)
    signal openImageInBrowser(string imagePath)
    signal copyImage(string imagePath)
    signal copyImageUrl(string imagePath)
    signal saveImage(string imagePath)
    signal openMobileUrl(string url)

    Rectangle{
        id: homepage
        width: parent.width
        height: parent.height
        color: "#f2f3ff"

        function getFamousWord() {
            let xhr_famousWord = new XMLHttpRequest()
            xhr_famousWord.onreadystatechange = function() {
                if (xhr_famousWord.readyState === XMLHttpRequest.DONE) {
                    if (xhr_famousWord.status === 200) {
                        let response_famousWord = JSON.parse(xhr_famousWord.responseText)

                        let word = response_famousWord.hitokoto + ' 来自《' + response_famousWord.from + '》'
                        if(response_famousWord.from_who !== null && response_famousWord.from_who !== undefined){
                            word += response_famousWord.from_who
                        }

                        famous_word.text = word
                        famous_word.visible = true
                    } else {
                        console.log("HTTP request failed with status: " + xhr_famousWord.status)
                    }
                }
            }

            xhr_famousWord.open("GET", "https://v1.hitokoto.cn/", true)
            xhr_famousWord.send()
        }

        function getOverview() {
            let date = new Date()
            date.setDate(date.getDate() - 1)
            let dateIs = date.toISOString().split("T")[0]
            let xhr_overview = new XMLHttpRequest()

            xhr_overview.onreadystatechange = function() {
                if (xhr_overview.readyState === XMLHttpRequest.DONE) {
                    if (xhr_overview.status === 200) {
                        let response_overview = JSON.parse(xhr_overview.responseText)

                        overview_sales_count_number.text = response_overview.sales.count
                        overview_sales_amount_number.text = response_overview.sales.amount
                        overview_refunds_count_number.text = response_overview.refunds.count
                        overview_refunds_amount_number.text = response_overview.refunds.amount
                        overview_real_sales_amount_number.text = response_overview.sales.amount - response_overview.refunds.amount

                        let income = response_overview.sales.income - response_overview.refunds.income
                        let formattedIncome = income.toFixed(2)
                        if (formattedIncome.endsWith('.00')) {
                            formattedIncome = formattedIncome.slice(0, -3)
                        }
                        overview_income_number.text = formattedIncome

                        overview_loading.visible = false
                        overview_title.visible = true
                        overview.visible = true
                    } else {
                        console.log("HTTP request failed with status: " + xhr_overview.status)
                    }
                }
            }

            overview_title.text = "数据概览 (" + dateIs + ")："
            let url = serve + "/analysis/sum?store-id=" + store + "&brand=" + brand + "&mode=日&time=" + dateIs

            xhr_overview.open("GET", url, true)
            xhr_overview.send()
        }

        Label {
            x: 18
            y: 13
            id: welcome
            text: "欢迎回来！"
            font.pointSize: 16
        }

        TextEdit {
            anchors.top: welcome.bottom
            anchors.left: welcome.left
            anchors.topMargin: 5
            id: famous_word
            text: parent.getFamousWord()
            font.pointSize: 10
            readOnly: true
            selectByMouse: true
            visible: false
        }

        Rectangle {
            anchors.top: famous_word.bottom
            anchors.left: famous_word.left
            anchors.topMargin: 15
            width: parent.width - 30
            height: 100
            radius: 12
            id: overview_loading

            AnimatedImage {
                anchors.centerIn: parent
                anchors.horizontalCenterOffset: -50
                width: 32
                height: 32
                source: "qrc:/images/loading.gif"
            }

            Label {
                anchors.centerIn: parent
                anchors.horizontalCenterOffset: 10
                id: overview_loading_text
                text: "数据概览加载中..."
                font.pointSize: 10
            }
        }

        Label {
            anchors.top: famous_word.bottom
            anchors.left: famous_word.left
            anchors.topMargin: 15
            id: overview_title
            text: parent.getOverview()
            font.pointSize: 10
            wrapMode: Text.Wrap
            visible: false
        }

        Grid {
            anchors.top: overview_title.bottom
            anchors.left: overview_title.left
            anchors.topMargin: 5
            width: parent.width - 36
            height: 100
            rows: 3
            columns: 5
            spacing: 10
            id: overview
            visible: false
            onWidthChanged: {
                let rectCount = children.length

                if(width >= 1220){
                    columns = 6

                    overview_sales_amount.visible = true
                    overview_refunds_count.visible = true
                    overview_refunds_amount.visible = true
                    overview_real_sales_amount.visible = true
                }
                else
                if(width < 1220 && width >= 1020){
                    columns = 5

                    overview_sales_amount.visible = true
                    overview_refunds_count.visible = true
                    overview_refunds_amount.visible = true
                    overview_real_sales_amount.visible = false
                }
                else
                if(width < 1020 && width >= 820){
                    columns = 4

                    overview_sales_amount.visible = false
                    overview_refunds_count.visible = true
                    overview_refunds_amount.visible = false
                    overview_real_sales_amount.visible = true
                }
                else
                if(width < 820 && width >= 620){
                    columns = 3

                    overview_sales_amount.visible = false
                    overview_refunds_count.visible = true
                    overview_refunds_amount.visible = false
                    overview_real_sales_amount.visible = false
                }
                else
                {
                    columns = 2

                    overview_sales_amount.visible = false
                    overview_refunds_count.visible = false
                    overview_refunds_amount.visible = false
                    overview_real_sales_amount.visible = false
                }

                for (var i = 0; i < rectCount; i++) {
                    children[i].width = (width - 10 * (columns - 1)) / columns
                }
            }

            FontLoader {
                id: fontLoader
                source: "qrc:/font/Dosis-Regular.ttf"
                onStatusChanged: {
                    if (status === FontLoader.Ready) {
                        textEdit.font.family = fontLoader.name
                    }
                }
            }

            Rectangle {
                width: (parent.width - 40) / 5
                height: 100
                radius: 12
                id: overview_sales_count

                Rectangle {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    height: parent.height - 32
                    anchors.topMargin: 15
                    anchors.leftMargin: 24

                    TextEdit {
                        anchors.top: parent.top
                        anchors.left: parent.left
                        width: (parent.width - 30) / 6
                        id: overview_sales_count_title
                        text: "销售数量"
                        font.pointSize: 9
                        readOnly: true
                        selectByMouse: true
                    }

                    TextEdit {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        id: overview_sales_count_number
                        text: "0"
                        font.pointSize: 20
                        font.family: fontLoader.name
                        readOnly: true
                        selectByMouse: true
                    }

                    TextEdit {
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        id: overview_sales_count_unit
                        text: "件"
                        font.pointSize: 9
                        readOnly: true
                        selectByMouse: true
                    }
                }

                Image {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 15
                    source: "qrc:/images/tas/2efd7e36.png"
                    fillMode: Image.PreserveAspectFit
                }
            }

            Rectangle {
                width: (parent.width - 40) / 5
                height: 100
                radius: 12
                id: overview_sales_amount

                Rectangle {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    height: parent.height - 32
                    anchors.topMargin: 15
                    anchors.leftMargin: 24

                    TextEdit {
                        anchors.top: parent.top
                        anchors.left: parent.left
                        width: (parent.width - 30) / 6
                        id: overview_sales_amount_title
                        text: "销售金额"
                        font.pointSize: 9
                        readOnly: true
                        selectByMouse: true
                    }

                    TextEdit {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        id: overview_sales_amount_number
                        text: "0"
                        font.pointSize: 20
                        font.family: fontLoader.name
                        readOnly: true
                        selectByMouse: true
                    }

                    TextEdit {
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        id: overview_sales_amount_unit
                        text: "元"
                        font.pointSize: 9
                        readOnly: true
                        selectByMouse: true
                    }
                }

                Image {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 15
                    source: "qrc:/images/tas/dcc1c3ef.png"
                    fillMode: Image.PreserveAspectFit
                }
            }

            Rectangle {
                width: (parent.width - 40) / 5
                height: 100
                radius: 12
                id: overview_refunds_count

                Rectangle {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    height: parent.height - 32
                    anchors.topMargin: 15
                    anchors.leftMargin: 24

                    TextEdit {
                        anchors.top: parent.top
                        anchors.left: parent.left
                        width: (parent.width - 30) / 6
                        id: overview_refunds_count_title
                        text: "退款数量"
                        font.pointSize: 9
                        readOnly: true
                        selectByMouse: true
                    }

                    TextEdit {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        id: overview_refunds_count_number
                        text: "0"
                        font.pointSize: 20
                        font.family: fontLoader.name
                        readOnly: true
                        selectByMouse: true
                    }

                    TextEdit {
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        id: overview_refunds_count_unit
                        text: "件"
                        font.pointSize: 9
                        readOnly: true
                        selectByMouse: true
                    }
                }

                Image {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 15
                    source: "qrc:/images/tas/5b9b44b3.png"
                    fillMode: Image.PreserveAspectFit
                }
            }

            Rectangle {
                width: (parent.width - 40) / 5
                height: 100
                radius: 12
                id: overview_refunds_amount

                Rectangle {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    height: parent.height - 32
                    anchors.topMargin: 15
                    anchors.leftMargin: 24

                    TextEdit {
                        anchors.top: parent.top
                        anchors.left: parent.left
                        width: (parent.width - 30) / 6
                        id: overview_refunds_amount_title
                        text: "退款金额"
                        font.pointSize: 9
                        readOnly: true
                        selectByMouse: true
                    }

                    TextEdit {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        id: overview_refunds_amount_number
                        text: "0"
                        font.pointSize: 20
                        font.family: fontLoader.name
                        readOnly: true
                        selectByMouse: true
                    }

                    TextEdit {
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        id: overview_refunds_amount_unit
                        text: "元"
                        font.pointSize: 9
                        readOnly: true
                        selectByMouse: true
                    }
                }

                Image {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 15
                    source: "qrc:/images/tas/eb74f5be.png"
                    fillMode: Image.PreserveAspectFit
                }
            }

            Rectangle {
                width: (parent.width - 40) / 5
                height: 100
                radius: 12
                id: overview_real_sales_amount

                Rectangle {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    height: parent.height - 32
                    anchors.topMargin: 15
                    anchors.leftMargin: 24

                    TextEdit {
                        anchors.top: parent.top
                        anchors.left: parent.left
                        width: (parent.width - 30) / 6
                        id: overview_real_sales_amount_title
                        text: "实销金额"
                        font.pointSize: 9
                        readOnly: true
                        selectByMouse: true
                    }

                    TextEdit {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        id: overview_real_sales_amount_number
                        text: "0"
                        font.pointSize: 20
                        font.family: fontLoader.name
                        readOnly: true
                        selectByMouse: true
                    }

                    TextEdit {
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        id: overview_real_sales_amount_unit
                        text: "元"
                        font.pointSize: 9
                        readOnly: true
                        selectByMouse: true
                    }
                }

                Image {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 15
                    source: "qrc:/images/tas/dcc1c3ef.png"
                    fillMode: Image.PreserveAspectFit
                }
            }

            Rectangle {
                width: (parent.width - 40) / 5
                height: 100
                radius: 12
                id: overview_income

                Rectangle {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    height: parent.height - 32
                    anchors.topMargin: 15
                    anchors.leftMargin: 24

                    TextEdit {
                        anchors.top: parent.top
                        anchors.left: parent.left
                        width: (parent.width - 30) / 6
                        id: overview_income_title
                        text: "净收金额"
                        font.pointSize: 9
                        readOnly: true
                        selectByMouse: true
                    }

                    TextEdit {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        id: overview_income_number
                        text: "0"
                        font.pointSize: 20
                        font.family: fontLoader.name
                        readOnly: true
                        selectByMouse: true
                    }

                    TextEdit {
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        id: overview_income_unit
                        text: "元"
                        font.pointSize: 9
                        readOnly: true
                        selectByMouse: true
                    }
                }

                Image {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 15
                    source: "qrc:/images/tas/ee93b257.png"
                    fillMode: Image.PreserveAspectFit
                }
            }
        }

        function getMirro() {
            let xhr_mirro = new XMLHttpRequest()

            xhr_mirro.onreadystatechange = function() {
                if (xhr_mirro.readyState === XMLHttpRequest.DONE) {
                    if (xhr_mirro.status === 200) {
                        let response_mirro = JSON.parse(xhr_mirro.responseText)
                        rec_listView.model = response_mirro

                        rec_loading.visible = false
                        rec_listView.visible = true
                    } else {
                        console.log("HTTP request failed with status: " + xhr_mirro.status)
                    }
                }
            }

            rec_title.text = "商品推荐："
            rec_title.visible = true
            let url = serve + "/goods/mirros"

            xhr_mirro.open("GET", url, true)
            xhr_mirro.send()
        }

        Label {
            anchors.top: overview.bottom
            anchors.left: overview.left
            anchors.topMargin: 15
            id: rec_title
            text: parent.getMirro()
            font.pointSize: 10
            visible: false
        }

        Rectangle {
            anchors.top: rec_title.bottom
            anchors.left: rec_title.left
            anchors.topMargin: 5
            width: parent.width - 30
            height: parent.height - 255
            radius: 12
            id: rec_loading

            AnimatedImage {
                anchors.centerIn: parent
                anchors.horizontalCenterOffset: -50
                width: 32
                height: 32
                source: "qrc:/images/loading.gif"
            }

            Label {
                anchors.centerIn: parent
                anchors.horizontalCenterOffset: 10
                text: "商品推荐加载中..."
                font.pointSize: 10
            }
        }

        ListView {
            anchors.top: rec_title.bottom
            anchors.left: rec_title.left
            anchors.topMargin: 5
            id: rec_listView
            width: parent.width - 30
            height: parent.height - 255
            model: []
            spacing: 7
            clip: true
            visible: false

            ScrollBar.vertical: ScrollBar {
                id: rec_listView_scrollBar
                onActiveChanged: {
                    active = true;
                }
            }

            delegate: Rectangle {
                height: 112000 / parent.width
                width: parent.width
                radius: height / 12
                id: rec_listView_item
                onWidthChanged: {
                    if(width < 620){
                        list_item_shop.visible = false
                    } else {
                        list_item_shop.visible = true
                    }
                }

                MouseArea {
                    id: imageRect
                    height: parent.height / 6 * 5
                    width: height
                    anchors.top: parent.top
                    anchors.topMargin: (parent.height - height) / 2
                    anchors.left: parent.left
                    anchors.leftMargin: (parent.height - height) / 2
                    acceptedButtons: Qt.LeftButton | Qt.RightButton
                    onClicked: {
                        if(mouse.button === Qt.LeftButton){
                            openImage(JSON.parse(modelData["main-image"])[0])
                        }
                        if(mouse.button === Qt.RightButton){
                            option_menu.popup()
                        }
                    }

                    Image {
                        id: nameIamge
                        anchors.fill: parent
                        source: "https://image.fixeam.com/?width=" + width.toFixed(0) + "&link=" + JSON.parse(modelData["main-image"])[0]
                        visible: false
                    }

                    Rectangle{
                        id: mask
                        anchors.fill: parent
                        radius: height / 15
                        visible: false
                        color: "#f3f3f3"
                    }

                    OpacityMask {
                        anchors.fill: parent
                        source: nameIamge
                        maskSource: mask
                    }

                    Menu {
                        id: option_menu

                        background: Rectangle {
                            anchors.topMargin: 5
                            anchors.bottomMargin: 5
                            implicitWidth: 175
                            implicitHeight: 32
                            color: "#ffffff"
                            border.color: "#f3f3f3"
                            radius: 7
                        }

                        Action { text: ""; enabled: false; }
                        Action { text: "使用默认照片查看器打开"; onTriggered: {
                                openImage(JSON.parse(modelData["main-image"])[0])
                            } }
                        Action { text: "使用浏览器打开"; onTriggered: {
                                openImageInBrowser(JSON.parse(modelData["main-image"])[0])
                            } }
                        Action { text: "使用默认浏览器打开"; onTriggered: {
                                Qt.openUrlExternally(JSON.parse(modelData["main-image"])[0])
                            } }
                        Action { text: "复制图片"; onTriggered: {
                                copyImage(JSON.parse(modelData["main-image"])[0])
                            } }
                        Action { text: "复制图片链接"; onTriggered: {
                                copyImageUrl(JSON.parse(modelData["main-image"])[0])
                            } }
                        Action { text: "图片另存为"; onTriggered: {
                                saveImage(JSON.parse(modelData["main-image"])[0])
                            } }
                        Action { text: ""; enabled: false; }

                        delegate: MenuItem {
                            id: menuItem
                            implicitWidth: 175
                            implicitHeight: menuItem.enabled ? 32 : 7

                            contentItem: Text {
                                anchors.left: parent.left
                                anchors.leftMargin: 20
                                text: menuItem.text
                                opacity: enabled ? 1.0 : 0.3
                                color: "#2c2c2c"
                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignVCenter
                                elide: Text.ElideRight
                                font.pointSize: 9
                            }

                            background: Rectangle {
                                opacity: enabled ? 1 : 0.3
                                color: menuItem.highlighted ? "#f3f3f3" : "transparent"
                                clip: true
                            }
                        }
                    }
                }

                TextArea {
                    anchors.top: parent.top
                    anchors.left: imageRect.right
                    anchors.topMargin: 5
                    anchors.leftMargin: 9
                    id: list_item_name
                    width: parent.width - parent.height - 110
                    text: modelData.name + "【喵街ID：" + modelData["miaostreet-id"] + "】"
                    font.pointSize: 10
                    readOnly: true
                    selectByMouse: true
                    selectByKeyboard: true
                    wrapMode: Text.Wrap
                }

                Label {
                    anchors.bottom: parent.bottom
                    anchors.left: imageRect.right
                    anchors.bottomMargin: 12
                    anchors.leftMargin: 18
                    id: list_item_sct
                    text: "销量"
                    font.pointSize: 10
                }

                Label {
                    anchors.bottom: parent.bottom
                    anchors.left: list_item_sct.right
                    anchors.bottomMargin: 10
                    anchors.leftMargin: 7
                    id: list_item_sc
                    text: modelData.salesCount
                    font.pointSize: 16
                    font.family: fontLoader.name
                    color: "#f6685d"
                }

                Label {
                    anchors.bottom: list_item_sct.bottom
                    anchors.left: list_item_sc.right
                    anchors.leftMargin: 13
                    id: list_item_sat
                    text: "金额"
                    font.pointSize: 10
                }

                Label {
                    anchors.bottom: list_item_sc.bottom
                    anchors.left: list_item_sat.right
                    anchors.leftMargin: 7
                    id: list_item_sa
                    text: modelData.salesAmount
                    font.pointSize: 16
                    font.family: fontLoader.name
                    color: "#f6685d"
                }

                TextArea {
                    anchors.right: list_view_open.left
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.rightMargin: 10
                    id: list_item_shop
                    text: modelData["store-id"] + " / " + modelData.brand
                    font.pointSize: 10
                    readOnly: true
                    selectByMouse: true
                    selectByKeyboard: true
                }

                Button {
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.rightMargin: 24
                    width: 90
                    height: 28
                    id: list_view_open
                    text: "查看"
                    background: Rectangle {
                        color: "#f2f3ff"

                        radius: 5
                        border.width: 0

                        MouseArea {
                            id: mouseArea
                            anchors.fill: parent
                            hoverEnabled: true
                            onClicked: {
                                openMobileUrl("https://www.miaostreet.com/clmj/hybrid/miaojieWeex?pageName=goods-detail&itemId=" + modelData["miaostreet-id"])
                            }
                            onEntered: {
                                cursorShape = Qt.PointingHandCursor
                                parent.color = "#d9e1ff"
                            }
                            onExited: {
                                cursorShape = Qt.ArrowCursor
                                parent.color = "#f2f3ff"
                            }
                        }
                    }
                }
            }
        }
    }
}
