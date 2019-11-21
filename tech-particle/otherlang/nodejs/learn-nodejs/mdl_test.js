/* 从文件模块缓存中加载=》从原生模块加载=》从文件加载
 */
var mdl_a = require('./mdl_a');
mdl_a.test();

var mdl_b = require('./mdl_b');
hiwork = new mdl_b();
hiwork.setName('sdfasfa');
hiwork.sayHi();
