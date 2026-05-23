#include "date.h"
#include <QDate>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <QDebug>

int Date::daysInMonth() const {
    static const int days[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && IsLeap()) return 29;
    return days[month];
}

void Date::validate() const {
    if (year < 1 || month < 1 || month > 12 || day < 1 || day > daysInMonth()) {
        throw InvalidDateException();
    }
}

Date::Date() {
    QDate today = QDate::currentDate();
    day = today.day();
    month = today.month();
    year = today.year();
}

Date::Date(int d, int m, int y) : day(d), month(m), year(y) {
    validate();
}

Date::Date(const std::string& dateString) {
    char dot1 = 0, dot2 = 0;
    day = 0;
    month = 0;
    year = 0;

    std::istringstream iss(dateString);
    iss >> day >> dot1 >> month >> dot2 >> year;

    if (dot1 != '.' || dot2 != '.') {
        throw InvalidDateException();
    }

    if (day == 0 || month == 0 || year == 0) {
        throw InvalidDateException();
    }

    validate();
}

std::string Date::toString() const {
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << day << "."
       << std::setfill('0') << std::setw(2) << month << "."
       << year;
    return ss.str();
}

Date Date::NextDay() const {
    int d = day, m = month, y = year;
    d++;
    if (d > daysInMonth()) {
        d = 1; m++;
        if (m > 12) { m = 1; y++; }
    }
    return Date(d, m, y);
}

Date Date::PreviousDay() const {
    int d = day, m = month, y = year;
    d--;
    if (d < 1) {
        m--;
        if (m < 1) { m = 12; y--; }
        Date temp(1, m, y);
        d = temp.daysInMonth();
    }
    return Date(d, m, y);
}

bool Date::IsLeap() const {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int Date::Duration(const Date& other) const {
    QDate d1(year, month, day);
    QDate d2(other.year, other.month, other.day);
    return std::abs(d1.daysTo(d2));
}

short Date::WeekNumber() const {
    QDate d(year, month, day);
    int yearNumber;
    return d.weekNumber(&yearNumber);
}

int Date::DaysTillYourBithday(const Date& birthdayDate) const {
    QDate currentDate(year, month, day);

    int birthYear = currentDate.year();
    int birthMonth = birthdayDate.getMonth();
    int birthDay = birthdayDate.getDay();

    QDate birthdayThisYear(birthYear, birthMonth, birthDay);

    if (birthdayThisYear < currentDate) {

        birthdayThisYear = birthdayThisYear.addYears(1);
    }

    int daysLeft = currentDate.daysTo(birthdayThisYear);

    return daysLeft;
}
