import bb.cascades 1.4
import bb.device 1.4
import bb.system 1.2

CustomListItem {
    id: listItem
    
    property string currentPath: "/"
    
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
                        _fileController.currentPath = listItem.currentPath;
                        _fileController.clearSelectedFiles();
                        _fileController.selectFile(ListItemData);
                    }
                }
            ]
        }
    ]
}
