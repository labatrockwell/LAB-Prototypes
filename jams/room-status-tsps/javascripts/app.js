/* tsps protocol */
var socket;
var outputDiv;
var TSPSObjects = [];
var canvas;
var ctx;

var vid_width       = 640;
var vid_height      = 480;

var tsps;


var entered_room = {};
var left_room = {};
var _present = {};

/**
* CouchDB Connection
* we store room status in couchdb
* this allows us to later visualize activity over time
* the couchdb _changes feed can be used to trigger "on air" lights, mute music, etc.
*/

// send via any same-domain proxy to prevent cross-domain blocking
$.couch.urlPrefix = "proxy/http://localhost:5984";
var db = $.couch.db("presence");


/**
* Samples
* 
* we don't need the full openTSPS speed
* only sample data every half second
*/
function sample() {

    var did_change = false; // any updates since last sample?

    var before_count = Object.size(_present); // how many people were here before?


    // update the room to reflect latest activity
    for (var i in entered_room) {
        var new_person = entered_room[i];
        _present[new_person.id] = new_person;
        did_change = true;
    }

    for (var j in left_room) {
        var old_person = left_room[j];
        delete _present[old_person.id];            
        did_change = true;
    }

    var after_count = Object.size(_present); // how many people are here now?
    var delta_count = after_count - before_count; // what is the people delta?

    if (did_change) {
        var people_count = Object.size(_present); // number of people present

        console.log("------------- SAMPLE -------------");                
        console.log("Now Active: ", people_count);
        console.log("Presence Delta: ", delta_count);

        if (people_count >= 2) {

            // the greater the room activity, the more red the screen will turn
            $("body").css("background", "rgba(255, 0,0," + people_count*0.1 +")");


            // build a couchdb document to remember activity
            var people = {};
            for (var idx in _present) {
                people[idx] = _present[idx].age;
            }
            var doc = {
                created_at: new Date().getTime(),
                people: people
            };

            // save activity to database for later stats
            db.saveDoc(doc);
        }
        else {
            $("body").css("background", "white");
        }
    }

    // clear it out for next sample
    entered_room = [];
    left_room = [];
}



$(document).ready( function() {
    outputDiv   = document.getElementById('output');
    canvas      = document.getElementById('contourCanvas');
    canvas.width = vid_width;
    canvas.height = vid_height;
    ctx         = canvas.getContext('2d');
    ctx.fillStyle = 'rgba(0,0,0,0)';

    // connect with default settings
    tsps = new TSPS.Connection("lab-mini-06.local");
    tsps.connect();
    
    // render each time we get a message
    tsps.onMessageReceived = render;

    // track all people who enter field of vision
    tsps.onPersonEntered = function(data) {
        entered_room[data.id] = data;
    };

    // track all people who leave the field of vision
    tsps.onPersonLeft = function(data) {
        left_room[data.id] = data;
    };

    // sample every half-second
    setInterval(sample, 500);

});

function render(){
    canvas.width = canvas.width;
    // draw contours
    for (var id in tsps.people){

        var person = tsps.people[id];

        // draw rect
        ctx.strokeStyle = 'black';
        ctx.strokeRect(person.boundingrect.x*vid_width,person.boundingrect.y*vid_height, person.boundingrect.width*vid_width, person.boundingrect.height*vid_height);
        ctx.stroke();

        // draw contours
        ctx.fillStyle = '#111111';
        ctx.font      = "11pt Helvetica";
        ctx.fillText("id: "+person.id, person.centroid.x*vid_width, person.centroid.y*vid_height);
        ctx.fillText("age: "+person.age, person.centroid.x*vid_width, person.centroid.y*vid_height+20);
        ctx.fillText("depth: "+person.depth, person.centroid.x*vid_width, person.centroid.y*vid_height+40);
        ctx.strokeStyle = '#ff0000';
        ctx.beginPath();
        ctx.moveTo(person.contours[0].x*vid_width,person.contours[0].y*vid_height);

        for (var j=1; j<person.contours.length; j++ ){
            ctx.lineTo( person.contours[j].x*vid_width,person.contours[j].y*vid_height );
        }               
        ctx.stroke();
    }
}
