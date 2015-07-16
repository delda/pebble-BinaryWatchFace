
var shape;

function readConfig(){
  console.log('readConfig()');
  shape = localStorage.getItem('shape');
  console.log("shape1: " + shape);
  shape = shape ? shape : 0;
  console.log("shape2: " + shape);
}

function readyCallback(event){
  console.log('readyCallback()');
  readConfig();
  var jsonConfig = JSON.parse('{"shape":'+shape+'}');
  Pebble.sendAppMessage(jsonConfig);
}

function appMessage(event){
  console.log('appMessage()');
}

function showConfiguration(event){
  console.log('showConfiguration()');
  readConfig();
  var attributes = 'shape='+shape;
  var url = 'http://delda.github.io/pebble_binaryWatch/index.html?'+attributes;
  console.log("url: " + url);
  Pebble.openURL(url);
}

function webViewClosed(event){
  console.log('webViewClosed');
  var resp = event.response;  
  var message = prepareConfiguration(resp);
  transmitConfiguration(message);
  
  setLocalStorage(resp);
}

// Takes a string containing serialized JSON as input.  This is the
// format that is sent back from the configuration web UI.  Produces
// a JSON message to send to the watch face.
function prepareConfiguration(serialized_settings){
  console.log('prepareConfiguration');
  var settings = JSON.parse(serialized_settings);
  var shape = 0;
  var color = 0;

  shape = (settings.shape) ? Number(settings.shape) : 0;
  
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

function setLocalStorage(settings){
  console.log('setLocalStorage()');

  var obj = JSON.parse(decodeURIComponent(settings));

  console.log('shape: ' + obj.shape);
  shape = obj.shape;
  localStorage.setItem('shape', shape);
}

Pebble.addEventListener("ready", readyCallback);
Pebble.addEventListener("appmessage", appMessage);
Pebble.addEventListener("showConfiguration", showConfiguration);
Pebble.addEventListener("webviewclosed", webViewClosed);