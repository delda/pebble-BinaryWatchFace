// https://github.com/hallettj/Fuzzy-Text-International/blob/master/src/js/pebble-js-app.js
// https://github.com/pebble/pebble-sdk-examples/blob/master/pebblekit-js/weather/src/weather.c

// https://material.angularjs.org/latest/#/demo/material.components.button
// http://codepen.io/pen

function readyCallback(event){
  console.log('JavaScript app ready and running!');
}
//function appMessage(event){}
function showConfiguration(event){
  var url = 'http://delda.altervista.org/index.html';
  Pebble.openURL(url);
}

function webViewClosed(event){
  var resp = event.response;
  localStorage.setItem("options", resp);
  
  var message = prepareConfiguration(resp);
  transmitConfiguration(message);
}

// Takes a string containing serialized JSON as input.  This is the
// format that is sent back from the configuration web UI.  Produces
// a JSON message to send to the watch face.
function prepareConfiguration(serialized_settings){
  console.log('prepareConfiguration');
  var settings = JSON.parse(serialized_settings);
  var shape = 1;

  switch(settings.shape){
    case 'cerchio':
      shape = 0;
      break;
    case 'rettangolo':
      shape = 1;
      break;
    case 'quadrato':
      shape = 2;
      break;
  }
  
  return {
    "0": shape,
  };
}


// Takes a JSON message as input.  Sends the message to the watch.
function transmitConfiguration(dictionary){
  console.log('transmitConfiguration');
  Pebble.sendAppMessage(dictionary);
}

Pebble.addEventListener("ready", readyCallback);
//Pebble.addEventListener("appmessage", appMessage);
Pebble.addEventListener("showConfiguration", showConfiguration);
Pebble.addEventListener("webviewclosed", webViewClosed);