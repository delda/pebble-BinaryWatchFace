var platform, shape, color, number, bluetooth, battery, battery_modality, date, locale;
var version = '2.9';

function readConfig(){
  console.log('readConfig()');
  if(Pebble.getActiveWatchInfo && Pebble.getActiveWatchInfo().platform === 'basalt') {
    // This is the Basalt platform
    console.log('PebbleKit JS ready on Basalt!');
    platform = 1;
  } else {
    // This is the Aplite platform
    console.log('PebbleKit JS ready on Aplite!');
    platform = 0;
  }
  shape = localStorage.getItem('shape');
  shape = shape ? shape : 0;
  
  if(localStorage.key('color')){
    color = localStorage.getItem('color');
  }
  color = color ? color : 0;
  
  if(localStorage.getItem('number')){
    number = localStorage.getItem('number');
  }
  number = number ? number : 0;
  
  if(localStorage.getItem('bluetooth')){
    bluetooth = localStorage.getItem('bluetooth');
  }
  bluetooth = bluetooth ? bluetooth : 0;
  
  if(localStorage.getItem('battery')){
    battery = localStorage.getItem('battery');
  }
  battery = battery ? battery : 0;
  
  if(localStorage.getItem('battery_modality')){
    battery_modality = localStorage.getItem('battery_modality');
  }
  battery_modality = battery_modality ? battery_modality : 0;
  
  locale = (Pebble.getActiveWatchInfo()) ? Pebble.getActiveWatchInfo().language : 'en_US';

  if(localStorage.getItem('date')){
    date = localStorage.getItem('date');
  }
  date = date ? date : 0;
}

function readyCallback(event){
  console.log('readyCallback()');
  readConfig();
  var jsonConfig = JSON.parse('{"shape":'+shape+
                              ',"color":'+color+
                              ',"number":'+number+
                              ',"bluetooth":'+bluetooth+
                              ',"battery":'+battery+
                              ',"battery_modality":'+battery_modality+
                              ',"date":'+date+
                              ',"locale":'+locale+
                              '}');
  Pebble.sendAppMessage(jsonConfig);
}

function appMessage(event){
  console.log('appMessage()');
}

function showConfiguration(event){
  console.log('showConfiguration()');
  readConfig();
  var attributes = '?platform='+platform+
      '&version='+version+
      '&shape='+shape+
      '&color='+color+
      '&number='+number+
      '&bluetooth='+bluetooth+
      '&battery='+battery+
      '&battery_modality='+battery_modality+
      '&date='+date+
      '&locale='+locale;
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

// Format sent BACK from the configuration web UI
function prepareConfiguration(serialized_settings){
  console.log('prepareConfiguration');
  var settings = JSON.parse(serialized_settings);
  var shape = 0;
  var color = 0;
  var number = 0;
  var bluetooth = 0;
  var battery = 0;
  var battery_modality = 0;
  var date = 0;
  console.log('battery_modality: ' + battery_modality);

  shape            = (settings.shape)            ? Number(settings.shape)            : 0;
  color            = (settings.color)            ? Number(settings.color)            : 0;
  number           = (settings.number)           ? Number(settings.number)           : 0;
  bluetooth        = (settings.bluetooth)        ? Number(settings.bluetooth)        : 0;
  battery          = (settings.battery)          ? Number(settings.battery)          : 0;
  battery_modality = (settings.battery_modality) ? Number(settings.battery_modality) : 0;
  date             = (settings.date)             ? Number(settings.date)             : 0;
  console.log('battery_modality: ' + battery_modality);
  
  return {
    '0': shape,
    '1': color,
    '2': number,
    '3': bluetooth,
    '4': battery,
    '5': battery_modality,
    '6': date,
  };
}

// Sends the message to the watch
function transmitConfiguration(dictionary){
  console.log('transmitConfiguration');
  console.log(JSON.stringify(dictionary));
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
  
  shape            = obj.shape;
  color            = obj.color;
  number           = obj.number;
  bluetooth        = obj.bluetooth;
  battery          = obj.battery;
  battery_modality = obj.battery_modality;
  date             = obj.date;
  
  localStorage.setItem('shape',            shape);
  localStorage.setItem('color',            color);
  localStorage.setItem('number',           number);
  localStorage.setItem('bluetooth',        bluetooth);
  localStorage.setItem('battery',          battery);
  localStorage.setItem('battery_modality', battery_modality);
  localStorage.setItem('date',             date);
}

Pebble.addEventListener("ready", readyCallback);
Pebble.addEventListener("appmessage", appMessage);
Pebble.addEventListener("showConfiguration", showConfiguration);
Pebble.addEventListener("webviewclosed", webViewClosed);