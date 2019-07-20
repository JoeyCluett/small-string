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

    cout << "Testing iterating over smallstring\n";

    small_string_t s;

    {
        small_string_t str("Hello World");
        for(char c : str) {
            //cout << "Current character: " << c << endl;
            s.push_back(c);
        }

        cout << "Original string: " << str << endl;

    }

    cout << "Modified string: " << s << endl;
    cout << "Test modifying same string while iterating...\n";

    {

        for(char c : s)
            s.push_back(c);

    }

    cout << "Self-modified string: " << s << endl;

    return 0;
}