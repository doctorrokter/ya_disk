import bb.cascades 1.4
import bb.device 1.4
import bb.system 1.2

Page {
    id: root
    
    signal loadPath(string dirName, string path)
    signal loadFile(string filename, string path)
    signal openFile(string filename, string path)
    signal upload(string path)
    signal uploadsPageRequested();
    
    property string dirName: "Root"
    property variant data: undefined
    property string path: "/"
    property string fileOrDirToDelete: ""
    property string pathToDelete: ""
    
    actionBarAutoHideBehavior: ActionBarAutoHideBehavior.HideOnScroll
    actionBarVisibility: ChromeVisibility.Overlay
    
    titleBar: TitleBar {
        title: dirName
    }
    
    Container {
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        
        layout: DockLayout {}
        
        Header {
            id: header
            visible: path !== "/"
            title: path
            verticalAlignment: VerticalAlignment.Top
            
            attachedObjects: [
                LayoutUpdateHandler {
                    id: headerLUH
                }
            ]
        }
        
        ListView {
            id: listView
            scrollRole: ScrollRole.Main
            
            verticalAlignment: VerticalAlignment.Bottom
            
            margin.leftOffset: ui.du(0.5)
            margin.topOffset: header.visible ? headerLUH.layoutFrame.height : 0
            margin.rightOffset: ui.du(0.5)
            
            dataModel: ArrayDataModel {
                id: dataModel
            }
            
            layout: gridListLayout
            
            onTriggered: {
                var data = dataModel.data(indexPath);
                console.debug("OPEN: ", data.path);
                if (data.dir) {
                    loadPath(data.name, data.path);
                } else {
                    spinner.start();
                    openFile(data.name, data.path);
                }
            }
            
            function itemType(data, indexPath) {
                if (layout.objectName === "stackListLayout") {
                    return "listItem";
                } else {
                    return "gridItem";
                }
            }
            
            listItemComponents: [
                ListItemComponent {
                    type: "listItem"
                    CustomListItem {
                        id: listListItem
                        
                        function getImage() {
                            if (!ListItemData.dir) {
                                var ext = ListItemData.ext.toLowerCase();
                                if (_file.isImage(ext)) {
                                    return "asset:///images/ic_doctype_picture.png";
                                } else if (_file.isVideo(ext)) {
                                    return "asset:///images/ic_doctype_video.png";
                                } else if (_file.isAudio(ext)) {
                                    return "asset:///images/ic_doctype_music.png";
                                } else if (_file.isPdf(ext)) {
                                    return "asset:///images/ic_doctype_pdf.png";
                                } else if (_file.isDoc(ext)) {
                                    return "asset:///images/ic_doctype_doc.png";
                                } else if (_file.isSpreadSheet(ext)) {
                                    return "asset:///images/ic_doctype_xls.png";
                                } else if (_file.isPresentation(ext)) {
                                    return "asset:///images/ic_doctype_ppt.png";
                                } else {
                                    return "asset:///images/ic_doctype_generic.png";
                                }
                            }
                            return "asset:///images/ic_folder.png";
                        }
                        
                        function filterColor() {
                            if (!ListItemData.dir) {
                                return ui.palette.background;
                            }
                            return ui.palette.primary;
                        }
                        
                        Container {
                            horizontalAlignment: HorizontalAlignment.Fill
                            verticalAlignment: VerticalAlignment.Fill
                            
                            layout: StackLayout {
                                orientation: LayoutOrientation.LeftToRight
                            }
                            
                            Container {
                                background: ui.palette.plain
                                ImageView {
                                    imageSource: listListItem.getImage();
                                    filterColor: listListItem.filterColor();
                                    
                                    opacity: ListItemData.dir ? 0.25 : 1.0
                                    preferredWidth: ui.du(11)
                                    preferredHeight: ui.du(11)
                                }
                            }                            
                            
                            Container {
                                horizontalAlignment: HorizontalAlignment.Fill
                                
                                leftPadding: ui.du(1)
                                topPadding: {
                                    if (!ListItemData.dir) {
                                        return ui.du(1);
                                    }
                                }
                                bottomPadding: {
                                    if (!ListItemData.dir) {
                                        return ui.du(1);
                                    }
                                }
                                
                                verticalAlignment: {
                                    if (ListItemData.dir) {
                                        return VerticalAlignment.Center
                                    }
                                    return VerticalAlignment.Fill
                                }
                                
                                layout: DockLayout {}
                                
                                Label {
                                    text: ListItemData.name
                                    verticalAlignment: VerticalAlignment.Top
                                }
                                
                                Label {
                                    visible: !ListItemData.dir
                                    verticalAlignment: VerticalAlignment.Bottom
                                    text: Qt.formatDateTime(ListItemData.lastModified, "dd.MM.yyyy, HH:mm")
                                    textStyle.base: SystemDefaults.TextStyles.SubtitleText
                                }
                            }
                        }
                    }
                },
                
                ListItemComponent {
                    type: "gridItem"
                    CustomListItem {
                        id: listItem
                        
                        function getTextStyle() {
                            if (displayInfo.pixelSize.width === 1440) {
                                return SystemDefaults.TextStyles.BodyText;
                            }
                            return SystemDefaults.TextStyles.SubtitleText;
                        }
                        
                        function getImage() {
                            if (!ListItemData.dir) {
                                var ext = ListItemData.ext.toLowerCase();
                                if (_file.isImage(ext)) {
                                    return "asset:///images/ic_doctype_picture.png";
                                } else if (_file.isVideo(ext)) {
                                    return "asset:///images/ic_doctype_video.png";
                                } else if (_file.isAudio(ext)) {
                                    return "asset:///images/ic_doctype_music.png";
                                } else if (_file.isPdf(ext)) {
                                    return "asset:///images/ic_doctype_pdf.png";
                                } else if (_file.isDoc(ext)) {
                                    return "asset:///images/ic_doctype_doc.png";
                                } else if (_file.isSpreadSheet(ext)) {
                                    return "asset:///images/ic_doctype_xls.png";
                                } else if (_file.isPresentation(ext)) {
                                    return "asset:///images/ic_doctype_ppt.png";
                                } else {
                                    return "asset:///images/ic_doctype_generic.png";
                                }
                            }
                            return "asset:///images/ic_folder.png";
                        }
                        
                        function filterColor() {
                            if (!ListItemData.dir) {
                                return ui.palette.background;
                            }
                            return ui.palette.primary;
                        }
                        
                        Container {
                            horizontalAlignment: HorizontalAlignment.Fill
                            verticalAlignment: VerticalAlignment.Fill
                            
                            background: ui.palette.plain
                            
                            layout: DockLayout {}
                            
                            ImageView {
                                imageSource: listItem.getImage();
                                filterColor: listItem.filterColor();
                                
                                opacity: ListItemData.dir ? 0.25 : 1.0
                                preferredWidth: ListItemData.dir ? listItemLUH.layoutFrame.width : ui.du(20)
                                preferredHeight: ListItemData.dir ? listItemLUH.layoutFrame.height - ui.du(2) : ui.du(20)
                                verticalAlignment: ListItemData.dir ? VerticalAlignment.Bottom : VerticalAlignment.Center
                                horizontalAlignment: ListItemData.dir ? HorizontalAlignment.Fill : HorizontalAlignment.Center
                            }
                            
                            ImageView {
                                visible: !ListItemData.dir
                                imageSource: "asset:///images/opac_bg.png"
                                horizontalAlignment: HorizontalAlignment.Fill
                                verticalAlignment: VerticalAlignment.Bottom
                                opacity: 0.5
                            }
                            
                            Label {
                                verticalAlignment: VerticalAlignment.Bottom
                                text: ListItemData.name
                                textStyle.base: SystemDefaults.TextStyles.SubtitleText
                                
                                margin.leftOffset: ui.du(1);
                                margin.bottomOffset: ListItemData.dir ? ui.du(5.5) : ui.du(4)
                                
                                textStyle.color: ListItemData.dir ? ui.palette.textOnPlain : ui.palette.textOnPrimary
                            }
                            
                            Label {
                                visible: !ListItemData.dir
                                
                                text: Qt.formatDateTime(ListItemData.lastModified, "dd.MM.yyyy, HH:mm")
                                verticalAlignment: VerticalAlignment.Bottom
                                textStyle.color: ui.palette.textOnPrimary
                                textStyle.base: SystemDefaults.TextStyles.SmallText
                                textStyle.fontWeight: FontWeight.W100
                                
                                margin.leftOffset: ui.du(1);
                                margin.bottomOffset: ui.du(1)
                            }
                            
                            attachedObjects: [
                                LayoutUpdateHandler {
                                    id: listItemLUH
                                }
                            ]
                        }
                        
                        attachedObjects: [
                            DisplayInfo {
                                id: displayInfo
                            }
                        ]
                        
                        contextActions: [
                            ActionSet {
                                DeleteActionItem {
                                    id: deleteAction
                                    
                                    onTriggered: {
                                        var data = ListItemData;
                                        _fileController.requestDeletion(data.name, data.path);
                                    }
                                }
                            }
                        ]
                    }
                }
            ]
        }
        
        ActivityIndicator {
            id: spinner

            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            
            minWidth: ui.du(10)
        }
    }
    
    attachedObjects: [
        GridListLayout {
            id: gridListLayout
            objectName: "gridListLayout"
            columnCount: {
                if (rootDisplayInfo.pixelSize.width === 1440) {
                    return 4;
                }
                return 3;
            }
        },
        
        StackListLayout {
            id: stackListLayout
            objectName: "stackListLayout"
        },
        
        DisplayInfo {
            id: rootDisplayInfo
        },
        
        SystemPrompt {
            id: createDirPrompt
            
            title: qsTr("Enter folder name") + Retranslate.onLocaleOrLanguageChanged
            dismissAutomatically: true
            
            onFinished: {
                if (value === 2) {
                    spinner.start();
                    _fileController.createDir(createDirPrompt.inputFieldTextEntry(), root.path);
                }
            }
        },
        
        SystemDialog {
            id: deleteDialog
            
            title: qsTr("Confirm the deleting") + Retranslate.onLocaleOrLanguageChanged
            body: qsTr("This action cannot be undone. Continue?") + Retranslate.onLocaleOrLanguageChanged
            
            includeRememberMe: true
            rememberMeText: qsTr("Don't ask again") + Retranslate.onLocaleOrLanguageChanged
            rememberMeChecked: {
                var dontAsk = _appConfig.get("do_not_ask_before_deleting");
                return dontAsk !== "" && dontAsk === "true";
            }
            
            onFinished: {
                if (value === 2) {
                    spinner.start();
                    _appConfig.set("do_not_ask_before_deleting", deleteDialog.rememberMeSelection() + "");
                    _fileController.deleteFileOrDir(root.fileOrDirToDelete, root.pathToDelete);
                } else {
                    root.pathToDelete = "";
                    root.fileOrDirToDelete = "";
                }
            }
        }
    ]
    
    actions: [
        ActionItem {
            id: createDirAction
            imageSource: "asset:///images/ic_add_folder.png"
            title: qsTr("Create folder") + Retranslate.onLocaleOrLanguageChanged
            ActionBar.placement: ActionBarPlacement.Signature
            
            onTriggered: {
                createDirPrompt.show();
            }
        },
        
        ActionItem {
            id: uploadAction
            title: qsTr("Upload to Disk") + Retranslate.onLocaleOrLanguageChanged
            imageSource: "asset:///images/ic_upload.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            
            onTriggered: {
                upload(root.path);
            }
        },
        
        ActionItem {
            id: uploadsAction
            imageSource: "asset:///images/ic_uploads.png"
            title: qsTr("Uploads") + Retranslate.onLocaleOrLanguageChanged
            
            onTriggered: {
                root.uploadsPageRequested();
            }
        },
        
        ActionItem {
            id: viewActionItem
            imageSource: {
                var view = _appConfig.get("files_view");
                if (view === "" || view === "grid") {
                    return "asset:///images/ic_view_list.png";
                }
                return "asset:///images/ic_view_grid.png";
            }
            title: {
                var view = _appConfig.get("files_view");
                if (view === "" || view === "grid") {
                    return qsTr("List") + Retranslate.onLocaleOrLanguageChanged;
                }
                return qsTr("Grid") + Retranslate.onLocaleOrLanguageChanged;
            }
            ActionBar.placement: ActionBarPlacement.OnBar
            
            onTriggered: {
                var view = _appConfig.get("files_view");
                if (view === "" || view === "grid") {
                    _appConfig.set("files_view", "list");
                    viewActionItem.imageSource = "asset:///images/ic_view_grid.png";
                    viewActionItem.title = qsTr("Grid") + Retranslate.onLocaleOrLanguageChanged
                    listView.layout = stackListLayout;
                } else {
                    _appConfig.set("files_view", "grid");
                    viewActionItem.imageSource = "asset:///images/ic_view_list.png";
                    viewActionItem.title = qsTr("List") + Retranslate.onLocaleOrLanguageChanged
                    listView.layout = gridListLayout;
                }
            }
        }
    ]
    
    onCreationCompleted: {
        spinner.start();
        _fileController.fileLoaded.connect(root.stopSpinner);
        _fileController.fileOpened.connect(root.stopSpinner);
        _fileController.dirCreated.connect(root.onDirCreated);
        _fileController.deletionRequested.connect(root.onDeletionRequested);
        _fileController.fileOrDirDeleted.connect(root.deleteFileOrDir);
        _appConfig.settingsChanged.connect(root.onSettingsChanged);
        
//        var data = [];
//        data.push({path: "sdfsdfsdf/sdfsdf/", name: "dfsdfsdfsdf", ext: "", dir: true, lastModified: new Date()});
//        data.push({path: "sdfsdfsdf/sdfsdf/", name: "dfsdfsdfsdf", ext: "", dir: true, lastModified: new Date()});
//        data.push({path: "sdfsdfsdf/sdfsdf/", name: "dfsdfsdfsdf", ext: "", dir: true, lastModified: new Date()});
//        data.push({path: "sdfsdfsdf/sdfsdf/", name: "dfsdfsdfsdf", ext: "", dir: true, lastModified: new Date()});
//        data.push({path: "sdfsdfsdf/sdfsdf/dfsdfsdfsdf.pdf", name: "dfsdfsdfsdf.pdf", ext: "pdf", dir: false, lastModified: new Date()});
//        dataModel.append(data);
    }
    
    onDataChanged: {
        dataModel.clear();
        if (data) {
            dataModel.append(data);
        }
        spinner.stop();
    }
    
    function stopSpinner() {
        spinner.stop();
    }
    
    function onDirCreated(dirname, path) {
        spinner.stop();
        if (path === root.path) {
            var dirObj = {path: path + dirname + "/", name: dirname, ext: "", dir: true, lastModified: new Date()};
            dataModel.insert(0, dirObj);
        }
    }
    
    function onDeletionRequested(name, path) {
        var dir = isDir(path);
        var comparablePath = getComparablePath(dir, name);
        if (path === comparablePath) {
            
            root.pathToDelete = path;
            root.fileOrDirToDelete = name;
            
            var doNotAsk = _appConfig.get("do_not_ask_before_deleting");
            if (doNotAsk && doNotAsk === "true") {
                spinner.start();
                _fileController.deleteFileOrDir(root.fileOrDirToDelete, root.pathToDelete);
            } else {
                deleteDialog.show();
            }
        }
    }
    
    function deleteFileOrDir(name, path) {
        spinner.stop();
        var dir = isDir(path);
        var comparablePath = getComparablePath(dir, name);
        if (path === comparablePath) {
            for (var i = 0; i < dataModel.size(); i++) {
                var d = dataModel.value(i);
                if (d.path === path) {
                    dataModel.removeAt(i);
                }
            }
        }
    }
    
    function getComparablePath(isDir, name) {
        return root.path + name + (isDir ? "/" : "");
    }
    
    function isDir(path) {
        return path.lastIndexOf("/") === path.length - 1;
    }
    
    function onSettingsChanged() {
        var dontAsk = _appConfig.get("do_not_ask_before_deleting");
        var dontAskBool = dontAsk !== "" && dontAsk === "true";
        deleteDialog.rememberMeChecked = dontAskBool;
    }
    
    function cleanUp() {
        _fileController.fileLoaded.disconnect(root.stopSpinner);
        _fileController.fileOpened.disconnect(root.stopSpinner);
        _fileController.dirCreated.disconnect(root.onDirCreated);
        _fileController.deletionRequested.disconnect(root.onDeletionRequested);
        _fileController.fileOrDirDeleted.disconnect(root.deleteFileOrDir);
        _appConfig.settingsChanged.disconnect(root.onSettingsChanged);
    }
}
