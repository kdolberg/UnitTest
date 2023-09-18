#include <assert.h>
#include <vector>
#include <string>
#include <sstream>
#include "test.h"

/**
 * @brief The number of tests executed that have passed
 */
static int num_passes=0;
/**
 * @brief The number of tests executed that have failed
 */
static int num_fails=0;
/**
 * @brief The number of tests executed that have failed as a result of throwing an unexpected error
 */
static int num_unexpected_errors=0;

/**
 * @brief Returns the total number of tests that have been executed
 */
int total_tests_executed() {
	return num_passes+num_fails;
}

int total_tests_passed() {
	return num_passes;
}

int total_tests_failed() {
	return num_fails;
}

int total_tests_failed_due_to_error() {
	return num_unexpected_errors;
}

float percent_pass() {
	return FLOAT_AS_PERCENTAGE(num_passes);
}

float percent_fail() {
	return FLOAT_AS_PERCENTAGE(num_fails);
}

float percent_error_fail() {
	return FLOAT_AS_PERCENTAGE(num_unexpected_errors);
}

std::ostream& operator<<(std::ostream& os,const ResultType& t) {
	assert(t!=not_executed);
	switch (t) {
	case pass:
		os << "PASS";
		break;
	case fail:
		os << "FAIL";
		break;
	case error_fail:
		os << "ERROR, FAIL";
		break;
	default:
		throw (t);
	}
	return os;
}

void record_result(ResultType t) {
	assert(t!=not_executed);
	switch (t) {
	case pass:
		++num_passes;
		break;
	case fail:
		++num_fails;
		break;
	case error_fail:
		record_result(fail);
		++num_unexpected_errors;
		break;
	default:
		throw (t);
	}
}

std::string repeat_character(const char * c, int n) {
	std::string ret = "";
	for (int i = 0; i < n; ++i) {
		ret += *c;
	}
	return ret;
}

#define REPORT_CARD_HEADER_LENGTH 150
#define REPORT_CARD_HEADING "UNIT TESTING REPORT CARD"
#define HEADING_CHAR "-"
#define PASS_THRESHOLD 0.5f

std::string report_card_header(const std::string& heading,int header_length) {
	std::stringstream ss;
	ss << std::endl << std::endl;
	ss << repeat_character(HEADING_CHAR,header_length) << std::endl;
	ss << repeat_character(" ",(header_length-heading.length())/2) << heading << std::endl;
	ss << repeat_character(HEADING_CHAR,header_length) << std::endl;
	ss << std::endl;
	return ss.str();
}

std::string letter_grade(float score) {
	if(score < PASS_THRESHOLD) {
		return "F";
	}
	if(score >= 1.0f) {
		return "A+";
	}
	std::vector<std::string> passing_grades = {"D-","D","D+","-C","C","C+","B-","B","B+","A-","A","A+"};
	float increment = PASS_THRESHOLD/(passing_grades.size());
	int index = (int)(((score-PASS_THRESHOLD)/increment));
	return (passing_grades[index]);
}

std::string letter_grade() {
	return letter_grade(percent_pass()*0.01f);
}

void test_letter_grade() {
	for (float s = 0.0f; s <= 1.0f; s+=0.005f) {
		LOG << 100*s << "%" << ":\t" << letter_grade(s) << std::endl;
	}
}

void print_report_card() {
	LOG << report_card_header(REPORT_CARD_HEADING,REPORT_CARD_HEADER_LENGTH);

	// Print relevant statistics for each test
	if(total_tests_passed())
		LOG << percent_pass() << "%" << " (" << total_tests_passed() << " tests) passed\n";
	if(total_tests_failed())
		LOG << percent_fail() << "%" << " (" << total_tests_failed() << " tests) failed\n";
	if(total_tests_failed_due_to_error())
		LOG << percent_error_fail() << "%"  << " (" << total_tests_failed_due_to_error() << " tests) failed due to error\n";

	LOG << total_tests_executed() << " tests executed in total\n";
	LOG << "Grade: " << letter_grade() << std::endl;
}