var fs = require("fs");

var data = fs.readFileSync('nodejs.txt');

console.log(data.toString());
console.log("程序执行结束!");
