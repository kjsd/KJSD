/**
 * @file testmain.cpp
 *
 * @version $Id: 0812c38b0e71bae7677160f9b0640810abc45bb9 $
 *
 * @brief A main program for all test suits
 *
 * @author Kenji MINOURA / kenji@kandj.org
 *
 * Copyright (c) 2012 K&J Software Design, Ltd. All rights reserved.
 *
 * @see <related_items>
 ***********************************************************************/
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>


int main(int argc, char* argv[])
{
  // イベント・マネージャとテスト・コントローラを生成する
  CppUnit::TestResult controller;

  // テスト結果収集リスナをコントローラにアタッチする
  CppUnit::TestResultCollector result;
  controller.addListener( &result );

  // 「.」で進行状況を出力するリスナをアタッチする
  CppUnit::BriefTestProgressListener progress;
  controller.addListener( &progress );

  // テスト・ランナーにテスト群を与え、テストする
  CppUnit::TestRunner runner;
  runner.addTest( CppUnit::TestFactoryRegistry::getRegistry().makeTest() );
  runner.run( controller );

  // テスト結果を標準出力に吐き出す
  CppUnit::CompilerOutputter outputter( &result, CppUnit::stdCOut() );
  outputter.write();

  return result.wasSuccessful() ? 0 : 1;
}
