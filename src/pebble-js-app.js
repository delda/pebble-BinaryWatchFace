
var PBL_COLOR, shape, color;

function readConfig(){
  console.log('readConfig()');
  shape = localStorage.getItem('shape');
  shape = shape ? shape : 0;
  color = color ? color : 0;
}

function readyCallback(event){
  console.log('readyCallback()');
  readConfig();
  var jsonConfig = JSON.parse('{"shape":'+shape+',"color":'+color+'}');
  Pebble.sendAppMessage(jsonConfig);
}

function appMessage(event){
  console.log('appMessage()');
}

function showConfiguration(event){
  console.log('showConfiguration()');
  readConfig();
  var attributes = '?shape='+shape+'&color='+color;
  var url = 'http://delda.github.io/pebble_binaryWatch/index.html'+attributes;
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

// Format sent back from the configuration web UI
function prepareConfiguration(serialized_settings){
  console.log('prepareConfiguration');
  var settings = JSON.parse(serialized_settings);
  var shape = 0;
  var color = 0;

  shape = (settings.shape) ? Number(settings.shape) : 0;
  color = (settings.color) ? Number(settings.color) : 0;
  
  return {
    '0': shape,
    '1': color,
  };
}

// Sends the message to the watch.
function transmitConfiguration(dictionary){
  console.log('setLocalStorage()');
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
  
  shape = obj.shape;
  color = obj.color;
  
  localStorage.setItem('shape', shape);
  localStorage.setItem('color', color);
}

Pebble.addEventListener("ready", readyCallback);
Pebble.addEventListener("appmessage", appMessage);
Pebble.addEventListener("showConfiguration", showConfiguration);
Pebble.addEventListener("webviewclosed", webViewClosed);