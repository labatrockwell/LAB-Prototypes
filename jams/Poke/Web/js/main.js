var socket;

var messageDiv;
var button;
var textField;
var initialMessage = "";

// input buddies
var message, check, color;

var getQueryString = function (key){
	key = key.replace(/[\[]/,"\\\[").replace(/[\]]/,"\\\]");
	var regex = new RegExp("[\\?&]"+key+"=([^&#]*)");
	var qs = regex.exec(window.location.href);
	if(qs == null)
		return '';
	else
		return qs[1];
}

var stringMessage 	= {"type":"string", "data":""};
var intMessage	 	= {"type":"number", "data":0};
var boolMessage	 	= {"type":"boolean", "data":0};

$(document).ready( function() {
	setupSocket();

	//setup message sending button
	message = document.getElementById("message");
	check = document.getElementById("check");
	color = document.getElementById("color");

	button = document.getElementById("button");

	// send the form when you press enter 
	// or when you press the button
	button.onclick = function(e){
		sendMessageForm();
	};

	$("#message").keyup(function(event){
    	if(event.keyCode == 13){
    		sendMessageForm()
    	}
    })

	initialMessage = getQueryString("message");
});

// send value from text input
function sendMessageForm(){
	stringMessage.data = message.value;
	intMessage.data = color.value;
	boolMessage.data = check.checked ? 1 : 0;
	socket.send(stringMessage.data);
	//socket.send(JSON.stringify(stringMessage));
	//socket.send(JSON.stringify(intMessage));
	//socket.send(JSON.stringify(boolMessage));
	//message.value = "";
}

// setup web socket
function setupSocket(){

	// setup websocket
	// get_appropriate_ws_url is a nifty function by the libwebsockets people
	// it decides what the websocket url is based on the broswer url
	// e.g. https://mygreathost:9099 = wss://mygreathost:9099

	socket = new WebSocket(get_appropriate_ws_url(8888));
	
	// open
	try {
		socket.onopen = function() {		
		    if ( initialMessage != ""){
				//socket.send(initialMessage);    	
		    }
		} 

		// received message
		socket.onmessage =function got_packet(msg) {
			messageDiv.innerHTML = msg.data + "<br />" + messageDiv.innerHTML;
		}

		socket.onclose = function(){
		}
	} catch(exception) {
		alert('<p>Error' + exception);  
	}
}