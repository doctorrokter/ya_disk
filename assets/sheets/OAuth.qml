import bb.cascades 1.4

Sheet {
    id: authPage
    
    property string clientId: "4dbce468bbc44a099ed8a2d433f84365"
    property string display: "popup"
    property string responseType: "token"
    
    signal accessTokeReceived(string accessToken, int expiresIn)
    
    function init() {
        webView.url = "https://oauth.yandex.ru/authorize?client_id=" + clientId + "&display=" + display + "&response_type=" + responseType;
    }
    
    Page {
        ScrollView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            
            scrollViewProperties {
                scrollMode: ScrollMode.Both
                pinchToZoomEnabled: true
            }
                        
            Container {
                WebView {
                    id: webView    
                    
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
}