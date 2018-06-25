/* @brief test boost::ptime
 * @author renzq
 * @date 2017-11-23
 * @description
 * 	1.is_a_date_time可以判断ptime，不能判断time_duration
 *	2.当ptime是整数秒时，调用to_simple_string等函数，没有毫秒及纳秒位
 *	3.
 * */
#include <iostream>
#include <cassert>

#include <boost/date_time.hpp>

using namespace std;
using namespace boost::posix_time;

int main(int argc, char *argv[])
{
	assert(true);

	time_duration time_err(duration_from_string("24:00:01"));
	time_duration time_max(duration_from_string("24:00:00"));
	time_duration time_min(duration_from_string("00:00:00"));
	time_duration time_mid(duration_from_string("12:00:00"));
	cout << "time_err" << time_err << endl;
	cout << "time_max" << time_max << endl;
	cout << "time_min" << time_min << endl << endl;
	assert(time_min < time_mid);
	assert(time_mid < time_max);

	ptime non_init_ptime;
	time_duration non_init_time;
	ptime init_ptime(time_from_string("2017-05-03 3:21:32.000"));
	time_duration init_time = init_ptime.time_of_day();
	ptime now(second_clock::local_time());
	boost::gregorian::date now_date = now.date();
	time_duration now_time = now.time_of_day();
	cout << "non_init_ptime:" << non_init_ptime << endl;
	cout << "non_init_time:" << non_init_time << endl;
	cout << "init_ptime:" << init_ptime << endl;
	cout << "init_time:" << init_time << endl;
	cout << "now:" << now << endl;
	cout << "now_date:" << now_date << endl;
	cout << "now_time:" << now_time << endl << endl;

	assert(init_time > non_init_time);
	//assert(now_time > init_time);
	cout << "non_init_time.total_seconds():" << non_init_time.total_seconds() << endl;
	cout << "init_time.total_seconds():" << init_time.total_seconds() << endl;
	cout << "now_time.total_seconds():" << now_time.total_seconds() << endl;

	assert(now > init_ptime);
	cout << "(now - init_ptime).total_seconds():" << (now - init_ptime).total_seconds() << endl;
	cout << "(init_ptime - non_init_ptime).total_seconds():"
		<< (init_ptime - non_init_ptime).total_seconds() << endl << endl;

	cout << "to_simple_string(now):" << to_simple_string(now) << endl;
	//cout << "to_simple_wstring(now):" << to_simple_wstring(now) << endl;
	cout << "to_iso_string(now):" << to_iso_string(now) << endl;
	//cout << "to_iso_wstring(now):" << to_iso_wstring(now) << endl;
	cout << "to_iso_extended_string(now):" << to_iso_extended_string(now) << endl;
	//cout << "to_iso_extended_wstring(now):" << to_iso_extended_wstring(now) << endl << endl;

	cout << "to_simple_string(now_date):" << to_simple_string(now_date) << endl;
	//cout << "to_simple_wstring(now_date):" << to_simple_wstring(now_date) << endl;
	cout << "to_iso_string(now_date):" << to_iso_string(now_date) << endl;
	//cout << "to_iso_wstring(now_date):" << to_iso_wstirng(now_date) << endl;
	cout << "to_iso_extended_string(now_date):" << to_iso_extended_string(now_date) << endl;
	//cout << "to_iso_extended_wstring(now_date):" << to_iso_extended_wstirng(now_date) << endl << endl;
	
	cout << "to_simple_string(now_time):" << to_simple_string(now_time) << endl;
	//cout << "to_simple_wstring(now_time):" << to_simple_wstring(now_time) << endl;
	cout << "to_iso_string(now_time):" << to_iso_string(now_time) << endl;
	//cout << "to_iso_wstring(now_time):" << to_iso_wstring(now_time) << endl;
	//cout << "to_iso_extended_string(now_time):" << to_iso_extended_string(now_time) << endl;
	//cout << "to_iso_extended_wstring(now_time):" << to_iso_extended_wstring(now_time) << endl << endl;
	cout << endl;

	cout << "now.date().year():" << now.date().year() << endl;
	cout << "now.date().month().as_number():" << now.date().month().as_number() << endl;
	cout << "now.date().day():" << now.date().day() << endl;
	cout << "now.time_of_day().hours():" << now.time_of_day().hours() << endl;
	cout << "now.time_of_day().minutes():" << now.time_of_day().minutes() << endl;
	cout << "now.time_of_day().seconds():" << now.time_of_day().seconds() << endl;
	cout << "now.time_of_day().fractional_seconds():" << now.time_of_day().fractional_seconds() << endl;

	return 0;
}
