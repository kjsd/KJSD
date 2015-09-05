/**
 * @file test_json.cpp
 *
 * @version $Id: 1dfac53cdf1068d28f0ab483695c256624b91bf3 $
 *
 * @brief A unit test suite of Json
 *
 * @author Kenji MINOURA / kenji@kandj.org
 *
 * Copyright (c) 2013 K&J Software Design, Ltd. All rights reserved.
 *
 * @see <related_items>
 ***********************************************************************/
#include <cppunit/extensions/HelperMacros.h>

#include <cassert>
#include <iostream>
#include <cstring>
#include "../include/kjsd/json.h"

using namespace std;

/**
 * @class TEST_Json
 *
 * @brief Jsonクラスのユニットテストスイート
 *
 * @see Json
 */
// CppUnit::TestFixtureを継承させる
class TEST_Json : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TEST_Json);

    CPPUNIT_TEST(test_deserialize);
    CPPUNIT_TEST(test_deserializeS);
    CPPUNIT_TEST(test_valueGetObject);
    CPPUNIT_TEST(test_valueGetType);
    CPPUNIT_TEST(test_objectGetValue);
    CPPUNIT_TEST(test_objectGetObject);
    CPPUNIT_TEST(test_objectGetArray);
    CPPUNIT_TEST(test_objectGetString);
    CPPUNIT_TEST(test_objectGetNumber);
    CPPUNIT_TEST(test_objectGetBool);
    CPPUNIT_TEST(test_objectDotGetValue);
    CPPUNIT_TEST(test_objectDotGetObject);
    CPPUNIT_TEST(test_objectDotGetArray);
    CPPUNIT_TEST(test_objectDotGetString);
    CPPUNIT_TEST(test_objectDotGetNumber);
    CPPUNIT_TEST(test_objectDotGetBool);
    CPPUNIT_TEST(test_objectGetCount);
    CPPUNIT_TEST(test_objectGetName);
    CPPUNIT_TEST(test_arrayGetValue);
    CPPUNIT_TEST(test_arrayGetObject);
    CPPUNIT_TEST(test_arrayGetArray);
    CPPUNIT_TEST(test_arrayGetString);
    CPPUNIT_TEST(test_arrayGetNumber);
    CPPUNIT_TEST(test_arrayGetBool);
    CPPUNIT_TEST(test_arrayGetCount);
    CPPUNIT_TEST(test_valueGetArray);
    CPPUNIT_TEST(test_valueGetString);
    CPPUNIT_TEST(test_valueGetNumber);
    CPPUNIT_TEST(test_valueGetBool);

    CPPUNIT_TEST(test_createRoot);
    CPPUNIT_TEST(test_objectAddObject);
    CPPUNIT_TEST(test_objectAddArray);
    CPPUNIT_TEST(test_objectAddString);
    CPPUNIT_TEST(test_objectAddNumber);
    CPPUNIT_TEST(test_objectAddBool);
    CPPUNIT_TEST(test_arrayAddObject);
    CPPUNIT_TEST(test_arrayAddArray);
    CPPUNIT_TEST(test_arrayAddString);
    CPPUNIT_TEST(test_arrayAddNumber);
    CPPUNIT_TEST(test_arrayAddBool);

    CPPUNIT_TEST(test_sizeof);
    CPPUNIT_TEST(test_serialize);
    CPPUNIT_TEST(test_serializeS);

    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void test_deserialize();
    void test_deserializeS();
    void test_valueGetObject();
    void test_valueGetType();
    void test_objectGetValue();
    void test_objectGetObject();
    void test_objectGetArray();
    void test_objectGetString();
    void test_objectGetNumber();
    void test_objectGetBool();
    void test_objectDotGetValue();
    void test_objectDotGetObject();
    void test_objectDotGetArray();
    void test_objectDotGetString();
    void test_objectDotGetNumber();
    void test_objectDotGetBool();
    void test_objectGetCount();
    void test_objectGetName();
    void test_arrayGetValue();
    void test_arrayGetObject();
    void test_arrayGetArray();
    void test_arrayGetString();
    void test_arrayGetNumber();
    void test_arrayGetBool();
    void test_arrayGetCount();
    void test_valueGetArray();
    void test_valueGetString();
    void test_valueGetNumber();
    void test_valueGetBool();
    void test_createRoot();
    void test_objectAddObject();
    void test_objectAddArray();
    void test_objectAddString();
    void test_objectAddNumber();
    void test_objectAddBool();
    void test_arrayAddObject();
    void test_arrayAddArray();
    void test_arrayAddString();
    void test_arrayAddNumber();
    void test_arrayAddBool();
    void test_sizeof();
    void test_serialize();
    void test_serializeS();

    KJSD_JSON_Value* root_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(TEST_Json);

void TEST_Json::setUp()
{
    root_ = 0;
}

void TEST_Json::tearDown()
{
    KJSD_JSON_free(root_);
}

void TEST_Json::test_deserialize()
{
    FILE* in;

    CPPUNIT_ASSERT(KJSD_JSON_deserialize(0) == 0);

    in = fopen("test/json/test_1_1.txt", "r");
    root_ = KJSD_JSON_deserialize(in);
    CPPUNIT_ASSERT(root_ != 0);
    fclose(in);
}

void TEST_Json::test_deserializeS()
{
    CPPUNIT_ASSERT(KJSD_JSON_deserializeS(0) == 0);

    root_ = KJSD_JSON_deserializeS("{ \"aaa\": \"bbb\" }");
    CPPUNIT_ASSERT(root_ != 0);
}

void TEST_Json::test_valueGetObject()
{
    root_ = KJSD_JSON_deserializeS("{ \"aaa\": \"bbb\" }");
    CPPUNIT_ASSERT(root_ != 0);

    CPPUNIT_ASSERT(KJSD_JSON_valueGetObject(0) == 0);
    CPPUNIT_ASSERT(KJSD_JSON_valueGetObject(root_) != 0);
}

void TEST_Json::test_valueGetType()
{
    KJSD_JSON_Object* obj = 0;
    KJSD_JSON_Value* val = 0;

    CPPUNIT_ASSERT(KJSD_JSON_valueGetType(0) == KJSD_JSON_ERROR);

    root_ = KJSD_JSON_deserializeS("{ \"obj\": {},"
                                   "  \"ary\": [],"
                                   "  \"str\": \"aaa\","
                                   "  \"num\": 12,"
                                   "  \"bool\": true,"
                                   "  \"null\": null }");
    CPPUNIT_ASSERT(root_ != 0);

    obj = KJSD_JSON_valueGetObject(root_);

    val = KJSD_JSON_objectGetValue(obj, "obj");
    CPPUNIT_ASSERT(KJSD_JSON_valueGetType(val) == KJSD_JSON_OBJECT);
    val = KJSD_JSON_objectGetValue(obj, "ary");
    CPPUNIT_ASSERT(KJSD_JSON_valueGetType(val) == KJSD_JSON_ARRAY);
    val = KJSD_JSON_objectGetValue(obj, "str");
    CPPUNIT_ASSERT(KJSD_JSON_valueGetType(val) == KJSD_JSON_STRING);
    val = KJSD_JSON_objectGetValue(obj, "num");
    CPPUNIT_ASSERT(KJSD_JSON_valueGetType(val) == KJSD_JSON_NUMBER);
    val = KJSD_JSON_objectGetValue(obj, "bool");
    CPPUNIT_ASSERT(KJSD_JSON_valueGetType(val) == KJSD_JSON_BOOL);
    val = KJSD_JSON_objectGetValue(obj, "null");
    CPPUNIT_ASSERT(KJSD_JSON_valueGetType(val) == KJSD_JSON_NULL);
}

void TEST_Json::test_objectGetValue()
{
    KJSD_JSON_Object* obj = 0;

    root_ = KJSD_JSON_deserializeS("{ \"aaa\": \"bbb\" }");
    CPPUNIT_ASSERT(root_ != 0);

    obj = KJSD_JSON_valueGetObject(root_);

    CPPUNIT_ASSERT(KJSD_JSON_objectGetValue(0, 0) == 0);
    CPPUNIT_ASSERT(KJSD_JSON_objectGetValue(obj, 0) == 0);
    CPPUNIT_ASSERT(KJSD_JSON_objectGetValue(obj, "zzz") == 0);

    CPPUNIT_ASSERT(KJSD_JSON_objectGetValue(obj, "aaa") != 0);
}

void TEST_Json::test_objectGetObject()
{
    KJSD_JSON_Object* obj = 0;

    root_ = KJSD_JSON_deserializeS("{ \"aaa\": {} }");
    CPPUNIT_ASSERT(root_ != 0);

    obj = KJSD_JSON_valueGetObject(root_);

    CPPUNIT_ASSERT(KJSD_JSON_objectGetObject(0, 0) == 0);
    CPPUNIT_ASSERT(KJSD_JSON_objectGetObject(obj, 0) == 0);
    CPPUNIT_ASSERT(KJSD_JSON_objectGetObject(obj, "zzz") == 0);

    CPPUNIT_ASSERT(KJSD_JSON_objectGetObject(obj, "aaa") != 0);
}

void TEST_Json::test_objectGetArray()
{
    KJSD_JSON_Object* obj = 0;

    root_ = KJSD_JSON_deserializeS("{ \"aaa\": [] }");
    CPPUNIT_ASSERT(root_ != 0);

    obj = KJSD_JSON_valueGetObject(root_);

    CPPUNIT_ASSERT(KJSD_JSON_objectGetArray(0, 0) == 0);
    CPPUNIT_ASSERT(KJSD_JSON_objectGetArray(obj, 0) == 0);
    CPPUNIT_ASSERT(KJSD_JSON_objectGetArray(obj, "zzz") == 0);

    CPPUNIT_ASSERT(KJSD_JSON_objectGetArray(obj, "aaa") != 0);
}

void TEST_Json::test_objectGetString()
{
    KJSD_JSON_Object* obj = 0;
    const char* val = 0;

    root_ = KJSD_JSON_deserializeS("{ \"aaa\": \"bbb\" }");
    CPPUNIT_ASSERT(root_ != 0);

    obj = KJSD_JSON_valueGetObject(root_);

    CPPUNIT_ASSERT(KJSD_JSON_objectGetString(0, 0) == 0);
    CPPUNIT_ASSERT(KJSD_JSON_objectGetString(obj, 0) == 0);
    CPPUNIT_ASSERT(KJSD_JSON_objectGetString(obj, "zzz") == 0);

    val = KJSD_JSON_objectGetString(obj, "aaa");
    CPPUNIT_ASSERT(val != 0);
    CPPUNIT_ASSERT(strcmp(val, "bbb") == 0);
}

void TEST_Json::test_objectGetNumber()
{
    KJSD_JSON_Object* obj = 0;

    root_ = KJSD_JSON_deserializeS("{ \"aaa\": 1,"
                                   "  \"bbb\": 1.123 }");
    CPPUNIT_ASSERT(root_ != 0);

    obj = KJSD_JSON_valueGetObject(root_);

    CPPUNIT_ASSERT(KJSD_JSON_objectGetNumber(0, 0) == 0);
    CPPUNIT_ASSERT(KJSD_JSON_objectGetNumber(obj, 0) == 0);
    CPPUNIT_ASSERT(KJSD_JSON_objectGetNumber(obj, "zzz") == 0);

    CPPUNIT_ASSERT(KJSD_JSON_objectGetNumber(obj, "aaa") == 1);
    CPPUNIT_ASSERT(KJSD_JSON_objectGetNumber(obj, "bbb") == 1.123);
}

void TEST_Json::test_objectGetBool()
{
    KJSD_JSON_Object* obj = 0;

    root_ = KJSD_JSON_deserializeS("{ \"aaa\": true,"
                                   "  \"bbb\": false }");
    CPPUNIT_ASSERT(root_ != 0);

    obj = KJSD_JSON_valueGetObject(root_);

    CPPUNIT_ASSERT(KJSD_JSON_objectGetBool(0, 0) < 0);
    CPPUNIT_ASSERT(KJSD_JSON_objectGetBool(obj, 0) < 0);
    CPPUNIT_ASSERT(KJSD_JSON_objectGetBool(obj, "zzz") < 0);

    CPPUNIT_ASSERT(KJSD_JSON_objectGetBool(obj, "aaa") == 1);
    CPPUNIT_ASSERT(KJSD_JSON_objectGetBool(obj, "bbb") == 0);
}

void TEST_Json::test_objectDotGetValue()
{
    KJSD_JSON_Object* obj = 0;

    root_ = KJSD_JSON_deserializeS("{ \"aaa\":{ \"bbb\":{ \"ccc\": 1 }}}");
    CPPUNIT_ASSERT(root_ != 0);

    obj = KJSD_JSON_valueGetObject(root_);

    CPPUNIT_ASSERT(KJSD_JSON_objectDotGetValue(0, 0) == 0);
    CPPUNIT_ASSERT(KJSD_JSON_objectDotGetValue(obj, 0) == 0);
    CPPUNIT_ASSERT(KJSD_JSON_objectDotGetValue(obj, "zzz") == 0);

    CPPUNIT_ASSERT(KJSD_JSON_objectDotGetValue(obj, "aaa.bbb.ccc") != 0);
}

void TEST_Json::test_objectDotGetObject()
{
    KJSD_JSON_Object* obj = 0;

    root_ = KJSD_JSON_deserializeS("{ \"aaa\":{ \"bbb\":{ \"ccc\": {} }}}");
    CPPUNIT_ASSERT(root_ != 0);

    obj = KJSD_JSON_valueGetObject(root_);

    CPPUNIT_ASSERT(KJSD_JSON_objectDotGetObject(0, 0) == 0);
    CPPUNIT_ASSERT(KJSD_JSON_objectDotGetObject(obj, 0) == 0);
    CPPUNIT_ASSERT(KJSD_JSON_objectDotGetObject(obj, "zzz") == 0);

    CPPUNIT_ASSERT(KJSD_JSON_objectDotGetObject(obj, "aaa.bbb.ccc") != 0);
}

void TEST_Json::test_objectDotGetArray()
{
    KJSD_JSON_Object* obj = 0;

    root_ = KJSD_JSON_deserializeS("{ \"aaa\":{ \"bbb\":{ \"ccc\": [] }}}");
    CPPUNIT_ASSERT(root_ != 0);

    obj = KJSD_JSON_valueGetObject(root_);

    CPPUNIT_ASSERT(KJSD_JSON_objectDotGetArray(0, 0) == 0);
    CPPUNIT_ASSERT(KJSD_JSON_objectDotGetArray(obj, 0) == 0);
    CPPUNIT_ASSERT(KJSD_JSON_objectDotGetArray(obj, "zzz") == 0);

    CPPUNIT_ASSERT(KJSD_JSON_objectDotGetArray(obj, "aaa.bbb.ccc") != 0);
}

void TEST_Json::test_objectDotGetString()
{
    KJSD_JSON_Object* obj = 0;
    const char* val = 0;

    root_ = KJSD_JSON_deserializeS("{ \"aaa\":{ \"bbb\":"
                                   "  { \"ccc\": \"str\" }}}");
    CPPUNIT_ASSERT(root_ != 0);

    obj = KJSD_JSON_valueGetObject(root_);

    CPPUNIT_ASSERT(KJSD_JSON_objectDotGetString(0, 0) == 0);
    CPPUNIT_ASSERT(KJSD_JSON_objectDotGetString(obj, 0) == 0);
    CPPUNIT_ASSERT(KJSD_JSON_objectDotGetString(obj, "zzz") == 0);

    val = KJSD_JSON_objectDotGetString(obj, "aaa.bbb.ccc");
    CPPUNIT_ASSERT(val != 0);
    CPPUNIT_ASSERT(strcmp(val, "str") == 0);
}

void TEST_Json::test_objectDotGetNumber()
{
    KJSD_JSON_Object* obj = 0;

    root_ = KJSD_JSON_deserializeS("{ \"aaa\":{ \"bbb\":"
                                   "  { \"ccc\": 1,"
                                   "    \"ddd\": 1.123 }}}");
    CPPUNIT_ASSERT(root_ != 0);

    obj = KJSD_JSON_valueGetObject(root_);

    CPPUNIT_ASSERT(KJSD_JSON_objectDotGetNumber(0, 0) == 0);
    CPPUNIT_ASSERT(KJSD_JSON_objectDotGetNumber(obj, 0) == 0);
    CPPUNIT_ASSERT(KJSD_JSON_objectDotGetNumber(obj, "zzz") == 0);

    CPPUNIT_ASSERT(KJSD_JSON_objectDotGetNumber(obj, "aaa.bbb.ccc") == 1);
    CPPUNIT_ASSERT(KJSD_JSON_objectDotGetNumber(obj, "aaa.bbb.ddd") == 1.123);
}

void TEST_Json::test_objectDotGetBool()
{
    KJSD_JSON_Object* obj = 0;

    root_ = KJSD_JSON_deserializeS("{ \"aaa\":{ \"bbb\":"
                                   "  { \"ccc\": true,"
                                   "    \"ddd\": false }}}");
    CPPUNIT_ASSERT(root_ != 0);

    obj = KJSD_JSON_valueGetObject(root_);

    CPPUNIT_ASSERT(KJSD_JSON_objectDotGetBool(0, 0) < 0);
    CPPUNIT_ASSERT(KJSD_JSON_objectDotGetBool(obj, 0) < 0);
    CPPUNIT_ASSERT(KJSD_JSON_objectDotGetBool(obj, "zzz") < 0);

    CPPUNIT_ASSERT(KJSD_JSON_objectDotGetBool(obj, "aaa.bbb.ccc") == 1);
    CPPUNIT_ASSERT(KJSD_JSON_objectDotGetBool(obj, "aaa.bbb.ddd") == 0);
}

void TEST_Json::test_objectGetCount()
{
    KJSD_JSON_Object* obj = 0;

    CPPUNIT_ASSERT(KJSD_JSON_objectGetCount(0) == 0);

    root_ = KJSD_JSON_deserializeS("{ \"aaa\":{ \"bbb\": 1},"
                                   "  \"ccc\": [],"
                                   "  \"ddd\": \"str\"}");
    CPPUNIT_ASSERT(root_ != 0);

    obj = KJSD_JSON_valueGetObject(root_);

    CPPUNIT_ASSERT(KJSD_JSON_objectGetCount(obj) == 3);
}

void TEST_Json::test_objectGetName()
{
    KJSD_JSON_Object* obj = 0;
    const char* name = 0;

    root_ = KJSD_JSON_deserializeS("{ \"aaa\": 1,"
                                   "  \"bbb\": 2 }");
    CPPUNIT_ASSERT(root_ != 0);

    obj = KJSD_JSON_valueGetObject(root_);

    CPPUNIT_ASSERT(KJSD_JSON_objectGetName(0, 0) == 0);
    CPPUNIT_ASSERT(KJSD_JSON_objectGetName(obj, 255) == 0);

    name = KJSD_JSON_objectGetName(obj, 0);
    CPPUNIT_ASSERT(name != 0);
    CPPUNIT_ASSERT(strcmp(name, "aaa") == 0);

    name = KJSD_JSON_objectGetName(obj, 1);
    CPPUNIT_ASSERT(name != 0);
    CPPUNIT_ASSERT(strcmp(name, "bbb") == 0);
}

void TEST_Json::test_arrayGetValue()
{
    KJSD_JSON_Array* ary = 0;

    root_ = KJSD_JSON_deserializeS("[ {}, [], \"str\", 1, true ]");
    CPPUNIT_ASSERT(root_ != 0);

    ary = KJSD_JSON_valueGetArray(root_);

    CPPUNIT_ASSERT(KJSD_JSON_arrayGetValue(0, 0) == 0);
    CPPUNIT_ASSERT(KJSD_JSON_arrayGetValue(ary, 255) == 0);

    CPPUNIT_ASSERT(KJSD_JSON_arrayGetValue(ary, 0) != 0);
}

void TEST_Json::test_arrayGetObject()
{
    KJSD_JSON_Array* ary = 0;

    root_ = KJSD_JSON_deserializeS("[ {}, [], \"str\", 1, true ]");
    CPPUNIT_ASSERT(root_ != 0);

    ary = KJSD_JSON_valueGetArray(root_);

    CPPUNIT_ASSERT(KJSD_JSON_arrayGetObject(0, 0) == 0);
    CPPUNIT_ASSERT(KJSD_JSON_arrayGetObject(ary, 255) == 0);

    CPPUNIT_ASSERT(KJSD_JSON_arrayGetObject(ary, 0) != 0);
}

void TEST_Json::test_arrayGetArray()
{
    KJSD_JSON_Array* ary = 0;

    root_ = KJSD_JSON_deserializeS("[ {}, [], \"str\", 1, true ]");
    CPPUNIT_ASSERT(root_ != 0);

    ary = KJSD_JSON_valueGetArray(root_);

    CPPUNIT_ASSERT(KJSD_JSON_arrayGetArray(0, 0) == 0);
    CPPUNIT_ASSERT(KJSD_JSON_arrayGetArray(ary, 255) == 0);

    CPPUNIT_ASSERT(KJSD_JSON_arrayGetArray(ary, 1) != 0);
}

void TEST_Json::test_arrayGetString()
{
    KJSD_JSON_Array* ary = 0;
    const char* val = 0;

    root_ = KJSD_JSON_deserializeS("[ {}, [], \"str\", 1, true ]");
    CPPUNIT_ASSERT(root_ != 0);

    ary = KJSD_JSON_valueGetArray(root_);

    CPPUNIT_ASSERT(KJSD_JSON_arrayGetString(0, 0) == 0);
    CPPUNIT_ASSERT(KJSD_JSON_arrayGetString(ary, 255) == 0);

    val = KJSD_JSON_arrayGetString(ary, 2);
    CPPUNIT_ASSERT(val != 0);
    CPPUNIT_ASSERT(strcmp(val, "str") == 0);
}

void TEST_Json::test_arrayGetNumber()
{
    KJSD_JSON_Array* ary = 0;

    root_ = KJSD_JSON_deserializeS("[ {}, [], \"str\", 1, true ]");
    CPPUNIT_ASSERT(root_ != 0);

    ary = KJSD_JSON_valueGetArray(root_);

    CPPUNIT_ASSERT(KJSD_JSON_arrayGetNumber(0, 0) == 0);
    CPPUNIT_ASSERT(KJSD_JSON_arrayGetNumber(ary, 255) == 0);

    CPPUNIT_ASSERT(KJSD_JSON_arrayGetNumber(ary, 3) == 1);
}

void TEST_Json::test_arrayGetBool()
{
    KJSD_JSON_Array* ary = 0;

    root_ = KJSD_JSON_deserializeS("[ {}, [], \"str\", 1, true ]");
    CPPUNIT_ASSERT(root_ != 0);

    ary = KJSD_JSON_valueGetArray(root_);

    CPPUNIT_ASSERT(KJSD_JSON_arrayGetBool(0, 0) < 0);
    CPPUNIT_ASSERT(KJSD_JSON_arrayGetBool(ary, 255) < 0);

    CPPUNIT_ASSERT(KJSD_JSON_arrayGetBool(ary, 4) == 1);
}

void TEST_Json::test_arrayGetCount()
{
    KJSD_JSON_Array* ary = 0;

    root_ = KJSD_JSON_deserializeS("[ {}, [], \"str\", 1, true ]");
    CPPUNIT_ASSERT(root_ != 0);

    ary = KJSD_JSON_valueGetArray(root_);

    CPPUNIT_ASSERT(KJSD_JSON_arrayGetCount(0) == 0);

    CPPUNIT_ASSERT(KJSD_JSON_arrayGetCount(ary) == 5);
}

void TEST_Json::test_valueGetArray()
{
    root_ = KJSD_JSON_deserializeS("[ \"aaa\", \"bbb\" ]");
    CPPUNIT_ASSERT(root_ != 0);

    CPPUNIT_ASSERT(KJSD_JSON_valueGetArray(0) == 0);
    CPPUNIT_ASSERT(KJSD_JSON_valueGetArray(root_) != 0);
}

void TEST_Json::test_valueGetString()
{
    KJSD_JSON_Value* val = 0;
    const char* str = 0;

    CPPUNIT_ASSERT(KJSD_JSON_valueGetString(0) == 0);

    root_ = KJSD_JSON_deserializeS("{ \"aaa\": \"bbb\" }");
    CPPUNIT_ASSERT(root_ != 0);

    val = KJSD_JSON_objectGetValue(KJSD_JSON_valueGetObject(root_), "aaa");

    str = KJSD_JSON_valueGetString(val);
    CPPUNIT_ASSERT(str != 0);
    CPPUNIT_ASSERT(strcmp(str, "bbb") == 0);
}

void TEST_Json::test_valueGetNumber()
{
    KJSD_JSON_Object* obj = 0;
    KJSD_JSON_Value* val = 0;

    CPPUNIT_ASSERT(KJSD_JSON_valueGetNumber(0) == 0);

    root_ = KJSD_JSON_deserializeS("{ \"aaa\": 1,"
                                   "  \"bbb\": 1.123 }");
    CPPUNIT_ASSERT(root_ != 0);

    obj = KJSD_JSON_valueGetObject(root_);

    val = KJSD_JSON_objectGetValue(obj, "aaa");
    CPPUNIT_ASSERT(KJSD_JSON_valueGetNumber(val) == 1);

    val = KJSD_JSON_objectGetValue(obj, "bbb");
    CPPUNIT_ASSERT(KJSD_JSON_valueGetNumber(val) == 1.123);
}

void TEST_Json::test_valueGetBool()
{
    KJSD_JSON_Object* obj = 0;
    KJSD_JSON_Value* val = 0;

    CPPUNIT_ASSERT(KJSD_JSON_valueGetBool(0) < 0);

    root_ = KJSD_JSON_deserializeS("{ \"aaa\": true,"
                                   "  \"bbb\": false }");
    CPPUNIT_ASSERT(root_ != 0);

    obj = KJSD_JSON_valueGetObject(root_);

    val = KJSD_JSON_objectGetValue(obj, "aaa");
    CPPUNIT_ASSERT(KJSD_JSON_valueGetBool(val) == 1);

    val = KJSD_JSON_objectGetValue(obj, "bbb");
    CPPUNIT_ASSERT(KJSD_JSON_valueGetBool(val) == 0);
}

void TEST_Json::test_createRoot()
{
    root_ = KJSD_JSON_createRoot();
    CPPUNIT_ASSERT(root_ != 0);
}

void TEST_Json::test_objectAddObject()
{
    KJSD_JSON_Object* base = 0;

    root_ = KJSD_JSON_createRoot();
    CPPUNIT_ASSERT(root_ != 0);

    base = KJSD_JSON_valueGetObject(root_);
    CPPUNIT_ASSERT(base != 0);

    CPPUNIT_ASSERT(KJSD_JSON_objectAddObject(0, 0) == 0);
    CPPUNIT_ASSERT(KJSD_JSON_objectAddObject(base, 0) == 0);

    CPPUNIT_ASSERT(KJSD_JSON_objectAddObject(base, "obj") != 0);
    CPPUNIT_ASSERT(KJSD_JSON_objectGetObject(base, "obj") != 0);
}

void TEST_Json::test_objectAddArray()
{
    KJSD_JSON_Object* base = 0;

    root_ = KJSD_JSON_createRoot();
    CPPUNIT_ASSERT(root_ != 0);

    base = KJSD_JSON_valueGetObject(root_);
    CPPUNIT_ASSERT(base != 0);

    CPPUNIT_ASSERT(KJSD_JSON_objectAddArray(0, 0) == 0);
    CPPUNIT_ASSERT(KJSD_JSON_objectAddArray(base, 0) == 0);

    CPPUNIT_ASSERT(KJSD_JSON_objectAddArray(base, "ary") != 0);
    CPPUNIT_ASSERT(KJSD_JSON_objectGetArray(base, "ary") != 0);
}

void TEST_Json::test_objectAddString()
{
    const char* str = 0;

    KJSD_JSON_Object* base = 0;

    root_ = KJSD_JSON_createRoot();
    CPPUNIT_ASSERT(root_ != 0);

    base = KJSD_JSON_valueGetObject(root_);
    CPPUNIT_ASSERT(base != 0);

    CPPUNIT_ASSERT(KJSD_JSON_objectAddString(0, 0, 0) == 0);
    CPPUNIT_ASSERT(KJSD_JSON_objectAddString(base, 0, 0) == 0);

    CPPUNIT_ASSERT(KJSD_JSON_objectAddString(base, "foo", 0) > 0);
    CPPUNIT_ASSERT(KJSD_JSON_objectAddString(base, "string", "aaa") > 0);
    str = KJSD_JSON_objectGetString(base, "string");
    CPPUNIT_ASSERT(strcmp(str, "aaa") == 0);
}

void TEST_Json::test_objectAddNumber()
{
    KJSD_JSON_Object* base = 0;

    root_ = KJSD_JSON_createRoot();
    CPPUNIT_ASSERT(root_ != 0);

    base = KJSD_JSON_valueGetObject(root_);
    CPPUNIT_ASSERT(base != 0);

    CPPUNIT_ASSERT(KJSD_JSON_objectAddNumber(0, 0, 0) == 0);
    CPPUNIT_ASSERT(KJSD_JSON_objectAddNumber(base, 0, 0) == 0);

    CPPUNIT_ASSERT(KJSD_JSON_objectAddNumber(base, "number", 1.12) > 0);
    CPPUNIT_ASSERT(KJSD_JSON_objectGetNumber(base, "number") == 1.12);
}

void TEST_Json::test_objectAddBool()
{
    KJSD_JSON_Object* base = 0;

    root_ = KJSD_JSON_createRoot();
    CPPUNIT_ASSERT(root_ != 0);

    base = KJSD_JSON_valueGetObject(root_);
    CPPUNIT_ASSERT(base != 0);

    CPPUNIT_ASSERT(KJSD_JSON_objectAddBool(0, 0, 0) == 0);
    CPPUNIT_ASSERT(KJSD_JSON_objectAddBool(base, 0, 0) == 0);

    CPPUNIT_ASSERT(KJSD_JSON_objectAddBool(base, "bool", 0) > 0);
    CPPUNIT_ASSERT(KJSD_JSON_objectGetBool(base, "bool") == 0);
}

void TEST_Json::test_arrayAddObject()
{
    KJSD_JSON_Object* base = 0;
    KJSD_JSON_Array* ary = 0;

    root_ = KJSD_JSON_createRoot();
    CPPUNIT_ASSERT(root_ != 0);
    base = KJSD_JSON_valueGetObject(root_);
    CPPUNIT_ASSERT(base != 0);
    ary = KJSD_JSON_objectAddArray(base, "ary");
    CPPUNIT_ASSERT(ary != 0);

    CPPUNIT_ASSERT(KJSD_JSON_arrayAddObject(0) == 0);

    CPPUNIT_ASSERT(KJSD_JSON_arrayAddObject(ary) != 0);
    CPPUNIT_ASSERT(KJSD_JSON_arrayGetObject(ary, 0) != 0);
}

void TEST_Json::test_arrayAddArray()
{
    KJSD_JSON_Object* base = 0;
    KJSD_JSON_Array* ary = 0;

    root_ = KJSD_JSON_createRoot();
    CPPUNIT_ASSERT(root_ != 0);
    base = KJSD_JSON_valueGetObject(root_);
    CPPUNIT_ASSERT(base != 0);
    ary = KJSD_JSON_objectAddArray(base, "ary");
    CPPUNIT_ASSERT(ary != 0);

    CPPUNIT_ASSERT(KJSD_JSON_arrayAddArray(0) == 0);

    CPPUNIT_ASSERT(KJSD_JSON_arrayAddArray(ary) != 0);
    CPPUNIT_ASSERT(KJSD_JSON_arrayGetArray(ary, 0) != 0);
}

void TEST_Json::test_arrayAddString()
{
    const char* str = 0;

    KJSD_JSON_Object* base = 0;
    KJSD_JSON_Array* ary = 0;

    root_ = KJSD_JSON_createRoot();
    CPPUNIT_ASSERT(root_ != 0);
    base = KJSD_JSON_valueGetObject(root_);
    CPPUNIT_ASSERT(base != 0);
    ary = KJSD_JSON_objectAddArray(base, "ary");
    CPPUNIT_ASSERT(ary != 0);

    CPPUNIT_ASSERT(KJSD_JSON_arrayAddString(0, 0) == 0);

    CPPUNIT_ASSERT(KJSD_JSON_arrayAddString(ary, 0) > 0);
    CPPUNIT_ASSERT(KJSD_JSON_arrayAddString(ary, "aaa") > 0);
    str = KJSD_JSON_arrayGetString(ary, 1);
    CPPUNIT_ASSERT(strcmp(str, "aaa") == 0);
}

void TEST_Json::test_arrayAddNumber()
{
    KJSD_JSON_Object* base = 0;
    KJSD_JSON_Array* ary = 0;

    root_ = KJSD_JSON_createRoot();
    CPPUNIT_ASSERT(root_ != 0);
    base = KJSD_JSON_valueGetObject(root_);
    CPPUNIT_ASSERT(base != 0);
    ary = KJSD_JSON_objectAddArray(base, "ary");
    CPPUNIT_ASSERT(ary != 0);

    CPPUNIT_ASSERT(KJSD_JSON_arrayAddNumber(0, 0) == 0);

    CPPUNIT_ASSERT(KJSD_JSON_arrayAddNumber(ary, 1.12) > 0);
    CPPUNIT_ASSERT(KJSD_JSON_arrayGetNumber(ary, 0) == 1.12);
}

void TEST_Json::test_arrayAddBool()
{
    KJSD_JSON_Object* base = 0;
    KJSD_JSON_Array* ary = 0;

    root_ = KJSD_JSON_createRoot();
    CPPUNIT_ASSERT(root_ != 0);
    base = KJSD_JSON_valueGetObject(root_);
    CPPUNIT_ASSERT(base != 0);
    ary = KJSD_JSON_objectAddArray(base, "ary");
    CPPUNIT_ASSERT(ary != 0);

    CPPUNIT_ASSERT(KJSD_JSON_arrayAddBool(0, 0) == 0);

    CPPUNIT_ASSERT(KJSD_JSON_arrayAddBool(ary, 0) > 0);
    CPPUNIT_ASSERT(KJSD_JSON_arrayGetBool(ary, 0) == 0);
}

void TEST_Json::test_sizeof()
{
    static const char* json_str = "[{},[],\"str\",1,true]";

    root_ = KJSD_JSON_deserializeS(json_str);
    CPPUNIT_ASSERT(root_ != 0);

    CPPUNIT_ASSERT(KJSD_JSON_sizeof(0) == 0);

    CPPUNIT_ASSERT(KJSD_JSON_sizeof(root_) == strlen(json_str));
}

void TEST_Json::test_serialize()
{
    static const char* json_str = "[{},[],\"str\",1,true]";

	cout << endl;

    root_ = KJSD_JSON_deserializeS(json_str);
    CPPUNIT_ASSERT(root_ != 0);

    CPPUNIT_ASSERT(KJSD_JSON_serialize(0, 0) == 0);
    CPPUNIT_ASSERT(KJSD_JSON_serialize(root_, 0) == 0);

    CPPUNIT_ASSERT(KJSD_JSON_serialize(root_, stdout) == strlen(json_str));

	cout << endl;
}

void TEST_Json::test_serializeS()
{
    static const char* json_str = "[{},[],\"str\",1,true]";
    char* out = 0;

    root_ = KJSD_JSON_deserializeS(json_str);
    CPPUNIT_ASSERT(root_ != 0);

    size_t sz = KJSD_JSON_sizeof(root_) + 1;
    out = new char[sz];

    CPPUNIT_ASSERT(KJSD_JSON_serializeS(0, 0, 0) == 0);
    CPPUNIT_ASSERT(KJSD_JSON_serializeS(root_, 0, 0) == 0);

    CPPUNIT_ASSERT(KJSD_JSON_serializeS(root_, out, sz) == (sz - 1));
}
