import bb.cascades 1.4

Page {
    id: root
    
    titleBar: TitleBar {
        title: qsTr("Uploads") + Retranslate.onLocaleOrLanguageChanged
    }
    
    Container {
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        
        ListView {
            id: uploadsListView
            
            dataModel: GroupDataModel {
                id: uploadsDadaModel
                
                sortingKeys: ["targetPath"]
                grouping: ItemGrouping.ByFullValue
            }
            
            layout: StackListLayout {
                headerMode: ListHeaderMode.Sticky
            }
            
            listItemComponents: [
                ListItemComponent {
                    type: "header"
                    
                    Header {
                        title: ListItemData
                    }
                },
                
                ListItemComponent {
                    type: "item"
                    CustomListItem {
                        id: listItem
                        
                        Container {
                            horizontalAlignment: HorizontalAlignment.Fill
                            verticalAlignment: VerticalAlignment.Fill
                            
                            margin.leftOffset: ui.du(1)
                            margin.topOffset: ui.du(1)
                            margin.rightOffset: ui.du(1)
                            margin.bottomOffset: ui.du(1)
                            
                            Label {
                                text: ListItemData.filename
                                textStyle.base: SystemDefaults.TextStyles.SubtitleText
                            }
                            
                            ProgressIndicator {
                                id: progress
                                
                                fromValue: 0
                                toValue: 0
                            }
                        }
                        
                        function updateProgress(remoteUri, sent, total) {
                            if (ListItemData !== undefined && ListItemData.remoteUri === remoteUri) {
                                progress.value = sent;
                                if (progress.toValue === 0) {
                                    progress.toValue = total;
                                }
                            }
                        }
                        
                        function finishUpload(remoteUri) {
                            if (ListItemData !== undefined && ListItemData.remoteUri === remoteUri) {
                                var dataModel = listItem.ListItem.view.dataModel;
                                dataModel.remove(ListItemData);
                            }
                        }
                        
                        onCreationCompleted: {
                            _fileController.uploadProgress.connect(listItem.updateProgress);
                            _fileController.uploadFinished.connect(listItem.finishUpload);
                        }
                    }
                }
            ]
        }
    }
    
    function updateQueue(queue) {
        uploadsDadaModel.clear();
        uploadsDadaModel.insertList(queue);
    }
    
    function cleanUp() {
        _fileController.queueChanged.disconnect(root.updateQueue);
    }
    
    onCreationCompleted: {
//        var data = [];
//        data.push({filename: "sdfsdfsdf", targetPath: "/Documents/"});
//        data.push({filename: "sdfsdfsdf", targetPath: "/Documents/"});
//        data.push({filename: "sdfsdfsdf", targetPath: "/Documents/"});
//        data.push({filename: "sdfsdwefdsdffsdf", targetPath: "/Downloads/"});
//        data.push({filename: "sdfsdfsdfsdf3234", targetPath: "/Downloads/"});
//        data.push({filename: "sdfsdfssdfsdf", targetPath: "/Downloads/"});
//        data.push({filename: "sdfsdfsdf", targetPath: "/Documents/photo/"});
//        data.push({filename: "sdfsdfsdf", targetPath: "/Documents/photo/"});
//        data.push({filename: "sdfsdfsdf", targetPath: "/Documents/photo/"});
//        uploadsDadaModel.insertList(data);
        uploadsDadaModel.insertList(_fileController.queue);
        _fileController.queueChanged.connect(root.updateQueue);
    }
}
