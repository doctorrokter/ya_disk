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
                    id: deleteAction
                    
                    onTriggered: {
                        var data = ListItemData;
                        _fileController.requestDeletion(data.name, data.path);
                    }
                    
                    shortcuts: [
                        Shortcut {
                            key: "d"
                            
                            onTriggered: {
                                deleteAction.triggered();
                            }
                        }
                    ]
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
                    
                    shortcuts: [
                        SystemShortcut {
                            type: SystemShortcuts.Edit
                            
                            onTriggered: {
                                renameAction.triggered();
                            }
                        }
                    ]
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
                    
                    shortcuts: [
                        Shortcut {
                            key: "m"
                            
                            onTriggered: {
                                moveAction.triggered();
                            }
                        }
                    ]
                },
                
                ActionItem {
                    id: propsAction
                    title: qsTr("Properties") + Retranslate.onLocaleOrLanguageChanged
                    imageSource: "asset:///images/ic_properties.png"
                    
                    onTriggered: {
                        _fileController.showProps(ListItemData);
                    }
                    
                    shortcuts: [
                        Shortcut {
                            key: "p"
                            
                            onTriggered: {
                                propsAction.triggered();
                            }
                        }
                    ]
                },
                
                ActionItem {
                    id: publishAction
                    title: qsTr("Share") + Retranslate.onLocaleOrLanguageChanged
                    imageSource: "asset:///images/ic_share.png"
                    
                    onTriggered: {
                        _fileController.makePublic(ListItemData.path);
                    }
                    
                    shortcuts: [
                        Shortcut {
                            key: "s"
                            
                            onTriggered: {
                                publishAction.triggered();
                            }
                        }
                    ]
                }
            ]
            
        }
    ]
    
    function getImage() {
        if (!ListItemData.dir) {
            var ext = ListItemData.ext.toLowerCase();
            if (_file.isImage(ext)) {
                if (ListItemData.previewPath !== undefined) {
                    return ListItemData.previewPath;
                }
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
            if (ListItemData.previewPath === undefined) {
                return ui.palette.textOnPlain;
            } else {
                return 0;
            }
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
            
            layout: DockLayout {
                
            }
            
            ImageView {
                imageSource: listListItem.getImage();
                filterColor: listListItem.filterColor();
                
                opacity: ListItemData.dir ? 0.25 : 1.0
                preferredWidth: ui.du(11)
                preferredHeight: ui.du(11)
            }
            
            ImageView {
                visible: !ListItemData.dir && ListItemData.previewPath === undefined
                imageSource: "asset:///images/opac_bg.png"
                
                opacity: 0.5
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
            rightPadding: ui.du(1)
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
            
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                
                Label {
                    text: ListItemData.name
                    verticalAlignment: VerticalAlignment.Top
                    
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1
                    }
                }
                
                ImageView {
                    visible: ListItemData.publicUrl !== undefined
                    horizontalAlignment: HorizontalAlignment.Right
                    imageSource: "asset:///images/Splat_96x96.png"
                    maxWidth: ui.du(5)
                    maxHeight: ui.du(5)
                }
            }
            
            
            Label {
                visible: !ListItemData.dir
                verticalAlignment: VerticalAlignment.Bottom
                text: _date.str(ListItemData.lastModified);
                textStyle.base: SystemDefaults.TextStyles.SubtitleText
                textStyle.fontWeight: FontWeight.W100
            }
            
            Label {
                visible: !ListItemData.dir
                verticalAlignment: VerticalAlignment.Bottom
                horizontalAlignment: HorizontalAlignment.Right
                text: Number(ListItemData.size / (1024 * 1024)).toFixed(1) + " " + qsTr("MB") + Retranslate.onLocaleOrLanguageChanged
                textStyle.base: SystemDefaults.TextStyles.SubtitleText
                textStyle.fontWeight: FontWeight.W100
            }
            
            
        }
    }
}
