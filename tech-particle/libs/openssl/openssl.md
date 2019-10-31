openssl enc -des3 -in test.txt -e -out test.des3 //使用des3加密test.txt输出到test.des3 <br>
openssl enc -des3 -in test.des3 -d -out test.txt		//使用des3解密test.des3输出到test.txt<br>

openssl dgst -md5 -hex test.txt		//使用md5双向加密（-hex十六进制表示，默认，可省略）获取test.txt的特征码<br>
md5sum test.txt		//同上<br>

openssl genrsa 2048 > my.key		//生成2048位rsa私钥到my.key<br>
openssl genrsa -des3 2048 > my.key		//生成2048位rsa私钥并用des3单向加密<br>
(umask 077;openssl genrsa -out my2.key 2048)		//作为一个shell执行脚本执行：默认权限700，即仅本人可访问使用；生成my2.key;<br>
openssl rsa -in my.key -pubout > pub.key		//从my.key提取出公钥<br>

如何申请证书及生成证书<br>
opessl req -new -key my.key -out my.csr			//使用my.key制作一个证书签署请求<br>

/etc/pki/tls/openssl.cnf配置CA路径为/etc/pki/CA<br>
cd /etc/pki/CA		//到openssl的证书生成路径下，由/etc/pki/tls/openssl.cnf配置<br>
(umask 077;openssl genrsa -out private/mykey.pem 2048)		//为CA生成一个私钥mykey.pem(private文件夹下)<br>
ls private/		//查看私钥<br>
openssl req -new -x509 -key private/mykey.pem -out cacert.pem -days 3656		//使用x509协议以mykey.pem生成公钥cacert.pem，有效时间为3656天。<br>
openssl ca -in my.csr -out my.crt -days 3656		//对证书请求my.csr生成证书my.crt<br>

x509 -in cacert.pem -text -noout		//查看证书pem格式证书的信息<br>
openssl x509 -in certificate.der -inform der -text -noout		//同上查看DER格式证书<br>


