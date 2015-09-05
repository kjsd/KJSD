/**
 * @file json.h
 *
 * @version $Id:$
 *
 * @brief
 *
 * @author Kenji MINOURA / kenji@kandj.org
 *
 * Copyright (c) 2012 The KJSD Project. All rights reserved.
 ***********************************************************************/
#ifndef KJSD_JSON_H
#define KJSD_JSON_H

#ifdef __cplusplus
extern "C"
{
#endif    
    
#include <stdio.h>
#include <stddef.h>   /* size_t */    

/** JSON総称型 */
typedef struct KJSD_JSON_ValueT KJSD_JSON_Value;
/** JSONオブジェクト型 */
typedef struct KJSD_JSON_ObjectT KJSD_JSON_Object;
/** JSON配列型 */
typedef struct KJSD_JSON_ArrayT KJSD_JSON_Array;

/** JSONデータ型種別定義 */
typedef enum
{
    /** 型不定 */
    KJSD_JSON_ERROR = 0,
    /** NULL */
    KJSD_JSON_NULL,
    /** 文字列型 */
    KJSD_JSON_STRING,
    /** 数値型 */
    KJSD_JSON_NUMBER,
    /** オブジェクト型 */
    KJSD_JSON_OBJECT,
    /** 配列型 */
    KJSD_JSON_ARRAY,
    /** 真偽値型 */
    KJSD_JSON_BOOL
} KJSD_JSON_Type;

/**
 *  @brief  JSONファイルパーサ
 *
 *  @param[in] in パース元ファイルストリーム
 *
 *  @retval NULL以外 JSONルートデータ
 *  @retval NULL パース失敗
 *
 *  @note inのopen/closeはしない
 *  @note 使用後はKJSD_JSON_freeで解放する
 */
KJSD_JSON_Value* KJSD_JSON_deserialize(FILE* in);

/**
 *  @brief  JSON文字列パーサ
 *
 *  @param  [in] in JSON文字列
 *
 *  @retval NULL以外 JSONルートデータ
 *  @retval NULL パース失敗
 *
 *  @note 使用後はKJSD_JSON_freeで解放する
 */
KJSD_JSON_Value* KJSD_JSON_deserializeS(const char *in);

/**
 *  @brief  JSONオブジェクトのメンバ取得(総称型)
 *
 *  @param  [in] object JSONオブジェクト
 *  @param  [in] name 取得するメンバの名称
 *
 *  @retval NULL以外 JSONデータ
 *  @retval NULL nameが見つからない
 */
KJSD_JSON_Value* KJSD_JSON_objectGetValue(
    const KJSD_JSON_Object *object, const char *name);

/**
 *  @brief  JSONオブジェクトのメンバ取得(オブジェクト)
 *
 *  @param  [in] object JSONオブジェクト
 *  @param  [in] name 取得する文字列データの名称
 *
 *  @retval NULL以外 JSONオブジェクトデータ
 *  @retval NULL nameが見つからない
 */
KJSD_JSON_Object* KJSD_JSON_objectGetObject(
    const KJSD_JSON_Object *object, const char *name);

/**
 *  @brief  JSONオブジェクトのメンバ取得(配列)
 *
 *  @param  [in] object JSONオブジェクト
 *  @param  [in] name 取得する配列データの名称
 *
 *  @retval NULL以外 JSON配列データ
 *  @retval NULL nameが見つからない
 */
KJSD_JSON_Array* KJSD_JSON_objectGetArray(
    const KJSD_JSON_Object *object, const char *name);

/**
 *  @brief  JSONオブジェクトのメンバ取得(文字列)
 *
 *  @param  [in] object JSONオブジェクト
 *  @param  [in] name 取得する文字列データの名称
 *
 *  @retval NULL以外 JSON文字列データ
 *  @retval NULL nameが見つからない
 */
const char* KJSD_JSON_objectGetString(
    const KJSD_JSON_Object *object, const char *name);

/**
 *  @brief  JSONオブジェクトのメンバ取得(数値)
 *
 *  @param  [in] object JSONオブジェクト
 *  @param  [in] name 取得する数値データの名称
 *
 *  @retval JSON数値データ
 *
 *  @note nameが見つからないときはゼロが返る
 */
double KJSD_JSON_objectGetNumber(
    const KJSD_JSON_Object *object, const char *name);

/**
 *  @brief  JSONオブジェクトのメンバ取得(真偽値)
 *
 *  @param  [in] object JSONオブジェクト
 *  @param  [in] name 取得する真偽値データの名称
 *
 *  @retval 0,1 JSON真偽値データ
 *  @retval 0未満 nameが見つからない
 */
int KJSD_JSON_objectGetBool(
    const KJSD_JSON_Object *object, const char *name);

/**
 *  @brief  JSONオブジェクト以下のメンバ取得(総称型)
 *
 *  @param  [in] object JSONオブジェクト
 *  @param  [in] name 取得するデータの名称．階層構造を表すdot(.)が含ま
 *  れている場合は階層を下って検索される(e.g. ObjectA.ObjectB.Foo)
 *
 *  @retval NULL以外 JSONデータ
 *  @retval NULL nameが見つからない
 */
KJSD_JSON_Value* KJSD_JSON_objectDotGetValue(
    const KJSD_JSON_Object *object, const char *name);

/**
 *  @brief  JSONオブジェクト以下のメンバ取得(オブジェクト)
 *
 *  @param  [in] object JSONオブジェクト
 *  @param  [in] name 取得するデータの名称．階層構造を表すdot(.)が含ま
 *  れている場合は階層を下って検索される(e.g. ObjectA.ObjectB.Foo)
 *
 *  @retval NULL以外 JSONオブジェクト
 *  @retval NULL nameが見つからない
 */
KJSD_JSON_Object* KJSD_JSON_objectDotGetObject(
    const KJSD_JSON_Object *object, const char *name);

/**
 *  @brief  JSONオブジェクト以下のメンバ取得(配列)
 *
 *  @param  [in] object JSONオブジェクト
 *  @param  [in] name 取得するデータの名称．階層構造を表すdot(.)が含ま
 *  れている場合は階層を下って検索される(e.g. ObjectA.ObjectB.Foo)
 *
 *  @retval NULL以外 JSON配列
 *  @retval NULL nameが見つからない
 */
KJSD_JSON_Array* KJSD_JSON_objectDotGetArray(
    const KJSD_JSON_Object *object, const char *name);

/**
 *  @brief  JSONオブジェクト以下のメンバ取得(文字列)
 *
 *  @param  [in] object JSONオブジェクト
 *  @param  [in] name 取得するデータの名称．階層構造を表すdot(.)が含ま
 *  れている場合は階層を下って検索される(e.g. ObjectA.ObjectB.Foo)
 *
 *  @retval NULL以外 文字列
 *  @retval NULL nameが見つからない
 */
const char* KJSD_JSON_objectDotGetString(
    const KJSD_JSON_Object *object, const char *name);

/**
 *  @brief  JSONオブジェクト以下のメンバ取得(数値)
 *
 *  @param  [in] object JSONオブジェクト
 *  @param  [in] name 取得するデータの名称．階層構造を表すdot(.)が含ま
 *  れている場合は階層を下って検索される(e.g. ObjectA.ObjectB.Foo)
 *
 *  @return JSON数値
 *
 *  @note nameが見つからないときはゼロが返る
 */
double KJSD_JSON_objectDotGetNumber(
    const KJSD_JSON_Object *object, const char *name);

/**
 *  @brief  JSONオブジェクト以下のメンバ取得(真偽値)
 *
 *  @param  [in] object JSONオブジェクト
 *  @param  [in] name 取得するデータの名称．階層構造を表すdot(.)が含ま
 *  れている場合は階層を下って検索される(e.g. ObjectA.ObjectB.Foo)
 *
 *  @retval 0,1 真偽値
 *  @retval 0未満 nameが見つからない
 */
int KJSD_JSON_objectDotGetBool(
    const KJSD_JSON_Object *object, const char *name);

/**
 *  @brief  JSONオブジェクトのメンバ数取得
 *
 *  @param  [in] object JSONオブジェクト
 *
 *  @return メンバ数
 */
size_t KJSD_JSON_objectGetCount(const KJSD_JSON_Object *object);

/**
 *  @brief  JSONオブジェクトのメンバ名称取得
 *
 *  @param  [in] object JSONオブジェクト
 *  @param  [in] index 取得するメンバのインデックス
 *
 *  @retval NULL以外 データ名称
 *  @retval NULL エラー
 */
const char* KJSD_JSON_objectGetName(
    const KJSD_JSON_Object *object, size_t index);

/**
 *  @brief  JSON配列の要素取得(型指定なし)
 *
 *  @param  [in] array JSON配列
 *  @param  [in] index 取得する配列要素のインデックス
 *
 *  @retval NULL以外 JSONデータ
 *  @retval NULL エラー
 */
KJSD_JSON_Value* KJSD_JSON_arrayGetValue(
    const KJSD_JSON_Array *array, size_t index);

/**
 *  @brief  JSON配列の要素取得(オブジェクト)
 *
 *  @param  [in] array JSON配列
 *  @param  [in] index 取得する配列要素のインデックス
 *
 *  @retval NULL以外 JSONオブジェクト
 *  @retval NULL エラー
 */
KJSD_JSON_Object* KJSD_JSON_arrayGetObject(
    const KJSD_JSON_Array *array, size_t index);

/**
 *  @brief  JSON配列の要素取得(配列)
 *
 *  @param  [in] array JSON配列
 *  @param  [in] index 取得する配列データのインデックス
 *
 *  @retval NULL以外 JSON配列
 *  @retval NULL エラー
 */
KJSD_JSON_Array* KJSD_JSON_arrayGetArray(
    const KJSD_JSON_Array *array, size_t index);

/**
 *  @brief  JSON配列の要素取得(文字列)
 *
 *  @param  [in] array JSON配列
 *  @param  [in] index 取得する配列要素のインデックス
 *
 *  @retval NULL以外 文字列
 *  @retval NULL エラー
 */
const char* KJSD_JSON_arrayGetString(
    const KJSD_JSON_Array *array, size_t index);

/**
 *  @brief  JSON配列の要素取得(数値)
 *
 *  @param  [in] array JSON配列
 *  @param  [in] index 取得する配列データのインデックス
 *
 *  @return JSON数値データ
 *  @attention エラー発生時はゼロが返る
 */
double KJSD_JSON_arrayGetNumber(
    const KJSD_JSON_Array *array, size_t index);

/**
 *  @brief  JSON配列の要素取得(真偽値)
 *
 *  @param  [in] array JSON配列
 *  @param  [in] index 取得する配列データのインデックス
 *
 *  @retval 0,1 JSON真偽値データ
 *  @retval 0未満 エラー
 */
int KJSD_JSON_arrayGetBool(const KJSD_JSON_Array *array, size_t index);

/**
 *  @brief  JSON配列の要素数取得
 *
 *  @param  [in] array JSON配列
 *
 *  @return 配列の要素数
 */
size_t KJSD_JSON_arrayGetCount(const KJSD_JSON_Array *array);

/**
 *  @brief  JSON総称型のデータ型取得
 *
 *  @param  [in] value JSON総称型データ
 *
 *  @return JSONデータ型
 */
KJSD_JSON_Type KJSD_JSON_valueGetType(const KJSD_JSON_Value *value);

/**
 *  @brief  JSON総称型データをオブジェクトとして取得
 *
 *  @param  [in] value JSONデータ
 *
 *  @retval NULL以外 JSONオブジェクト
 *  @retval NULL エラー
 */
KJSD_JSON_Object* KJSD_JSON_valueGetObject(const KJSD_JSON_Value *value);

/**
 *  @brief  JSON総称型データを配列として取得
 *
 *  @param  [in] value JSONデータ
 *
 *  @retval NULL以外 JSON配列
 *  @retval NULL エラー
 */
KJSD_JSON_Array* KJSD_JSON_valueGetArray(const KJSD_JSON_Value *value);

/**
 *  @brief  JSON総称型データを文字列として取得
 *
 *  @param  [in] value JSONデータ
 *
 *  @retval NULL以外 JSON文字列
 *  @retval NULL エラー
 */
const char* KJSD_JSON_valueGetString(const KJSD_JSON_Value *value);

/**
 *  @brief  JSON総称型データを数値として取得
 *
 *  @param  [in] value JSONデータ
 *
 *  @retval NULL以外 JSON数値
 *  @retval NULL エラー
 */
double KJSD_JSON_valueGetNumber(const KJSD_JSON_Value *value);

/**
 *  @brief  JSON総称型データを真偽値として取得
 *
 *  @param  [in] value JSONデータ
 *
 *  @retval 0,1 JSON真偽値
 *  @retval 0未満 エラー
 */
int KJSD_JSON_valueGetBool(const KJSD_JSON_Value *value);

/**
 *  @brief  JSONデータ解放
 *
 *  @param  [in] value JSONデータ
 *
 *  @return なし
 */
void KJSD_JSON_free(KJSD_JSON_Value *value);

/**
 *  @brief  JSONルートオブジェクト作成
 *
 *  @param  なし
 *
 *  @retval NULL以外 JSONルートオブジェクト
 *  @retval NULL エラー
 *
 *  @note 使用後はKJSD_JSON_freeで解放する
 */
KJSD_JSON_Value* KJSD_JSON_createRoot(void);

/**
 *  @brief  JSONオブジェクトに新規オブジェクト追加
 *
 *  @param[in,out] base 追加先オブジェクト
 *  @param[in] name 追加データの名前
 *
 *  @retval NULL以外 新規追加されたJSONオブジェクト
 *  @retval NULL エラー
 */
KJSD_JSON_Object* KJSD_JSON_objectAddObject(
    KJSD_JSON_Object* base, const char* name);

/**
 *  @brief  JSONオブジェクトに新規配列追加
 *
 *  @param[in,out] base 追加先オブジェクト
 *  @param[in] name 追加データの名前
 *
 *  @retval NULL以外 新規追加されたJSON配列
 *  @retval NULL エラー
 */
KJSD_JSON_Array* KJSD_JSON_objectAddArray(
    KJSD_JSON_Object* base, const char* name);

/**
 *  @brief  JSONオブジェクトに新規文字列追加
 *
 *  @param[in,out] base 追加先オブジェクト
 *  @param[in] name 追加データの名前
 *
 *  @retval 1 成功
 *  @retval 0 失敗
 */
int KJSD_JSON_objectAddString(KJSD_JSON_Object* base,
                              const char* name,
                              const char* value);

/**
 *  @brief  JSONオブジェクトに新規数値追加
 *
 *  @param[in,out] base 追加先オブジェクト
 *  @param[in] name 追加データの名前
 *
 *  @retval 1 成功
 *  @retval 0 失敗
 */
int KJSD_JSON_objectAddNumber(KJSD_JSON_Object* base,
                              const char* name,
                              double value);

/**
 *  @brief  JSONオブジェクトに新規真偽値追加
 *
 *  @param[in,out] base 追加先オブジェクト
 *  @param[in] name 追加データの名前
 *
 *  @retval 1 成功
 *  @retval 0 失敗
 */
int KJSD_JSON_objectAddBool(KJSD_JSON_Object* base,
                            const char* name,
                            int value);

/**
 *  @brief  JSON配列に新規オブジェクト追加
 *
 *  @param[in,out] base 追加先配列
 *
 *  @retval NULL以外 追加されたJSONオブジェクト
 *  @retval NULL 失敗
 */
KJSD_JSON_Object* KJSD_JSON_arrayAddObject(KJSD_JSON_Array* base);

/**
 *  @brief  JSON配列に新規配列追加
 *
 *  @param[in,out] base 追加先配列
 *
 *  @retval NULL以外 追加されたJSON配列
 *  @retval NULL 失敗
 */
KJSD_JSON_Array* KJSD_JSON_arrayAddArray(KJSD_JSON_Array* base);

/**
 *  @brief  JSON配列に新規文字列追加
 *
 *  @param[in,out] base 追加先配列
 *
 *  @retval 1 成功
 *  @retval 0 失敗
 */
int KJSD_JSON_arrayAddString(KJSD_JSON_Array* base, const char* value);

/**
 *  @brief  JSON配列に新規数値追加
 *
 *  @param[in,out] base 追加先配列
 *
 *  @retval 1 成功
 *  @retval 0 失敗
 */
int KJSD_JSON_arrayAddNumber(KJSD_JSON_Array* base, double value);

/**
 *  @brief  JSON配列に新規真偽値追加
 *
 *  @param[in,out] base 追加先配列
 *
 *  @retval 1 成功
 *  @retval 0 失敗
 */
int KJSD_JSON_arrayAddBool(KJSD_JSON_Array* base, int value);

/**
 *  @brief  JSONシリアライザ(ファイルストリーム出力)
 *
 *  @param[in] value ダンプするJSONデータ
 *  @param[out] out ダンプ先のファイルストリーム
 *
 *  @return ダンプした文字数
 *
 *  @note outのopen/closeはしない
 */
size_t KJSD_JSON_serialize(const KJSD_JSON_Value* value, FILE* out);

/**
 *  @brief  JSONシリアライザ(メモリ出力)
 *
 *  @param[in] value ダンプするJSONデータ
 *  @param[out] out ダンプ先の領域
 *  @param[in] size ダンプ先の領域サイズ
 *
 *  @return ダンプした文字数
 *
 *  @note outの領域確保/解放はしない
 */
size_t KJSD_JSON_serializeS(const KJSD_JSON_Value* value,
                            char* out, size_t size);

/**
 *  @brief  JSONシリアライズ後の文字数取得
 *
 *  @param[in] value サイズ取得するJSONデータ
 *
 *  @return シリアライズ後の文字数
 */
size_t KJSD_JSON_sizeof(const KJSD_JSON_Value* value);

#ifdef __cplusplus
}
#endif

#endif /* KJSD_JSON_H */
