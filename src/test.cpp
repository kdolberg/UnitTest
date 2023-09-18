#include <assert.h>
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

void print_report_card() {
	LOG << percent_pass() << "%" << " (" << total_tests_passed() << " tests) passed\n";
	LOG << percent_fail() << "%" << " (" << total_tests_failed() << " tests) failed\n";
	LOG << percent_error_fail() << "%"  << " (" << total_tests_failed_due_to_error() << " tests) failed due to error\n";
	LOG << total_tests_executed() << " tests executed in total\n";
}