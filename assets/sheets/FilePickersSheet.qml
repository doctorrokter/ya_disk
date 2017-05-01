import bb.cascades 1.4
import bb.cascades.pickers 1.0

Sheet {
    id: sheet
    
    signal uploadStarted();
    
    property string targetPath: ""
    
    Page {
        id: page
        
        titleBar: TitleBar {
            title: qsTr("Choose a type") + Retranslate.onLocaleOrLanguageChanged
            
            dismissAction: ActionItem {
                title: qsTr("Cancel") + Retranslate.onLocaleOrLanguageChanged
                
                onTriggered: {
                    sheet.close();
                }
            }
        }
        
        ListView {
            id: pickersList
            
            dataModel: ArrayDataModel {
                id: pickersDataModel
            }
            
            listItemComponents: [
                ListItemComponent {
                    CustomListItem {
                        horizontalAlignment: HorizontalAlignment.Fill
                        Container {
                            horizontalAlignment: HorizontalAlignment.Fill
                            verticalAlignment: VerticalAlignment.Fill
                            layout: StackLayout {
                                orientation: LayoutOrientation.LeftToRight
                            }
                            
                            ImageView {
                                verticalAlignment: VerticalAlignment.Center
                                filterColor: Color.create(ListItemData.color)
                                imageSource: ListItemData.icon
                            }
                            
                            Label {
                                verticalAlignment: VerticalAlignment.Center
                                text: ListItemData.title
                                textStyle.base: SystemDefaults.TextStyles.TitleText
                            }
                        }
                    }
                }
            ]
            
            onTriggered: {
                var item = pickersDataModel.data(indexPath);
                switch (item.picker) {
                    case FileType.Picture: picturePicker.open(); break;
                    case FileType.Document: docPicker.open(); break;
                    case FileType.Music: musicPicker.open(); break;
                    case FileType.Video: videoPicker.open(); break;
                }
            }
            
            onCreationCompleted: {
                var data = [];
                data.push({color: "#779933", icon: "asset:///images/ic_doctype_picture.png", title: qsTr("Picture") + Retranslate.onLocaleOrLanguageChanged, picker: FileType.Picture});
                data.push({color: "#969696", icon: "asset:///images/ic_doctype_generic.png", title: qsTr("Document") + Retranslate.onLocaleOrLanguageChanged, picker: FileType.Document});
                data.push({color: "#8F3096", icon: "asset:///images/ic_doctype_music.png", title: qsTr("Music") + Retranslate.onLocaleOrLanguageChanged, picker: FileType.Music});
                data.push({color: "#FF3333", icon: "asset:///images/ic_doctype_video.png", title: qsTr("Video") + Retranslate.onLocaleOrLanguageChanged, picker: FileType.Video});
                pickersDataModel.append(data);
            }
        }
        
        attachedObjects: [
            FilePicker {
                id: picturePicker
                
                type: FileType.Picture
                title: qsTr("Select a file") + Retranslate.onLocaleOrLanguageChanged
                mode: FilePickerMode.PickerMultiple

                onFileSelected: {
                    page.startUpload(selectedFiles);
                }
            },
            
            FilePicker {
                id: docPicker
                
                type: FileType.Document
                title: qsTr("Select a file") + Retranslate.onLocaleOrLanguageChanged
                mode: FilePickerMode.PickerMultiple

                onFileSelected: {
                    page.startUpload(selectedFiles);
                }
            },
            
            FilePicker {
                id: musicPicker
                
                type: FileType.Music
                title: qsTr("Select a file") + Retranslate.onLocaleOrLanguageChanged
                mode: FilePickerMode.PickerMultiple
                
                onFileSelected: {
                    page.startUpload(selectedFiles);
                }
            },
            
            FilePicker {
                id: videoPicker
                
                type: FileType.Video
                title: qsTr("Select a file") + Retranslate.onLocaleOrLanguageChanged
                mode: FilePickerMode.PickerMultiple
                
                onFileSelected: {
                    page.startUpload(selectedFiles);
                }
            }
        ]
        
        function startUpload(selectedFiles) {
            sheet.uploadStarted();
            selectedFiles.forEach(function(f) {
                _fileController.upload(f, targetPath);
            });
        }
    }
    
    onTargetPathChanged: {
        console.debug("taget path: ", targetPath);
    }
    
    onClosed: {
        sheet.targetPath = "";
    }
}