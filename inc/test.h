#ifndef TEST_H
#define TEST_H

#include <string.h>
#include <iostream>
#include <exception>
#include <stdbool.h>

typedef enum {
	not_executed = -1,
	fail = 0,
	pass = 1,
	error_fail = 2
} ResultType;

int total_tests_executed();

int total_tests_passed();

int total_tests_failed();

int total_tests_failed_due_to_error();

float percent_pass();

float percent_fail();

float percent_error_fail();

/**
 * @brief Defines how a ResultType is printed to an output stream
 */
std::ostream& operator<<(std::ostream& os,const ResultType& t);

/**
 * @brief Records a result of the specified type
 */
void record_result(ResultType t);

void print_report_card();

#define LOG std::cout

#define LINE_INFO __FILE__ << ", " << __FUNCTION__ << ", " << __LINE__

#define FLOAT_AS_PERCENTAGE(__num__) ((100.0f*__num__)/total_tests_executed())

#define RECORD_RESULT(__exp__,__result_type__) {\
	LOG << __result_type__ << " (" << LINE_INFO << "): " << __exp__ << " " << std::endl;\
	record_result(__result_type__);\
}

#define PASS(__arg1__,__op__,__arg2__) RECORD_RESULT(__arg1__ __op__ __arg2__,pass)

#define FAIL(__arg1__,__op__,__arg2__) RECORD_RESULT(__arg1__ __op__ __arg2__,fail)

#define ERROR_FAIL(__arg1__,__op__,__arg2__) RECORD_RESULT(__arg1__ __op__ __arg2__,error_fail);

/**
 * @brief Creates a test and documents the result. Prints results. Provides useful debugging info if it fails.
 */
#define EXPECT(__arg1__,__op__,__arg2__)\
	try {\
		if (__arg1__ __op__ __arg2__) {\
			PASS(#__arg1__,#__op__,#__arg2__);\
		} else {\
			FAIL(#__arg1__,#__op__,#__arg2__);\
		}\
	} catch (...) {\
		ERROR_FAIL(#__arg1__,#__op__,#__arg2__);\
	}

#define TEST_RETURN_FUNC(__function_to_be_tested,__op__,__correct_output) EXPECT(__function_to_be_tested,__op__,__correct_output);

#define TEST_VOID_FUNC(__function_to_be_tested,__output_to_be_tested,__op__,__correct_output)\
	try {\
		__function_to_be_tested;\
	} catch (...) {\
		ERROR_FAIL(#__output_to_be_tested,#__op__,#__correct_output);\
	}\
	EXPECT(__output_to_be_tested,__op__,__correct_output);

#define TEST_EXCEPTION(__function_to_be_tested,__correct_error_message)\
	try {\
		__function_to_be_tested;\
	} catch (std::exception& e) {\
		if(strcmp(e.what(),__correct_error_message)) {\
			PASS("e.what()","==",__correct_error_message);\
		} else {\
			FAIL("e.what()","==",__correct_error_message);\
		}\
	}

#endif // TEST_H