var fs = require("fs");

fs.readFile('nodejs.txt', function (err, data) {
    if (err) return console.error(err);
    console.log(data.toString());
});

console.log("程序执行结束!");
