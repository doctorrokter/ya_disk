import bb.cascades 1.4
import bb.system 1.2

CustomListItem {
    id: listListItem
    
    property string currentPath: "/"
    
    opacity: listListItem.ListItem.selected ? 0.5 : 1.0
    
    attachedObjects: [
        SystemPrompt {
            id: renamePrompt
            
            title: qsTr("Enter new name") + Retranslate.onLocaleOrLanguageChanged
            dismissAutomatically: true
            
            onFinished: {
                if (value === 2) {
                    var newName = renamePrompt.inputFieldTextEntry();
                    _fileController.rename(ListItemData.name, ListItemData.path, newName, ListItemData.dir, ListItemData.ext);
                }
            }    
        }
    ]
    
    contextActions: [
        ActionSet {
            
            actions: [
                DeleteActionItem {
                    onTriggered: {
                        var data = ListItemData;
                        _fileController.requestDeletion(data.name, data.path);
                    }
                },
                
                ActionItem {
                    id: renameAction
                    title: qsTr("Rename") + Retranslate.onLocaleOrLanguageChanged
                    imageSource: "asset:///images/ic_rename.png"
                    
                    onTriggered: {
                        if (!ListItemData.dir) {
                            var ext = "." + ListItemData.ext.toLowerCase();
                            var regEx = new RegExp(ext, "ig");
                            var name = ListItemData.name.replace(regEx, "");
                            renamePrompt.inputField.defaultText = name;
                        } else {
                            renamePrompt.inputField.defaultText = ListItemData.name;
                        }
                        renamePrompt.show();
                    }
                },
                
                ActionItem {
                    id: moveAction
                    title: qsTr("Move") + Retranslate.onLocaleOrLanguageChanged
                    imageSource: "asset:///images/ic_forward.png"
                    
                    onTriggered: {
                        _fileController.currentPath = listListItem.currentPath;
                        _fileController.clearSelectedFiles();
                        _fileController.selectFile(ListItemData);
                    }
                }
            ]
            
        }
    ]
    
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
            
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1
            }
            
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
