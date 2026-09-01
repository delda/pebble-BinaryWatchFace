var Clay = require('@rebble/clay');
var config = require('./config.json');

// Clay opens an offline configuration page and automatically sends the values
// associated with the message keys declared in package.json to the watch.
new Clay(config);
