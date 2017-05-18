import bb.cascades 1.4
import bb.system 1.2
import "../components"

Page {
    id: root
    
    property bool dir: false
    property string name: ""
    property string path: ""
    property string ext: ""
    property int size: 0
    property int contentSize: 0
    property date lastModified: new Date()
    property string publicUrl: "wfwerwe"
    
    signal propertiesDone()
    
    titleBar: TitleBar {
        title: qsTr("Properties") + Retranslate.onLocaleOrLanguageChanged
        
        acceptAction: ActionItem {
            title: qsTr("Done") + Retranslate.onLocaleOrLanguageChanged
            
            onTriggered: {
                propertiesDone();
            }
        }
    }
        
    ScrollView {
        Container {
            id: imageHolder
                
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            
            Container {
                horizontalAlignment: HorizontalAlignment.Fill
                background: ui.palette.plain
                preferredHeight: ui.du(26)
                
                Container {
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center
                    maxWidth: ui.du(22)
                    maxHeight: ui.du(26)
                    background: ui.palette.background
                    
                    margin.topOffset: ui.du(2)
                    
                    Container {
                        horizontalAlignment: HorizontalAlignment.Center
                        maxWidth: ui.du(21)
                        maxHeight: ui.du(20)
                        background: ui.palette.plain
                        
                        margin.leftOffset: ui.du(1)
                        margin.topOffset: ui.du(1)
                        margin.rightOffset: ui.du(1)
                        margin.bottomOffset: ui.du(1)
                        
                        layout: DockLayout {}
                        
                        ImageView {
                            id: preview
                            
                            imageSource: {
                                if (root.dir) {
                                    return "asset:///images/ic_folder.png";
                                } else {
                                    var ext = root.ext.toLowerCase();
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
                            }
                            preferredWidth: ui.du(21)
                            preferredHeight: ui.du(20)
                            filterColor: {
                                if (root.dir) {
                                    return ui.palette.primary;
                                }
                                return ui.palette.textOnPlain;
                            }
                            opacity: root.dir ? 0.25 : 1.0
                        }
                        
                        ImageView {
                            id: opacBackground
                            visible: !root.dir && !_file.isImage(root.ext)
                            imageSource: "asset:///images/opac_bg.png"
                            opacity: 0.5
                        }
                    }  
                }    
            }
            
            Header {
                title: {
                    if (root.dir) {
                        return qsTr("Folder properties") + Retranslate.onLocaleOrLanguageChanged
                    }
                    return qsTr("File properties") + Retranslate.onLocaleOrLanguageChanged
                }
            }
            
            PropListItem {
                name: qsTr("Filename") + Retranslate.onLocaleOrLanguageChanged + ":"
                value: root.name
            }
            
            PropListItem {
                visible: !root.dir
                name: qsTr("Type") + Retranslate.onLocaleOrLanguageChanged + ":"
                value: root.ext.toLowerCase()
            }
            
            PropListItem {
                name: qsTr("Last modified") + Retranslate.onLocaleOrLanguageChanged + ":"
                value: _date.str(root.lastModified);
            }
            
            PropListItem {
                visible: !root.dir
                name: qsTr("Size") + Retranslate.onLocaleOrLanguageChanged + ":"
                value: Number(root.size / (1024 * 1024)).toFixed(1) + " " + qsTr("MB") + Retranslate.onLocaleOrLanguageChanged
            }
            
            PropListItem {
                name: qsTr("Placement") + Retranslate.onLocaleOrLanguageChanged + ":"
                value: root.path
            }
            
            PropListItem {
                visible: root.publicUrl !== ""
                name: qsTr("Public URL") + Retranslate.onLocaleOrLanguageChanged + ":"
                value: root.publicUrl
            }
            
            Container {
                visible: root.publicUrl !== ""
                horizontalAlignment: HorizontalAlignment.Fill
                
                leftPadding: ui.du(1)
                topPadding: ui.du(1)
                rightPadding: ui.du(1)
                bottomPadding: ui.du(1)
                
                Button {
                    horizontalAlignment: HorizontalAlignment.Fill
                    text: qsTr("Copy link to clipboard") + Retranslate.onLocaleOrLanguageChanged
                    
                    onClicked: {
                        if (_app.copyToClipboard(root.publicUrl)) {
                            toast.body = qsTr("Copied to clipboard") + Retranslate.onLocaleOrLanguageChanged;
                            toast.position = SystemUiPosition.MiddleCenter
                            toast.show();
                        }
                    }
                }
            }
        }
    }
    
    function setPreview(path, previewPath) {
        if (_file.isImage(root.ext)) {
            var localPath = "file://" + previewPath;
            preview.imageSource = localPath;
            opacBackground.visible = false;
        }
    }
    
    function cleanUp() {
        _fileController.previewLoaded.disconnect(root.setPreview);
        _fileController.publicityChecked.disconnect(root.setPublicUrl);
    }
    
    onCreationCompleted: {
        _fileController.previewLoaded.connect(root.setPreview);
        _fileController.publicityChecked.connect(root.setPublicUrl);
    }
    
    onPathChanged: {
        _fileController.loadPreview(root.name, root.path);
        _fileController.checkPublicity(root.path, root.dir);
    }
    
    function setPublicUrl(path, publicUrl) {
        root.publicUrl = publicUrl;
    }
    
    attachedObjects: [
        DateTimePicker {
            id: dateTimePicker
            mode: DateTimePickerMode.Time
            value: root.lastModified
        },
        
        SystemToast {
            id: toast
        }
    ]
}
