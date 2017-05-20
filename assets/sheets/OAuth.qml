import bb.cascades 1.4
import bb.device 1.4

Sheet {
    id: authPage
    
    property string clientId: "4dbce468bbc44a099ed8a2d433f84365"
    property string display: "popup"
    property string responseType: "token"
    
    property string signInUrl: "https://oauth.yandex." + _yaDomain + "/authorize?client_id=" + clientId + 
        "&display=" + display + 
        "&response_type=" + responseType + 
        "&device_id=" + _deviceId.replace("{", "").replace("}", "") + 
        "&device_name=" + hardware.deviceName
    
    property string registrationUrl: "https://passport.yandex." + _yaDomain + "/registration"
    
    signal accessTokeReceived(string accessToken, int expiresIn)
    
    Page {
        ScrollView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            
            Container {
                WebView {
                    id: webView
                    settings.customHttpHeaders: {
                        "User-Agent": "Mozilla/5.0 (Linux; Android 5.0; SM-G900P Build/LRX21T) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Mobile Safari/537.36"
                    }
                    
                    onUrlChanged: {
                        console.debug(url);
                        var urlStr = url + "";
                        if (urlStr.indexOf("#access_token") !== -1) {
                            var queryArray = urlStr.split("#")[1].split("&");
                            authPage.accessTokeReceived(queryArray[0].split("=")[1], parseInt(queryArray[2].split("=")[1]));
                        }
                    }
                }
            }
        }
    }
    
    onOpened: {
        webView.url = authPage.signInUrl;
    }
    
    attachedObjects: [
        HardwareInfo {
            id: hardware
        }
    ]
}