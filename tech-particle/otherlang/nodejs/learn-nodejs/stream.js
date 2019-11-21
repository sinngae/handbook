var fs = require("fs");
var data = '';

var readerStream = fs.createReadStream('input.txt');
readerStream.setEncoding('UTF8');
readerStream.on('data', function(chunk){
	data += chunk;
});
readerStream.on('end', function(chunk){
	console.log(data);
});
readerStream.on('error', function(err){
	console.log(err.stack);
});

var wdata = "0987654321";
var writerStream = fs.createWriteStream('ouput.txt');
writerStream.write(wdata, 'UTF8');
writerStream.end();
writerStream.on('finish', function(){
	console.log("write over");
});
writerStream.on('error', function(err){
	console.log(err.stack);
});

/* 管道读写操作 */
//readerStream.pipe(writerStream);

console.log('over...');
