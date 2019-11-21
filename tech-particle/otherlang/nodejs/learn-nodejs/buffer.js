const buf1 = Buffer.alloc(10, 0x01);
const buf2 = Buffer.allocUnsafe(10);
const buf3 = Buffer.from([0x01, 0x02, 0x03]);
const buf4 = Buffer.from('tést');
const buf5 = Buffer.from('tést', 'latin1');

buf = Buffer.alloc(256);
len = buf.write('hi, this is work');

console.log('write ' + len + ' bytes');


