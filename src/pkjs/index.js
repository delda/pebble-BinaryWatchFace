var Clay = require('@rebble/clay');
var config = require('../js/config.json');
var weather = require('./weather');

new Clay(config);

function setWeatherEnabled(enabled) {
  if (enabled) {
    weather.start();
  } else {
    weather.stop();
  }
}

Pebble.addEventListener('ready', function() {
  Pebble.sendAppMessage({ WEATHER_REQUEST_KEY: 1 });
});

Pebble.addEventListener('appmessage', function(event) {
  if (event.payload.WEATHER_ENABLED_KEY !== undefined) {
    setWeatherEnabled(event.payload.WEATHER_ENABLED_KEY !== 0);
  }
});
