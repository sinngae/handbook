#!/bin/bash
rm -rf gen-cpp/*
/opt/local/thrift/bin/thrift --gen cpp myserver.thrift
