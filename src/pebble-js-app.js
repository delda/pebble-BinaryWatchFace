// https://github.com/hallettj/Fuzzy-Text-International/blob/master/src/js/pebble-js-app.js
// https://github.com/pebble/pebble-sdk-examples/blob/master/pebblekit-js/weather/src/weather.c

// https://material.angularjs.org/latest/#/demo/material.components.button
// http://codepen.io/pen

function readyCallback(event){
  console.log('readyCallback()');
}

function appMessage(event){
  console.log('appMessage()');
}

function showConfiguration(event){
  console.log('showConfiguration()');
  var url = 'http://delda.altervista.org/index.html';
  Pebble.openURL(url);
}

function webViewClosed(event){
  console.log('webViewClosed');
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
  var shape = 0;
  var color = 0;

  console.log(settings.shape);
  switch(settings.shape){
    case 'cerchio':
      shape = 1;
      break;
    case 'rettangolo':
      shape = 2;
      break;
    case 'quadrato':
      shape = 3;
      break;
    default:
      shape = 0;
      break;
  }
  
  console.log(settings.color);
  switch(settings.color){
    case 'wh-bl':
      color = 1;
      break;
    case 'bl-wh':
      color = 2;
      break;
    default:
      color = 0;
  }
  
  console.log('Shape: ' + shape);
  console.log('Color: ' + color);
  return {
    '0': shape,
    '1': color,
  };
}


// Takes a JSON message as input.  Sends the message to the watch.
function transmitConfiguration(dictionary){
  console.log('trasmetted configuration: ' + JSON.stringify(dictionary));
  Pebble.sendAppMessage(dictionary, 
    function(e) {
      console.log('Send successful: ' + JSON.stringify(e));
    },
    function(e) {
      console.log('Send failed! ' + JSON.stringify(e));
    }
  );
}


Pebble.addEventListener("ready", readyCallback);
Pebble.addEventListener("appmessage", appMessage);
Pebble.addEventListener("showConfiguration", showConfiguration);
Pebble.addEventListener("webviewclosed", webViewClosed);