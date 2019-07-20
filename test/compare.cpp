#include <iostream>
#include <map>
#include <utility>
#include <vector>
#include <assert.h>

//#define SMALLSTRING_USING_THREADS
//#include <smallstring/smallstring.h>
#include "../smallstring.h"

using namespace std;

int main(int argc, char* argv[]) {

    cout << boolalpha; // true/false instead of 1/0

    {
        small_string_t temp_stringa("Hello World");
        small_string_t temp_stringb("hEllo World");
        small_string_t empty_string("");

        cout << "Testing equivalence operators\n\n";
        
        // rvalue refs are on the left
        cout << "rvalue refs on the left...\n";
        cout << "Expecting true:  " << (small_string_t("Hello World") == "Hello World") << endl;
        cout << "Expecting true:  " << (small_string_t("Hello World") == small_string_t("Hello World")) << endl;
        cout << "Expecting true:  " << (small_string_t("Hello World") == temp_stringa)                  << endl;
        cout << "Expecting false: " << (small_string_t("Hello World") == "hEllo World")                 << endl;
        cout << "Expecting false: " << (small_string_t("Hello World") == small_string_t("hEllo World")) << endl;
        cout << "Expecting false: " << (small_string_t("Hello World") == temp_stringb)                  << endl;

        cout << "Expecting false: " << (small_string_t("Hello World") != "Hello World")                 << endl;
        cout << "Expecting false: " << (small_string_t("Hello World") != small_string_t("Hello World")) << endl;
        cout << "Expecting false: " << (small_string_t("Hello World") != temp_stringa)                  << endl;
        cout << "Expecting true:  " << (small_string_t("Hello World") != "hEllo World")                 << endl;
        cout << "Expecting true:  " << (small_string_t("Hello World") != small_string_t("hEllo World")) << endl;
        cout << "Expecting true:  " << (small_string_t("Hello World") != temp_stringb) << endl << flush;

        cout << "Expecting false: " << (small_string_t("Hello World") == "") << endl << flush;
        cout << "Expecting true:  " << (small_string_t("") == "")            << endl << flush;
        cout << "Expecting true:  " << (small_string_t("") == empty_string)  << endl << flush;

        small_string_t::print_info(cout) << "\n";


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

        small_string_t::print_info(cout) << "\n";

    }

    cout << "Should be empty...\n";
    small_string_t::print_info(cout) << "\n";

}