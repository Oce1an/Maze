#include "student.h"
#include <sstream>
#include <iomanip>

void Student::calculateAverages() {
    if (!winterGrades.empty()) {
        double sum = 0;
        for (int grade : winterGrades) {
            sum += grade;
        }
        winterAverage = sum / winterGrades.size();
    } else {
        winterAverage = 0;
    }

    if (!summerGrades.empty()) {
        double sum = 0;
        for (int grade : summerGrades) {
            sum += grade;
        }
        summerAverage = sum / summerGrades.size();
    } else {
        summerAverage = 0;
    }
}

Student::Student() 
    : groupNumber(""), fullName(""), winterAverage(0), summerAverage(0) {}

Student::Student(const std::string& group, const std::string& name,
                 const std::vector<int>& winter, const std::vector<int>& summer)
    : groupNumber(group), fullName(name), winterGrades(winter), summerGrades(summer) {
    calculateAverages();
}

void Student::setWinterGrades(const std::vector<int>& grades) {
    winterGrades = grades;
    calculateAverages();
}

void Student::setSummerGrades(const std::vector<int>& grades) {
    summerGrades = grades;
    calculateAverages();
}

bool Student::hasWinterDebts() const {
    for (int grade : winterGrades) {
        if (grade < 4) return true;
    }
    return false;
}

bool Student::shouldBeExpelled() const {
    int summerDebts = 0;
    for (int grade : summerGrades) {
        if (grade < 4) summerDebts++;
    }
    return (summerDebts >= 2 && hasWinterDebts());
}

int Student::getWinterDebtCount() const {
    int count = 0;
    for (int grade : winterGrades) {
        if (grade < 4) count++;
    }
    return count;
}

int Student::getSummerDebtCount() const {
    int count = 0;
    for (int grade : summerGrades) {
        if (grade < 4) count++;
    }
    return count;
}

std::string Student::toString() const {
    std::stringstream ss;
    ss << groupNumber << "|" << fullName << "|";
    
    for (size_t i = 0; i < winterGrades.size(); ++i) {
        ss << winterGrades[i];
        if (i < winterGrades.size() - 1) ss << ",";
    }
    ss << "|";
    
    for (size_t i = 0; i < summerGrades.size(); ++i) {
        ss << summerGrades[i];
        if (i < summerGrades.size() - 1) ss << ",";
    }
    
    return ss.str();
}

Student Student::fromString(const std::string& str) {
    std::stringstream ss(str);
    std::string token;
    std::vector<std::string> tokens;
    
    while (std::getline(ss, token, '|')) {
        tokens.push_back(token);
    }
    
    if (tokens.size() >= 4) {
        std::string group = tokens[0];
        std::string name = tokens[1];
        
        std::vector<int> winter;
        std::stringstream winterStream(tokens[2]);
        std::string grade;
        while (std::getline(winterStream, grade, ',')) {
            if (!grade.empty()) {
                winter.push_back(std::stoi(grade));
            }
        }
        
        std::vector<int> summer;
        std::stringstream summerStream(tokens[3]);
        while (std::getline(summerStream, grade, ',')) {
            if (!grade.empty()) {
                summer.push_back(std::stoi(grade));
            }
        }
        
        return Student(group, name, winter, summer);
    }
    return Student();
}