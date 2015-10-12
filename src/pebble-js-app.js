var options = {shape: 0, color: 0, number: 1, bluetooth: 2, battery: 2, battery_modality: 0, date: 23, help_num: 1};
var version = '3.1';
var DEBUG = true;

options.platform = getPlatform();
options.locale = getLocale();

var AppOptions = {
  version: version,
  platform: 0,
  locale: 'en_EN',
  extend: function(extentions){
    if(DEBUG) console.log('AppOptions.extend()');
    for(prop in extentions){
      if(DEBUG) console.log('extend: ' + prop + '=' + extentions[prop]);
      this[prop] = extentions[prop];
    }
    return this;
  },
  overwriteLocalStorage: function(){
    if(DEBUG) console.log('AppOptions.overwriteLocalStorage()');
    for(prop in this){
      if(localStorage.key(prop)){
        if(DEBUG) console.log('override: ' + prop + '=' + this[prop]);
        this[prop] = localStorage.getItem(prop);
      }
    }
    return this;
  },
  getObjOptions: function(){
    if(DEBUG) console.log('AppOptions.getObjOptions()');
    var obj = {};
    var i = 0;
    for(prop in this){
      if(DEBUG) console.log('getObjOptions: ' + typeof this[prop]);
      if(typeof this[prop] !== 'function'){
        if(DEBUG) console.log('getObjOptions: ' + this[prop]);
        obj[i] = this[prop];
        i++;
      }
    }
    return obj;
  },
  getUrlOptions: function(){
    if(DEBUG) console.log('AppOptions.url()');
    var url = '';
    var i = 0;
    for(prop in this){
      url += (i == 0) ? '&' : '?';
      url += prop+'='+this[prop];
    }
    return url;
  },
  prepareConfiguration: function(serialized_settings){
    if(DEBUG) console.log('AppOptions.prepareConfiguration()');
    var obj = {};
    var settings = JSON.parse(serialized_settings);
    for(prop in settings){
      obj[prop] = Number(settings[prop]);
    }
    return obj;
  },
  sendConfiguration: function(dataMessage){
    Pebble.sendAppMessage(
      dataMessage,
      function(e) {
        console.log('Successfully delivered message with transactionId=' + e.data.transactionId);
      },
      function(e) {
        console.log('Unable to deliver message with transactionId=' + e.data.transactionId + ' Error is: ' + e.error.message);
      }
    );
  },
  setLocalStorage: function(settings){
    var obj = JSON.parse(decodeURIComponent(settings));
    for(prop in obj){
      localStorage.setItem(prop, obj[prop]);
    }
  },
};

function getPlatform(){
  var current_watch;
  if(Pebble.getActiveWatchInfo){
    try{
      current_watch = Pebble.getActiveWatchInfo();
    }catch(err){
      current_watch = { platform: "basalt", };
    }
  }else{
    current_watch = { platform: "aplite", };
  }
  if(current_watch.platform === 'aplite'){
    if(DEBUG) console.log('PebbleKit JS ready on Aplite!');
    return 0;
  }else{
    if(DEBUG) console.log('PebbleKit JS ready on Basalt!');
    return 1;
  }
}

function getLocale(){
  if(Pebble.getActiveWatchInfo) {
    try {
      var watch = Pebble.getActiveWatchInfo();
      return watch.language;
    }catch(err){
      if(DEBUG) console.log("Pebble.getActiveWatchInfo(); Error!");
    }
  }
}

// App configuration
// The watchapp has been launched and the PebbleKit JS component is now ready to receive events
function readyCallback(e){
  if(DEBUG) console.log('readyCallback()');
  
  var dataMessage = AppOptions.extend(options).overwriteLocalStorage().getObjOptions();
  transmitConfiguration(dataMessage);
}
// The watch sent an AppMessage to PebbleKit JS
function appMessageCallback(e){
  console.log('Received message: ' + JSON.stringify(e.payload));
}
// The user requests to display the app's configuration webview
function showConfigurationCallback(e){
  if(DEBUG) console.log('showConfigurationCallback()');

  var urlString = AppOptions.extend(options).overwriteLocalStorage().getUrlOptions();
  var url = 'http://delda.github.io/pebble_binaryWatch/index.html'+urlString;
  if(DEBUG) console.log("url: " + url);
  Pebble.openURL(url);
}
// The user closes the configuration webview
function webViewClosedCallback(e){
  var resp = e.response;
  if(DEBUG) console.log('Configuration window returned: ' + resp);
  var message = AppOptions.prepareConfiguration(resp);
  transmitConfiguration(message);
  AppOptions.setLocalStorage(resp);
}

// Sends the message to the watch
function transmitConfiguration(dictionary){
  if(DEBUG) console.log('transmitConfiguration()');
  if(DEBUG) console.log(JSON.stringify(dictionary));
  Pebble.sendAppMessage(
    dictionary, 
    function(e) {
      if(DEBUG) console.log('Send successful: ' + JSON.stringify(e));
    },
    function(e) {
      if(DEBUG) console.log('Send failed! ' + JSON.stringify(e));
    }
  );
}

Pebble.addEventListener("ready", readyCallback);
Pebble.addEventListener("appmessage", appMessageCallback);
Pebble.addEventListener("showConfiguration", showConfigurationCallback);
Pebble.addEventListener("webviewclosed", webViewClosedCallback);