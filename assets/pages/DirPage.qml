import bb.cascades 1.4
import bb.device 1.4
import bb.system 1.2
import "../components"

Page {
    id: root
    
    signal loadPath(string dirName, string path)
    signal loadFile(string filename, string path)
    signal openFile(string filename, string path)
    signal upload(string path)
    signal uploadsPageRequested();
    
    property string dirName: "Root"
    property variant data: undefined
    property string path: "/Music/"
    property string fileOrDirToDelete: ""
    property string pathToDelete: ""
    property variant selectedFiles: []
    
    actionBarAutoHideBehavior: ActionBarAutoHideBehavior.HideOnScroll
    actionBarVisibility: ChromeVisibility.Overlay
    
    titleBar: TitleBar {
        title: dirName
    }
    
    Container {
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        
        layout: DockLayout {}
        
        Container {
            verticalAlignment: VerticalAlignment.Center
            
            Header {
                id: header
                visible: path !== "/"
                title: path
            }
            
            FilesMover {}
            
            ListView {
                id: listView
                scrollRole: ScrollRole.Main
                
                property string currentPath: root.path
                
                verticalAlignment: VerticalAlignment.Bottom
                
                margin.leftOffset: ui.du(0.5)
                margin.rightOffset: ui.du(0.5)
                
                dataModel: ArrayDataModel {
                    id: dataModel
                    
                    onItemAdded: {
                        root.loadPreview(data(indexPath));
                    }
                }
                
                layout: {
                    var view = _appConfig.get("files_view");
                    if (view ==="" || view === "grid") {
                        return gridListLayout;
                    }
                    return stackListLayout;
                }
                
                multiSelectAction: MultiSelectActionItem {}
                
                multiSelectHandler {
                    status: "0 " + (qsTr("files") + Retranslate.onLocaleOrLanguageChanged)
                    actions: [
                        DeleteActionItem {
                            id: multiDeleteAction
                            
                            onTriggered: {
                                var data = [];
                                listView.selectionList().forEach(function(indexPath) {
                                        data.push(dataModel.data(indexPath));
                                });
                            root.selectedFiles = data;
                            
                            var doNotAsk = _appConfig.get("do_not_ask_before_deleting");
                            if (doNotAsk && doNotAsk === "true") {
                                deleteSelectedFiles();
                            } else {
                                deleteMultipleDialog.show();
                            }
                            }
                        },
                        
                        ActionItem {
                            id: moveAction
                            title: qsTr("Move") + Retranslate.onLocaleOrLanguageChanged
                            imageSource: "asset:///images/ic_forward.png"
                            
                            onTriggered: {
                                _fileController.currentPath = root.path;
                                listView.selectionList().forEach(function(indexPath) {
                                    _fileController.selectFile(dataModel.data(indexPath));
                                });
                            }
                        }
                    ]
                }
                
                onSelectionChanged: {
                    if (selectionList().length > 1) {
                        multiSelectHandler.status = selectionList().length + " " + (qsTr("files") + Retranslate.onLocaleOrLanguageChanged);
                    } else if (selectionList().length == 1) {
                        multiSelectHandler.status = "1 " + (qsTr("file") + Retranslate.onLocaleOrLanguageChanged);
                    } else {
                        multiSelectHandler.status = qsTr("None selected") + Retranslate.onLocaleOrLanguageChanged;
                    }
                }
                
                onTriggered: {
                    var data = dataModel.data(indexPath);
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
                        StackListItem {
                            currentPath: ListItem.view.currentPath
                        }
                    },
                    
                    ListItemComponent {
                        type: "gridItem"
                        GridListItem {
                            currentPath: ListItem.view.currentPath
                        }
                    }
                ]
            }
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
        },
        
        SystemDialog {
            id: deleteMultipleDialog
            
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
                    _appConfig.set("do_not_ask_before_deleting", deleteDialog.rememberMeSelection() + "");
                    deleteSelectedFiles();
                } else {
                    root.selectedFiles = [];
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
            
            shortcuts: [
                SystemShortcut {
                    type: SystemShortcuts.CreateNew
                    
                    onTriggered: {
                        createDirAction.triggered();
                    }
                }
            ]
        },
        
        ActionItem {
            id: uploadAction
            title: qsTr("Upload") + Retranslate.onLocaleOrLanguageChanged
            imageSource: "asset:///images/ic_upload.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            
            onTriggered: {
                upload(root.path);
            }
            
            shortcuts: [
                Shortcut {
                    key: "u"
                    
                    onTriggered: {
                        uploadAction.triggered();
                    }
                }
            ]
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
                } else {
                    _appConfig.set("files_view", "grid");
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
        _fileController.fileUploaded.connect(root.onFileUploaded);
        _fileController.fileRenamed.connect(root.onFileRenamed);
        _fileController.fileMoved.connect(root.onFileMoved);
        _fileController.previewLoaded.connect(root.setPreview);
        _appConfig.settingsChanged.connect(root.onSettingsChanged);
//        
//        var data = [];
//        data.push({path: "sdfsdfsdf/sdfsdf/", name: "dfsdfsdfsdf", ext: "", dir: true, lastModified: new Date()});
//        data.push({path: "sdfsdfsdf/sdfsdf/", name: "dfsdfsdfsdf", ext: "", dir: true, lastModified: new Date()});
//        data.push({path: "sdfsdfsdf/sdfsdf/", name: "dfsdfsdfsdf", ext: "", dir: true, lastModified: new Date()});
//        data.push({path: "sdfsdfsdf/sdfsdf/", name: "dfsdfsdfsdf", ext: "", dir: true, lastModified: new Date()});
//        data.push({path: "sdfsdfsdf/sdfsdf/dfsdfsdfsdf.pdf", name: "dfsdfsdfsdf.jpg", ext: "jpg", dir: false, lastModified: new Date()});
//        dataModel.append(data);
    }
    
    onDataChanged: {
        dataModel.clear();
        if (data) {
            data.forEach(function(f) {
                dataModel.append(f);
            });
        }
        spinner.stop();
    }
    
    function loadPreview(file) {
        if (!file.dir && _file.isImage(file.ext.toLowerCase())) {
            _fileController.loadPreview(file.name, file.path);
        }
    }
    
    function setPreview(path, localPath) {
        for (var i = 0; i < dataModel.size(); i++) {
            var f = dataModel.value(i);
            if (_file.isImage(f.ext)) {
                if (f.path === path && localPath.indexOf(f.name) !== -1) {
                    f.previewPath = "file://" + localPath;
                    dataModel.replace(i, f);
                }
            }
        }
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
    
    function deleteSelectedFiles() {
        spinner.start();
        root.selectedFiles.forEach(function(file) {
            _fileController.deleteFileOrDir(file.name, file.path);
        });
        root.selectedFiles = [];
        spinner.stop();
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
        
        var view = _appConfig.get("files_view");
        var allItems = [];
        for (var i = 0; i < dataModel.size(); i++) {
            allItems.push(dataModel.value(i));
        }
        dataModel.clear();
        
        if (view === "" || view === "grid") {
            viewActionItem.imageSource = "asset:///images/ic_view_list.png";
            viewActionItem.title = qsTr("List") + Retranslate.onLocaleOrLanguageChanged
            listView.layout = gridListLayout;
        } else {
            viewActionItem.imageSource = "asset:///images/ic_view_grid.png";
            viewActionItem.title = qsTr("Grid") + Retranslate.onLocaleOrLanguageChanged
            listView.layout = stackListLayout;
        }
        dataModel.append(allItems);
    }
    
    function onFileUploaded(targetPath, file) {
        if (root.path === targetPath) {
            var exists = false;
            for (var i = 0; i < dataModel.size(); i++) {
                if (dataModel.value(i).path === file.path) {
                    exists = true;
                }
            }
            if (!exists) {
                dataModel.append(file);
            }
        }
    }
    
    function onFileRenamed(prevName, prevPath, newName, newPath) {
        spinner.stop();
        for (var i = 0; i < dataModel.size(); i++) {
            var data = dataModel.value(i);
            if (data.path === prevPath) {
                var newData = {};
                newData.ext = data.ext;
                newData.name = newName;
                newData.path = newPath;
                newData.lastModified = new Date();
                newData.dir = data.dir;
                dataModel.replace(i, newData);
            }
        }
    }
    
    function onFileMoved(name, prevPath, newPath, currentPath, isDir, ext) {
        if (root.path === currentPath) {
            var data = {};
            data.name = name;
            data.path = newPath;
            data.ext = ext;
            data.dir = isDir;
            data.lastModified = new Date();
            dataModel.append(data);
        }
        
        if (root.path === _fileController.currentPath) {
            for (var i = 0; i < dataModel.size(); i++) {
                if (dataModel.value(i).path === prevPath) {
                    dataModel.removeAt(i);
                }
            }
        }
    }
    
    function cleanUp() {
        _fileController.fileLoaded.disconnect(root.stopSpinner);
        _fileController.fileOpened.disconnect(root.stopSpinner);
        _fileController.dirCreated.disconnect(root.onDirCreated);
        _fileController.deletionRequested.disconnect(root.onDeletionRequested);
        _fileController.fileOrDirDeleted.disconnect(root.deleteFileOrDir);
        _fileController.fileUploaded.disconnect(root.onFileUploaded);
        _fileController.fileRenamed.disconnect(root.onFileRenamed);
        _fileController.fileMoved.disconnect(root.onFileMoved);
        _fileController.previewLoaded.disconnect(root.setPreview);
        _appConfig.settingsChanged.disconnect(root.onSettingsChanged);
    }
}
