var version = '3.0';
var DEBUG = false;

var options = {shape: 1, color: 2, number: 0, bluetooth: 2, battery: 2, battery_modality: 0, date: 23, help_num: 1};
var webOptions = {version: version, platform: getPlatform(), locale: getLocale()};

var AppOptions = {
  foreach: function(f, obj){
    for(var prop in obj){
      if(typeof obj[prop] !== 'function'){
        f(prop, obj[prop]);
      }
    }
  },
  clean: function(){
    if(DEBUG) console.log('AppOptions.clean()');
    var that = this;
    this.foreach(
      function(key, value){
        delete that[key];
      },
      that
    );
    return that;
  },
  extend: function(extentions){
    if(DEBUG) console.log('AppOptions.extend()');
    var that = this;
    this.foreach(
      function(key, value){ that[key] = value; },
      extentions
    );
    return that;
  },
  overrideLocalStorage: function(){
    if(DEBUG) console.log('AppOptions.overrideLocalStorage()');
    var that = this;
    this.foreach(
      function(key, value){
        var tmp = localStorage.getItem(key);
        if(typeof(tmp) != 'undefined' && typeof(tmp) != 'object' && tmp !== ''){
          that[key] = (!isNaN(parseFloat(tmp)) && isFinite(tmp)) ? Number(tmp) : tmp;
        }
      },
      that
    );
    return that;
  },
  getObjOptions: function(){
    if(DEBUG) console.log('AppOptions.getObjOptions()');
    var obj = {};
    var i = 0;
    var that = this;
    this.foreach(
      function(key, value){
        obj[i] = that[key];
        i++;
      },
      that
    );
    return obj;
  },
  prepareConfiguration: function(serialized_settings){
    if(DEBUG) console.log('AppOptions.prepareConfiguration()');
    var that = this;
    console.log(serialized_settings);
    var settings = JSON.parse(serialized_settings);
    this.foreach(
      function(key, value){
        if(that.hasOwnProperty(key)){
          that[key] = (!isNaN(parseFloat(value)) && isFinite(value)) ? Number(value) : value; 
        }
      },
      settings
    );
    return that;
  },
  getUrlOptions: function(){
    if(DEBUG) console.log('AppOptions.getUrlOptions()');
    var url = '';
    var i = 0;
    this.foreach(
      function(key, value){
        url += (i === 0) ? '?' : '&';
        url += key+'='+value;
        i++;
      },
      this
    );
    return url;
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
    if(DEBUG) console.log('AppOptions.setLocalStorage()');
    var obj = JSON.parse(decodeURIComponent(settings));
    this.foreach(
      function(key, value){ localStorage.setItem(key, obj[key]); },
      obj
    );
    for(var prop in obj){
      if(typeof this[prop] !== 'function'){
        localStorage.setItem(prop, obj[prop]);
      }
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
  
  var dataMessage = AppOptions.extend(options).overrideLocalStorage().getObjOptions();
  transmitConfiguration(dataMessage);
}
// The watch sent an AppMessage to PebbleKit JS
function appMessageCallback(e){
  console.log('Received message: ' + JSON.stringify(e.payload));
}
// The user requests to display the app's configuration webview
function showConfigurationCallback(e){
  if(DEBUG) console.log('showConfigurationCallback()');

  var urlString = AppOptions.extend(options).extend(webOptions).overrideLocalStorage().getUrlOptions();
  var url = 'http://delda.github.io/pebble_binaryWatch/index.html'+urlString;
  if(DEBUG) console.log("url: " + url);
  Pebble.openURL(url);
}
// The user closes the configuration webview
function webViewClosedCallback(e){
  if(DEBUG) console.log('webViewClosedCallback()');
  
  var resp = e.response;
  if(DEBUG) console.log('Configuration window returned: ' + resp);
  
  var message = AppOptions.clean().extend(options).prepareConfiguration(resp).getObjOptions();
  transmitConfiguration(message);
  AppOptions.setLocalStorage(resp);
}

// Sends the message to the watch
function transmitConfiguration(dictionary){
  if(DEBUG) console.log('transmitConfiguration()');
  if(DEBUG) console.log('dictionary: '+JSON.stringify(dictionary));
  return Pebble.sendAppMessage(
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