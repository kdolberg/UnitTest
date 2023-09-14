#include <assert.h>
#include "test.h"

static int num_passes=0;
static int num_fails=0;
static int num_unexpected_errors=0;

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

#define FLOAT_AS_PERCENTAGE(__num__) ((1.0f*__num__)/total_tests_executed())

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

// Test code - temp

int func_to_test() {
	return 5;
}

#define ERROR_MESSAGE "Just do it."

class myE : public std::exception {
public:
	const char * what() noexcept {
		return ERROR_MESSAGE;
	}
};

int bad_func_to_test() {
	myE e;
	throw (e);
	return 0;
}

int static_func(int in) {
	static int out = 0;
	out += in;
	return out;
}

void void_func_to_test() {
	static_func(5);
}

void print_report_card() {
	LOG << percent_pass() << "%" << " (" << total_tests_passed() << " tests) passed\n";
	LOG << percent_fail() << "%" << " (" << total_tests_failed() << " tests) failed\n";
	LOG << percent_error_fail() << "%"  << " (" << total_tests_failed_due_to_error() << " tests) failed due to error\n";
	LOG << total_tests_executed() << " tests executed in total\n";
}