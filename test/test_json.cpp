/**
 * @file test_json.cpp
 *
 * @brief A unit test suite of Json
 *
 * @author Kenji MINOURA / kenji@kandj.org
 *
 * Copyright (c) 2013 K&J Software Design, Ltd. All rights reserved.
 *
 * @see <related_items>
 ***********************************************************************/
#include <iostream>
#include <cstring>
#include <kjsd/cunit.h>
#include <kjsd/cutil.h>
#include <kjsd/json.h>

using namespace std;

static KJSD_JSON_Value* root_;

static void setUp()
{
    root_ = 0;
}

static void tearDown()
{
    KJSD_JSON_free(root_);
}

static const char* test_deserialize()
{
    FILE* in;

    KJSD_CUNIT_ASSERT(KJSD_JSON_deserialize(0) == 0);

    in = fopen("test/json/test_1_1.txt", "r");
    root_ = KJSD_JSON_deserialize(in);
    KJSD_CUNIT_ASSERT(root_ != 0);
    fclose(in);
    return 0;
}

static const char* test_deserializeS()
{
    KJSD_CUNIT_ASSERT(KJSD_JSON_deserializeS(0) == 0);

    root_ = KJSD_JSON_deserializeS("{ \"aaa\": \"bbb\" }");
    KJSD_CUNIT_ASSERT(root_ != 0);
    return 0;
}

static const char* test_valueGetObject()
{
    root_ = KJSD_JSON_deserializeS("{ \"aaa\": \"bbb\" }");
    KJSD_CUNIT_ASSERT(root_ != 0);

    KJSD_CUNIT_ASSERT(KJSD_JSON_valueGetObject(0) == 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_valueGetObject(root_) != 0);
    return 0;
}

static const char* test_valueGetType()
{
    KJSD_JSON_Object* obj = 0;
    KJSD_JSON_Value* val = 0;

    KJSD_CUNIT_ASSERT(KJSD_JSON_valueGetType(0) == KJSD_JSON_ERROR);

    root_ = KJSD_JSON_deserializeS("{ \"obj\": {},"
                                   "  \"ary\": [],"
                                   "  \"str\": \"aaa\","
                                   "  \"(num)\": 12,"
                                   "  \"bool\": true,"
                                   "  \"null\": null }");
    KJSD_CUNIT_ASSERT(root_ != 0);

    obj = KJSD_JSON_valueGetObject(root_);

    val = KJSD_JSON_objectGetValue(obj, "obj");
    KJSD_CUNIT_ASSERT(KJSD_JSON_valueGetType(val) == KJSD_JSON_OBJECT);
    val = KJSD_JSON_objectGetValue(obj, "ary");
    KJSD_CUNIT_ASSERT(KJSD_JSON_valueGetType(val) == KJSD_JSON_ARRAY);
    val = KJSD_JSON_objectGetValue(obj, "str");
    KJSD_CUNIT_ASSERT(KJSD_JSON_valueGetType(val) == KJSD_JSON_STRING);
    val = KJSD_JSON_objectGetValue(obj, "(num)");
    KJSD_CUNIT_ASSERT(KJSD_JSON_valueGetType(val) == KJSD_JSON_NUMBER);
    val = KJSD_JSON_objectGetValue(obj, "bool");
    KJSD_CUNIT_ASSERT(KJSD_JSON_valueGetType(val) == KJSD_JSON_BOOL);
    val = KJSD_JSON_objectGetValue(obj, "null");
    KJSD_CUNIT_ASSERT(KJSD_JSON_valueGetType(val) == KJSD_JSON_NULL);
    return 0;
}

static const char* test_objectGetValue()
{
    KJSD_JSON_Object* obj = 0;

    root_ = KJSD_JSON_deserializeS("{ \"aaa\": \"bbb\" }");
    KJSD_CUNIT_ASSERT(root_ != 0);

    obj = KJSD_JSON_valueGetObject(root_);

    KJSD_CUNIT_ASSERT(KJSD_JSON_objectGetValue(0, 0) == 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_objectGetValue(obj, 0) == 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_objectGetValue(obj, "zzz") == 0);

    KJSD_CUNIT_ASSERT(KJSD_JSON_objectGetValue(obj, "aaa") != 0);
    return 0;
}

static const char* test_objectGetObject()
{
    KJSD_JSON_Object* obj = 0;

    root_ = KJSD_JSON_deserializeS("{ \"aaa\": {} }");
    KJSD_CUNIT_ASSERT(root_ != 0);

    obj = KJSD_JSON_valueGetObject(root_);

    KJSD_CUNIT_ASSERT(KJSD_JSON_objectGetObject(0, 0) == 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_objectGetObject(obj, 0) == 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_objectGetObject(obj, "zzz") == 0);

    KJSD_CUNIT_ASSERT(KJSD_JSON_objectGetObject(obj, "aaa") != 0);
    return 0;
}

static const char* test_objectGetArray()
{
    KJSD_JSON_Object* obj = 0;

    root_ = KJSD_JSON_deserializeS("{ \"aaa\": [] }");
    KJSD_CUNIT_ASSERT(root_ != 0);

    obj = KJSD_JSON_valueGetObject(root_);

    KJSD_CUNIT_ASSERT(KJSD_JSON_objectGetArray(0, 0) == 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_objectGetArray(obj, 0) == 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_objectGetArray(obj, "zzz") == 0);

    KJSD_CUNIT_ASSERT(KJSD_JSON_objectGetArray(obj, "aaa") != 0);
    return 0;
}

static const char* test_objectGetString()
{
    KJSD_JSON_Object* obj = 0;
    const char* val = 0;

    root_ = KJSD_JSON_deserializeS("{ \"aaa\": \"bbb\" }");
    KJSD_CUNIT_ASSERT(root_ != 0);

    obj = KJSD_JSON_valueGetObject(root_);

    KJSD_CUNIT_ASSERT(KJSD_JSON_objectGetString(0, 0) == 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_objectGetString(obj, 0) == 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_objectGetString(obj, "zzz") == 0);

    val = KJSD_JSON_objectGetString(obj, "aaa");
    KJSD_CUNIT_ASSERT(val != 0);
    KJSD_CUNIT_ASSERT(strcmp(val, "bbb") == 0);
    return 0;
}

static const char* test_objectGetNumber()
{
    KJSD_JSON_Object* obj = 0;

    root_ = KJSD_JSON_deserializeS("{ \"aaa\": 1,"
                                   "  \"bbb\": 1.123 }");
    KJSD_CUNIT_ASSERT(root_ != 0);

    obj = KJSD_JSON_valueGetObject(root_);

    KJSD_CUNIT_ASSERT(KJSD_JSON_objectGetNumber(0, 0) == 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_objectGetNumber(obj, 0) == 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_objectGetNumber(obj, "zzz") == 0);

    KJSD_CUNIT_ASSERT(KJSD_JSON_objectGetNumber(obj, "aaa") == 1);
    KJSD_CUNIT_ASSERT(KJSD_JSON_objectGetNumber(obj, "bbb") == 1.123);
    return 0;
}

static const char* test_objectGetBool()
{
    KJSD_JSON_Object* obj = 0;

    root_ = KJSD_JSON_deserializeS("{ \"aaa\": true,"
                                   "  \"bbb\": false }");
    KJSD_CUNIT_ASSERT(root_ != 0);

    obj = KJSD_JSON_valueGetObject(root_);

    KJSD_CUNIT_ASSERT(KJSD_JSON_objectGetBool(0, 0) < 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_objectGetBool(obj, 0) < 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_objectGetBool(obj, "zzz") < 0);

    KJSD_CUNIT_ASSERT(KJSD_JSON_objectGetBool(obj, "aaa") == 1);
    KJSD_CUNIT_ASSERT(KJSD_JSON_objectGetBool(obj, "bbb") == 0);
    return 0;
}

static const char* test_objectDotGetValue()
{
    KJSD_JSON_Object* obj = 0;

    root_ = KJSD_JSON_deserializeS("{ \"aaa\":{ \"bbb\":{ \"ccc\": 1 }}}");
    KJSD_CUNIT_ASSERT(root_ != 0);

    obj = KJSD_JSON_valueGetObject(root_);

    KJSD_CUNIT_ASSERT(KJSD_JSON_objectDotGetValue(0, 0) == 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_objectDotGetValue(obj, 0) == 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_objectDotGetValue(obj, "zzz") == 0);

    KJSD_CUNIT_ASSERT(KJSD_JSON_objectDotGetValue(obj, "aaa.bbb.ccc") != 0);
    return 0;
}

static const char* test_objectDotGetObject()
{
    KJSD_JSON_Object* obj = 0;

    root_ = KJSD_JSON_deserializeS("{ \"aaa\":{ \"bbb\":{ \"ccc\": {} }}}");
    KJSD_CUNIT_ASSERT(root_ != 0);

    obj = KJSD_JSON_valueGetObject(root_);

    KJSD_CUNIT_ASSERT(KJSD_JSON_objectDotGetObject(0, 0) == 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_objectDotGetObject(obj, 0) == 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_objectDotGetObject(obj, "zzz") == 0);

    KJSD_CUNIT_ASSERT(KJSD_JSON_objectDotGetObject(obj, "aaa.bbb.ccc") != 0);
    return 0;
}

static const char* test_objectDotGetArray()
{
    KJSD_JSON_Object* obj = 0;

    root_ = KJSD_JSON_deserializeS("{ \"aaa\":{ \"bbb\":{ \"ccc\": [] }}}");
    KJSD_CUNIT_ASSERT(root_ != 0);

    obj = KJSD_JSON_valueGetObject(root_);

    KJSD_CUNIT_ASSERT(KJSD_JSON_objectDotGetArray(0, 0) == 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_objectDotGetArray(obj, 0) == 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_objectDotGetArray(obj, "zzz") == 0);

    KJSD_CUNIT_ASSERT(KJSD_JSON_objectDotGetArray(obj, "aaa.bbb.ccc") != 0);
    return 0;
}

static const char* test_objectDotGetString()
{
    KJSD_JSON_Object* obj = 0;
    const char* val = 0;

    root_ = KJSD_JSON_deserializeS("{ \"aaa\":{ \"bbb\":"
                                   "  { \"ccc\": \"str\" }}}");
    KJSD_CUNIT_ASSERT(root_ != 0);

    obj = KJSD_JSON_valueGetObject(root_);

    KJSD_CUNIT_ASSERT(KJSD_JSON_objectDotGetString(0, 0) == 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_objectDotGetString(obj, 0) == 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_objectDotGetString(obj, "zzz") == 0);

    val = KJSD_JSON_objectDotGetString(obj, "aaa.bbb.ccc");
    KJSD_CUNIT_ASSERT(val != 0);
    KJSD_CUNIT_ASSERT(strcmp(val, "str") == 0);
    return 0;
}

static const char* test_objectDotGetNumber()
{
    KJSD_JSON_Object* obj = 0;

    root_ = KJSD_JSON_deserializeS("{ \"aaa\":{ \"bbb\":"
                                   "  { \"ccc\": 1,"
                                   "    \"ddd\": 1.123 }}}");
    KJSD_CUNIT_ASSERT(root_ != 0);

    obj = KJSD_JSON_valueGetObject(root_);

    KJSD_CUNIT_ASSERT(KJSD_JSON_objectDotGetNumber(0, 0) == 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_objectDotGetNumber(obj, 0) == 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_objectDotGetNumber(obj, "zzz") == 0);

    KJSD_CUNIT_ASSERT(KJSD_JSON_objectDotGetNumber(obj, "aaa.bbb.ccc") == 1);
    KJSD_CUNIT_ASSERT(KJSD_JSON_objectDotGetNumber(obj, "aaa.bbb.ddd") == 1.123);
    return 0;
}

static const char* test_objectDotGetBool()
{
    KJSD_JSON_Object* obj = 0;

    root_ = KJSD_JSON_deserializeS("{ \"aaa\":{ \"bbb\":"
                                   "  { \"ccc\": true,"
                                   "    \"ddd\": false }}}");
    KJSD_CUNIT_ASSERT(root_ != 0);

    obj = KJSD_JSON_valueGetObject(root_);

    KJSD_CUNIT_ASSERT(KJSD_JSON_objectDotGetBool(0, 0) < 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_objectDotGetBool(obj, 0) < 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_objectDotGetBool(obj, "zzz") < 0);

    KJSD_CUNIT_ASSERT(KJSD_JSON_objectDotGetBool(obj, "aaa.bbb.ccc") == 1);
    KJSD_CUNIT_ASSERT(KJSD_JSON_objectDotGetBool(obj, "aaa.bbb.ddd") == 0);
    return 0;
}

static const char* test_objectGetCount()
{
    KJSD_JSON_Object* obj = 0;

    KJSD_CUNIT_ASSERT(KJSD_JSON_objectGetCount(0) == 0);

    root_ = KJSD_JSON_deserializeS("{ \"aaa\":{ \"bbb\": 1},"
                                   "  \"ccc\": [],"
                                   "  \"ddd\": \"str\"}");
    KJSD_CUNIT_ASSERT(root_ != 0);

    obj = KJSD_JSON_valueGetObject(root_);

    KJSD_CUNIT_ASSERT(KJSD_JSON_objectGetCount(obj) == 3);
    return 0;
}

static const char* test_objectGetName()
{
    KJSD_JSON_Object* obj = 0;
    const char* name = 0;

    root_ = KJSD_JSON_deserializeS("{ \"aaa\": 1,"
                                   "  \"bbb\": 2 }");
    KJSD_CUNIT_ASSERT(root_ != 0);

    obj = KJSD_JSON_valueGetObject(root_);

    KJSD_CUNIT_ASSERT(KJSD_JSON_objectGetName(0, 0) == 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_objectGetName(obj, 255) == 0);

    name = KJSD_JSON_objectGetName(obj, 0);
    KJSD_CUNIT_ASSERT(name != 0);
    KJSD_CUNIT_ASSERT(strcmp(name, "aaa") == 0);

    name = KJSD_JSON_objectGetName(obj, 1);
    KJSD_CUNIT_ASSERT(name != 0);
    KJSD_CUNIT_ASSERT(strcmp(name, "bbb") == 0);
    return 0;
}

static const char* test_arrayGetValue()
{
    KJSD_JSON_Array* ary = 0;

    root_ = KJSD_JSON_deserializeS("[ {}, [], \"str\", 1, true ]");
    KJSD_CUNIT_ASSERT(root_ != 0);

    ary = KJSD_JSON_valueGetArray(root_);

    KJSD_CUNIT_ASSERT(KJSD_JSON_arrayGetValue(0, 0) == 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_arrayGetValue(ary, 255) == 0);

    KJSD_CUNIT_ASSERT(KJSD_JSON_arrayGetValue(ary, 0) != 0);
    return 0;
}

static const char* test_arrayGetObject()
{
    KJSD_JSON_Array* ary = 0;

    root_ = KJSD_JSON_deserializeS("[ {}, [], \"str\", 1, true ]");
    KJSD_CUNIT_ASSERT(root_ != 0);

    ary = KJSD_JSON_valueGetArray(root_);

    KJSD_CUNIT_ASSERT(KJSD_JSON_arrayGetObject(0, 0) == 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_arrayGetObject(ary, 255) == 0);

    KJSD_CUNIT_ASSERT(KJSD_JSON_arrayGetObject(ary, 0) != 0);
    return 0;
}

static const char* test_arrayGetArray()
{
    KJSD_JSON_Array* ary = 0;

    root_ = KJSD_JSON_deserializeS("[ {}, [], \"str\", 1, true ]");
    KJSD_CUNIT_ASSERT(root_ != 0);

    ary = KJSD_JSON_valueGetArray(root_);

    KJSD_CUNIT_ASSERT(KJSD_JSON_arrayGetArray(0, 0) == 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_arrayGetArray(ary, 255) == 0);

    KJSD_CUNIT_ASSERT(KJSD_JSON_arrayGetArray(ary, 1) != 0);
    return 0;
}

static const char* test_arrayGetString()
{
    KJSD_JSON_Array* ary = 0;
    const char* val = 0;

    root_ = KJSD_JSON_deserializeS("[ {}, [], \"str\", 1, true ]");
    KJSD_CUNIT_ASSERT(root_ != 0);

    ary = KJSD_JSON_valueGetArray(root_);

    KJSD_CUNIT_ASSERT(KJSD_JSON_arrayGetString(0, 0) == 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_arrayGetString(ary, 255) == 0);

    val = KJSD_JSON_arrayGetString(ary, 2);
    KJSD_CUNIT_ASSERT(val != 0);
    KJSD_CUNIT_ASSERT(strcmp(val, "str") == 0);
    return 0;
}

static const char* test_arrayGetNumber()
{
    KJSD_JSON_Array* ary = 0;

    root_ = KJSD_JSON_deserializeS("[ {}, [], \"str\", 1, true ]");
    KJSD_CUNIT_ASSERT(root_ != 0);

    ary = KJSD_JSON_valueGetArray(root_);

    KJSD_CUNIT_ASSERT(KJSD_JSON_arrayGetNumber(0, 0) == 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_arrayGetNumber(ary, 255) == 0);

    KJSD_CUNIT_ASSERT(KJSD_JSON_arrayGetNumber(ary, 3) == 1);
    return 0;
}

static const char* test_arrayGetBool()
{
    KJSD_JSON_Array* ary = 0;

    root_ = KJSD_JSON_deserializeS("[ {}, [], \"str\", 1, true ]");
    KJSD_CUNIT_ASSERT(root_ != 0);

    ary = KJSD_JSON_valueGetArray(root_);

    KJSD_CUNIT_ASSERT(KJSD_JSON_arrayGetBool(0, 0) < 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_arrayGetBool(ary, 255) < 0);

    KJSD_CUNIT_ASSERT(KJSD_JSON_arrayGetBool(ary, 4) == 1);
    return 0;
}

static const char* test_arrayGetCount()
{
    KJSD_JSON_Array* ary = 0;

    root_ = KJSD_JSON_deserializeS("[ {}, [], \"str\", 1, true ]");
    KJSD_CUNIT_ASSERT(root_ != 0);

    ary = KJSD_JSON_valueGetArray(root_);

    KJSD_CUNIT_ASSERT(KJSD_JSON_arrayGetCount(0) == 0);

    KJSD_CUNIT_ASSERT(KJSD_JSON_arrayGetCount(ary) == 5);
    return 0;
}

static const char* test_valueGetArray()
{
    root_ = KJSD_JSON_deserializeS("[ \"aaa\", \"bbb\" ]");
    KJSD_CUNIT_ASSERT(root_ != 0);

    KJSD_CUNIT_ASSERT(KJSD_JSON_valueGetArray(0) == 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_valueGetArray(root_) != 0);
    return 0;
}

static const char* test_valueGetString()
{
    KJSD_JSON_Value* val = 0;
    const char* str = 0;

    KJSD_CUNIT_ASSERT(KJSD_JSON_valueGetString(0) == 0);

    root_ = KJSD_JSON_deserializeS("{ \"aaa\": \"bbb\" }");
    KJSD_CUNIT_ASSERT(root_ != 0);

    val = KJSD_JSON_objectGetValue(KJSD_JSON_valueGetObject(root_), "aaa");

    str = KJSD_JSON_valueGetString(val);
    KJSD_CUNIT_ASSERT(str != 0);
    KJSD_CUNIT_ASSERT(strcmp(str, "bbb") == 0);
    return 0;
}

static const char* test_valueGetNumber()
{
    KJSD_JSON_Object* obj = 0;
    KJSD_JSON_Value* val = 0;

    KJSD_CUNIT_ASSERT(KJSD_JSON_valueGetNumber(0) == 0);

    root_ = KJSD_JSON_deserializeS("{ \"aaa\": 1,"
                                   "  \"bbb\": 1.123 }");
    KJSD_CUNIT_ASSERT(root_ != 0);

    obj = KJSD_JSON_valueGetObject(root_);

    val = KJSD_JSON_objectGetValue(obj, "aaa");
    KJSD_CUNIT_ASSERT(KJSD_JSON_valueGetNumber(val) == 1);

    val = KJSD_JSON_objectGetValue(obj, "bbb");
    KJSD_CUNIT_ASSERT(KJSD_JSON_valueGetNumber(val) == 1.123);
    return 0;
}

static const char* test_valueGetBool()
{
    KJSD_JSON_Object* obj = 0;
    KJSD_JSON_Value* val = 0;

    KJSD_CUNIT_ASSERT(KJSD_JSON_valueGetBool(0) < 0);

    root_ = KJSD_JSON_deserializeS("{ \"aaa\": true,"
                                   "  \"bbb\": false }");
    KJSD_CUNIT_ASSERT(root_ != 0);

    obj = KJSD_JSON_valueGetObject(root_);

    val = KJSD_JSON_objectGetValue(obj, "aaa");
    KJSD_CUNIT_ASSERT(KJSD_JSON_valueGetBool(val) == 1);

    val = KJSD_JSON_objectGetValue(obj, "bbb");
    KJSD_CUNIT_ASSERT(KJSD_JSON_valueGetBool(val) == 0);
    return 0;
}

static const char* test_createRoot()
{
    root_ = KJSD_JSON_createRoot();
    KJSD_CUNIT_ASSERT(root_ != 0);
    return 0;
}

static const char* test_objectAddObject()
{
    KJSD_JSON_Object* base = 0;

    root_ = KJSD_JSON_createRoot();
    KJSD_CUNIT_ASSERT(root_ != 0);

    base = KJSD_JSON_valueGetObject(root_);
    KJSD_CUNIT_ASSERT(base != 0);

    KJSD_CUNIT_ASSERT(KJSD_JSON_objectAddObject(0, 0) == 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_objectAddObject(base, 0) == 0);

    KJSD_CUNIT_ASSERT(KJSD_JSON_objectAddObject(base, "obj") != 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_objectGetObject(base, "obj") != 0);
    return 0;
}

static const char* test_objectAddArray()
{
    KJSD_JSON_Object* base = 0;

    root_ = KJSD_JSON_createRoot();
    KJSD_CUNIT_ASSERT(root_ != 0);

    base = KJSD_JSON_valueGetObject(root_);
    KJSD_CUNIT_ASSERT(base != 0);

    KJSD_CUNIT_ASSERT(KJSD_JSON_objectAddArray(0, 0) == 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_objectAddArray(base, 0) == 0);

    KJSD_CUNIT_ASSERT(KJSD_JSON_objectAddArray(base, "ary") != 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_objectGetArray(base, "ary") != 0);
    return 0;
}

static const char* test_objectAddString()
{
    const char* str = 0;

    KJSD_JSON_Object* base = 0;

    root_ = KJSD_JSON_createRoot();
    KJSD_CUNIT_ASSERT(root_ != 0);

    base = KJSD_JSON_valueGetObject(root_);
    KJSD_CUNIT_ASSERT(base != 0);

    KJSD_CUNIT_ASSERT(KJSD_JSON_objectAddString(0, 0, 0) == 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_objectAddString(base, 0, 0) == 0);

    KJSD_CUNIT_ASSERT(KJSD_JSON_objectAddString(base, "foo", 0) > 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_objectAddString(base, "string", "aaa") > 0);
    str = KJSD_JSON_objectGetString(base, "string");
    KJSD_CUNIT_ASSERT(strcmp(str, "aaa") == 0);
    return 0;
}

static const char* test_objectAddNumber()
{
    KJSD_JSON_Object* base = 0;

    root_ = KJSD_JSON_createRoot();
    KJSD_CUNIT_ASSERT(root_ != 0);

    base = KJSD_JSON_valueGetObject(root_);
    KJSD_CUNIT_ASSERT(base != 0);

    KJSD_CUNIT_ASSERT(KJSD_JSON_objectAddNumber(0, 0, 0) == 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_objectAddNumber(base, 0, 0) == 0);

    KJSD_CUNIT_ASSERT(KJSD_JSON_objectAddNumber(base, "number", 1.12) > 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_objectGetNumber(base, "number") == 1.12);
    return 0;
}

static const char* test_objectAddBool()
{
    KJSD_JSON_Object* base = 0;

    root_ = KJSD_JSON_createRoot();
    KJSD_CUNIT_ASSERT(root_ != 0);

    base = KJSD_JSON_valueGetObject(root_);
    KJSD_CUNIT_ASSERT(base != 0);

    KJSD_CUNIT_ASSERT(KJSD_JSON_objectAddBool(0, 0, 0) == 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_objectAddBool(base, 0, 0) == 0);

    KJSD_CUNIT_ASSERT(KJSD_JSON_objectAddBool(base, "bool", 0) > 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_objectGetBool(base, "bool") == 0);
    return 0;
}

static const char* test_arrayAddObject()
{
    KJSD_JSON_Object* base = 0;
    KJSD_JSON_Array* ary = 0;

    root_ = KJSD_JSON_createRoot();
    KJSD_CUNIT_ASSERT(root_ != 0);
    base = KJSD_JSON_valueGetObject(root_);
    KJSD_CUNIT_ASSERT(base != 0);
    ary = KJSD_JSON_objectAddArray(base, "ary");
    KJSD_CUNIT_ASSERT(ary != 0);

    KJSD_CUNIT_ASSERT(KJSD_JSON_arrayAddObject(0) == 0);

    KJSD_CUNIT_ASSERT(KJSD_JSON_arrayAddObject(ary) != 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_arrayGetObject(ary, 0) != 0);
    return 0;
}

static const char* test_arrayAddArray()
{
    KJSD_JSON_Object* base = 0;
    KJSD_JSON_Array* ary = 0;

    root_ = KJSD_JSON_createRoot();
    KJSD_CUNIT_ASSERT(root_ != 0);
    base = KJSD_JSON_valueGetObject(root_);
    KJSD_CUNIT_ASSERT(base != 0);
    ary = KJSD_JSON_objectAddArray(base, "ary");
    KJSD_CUNIT_ASSERT(ary != 0);

    KJSD_CUNIT_ASSERT(KJSD_JSON_arrayAddArray(0) == 0);

    KJSD_CUNIT_ASSERT(KJSD_JSON_arrayAddArray(ary) != 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_arrayGetArray(ary, 0) != 0);
    return 0;
}

static const char* test_arrayAddString()
{
    const char* str = 0;

    KJSD_JSON_Object* base = 0;
    KJSD_JSON_Array* ary = 0;

    root_ = KJSD_JSON_createRoot();
    KJSD_CUNIT_ASSERT(root_ != 0);
    base = KJSD_JSON_valueGetObject(root_);
    KJSD_CUNIT_ASSERT(base != 0);
    ary = KJSD_JSON_objectAddArray(base, "ary");
    KJSD_CUNIT_ASSERT(ary != 0);

    KJSD_CUNIT_ASSERT(KJSD_JSON_arrayAddString(0, 0) == 0);

    KJSD_CUNIT_ASSERT(KJSD_JSON_arrayAddString(ary, 0) > 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_arrayAddString(ary, "aaa") > 0);
    str = KJSD_JSON_arrayGetString(ary, 1);
    KJSD_CUNIT_ASSERT(strcmp(str, "aaa") == 0);
    return 0;
}

static const char* test_arrayAddNumber()
{
    KJSD_JSON_Object* base = 0;
    KJSD_JSON_Array* ary = 0;

    root_ = KJSD_JSON_createRoot();
    KJSD_CUNIT_ASSERT(root_ != 0);
    base = KJSD_JSON_valueGetObject(root_);
    KJSD_CUNIT_ASSERT(base != 0);
    ary = KJSD_JSON_objectAddArray(base, "ary");
    KJSD_CUNIT_ASSERT(ary != 0);

    KJSD_CUNIT_ASSERT(KJSD_JSON_arrayAddNumber(0, 0) == 0);

    KJSD_CUNIT_ASSERT(KJSD_JSON_arrayAddNumber(ary, 1.12) > 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_arrayGetNumber(ary, 0) == 1.12);
    return 0;
}

static const char* test_arrayAddBool()
{
    KJSD_JSON_Object* base = 0;
    KJSD_JSON_Array* ary = 0;

    root_ = KJSD_JSON_createRoot();
    KJSD_CUNIT_ASSERT(root_ != 0);
    base = KJSD_JSON_valueGetObject(root_);
    KJSD_CUNIT_ASSERT(base != 0);
    ary = KJSD_JSON_objectAddArray(base, "ary");
    KJSD_CUNIT_ASSERT(ary != 0);

    KJSD_CUNIT_ASSERT(KJSD_JSON_arrayAddBool(0, 0) == 0);

    KJSD_CUNIT_ASSERT(KJSD_JSON_arrayAddBool(ary, 0) > 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_arrayGetBool(ary, 0) == 0);
    return 0;
}

static const char* test_sizeof()
{
    static const char* json_str = "[{},[],\"str\",1,true]";

    root_ = KJSD_JSON_deserializeS(json_str);
    KJSD_CUNIT_ASSERT(root_ != 0);

    KJSD_CUNIT_ASSERT(KJSD_JSON_sizeof(0) == 0);

    KJSD_CUNIT_ASSERT(KJSD_JSON_sizeof(root_) == strlen(json_str));
    return 0;
}

static const char* test_serialize()
{
    static const char* json_str = "[{},[],\"str\",1,true]";

    cout << endl;

    root_ = KJSD_JSON_deserializeS(json_str);
    KJSD_CUNIT_ASSERT(root_ != 0);

    KJSD_CUNIT_ASSERT(KJSD_JSON_serialize(0, 0) == 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_serialize(root_, 0) == 0);

    KJSD_CUNIT_ASSERT(KJSD_JSON_serialize(root_, stdout) == strlen(json_str));

    cout << endl;
    return 0;
}

static const char* test_serializeS()
{
    static const char* json_str = "[{},[],\"str\",1,true]";
    char* out = 0;

    root_ = KJSD_JSON_deserializeS(json_str);
    KJSD_CUNIT_ASSERT(root_ != 0);

    size_t sz = KJSD_JSON_sizeof(root_) + 1;
    out = new char[sz];

    KJSD_CUNIT_ASSERT(KJSD_JSON_serializeS(0, 0, 0) == 0);
    KJSD_CUNIT_ASSERT(KJSD_JSON_serializeS(root_, 0, 0) == 0);

    KJSD_CUNIT_ASSERT(KJSD_JSON_serializeS(root_, out, sz) == (sz - 1));
    return 0;
}

const char* test_json()
{
    const KJSD_CUNIT_Func f[] = {
        test_deserialize,
        test_deserializeS,
        test_valueGetObject,
        test_valueGetType,
        test_objectGetValue,
        test_objectGetObject,
        test_objectGetArray,
        test_objectGetString,
        test_objectGetNumber,
        test_objectGetBool,
        test_objectDotGetValue,
        test_objectDotGetObject,
        test_objectDotGetArray,
        test_objectDotGetString,
        test_objectDotGetNumber,
        test_objectDotGetBool,
        test_objectGetCount,
        test_objectGetName,
        test_arrayGetValue,
        test_arrayGetObject,
        test_arrayGetArray,
        test_arrayGetString,
        test_arrayGetNumber,
        test_arrayGetBool,
        test_arrayGetCount,
        test_valueGetArray,
        test_valueGetString,
        test_valueGetNumber,
        test_valueGetBool,
        test_createRoot,
        test_objectAddObject,
        test_objectAddArray,
        test_objectAddString,
        test_objectAddNumber,
        test_objectAddBool,
        test_arrayAddObject,
        test_arrayAddArray,
        test_arrayAddString,
        test_arrayAddNumber,
        test_arrayAddBool,
        test_sizeof,
        test_serialize,
        test_serializeS
    };

    for (int i = 0; i < KJSD_CUTIL_LENGTH(f); i++)
    {
        setUp();
        KJSD_CUNIT_RUN(f[i]);
        tearDown();
    }
    return 0;
}
