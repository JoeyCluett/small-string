#include <iostream>

#include <smallstring/smallstring.h>
#define BOOST_TEST_MODULE small_string_t test cases
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(suite1);

        small_string_t temp_stringa("Hello World");
        small_string_t temp_stringb("hEllo World");
        small_string_t empty_string("");


BOOST_AUTO_TEST_CASE(rvalue_equ_const_char_ptr) { small_string_t("Hello World") == "Hello World"; }
BOOST_AUTO_TEST_CASE(rvalue_equ_rvalue) {         small_string_t("Hello World") == small_string_t("Hello World"); }
BOOST_AUTO_TEST_CASE(rvalue_equ_lvalue) {         small_string_t("Hello World") == temp_stringa; }
BOOST_AUTO_TEST_CASE(rvalue_neq_const_char_ptr) { small_string_t("Hello World") != "hEllo World"; }
BOOST_AUTO_TEST_CASE(rvalue_neq_rvalue) {         small_string_t("Hello World") != small_string_t("hEllo World"); }
BOOST_AUTO_TEST_CASE(rvalue_neq_lvalue) {         small_string_t("Hello World") != temp_stringb; }

/*
BOOST_AUTO_TEST_CASE() {
    small_string_t("Hello World") != temp_stringa; }

BOOST_AUTO_TEST_CASE() {
    small_string_t("Hello World") != "hEllo World"; }

BOOST_AUTO_TEST_CASE() {
    small_string_t("Hello World") != small_string_t("hEllo World"); }

BOOST_AUTO_TEST_CASE() {
    small_string_t("Hello World") != temp_stringb; }

BOOST_AUTO_TEST_CASE() {
    small_string_t("Hello World") != ""; }

BOOST_AUTO_TEST_CASE() {
    small_string_t("") == ""; }

BOOST_AUTO_TEST_CASE() {
    small_string_t("") == empty_string; }
*/

BOOST_AUTO_TEST_SUITE_END()

/*
// rvalue refs are on the left
cout << "rvalue refs on the left...\n";
cout << "Expecting true:  " << (small_string_t("Hello World") == "Hello World") << endl;
cout << "Expecting true:  " << (small_string_t("Hello World") == small_string_t("Hello World")) << endl;
cout << "Expecting true:  " << (small_string_t("Hello World") == temp_stringa) << endl;
cout << "Expecting false: " << (small_string_t("Hello World") == "hEllo World") << endl;
cout << "Expecting false: " << (small_string_t("Hello World") == small_string_t("hEllo World")) << endl;
cout << "Expecting false: " << (small_string_t("Hello World") == temp_stringb) << endl;

cout << "Expecting false: " << (small_string_t("Hello World") != "Hello World") << endl;
cout << "Expecting false: " << (small_string_t("Hello World") != small_string_t("Hello World")) << endl;
cout << "Expecting false: " << (small_string_t("Hello World") != temp_stringa) << endl;
cout << "Expecting true:  " << (small_string_t("Hello World") != "hEllo World") << endl;
cout << "Expecting true:  " << (small_string_t("Hello World") != small_string_t("hEllo World")) << endl;
cout << "Expecting true:  " << (small_string_t("Hello World") != temp_stringb) << endl << flush;

cout << "Expecting false: " << (small_string_t("Hello World") == "") << endl << flush;
cout << "Expecting true:  " << (small_string_t("") == "")            << endl << flush;
cout << "Expecting true:  " << (small_string_t("") == empty_string)  << endl << flush;

// lvalue refs are on the left
cout << "\nlvalue refs on the left...\n";
cout << "Expecting true:  " << (temp_stringa == "Hello World") << endl;
cout << "Expecting true:  " << (temp_stringa == small_string_t("Hello World")) << endl;
cout << "Expecting true:  " << (temp_stringa == temp_stringa) << endl;
cout << "Expecting false: " << (temp_stringa == "hEllo World") << endl;
cout << "Expecting false: " << (temp_stringa == small_string_t("hEllo World")) << endl;
cout << "Expecting false: " << (temp_stringa == temp_stringb) << endl;

cout << "Expecting false: " << (temp_stringa != "Hello World") << endl;
cout << "Expecting false: " << (temp_stringa != small_string_t("Hello World")) << endl;
cout << "Expecting false: " << (temp_stringa != temp_stringa) << endl;
cout << "Expecting true:  " << (temp_stringa != "hEllo World") << endl;
cout << "Expecting true:  " << (temp_stringa != small_string_t("hEllo World")) << endl;
cout << "Expecting true:  " << (temp_stringa != temp_stringb) << endl;

cout << "Expecting false: " << (empty_string == small_string_t("Hello World")) << endl << flush; 
cout << "Expecting false: " << (temp_stringa == "") << endl << flush;
cout << "Expecting true:  " << (empty_string == "") << endl << flush;
cout << "Expecting true:  " << (empty_string == small_string_t("")) << endl << flush;
cout << "Expecting true:  " << (empty_string == empty_string) << endl << flush;
*/
