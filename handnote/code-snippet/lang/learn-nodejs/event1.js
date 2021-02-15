var EventEmitter = require('events').EventEmitter;
var event1 = new EventEmitter();

event1.on('event1', function(arg1, arg2){
	console.log('event1 listened 1', arg1, arg2);
});

event1.on('event1', function(arg1, arg2){
	console.log('event1 listened 2', arg1, arg2)
});

event1.emit('event1', 'arg1 value', 'argv2 value');

setTimeout(function(){
	event1.emit('event1', 'this is arg1', 'this is arg2');
}, 1000);
