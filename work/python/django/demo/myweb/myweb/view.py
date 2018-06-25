#!/usr/bin/env python
# coding=utf-8

#from django.http import HttpResponse
from django.shortcuts import render

def hello(request):
    context = {}
    context['hello'] = 'hi, django'
    #return HttpResponse("hi, django")
    return render(request, 'hello.html', context)
