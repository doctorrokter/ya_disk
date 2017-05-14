import bb.cascades 1.4
import bb.device 1.4
import bb.system 1.2

CustomListItem {
    id: listItem
    
    property string currentPath: ""
    
    opacity: listItem.ListItem.selected ? 0.5 : 1.0
    
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
                if (ListItemData.previewPath) {
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
        if (!ListItemData.previewPath) {
            if (!ListItemData.dir) {
                return ui.palette.textOnPlain;
            }
            return ui.palette.primary;
        }
        return 0;
    }
    
    Container {
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        
        background: ui.palette.plain
        
        layout: DockLayout {}
        
        ImageView {
            id: preview
            imageSource: listItem.getImage();
            filterColor: listItem.filterColor();
            
            opacity: ListItemData.dir ? 0.25 : 1.0
            preferredWidth: ListItemData.dir || (ListItemData.previewPath !== undefined) ? listItemLUH.layoutFrame.width : ui.du(20)
            preferredHeight: {
                if (ListItemData.dir) {
                    return listItemLUH.layoutFrame.height - ui.du(2);
                } else if (ListItemData.previewPath !== undefined) {
                    return listItemLUH.layoutFrame.height;
                } else {
                    return ui.du(20);
                } 
            }
            verticalAlignment: {
                if (ListItemData.dir) {
                    return VerticalAlignment.Bottom;
                } else if (ListItemData.previewPath !== undefined) {
                    return VerticalAlignment.Fill;
                } else {
                    return VerticalAlignment.Center;
                }
            }
            horizontalAlignment: ListItemData.dir ? HorizontalAlignment.Fill : HorizontalAlignment.Center
        }
        
        ImageView {
            visible: !ListItemData.dir && ListItemData.previewPath === undefined
            imageSource: "asset:///images/opac_bg.png"
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            opacity: 0.5
        }
        
        Container {
            visible: !ListItemData.dir && ListItemData.previewPath !== undefined
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Bottom
            
            background: Color.Black
            preferredHeight: ui.du(8)
            
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
            
            text: _date.str(ListItemData.lastModified);
            verticalAlignment: VerticalAlignment.Bottom
            textStyle.color: ui.palette.textOnPrimary
            textStyle.base: SystemDefaults.TextStyles.SmallText
            textStyle.fontWeight: FontWeight.W100
            
            margin.leftOffset: ui.du(1);
            margin.bottomOffset: ui.du(1)
        }
        
        ImageView {
            visible: ListItemData.publicUrl !== undefined
            horizontalAlignment: HorizontalAlignment.Right
            verticalAlignment: VerticalAlignment.Top
            imageSource: "asset:///images/Splat_96x96.png"
            maxWidth: ui.du(5)
            maxHeight: ui.du(5)
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
        },
        
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
                        _fileController.currentPath = listItem.currentPath;
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
}
