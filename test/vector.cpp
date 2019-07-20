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

    {
        cout << "Testing with std::vector...\n";
        std::vector<small_string_t> string_vector = { "string0", "string1", "string2", "string3" };

        for(int i = 0; i < string_vector.size(); i++) {
            cout << i << " : '" << string_vector[i] << "'\n";
        }
        small_string_t::print_info(cout) << "\n";

        // reverse the vector
        std::reverse(string_vector.begin(), string_vector.end());
        cout << "After reversing the vector...\n";
        for(int i = 0; i < string_vector.size(); i++) {
            cout << i << " : '" << string_vector[i] << "'\n";
        }
        small_string_t::print_info(cout) << "\n";

        string_vector.clear();
        cout << "After clearing the vector...\n";
        for(int i = 0; i < string_vector.size(); i++) {
            cout << i << " : '" << string_vector[i] << "'\n";
        }
        small_string_t::print_info(cout) << "\n";
    
    }

    

    // add enough strings to a vector that it has to reallocate storage
    {
        std::vector<small_string_t> string_vector;
        string_vector.reserve(8);
        cout << "\nString vector capacity: " << string_vector.capacity() << endl;
        auto cap = string_vector.capacity();
        for(int i = 0; i < cap+1; i++) {
            // force vector to reallocate eventually
            string_vector.emplace_back("Newstring");
        }

    }

    // test the push_back method
    {
        cout << "\nTesting .push_back\n";
        small_string_t a("Hello"), b("There"), c("General");
        small_string_t::print_info(cout) << "\n";

        // add a bunch of characters to the string
        for(int i = 0; i < 5; i++) {
            a.push_back('X');
            small_string_t::print_info(cout) << "\n";
        }

        cout << "Clearing string\n";
        a.clear();
        small_string_t::print_info(cout) << "\n";

        // add more characters to it
        for(int i = 0; i < 5; i++) {
            a.push_back('Z');
            small_string_t::print_info(cout) << "\n";
        }
    }

    cout << "Should be empty:\n";
    small_string_t::print_info(cout) << "\n";

    return 0;

}