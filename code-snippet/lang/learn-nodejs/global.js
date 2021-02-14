console.log("filename:" + __filename + ", dirname:" + __dirname + "");

function hiwork() {
	console.trace("this is a trace");
	console.log("hi, work global");
}


console.time("time_mark_a");
/* 仅执行一次 */
var timer = setTimeout(hiwork, 2000);

clearTimeout(timer);


/* 执行，直到被clearTimeout */
var timer2 = setInterval(hiwork, 2000);
console.timeEnd("time_mark_a");

clearTimeout(timer2);


console.log("this is a test");
console.log("this is a %d test", 100);

console.info("this is a test");
console.error("this is a test");
console.warn("this is a test");

console.assert(1 == 0);
console.dir(timer);

process.on('exit', function(code) {
	setTimeout(function() {
		console.log("only code, no output")
	}, 0);

	console.log("exit code:", code);
});

console.log(process.execPath);
console.log(process.platform);
console.log(process.cwd());
console.log(process.version);
console.log(process.memoryUsage());

console.log("over");
