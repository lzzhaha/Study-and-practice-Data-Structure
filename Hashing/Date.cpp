#include "Date.h"

Date::Date(int y, int m, int d):year(y),month(m),day(d){}

int Date::getYear()const{
	return year;
}

int Date::getMonth()const{
	return month;
}

int Date::getDay()const{
	return day;
}

bool Date::operator==(const Date& other) const{
	if(other.year == year){
		if(other.month == month){
			if(other.day == day){
				return true;
			}
		}
	}
	return false;
}

std::string Date::toString()const{
	std::stringstream s;
	s<<year<<"-"<<month<<"-"<<day;
	return s.str();	
}
