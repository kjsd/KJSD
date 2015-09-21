/**
 * @file testmain.cpp
 *
 * @brief A main program for all test suits
 *
 * @author Kenji MINOURA / kenji@kandj.org
 *
 * Copyright (c) 2012 K&J Software Design, Ltd. All rights reserved.
 *
 * @see <related_items>
 ***********************************************************************/
#include <iostream>
#include <kjsd/cunit.h>

using namespace std;

int KJSD_CUNIT_run_count = 0;

extern const char* test_argument();
extern const char* test_hashtable();
extern const char* test_command();
extern const char* test_delegate();
extern const char* test_json();
extern const char* test_shared_ptr();
extern const char* test_singleton();
extern const char* test_state_machine();
extern const char* test_util();

static void print(KJSD_CUNIT_Func f, const char* name)
{
    cout << name << ": Start >>>" << endl;
    const char* msg = f();
    if (msg) cout << msg << endl;
    else cout << "<<< " << name << ": OK." << endl;
}

#define RUN(f) print(f, #f)

int main(int argc, char* argv[])
{
    RUN(test_argument);
    RUN(test_singleton);
    RUN(test_shared_ptr);
    RUN(test_hashtable);
    RUN(test_delegate);
    RUN(test_command);
    RUN(test_json);
    RUN(test_state_machine);

    cout << "Test finished[" << KJSD_CUNIT_run_count << "]" << endl;
    return 0;
}
