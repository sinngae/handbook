function hiwork() {
	var name;
	this.setName = function(thyName) {
		name = thyName;
	};
	this.sayHi = function() {
		console.log('hi, work ' + name);
	};
};

module.exports = hiwork;
