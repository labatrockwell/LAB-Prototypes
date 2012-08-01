getNextStream = function() {
	var msg = {};
	msg["msg"] = "play";

	socket.send( JSON.stringify(msg));
}

startPlaying = function() {
	playNext( playing );
}

audioEnded = function() {
	console.log( "length: " + audios.length );
	console.log( "playing: " + playing );

	playing++;

	playNext( playing );
}

playNext = function() {
	/*
		this shoud be here, but...
		it wasn't working with it...
	*/
	//if( playing < audios.length ) {
		audios[playing].play();
	//}
}

createSocket = function() {
	socket = new WebSocket("ws://localhost:8888/audio");

	socket.onopen = function() { console.log("open..."); }
	socket.onclose = function() { console.log("closed."); }

	socket.onmessage = function(msg){  
		/*
			on message, create new audio objects
			push to array
		*/
    	var data = "data:audio/wav;base64,"+msg.data;
		temp = new Audio(data); // create the HTML5 audio element
		temp.addEventListener('ended', audioEnded, false);
		audios.push( temp );

		/*
			once you've loaded the audio
			go get the next item from the server
		*/
		getNextStream();
		
		/*
			stack up at least 5 audio clips
			then start playing
		*/
		if( audios.length > 5 ) {
			startPlaying();
		}
	}
}

var socket;
var audios = Array();
var playing = 0;
$(document).ready( function() {
	$("#play").click( function( event ) {
		event.preventDefault();

		getNextStream();
	});

	createSocket();
})