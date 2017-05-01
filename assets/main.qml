/*
 * Copyright (c) 2011-2015 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import bb.cascades 1.4
import "sheets"
import "pages"

NavigationPane {
    id: navPane
    
    Menu.definition: MenuDefinition {
        settingsAction: SettingsActionItem {
            onTriggered: {
                var sp = settingsPage.createObject();
                navPane.push(sp);
                Application.menuEnabled = false;
            }
        }
    }
    
    onPopTransitionEnded: {
        if (page.cleanUp !== undefined) {
            page.cleanUp();
        }
        page.destroy();
        Application.menuEnabled = true;
    }
    
    onCreationCompleted: {
        if (!_app.hasToken()) {
            oauth.init();
            oauth.open();
        }
        _fileController.dataLoaded.connect(newPage);
    }
    
    attachedObjects: [
        OAuth {
            id: oauth
            
            onAccessTokeReceived: {
                oauth.close();
                _app.setToken(accessToken, expiresIn);
            }
        },
        
        FilePickersSheet {
            id: pickersSheet
            
            onUploadStarted: {
                pickersSheet.close();
                navPane.push(uploadsPage.createObject());
            }
        },
        
        ComponentDefinition {
            id: uploadsPage
            UploadsPage {}    
        },
        
        ComponentDefinition {
            id: settingsPage
            SettingsPage {}    
        },
        
        ComponentDefinition {
            id: dirPage
            DirPage {
                onLoadPath: {
                    var dp = dirPage.createObject();
                    dp.path = path;
                    dp.dirName = dirName;
                    navPane.push(dp);
                    _fileController.loadPath(path);
                }
                
                onLoadFile: {
                    _fileController.loadFile(filename, path);
                }
                
                onOpenFile: {
                    _fileController.openFile(filename, path);
                }
                
                onUpload: {
                    pickersSheet.targetPath = path;
                    pickersSheet.open();
                }
                
                onUploadsPageRequested: {
                    navPane.push(uploadsPage.createObject());
                }
            }
        }
    ]
    
    function newPage(data) {
        if (navPane.count() === 0) {
            var dp = dirPage.createObject();
            dp.data = data;
            dp.path = "/";
            dp.dirName = qsTr("Root") + Retranslate.onLocaleOrLanguageChanged
            navPane.push(dp);
        } else {
            var dp = navPane.at(navPane.count() - 1);
            dp.data = data;
        }
    }
}

