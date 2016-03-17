

webSocket = new WebSocket("ws://localhost:9090/test");


webSocket.onopen = function() {
   alert("webSocket.onopen, successful websocket connection made!");
};


webSocket.onclose = function(closeEvent) {
   //alert("webSocket.onclose");
};	

webSocket.onmessage = function(messageEvent) {
   alert("webSocket.onmessage");
};

webSocket.onerror = function() {
   //alert("webSocket.onerror");
};