#!/usr/bin/env python
# coding=utf-8

import datetime

if __name__ == '__main__':

    print(datetime.date.today().strftime('%d/%m/%Y'))

    mybirthday = datetime.date(1941, 1, 6)

    print (mybirthday.strftime('%d/%m/%Y'))

    mybirthnextday = mybirthday + datetime.timedelta(days=1)

    print(mybirthnextday.strftime('%d/%m/%Y'))

    myfirstbirthday = mybirthday.replace(year=mybirthday.year + 1)
    print(myfirstbirthday.strftime('%d/%m/%Y'))
