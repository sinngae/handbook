# -*- coding: utf-8 -*-
from django.http import HttpResponse
from testmode1.models import Test

def testdb(request):
	test1 = Test(name='runnoob')
	test1.save()
	return HttpResponse('<p>数据添加成功！</p>')
