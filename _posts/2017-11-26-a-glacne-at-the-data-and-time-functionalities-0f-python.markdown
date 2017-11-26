---
layout: post
title:  "A Glance at the Date & Time Functionalities of Python"
date:   2017-11-26 14:57:00 -0500
categories: technical
cover_image: "Resources/Posts/PythonDateTime/Tz_world_mp-color.png"
---

# The beginning of the story

I was working on a project that has to deal with date & time within different timezones. The target system is written in Python. So I just googled it and spent a whole night to learn the techniques and theories of timezone.

I was so surprised that the theory of timezone is so complicated and, yet, very interesting. I have the naive concept that all of the complexity of the current status of the timezone over the globe is a miniaturized picture of human history. We created all the trouble and we are indeed the troublemaker! :)

Let me summarize what I've learned and the important points.

Most of the time zones on land are offset from the Coordinated Universal Time (UTC) by a whole number of hours (UTC−12 to UTC+14). ( Refer to [Time zone][Wiki_TimeZone])

[Wiki_TimeZone]: https://en.wikipedia.org/wiki/Time_zone

In fact, there is not a simple table that lists all the timezones and there offsets from UTC. You may be looking at some fancy tables like this one "[List of tz database time zones][Wiki_ListOfTZDTimeZones]". But you may be disappointed that things just work that way. There are mainly two reasons. 

[Wiki_ListOfTZDTimeZones]: https://en.wikipedia.org/wiki/List_of_tz_database_time_zones

First, there exists such a magical rule called DST, Daylight Saving Time. Lots of the western people may be familiar with the concept of DST. But it is hard for me, cause I'm an eastern guy. You have to specify a specific time and date in a year to fully decide what the offset from the UTC should be for a given timezone.

Second, things with DST get worse when you looking back in time. The rules of DST changes frequently in some areas of the land of Planet Earth. So in addition to the specific day ant time, you have to be very specific about the actual year you are interested in when you would like to deal with the DST rules.

The details of timezone are maintained by Arthur David Olson of IANA (Internet Assigned Numbers Authority) and the product is the [tz database][tzd].

[tzd]: https://www.iana.org/time-zones

As for python, there is a package called "[pytz][pytzWebsite]" that helps you to deal with issues with timezones. [pytz][pytzWebsite] brings the Olson tz database into Python. I think pytz is shipped with python 2.7 and python3.5 by default. 

[pytzWebsite]: http://pytz.sourceforge.net/

# The datetime package of python

Time flew when I was playing with pytz. Then I thought that since I had spent so much time on the timezone issue why shouldn't I continue to learn the data & time functionalities provided by python as well? So I did it.

After studying the content of the [associated documentation][DocOfDatetime] of python, I obtained a relatively clearer picture of the techniques we could adopt regarding date & time manipulation with python. Python has a `datetime` [package][DocOfDatetime] which is really handy when you want to deal with date and time.

[DocOfDatetime]: https://docs.python.org/2/library/datetime.html

# The notes

I composed a short python script (Yeah, I know, a lot of print() calls) to demonstrate the use of the datatime and pytz packages. This script serves as my learning notes. The functionalities that I used are those I suppose to be important and useful for everyday practice.

The following is the output of the simple script.

================== Beginning of the output. ================

{% highlight plaintext %}

Try some functionalities regarding date and time.

dt = datetime.now()
2017-11-26 14:56:47.146529

A new timedelta object with 10 days, 48 hours, 90 minutes, and 4000 seconds in the future.
That is 
12 days, 2:36:40
And dt will be
2017-12-08 17:33:27.146529
It is equivalent to say dt marches 1046200 seconds in time.

Create a date object with date.today()
2017-11-26
Access year, month, and the day in the month by 
d.year = 2017, d.month = 11, d.day = 26
Modify our delta. Let delta be a time shift of 31 days.
31 days, 0:00:00
Shift d by delta days and get dd
2017-12-27
We could do a subtraction operation between d and dd and get a timedelta objcet
perform td = d - dd, we should obtain a negative td
-31 days, 0:00:00

Construct a new date object with specific date information: 2012-12-12
2012-02-28
Change one of the values into some other value with replace() function.
2017-02-28
Here I attempt to change from 2012-2-29 directly into 2017-12-29 by replace(). An exception will be raised since the February of 2017 does not have 29 days.
The ValueError exception is caught.
Now let us use today() as the value of d
2017-11-26

Today is the 7th day of a week. (By using weekday())
Today is the 7th day of a week. (By using isoweekday())

Today is the 330th day of this year. (Not in ISO calendar)

What's more interesting is that the day of the year by the ISO standard.
Let's start with the date Monday 2018-01-01
It is the 1th day of the 1th week of the year 2018.

Let's look at the date Monday 2018-12-31
It is the 1th day of the 1th week of the year 2019.
Do you see the difference?

Let's use today() again.
Print the date details in different formats.
ISO format: 2017-11-26
Custom format: 11/26/17
Custom format: Sunday, November 26, 17
There are lots of other useful formats.

The datetime class essentially contains information from a date object and a time object.
We have datetime.tody() = 2017-11-26 14:56:47.147156
We have datetime.now() = 2017-11-26 14:56:47.147192
We have datetime.utcnow() = 2017-11-26 19:56:47.147222
We have datetime.combine(date, time)
We have datetime.date()
We have datetime.now()
We have datetime.replace()
We have datetime.weekday()
We have datetime.isoweekday()
We have datetime.isocalendar()
We have datetime.isoformat()
We have datetime.strftime()

Let n = datetime.now()
For isoformat() use n.replace(microsecond = 0).isoformat() to get
2017-11-26 14:56:47

n.year = 2017, n.month = 11, n.day = 26
n.hour = 14, n.minute = 56, n.second = 47

OK, the time class is roughly the same with date and datetime.

One thing we haven't talked about is the concept of timezone.

Get a tzinfo object (tzShanghai) from pytz to represent the 'Asia/Shanghai' timezone.
Get another tzinfo (tzNewYork) object from pytz to represent the 'America/New_York' timezone. (Note that the underscore is required.)

We can use datetime.now() to obtain a tz aware time specification.
nShanghai = datetime.now( tzShanghai )
2017-11-27 03:56:47.162960+08:00

We can convert nShanghai into the America/New_York timezone. It will be 
2017-11-26 14:56:47.162960-05:00

Both of the above time specification could be converted into UTC timezone.
2017-11-26 19:56:47.162960+00:00

The offset from UTC of the timezone 'Asia/Shanghai' AT THE MOMENT is 
8.00 hours

The offset from UTC of the timezone 'America/New_York' AT THE MOMENT is 
19.00 hours

All the timezones are listed in pytz.all_timezones
You can also refer to pytz.country_timezones which is a dictionary.
The index of each entry in the dictionary is an abbreviation of nation name according to ISO 3166.
The value of each entry in the dictionary is a list. The list contains all the timezones of that country.

{% endhighlight %}

====================== End of the output. ==================

# Acknowledgment

The cover image is copied from the Wikipedia page of the [tz database][TZDatabaseWebsite]

[TZDatabaseWebsite]: https://en.wikipedia.org/wiki/Tz_database

<p><img src="{{site.baseurl}}/Resources/Posts/PythonDateTime/Tz_world_mp-color.png" alt="<img>tz database partitions."></p>

# The source code

The source code is also available at my [GitHub Repo][GitHubRepo].

[GitHubRepo]: https://github.com/huyaoyu/TryPython

{% highlight python %}

#!/usr/bin/python
# Python 2.6+
from __future__ import print_function

from datetime import datetime
from datetime import timedelta
from datetime import date

import pytz

if __name__ == '__main__':
	print('Try some functionalities regarding date and time.\n')

	print('dt = datetime.now()')

	dt = datetime.now()

	print(dt)
	print("")

	print("A new timedelta object with 10 days, 48 hours, 90 minutes, and 4000 seconds in the future.")
	delta = timedelta(days = 10, hours = 48, minutes = 90, seconds = 4000)
	print("That is ")
	print(delta)

	print("And dt will be")
	print( dt + delta )

	print("It is equivalent to say dt marches %d seconds in time." % delta.total_seconds())
	
	print("")
	print("Create a date object with date.today()")

	d = date.today()

	print(d)

	print("Access year, month, and the day in the month by ")
	print("d.year = %d, d.month = %d, d.day = %d" % (d.year, d.month, d.day))

	print("Modify our delta. Let delta be a time shift of 31 days.")
	delta = timedelta(days = 31)
	print(delta)

	print("Shift d by delta days and get dd")
	dd = d + delta
	print( dd )

	print("We could do a subtraction operation between d and dd and get a timedelta objcet")
	td = d - dd
	print("perform td = d - dd, we should obtain a negative td")
	print(td)

	print("")
	print("Construct a new date object with specific date information: 2012-12-12")
	d = date(year = 2012, month = 2, day = 28)
	print(d)

	print("Change one of the values into some other value with replace() function.")
	print(d.replace(year = 2017))

	try:
		print("Here I attempt to change from 2012-2-29 directly into 2017-12-29 by replace(). An exception will be raised since the February of 2017 does not have 29 days.")
		d = date(year = 2012, month = 2, day = 29)
		d.replace(year = 2017)
	except ValueError as e:
		print("The ValueError exception is caught.")
	else:
		pass
	finally:
		pass

	print("Now let us use today() as the value of d")
	d = date.today()
	print(d)

	print("")
	print("Today is the %dth day of a week. (By using weekday())" % (d.weekday() + 1))
	print("Today is the %dth day of a week. (By using isoweekday())" % (d.isoweekday()))

	print("")
	d = date.today()
	print("Today is the %dth day of this year. (Not in ISO calendar)" % (d.timetuple().tm_yday))

	print("")
	print("What's more interesting is that the day of the year by the ISO standard.")
	print("Let's start with the date Monday 2018-01-01")
	d = date(year = 2018, month = 1, day = 1)
	isoc = d.isocalendar()

	print("It is the %dth day of the %dth week of the year %d." % (isoc[2], isoc[1], isoc[0]))

	print("")
	print("Let's look at the date Monday 2018-12-31")
	d = date(year = 2018, month = 12, day = 31)
	isoc = d.isocalendar()
	print("It is the %dth day of the %dth week of the year %d." % (isoc[2], isoc[1], isoc[0]))

	print("Do you see the difference?")

	print("")
	print("Let's use today() again.")
	d = date.today()
	print("Print the date details in different formats.")
	print("ISO format: %s" % (d.isoformat()))
	print("Custom format: %s" % ( d.strftime("%m/%d/%y") ))
	print("Custom format: %s" % ( d.strftime("%A, %B %d, %y") ))
	print("There are lots of other useful formats.")

	# ================= The datetime class. =================
	
	print("")
	print("The datetime class essentially contains information from a date object and a time object.")
	print("We have datetime.tody() = ", end = "")
	print(datetime.today())
	print("We have datetime.now() = ", end = "")
	print(datetime.now())
	print("We have datetime.utcnow() = ", end = "")
	print(datetime.utcnow())
	print("We have datetime.combine(date, time)")
	print("We have datetime.date()")
	print("We have datetime.now()")
	print("We have datetime.replace()")
	print("We have datetime.weekday()")
	print("We have datetime.isoweekday()")
	print("We have datetime.isocalendar()")
	print("We have datetime.isoformat()")
	print("We have datetime.strftime()")

	print("")
	print("Let n = datetime.now()")
	n = datetime.now()
	print("For isoformat() use n.replace(microsecond = 0).isoformat(" ") to get")
	print( n.replace(microsecond = 0).isoformat(" ") )

	print("")
	print("n.year = %d, n.month = %d, n.day = %d" % (n.year, n.month, n.day))
	print("n.hour = %d, n.minute = %d, n.second = %d" % (n.hour, n.minute, n.second))

	# ================== The time class. =======================

	print("")
	print("OK, the time class is roughly the same with date and datetime.")

	# ================== The tzinfo and pytz. =================

	print("")
	print("One thing we haven't talked about is the concept of timezone.")

	print("")
	print("Get a tzinfo object (tzShanghai) from pytz to represent the 'Asia/Shanghai' timezone.")
	tzShanghai = pytz.timezone('Asia/Shanghai')

	print("Get another tzinfo (tzNewYork) object from pytz to represent the 'America/New_York' timezone. (Note that the underscore is required.)")
	tzNewYork  = pytz.timezone('America/New_York')

	print("")
	print("We can use datetime.now() to obtain a tz aware time specification.")
	print("nShanghai = datetime.now( tzShanghai )")
	nShanghai = datetime.now( tzShanghai )
	print(nShanghai)

	print("")
	print("We can convert nShanghai into the America/New_York timezone. It will be ")
	nNewYork = nShanghai.astimezone( tzNewYork )
	print(nNewYork)

	print("")
	print("Both of the above time specification could be converted into UTC timezone.")
	print( nShanghai.astimezone( pytz.utc ) )

	print("")
	print("The offset from UTC of the timezone 'Asia/Shanghai' AT THE MOMENT is ")
	print("%.2f hours" % (nShanghai.utcoffset().seconds / 3600))

	print("")
	print("The offset from UTC of the timezone 'America/New_York' AT THE MOMENT is ")
	print("%.2f hours" % (nNewYork.utcoffset().seconds / 3600))

	print("")
	print("All the timezones are listed in pytz.all_timezones")
	print("You can also refer to pytz.country_timezones which is a dictionary.")
	print("The index of each entry in the dictionary is an abbreviation of nation name according to ISO 3166.")
	print("The value of each entry in the dictionary is a list. The list contains all the timezones of that country.")


{% endhighlight %}
