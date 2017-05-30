import bb.cascades 1.4
import chachkouski.type 1.0

Page {
    id: root
    
    titleBar: TitleBar {
        title: qsTr("Downloads") + Retranslate.onLocaleOrLanguageChanged
    }
    
    Container {
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        
        ListView {
            id: downloadsListView
            
            dataModel: ArrayDataModel {
                id: downloadsDadaModel
            }
            
            listItemComponents: [
                ListItemComponent {
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
                                toValue: ListItemData.total
                                value: ListItemData.received
                            }
                        }                        
                    }
                }
            ]
        }
    }
    
    function addToQueue(queue) {
        downloadsDadaModel.append(queue);
    }
    
    function removeFromQueue(filename, path) {
        for (var i = 0; i < downloadsDadaModel.size(); i++) {
            var data = downloadsDadaModel.value(i);
            if (data.path === path) {
                downloadsDadaModel.removeAt(i);
            }
        }
    }
    
    function updateProgress(path, received, total) {
        for (var i = 0; i < downloadsDadaModel.size(); i++) {
            var data = downloadsDadaModel.value(i);
            if (data.path === path) {
                var newData = {
                    filename: data.filename, path: data.path, received: received, total: total
                };
                downloadsDadaModel.replace(i, newData);
            }
        }
    }
    
    function refresh() {
        downloadsDadaModel.clear();
        downloadsDadaModel.append(_fileController.downloader.queue);
    }
    
    function cleanUp() {
        _fileController.downloader.addedToQueue.disconnect(root.addToQueue);
        _fileController.downloader.finish.disconnect(root.removeFromQueue);
        _fileController.downloader.downloadProgress.disconnect(root.updateProgress);
    }
    
    onCreationCompleted: {
        _fileController.downloader.addedToQueue.connect(root.addToQueue);
        _fileController.downloader.finish.connect(root.refresh);
        _fileController.downloader.downloadProgress.connect(root.updateProgress);
        refresh();
    }
}
