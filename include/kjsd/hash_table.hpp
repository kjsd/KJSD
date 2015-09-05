/**
 * @file hash_table.hpp
 *
 * @version $Id:$
 *
 * @brief 動的ハッシュテーブル．
 *
 * 順序性を犠牲にして高速アクセスに特化したKey-Valueコンテナ．
 * エレメントの検索，追加，削除いずれもほぼO(1)である．
 * データ構造の順序性保証，sort機能などが求められる場合はstd::mapを使
 * う必要があるが，その他のスループットに関してはあらゆる点でstd::map
 * と同等以上．
 *
 * 実装パターンはリニアハッシュとして良く知られるオーソドックスな機構
 * (複数のリンクリストを束ねるバケットを用意したセグメント構造)である．
 * データ構造のイメージは下図参照．
 * 
 *  Segment　　　Element lists @n
 *  +-------+ @n
 *  |Bucket0|->□->□->... @n
 *  +-------+ @n
 *  |Bucket1|->□->□->□->... @n
 *  +-------+ @n
 *  |Bucket2|->□->... @n
 *  +-------+ @n
 *
 * @author Kenji MINOURA / kenji@kandj.org
 *
 * Copyright (c) 2012 The KJSD Project. All rights reserved.
 *
 * @see <related_items>
 ***********************************************************************/
#ifndef KJSD_HASH_TABLE_HPP
#define KJSD_HASH_TABLE_HPP

#include <utility>
#include <vector>
#include <string>
#include <iterator>
#include <functional>
#include <cstring>
#include <kjsd/util.hpp>


namespace std
{
    /**
     * @brief C文字列比較用関数オブジェクト
     */
    template<> class equal_to<const char*>
    {
    public:
        bool operator()(const char* x, const char* y) const
        {
            return std::strcmp(x, y) == 0;
        }
    };
}

namespace kjsd
{
    /// ハッシュ関数値型
    typedef std::size_t hash_type;

    /**
     * @brief ハッシュ関数オブジェクト
     *
     * 独自クラスをキーにする場合は，使用者がこのクラスを拡張特殊化し
     * なくてはならないが，その際以下の要件が求められる．
     *
     * @li あるキーに対するハッシュ値は常に同じ値でなければならない
     * @li 算出されるハッシュ値の分布が偏るほどパフォーマンスが低下す
     * る
     */
    template<typename K> class Hash
    {
    public:
        kjsd::hash_type operator()(const K& k) const
        {
            return static_cast<hash_type>(k);
        }
    };
    /**
     * @brief ハッシュ関数オブジェクト(C文字列特殊化)
     */
    template<> class Hash<const char*>
    {
    public:
        kjsd::hash_type operator()(const char* k) const
        {
            kjsd::hash_type len = std::strlen(k);
            return (len == 0) ? 0: len + 4 * (k[0] + 4 * k[len / 2]);
        }
    };
    /**
     * @brief ハッシュ関数オブジェクト(文字列コンテナ特殊化)
     */
    template<> class Hash<std::string>
    {
    public:
        kjsd::hash_type operator()(const std::string& k) const
        {
            return (k.size() == 0) ? 0:
                k.size() + 4 * (k.at(0) + 4 * k.at(k.size() / 2));
        }
    };
    /**
     * @brief ハッシュ関数オブジェクト(ポインタ特殊化)
     */
    template<typename K> class Hash<K*>
    {
    public:
        kjsd::hash_type operator()(const K* k) const
        {
            return reinterpret_cast<hash_type>(k);
        }
    };

    template<typename K, typename T, typename H, typename P>
    class HashTable;

    /**
     * @brief イテレータ定義
     *
     * HashTableに格納されている全データに順次アクセス可能な前方アクセ
     * スイテレータ．
     * 対象となるHashTableオブジェクトを介して取得する．単独でインスタ
     * ンス化はできない．
     * 対象となるHashTableオブジェクトの構造が変更された場合
     * (insert,erase,rehash..)，それまでに取得したイテレータに対するア
     * クセスの結果は未定義である．HashTable::end()で取得できるイテレー
     * タに対しても同様．
     *
     * @see std::forward_iterator_tag
     */
    template<typename K, typename T, typename H, typename P>
    class HashTableIterator :
        public std::iterator<std::forward_iterator_tag,
                             typename HashTable<K,T,H,P>::value_type>
    {
        friend class HashTable<K,T,H,P>;
    public:
        typename HashTable<K,T,H,P>::value_type& operator*() const
        { return *lit_; }
        HashTableIterator& operator++()
        {
            typename HashTable<K,T,H,P>::local_iterator last_it = 
                ht_->buckets_[ht_->bucket_count() - 1].end();

            if (idx_ >= ht_->bucket_count()) return *this;
            if (lit_ == last_it) return *this;
            
            if (++lit_ != ht_->buckets_[idx_].end()) return *this;

            for (++idx_; idx_ < ht_->bucket_count(); ++idx_)
            {
                if (!ht_->buckets_[idx_].empty())
                {
                    lit_ = ht_->buckets_[idx_].begin();
                    break;
                }
                lit_ = last_it;
            }

            return *this;
        }
        HashTableIterator operator++(int)
        {
            HashTableIterator prev = *this;
            this->operator++();
            return prev;
        }
        bool operator==(const HashTableIterator& it) const
        {
            return (idx_ == it.idx_) && (lit_ == it.lit_);
        }
        bool operator!=(const HashTableIterator& it) const
        {
            return !operator==(it);
        }

    private:
        HashTableIterator(const HashTable<K,T,H,P>* ht, kjsd::hash_type idx,
                          typename HashTable<K,T,H,P>::local_iterator lit)
            : ht_(ht), idx_(idx), lit_(lit) {}

        const HashTable<K,T,H,P>* ht_;
        kjsd::hash_type idx_;
        typename HashTable<K,T,H,P>::local_iterator lit_;
    };

    /**
     * @brief 動的ハッシュテーブル管理クラスの雛形．
     *
     * 管理するキーと値の型を指定して作成する．このペアをエレメントとみなす．
     * 格納系メソッドにはエレメントを指定する．取得系メソッドはキーを指定することで
     * 対象エレメントを指すイテレータを得ることができる．
     * キーに独自クラスなど組み込み型以外の型を使う場合はハッシュ関数
     * オブジェクトおよびキー比較関数オブジェクトを指定する必要がある．
     * C文字列型(const char*)と文字列コンテナ型(std::string)はデフォル
     * ト値で使用可能．
     *
     * @param[in] K キーの型
     * @param[in] T 扱うデータの型．デフォルトコンストラクタ必須
     * @param[in] H ハッシュ関数クラス型
     * @param[in] P キー比較関数クラス型
     *
     * @note キーは一意であることが求められる
     */
    template<typename K, typename T,
             typename H = kjsd::Hash<K>, typename P = std::equal_to<K> >
    class HashTable
    {
        friend class HashTableIterator<K,T,H,P>;
    public:
        typedef kjsd::hash_type size_type;

        /**
         * @brief バケットの各エレメントの型定義．キーと値のペア．
         *
         * @c value_type::first でキーに対するアクセス．
         * @c value_type::second で値に対するアクセス．となる．
         *
         * @see std::pair
         */
        typedef typename std::pair<K, T> value_type;

        /// バケットの型定義．
        typedef std::vector<value_type> Bucket;

        /// コンテナ全体にアクセスするためのイテレータ
        typedef typename kjsd::HashTableIterator<K,T,H,P> iterator;

        /// 単体バケット内部にアクセスするためのイテレータ
        typedef typename Bucket::iterator local_iterator;


        /**
         * @brief デフォルトのバケット数(セグメントサイズ)．これを素数にすることで
         * ハッシュ値の重複を最小にする．
         *
         * @see next_size
         *
         * 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61,
         * 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137,
         * 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197,
         * 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269,
         * 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347,
         * 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419,
         * 421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487,
         * 491, 499, 503, 509, 521, 523, 541, 547, 557, 563, 569, 571,
         * 577, 587, 593, 599, 601, 607, 613, 617, 619, 631, 641, 643,
         * 647, 653, 659, 661, 673, 677, 683, 691, 701, 709, 719, 727,
         * 733, 739, 743, 751, 757, 761, 769, 773, 787, 797, 809, 811,
         * 821, 823, 827, 829, 839, 853, 857, 859, 863, 877, 881, 883,
         * 887, 907, 911, 919, 929, 937, 941, 947, 953, 967, 971, 977,
         * 983, 991, 997, 1009, 1013, 1019, 1021, 1031, 1033, 1039,
         * 1049
         */
        static const size_type DEFAULT_EXPECTED_SIZE = 2;

        /**
         * @brief コンストラクタ
         *
         * 初期セグメントを確保．パフォーマンスをO(1)にするため，指定
         * サイズのバケットを格納できる最小の素数をバケット数とする．
         * 以後，追加されるデータ数に応じてバケット数は自動で拡張され
         * るが，その数は常に素数個であることを確率的に保証する．
         *
         * @param[in] expected_size 格納予定のデータ数
         */
        explicit HashTable(size_type expected_size = DEFAULT_EXPECTED_SIZE)
            : total_size_(0)
        {
            bkt_cnt_ = next_size(expected_size);
            buckets_ = new Bucket[bkt_cnt_];
        }

        /**
         * @brief デストラクタ．セグメントを解放
         */
        virtual ~HashTable()
        {
            delete[] buckets_;
        }

        /**
         * @brief 現在のバケット数取得
         */
        size_type bucket_count() const
        {
            return bkt_cnt_;
        }

        /**
         * @brief キーによるランダムアクセス
         *
         * キーを添字として値を取得する連想配列的アクセス方法を提供す
         * る．
         * 直接格納されている値を更新することができる．
         * 存在しないキーを指定されると自動で値が確保される．
         *
         * @param[in] k 検索キー
         *
         * @return キーに対応する値への参照
         */
        T& operator[](const K& k)
        {
            iterator it = find(k);
            if (it == end())
            {
                it = insert(std::make_pair(k, T()));
            }

            return (*it).second;
        }

        /**
         * @brief エレメント格納
         *
         * 現在確保されているバケット数を超えるエレメントを格納しよう
         * とした場合は自動でバケット数が拡張される．
         * 既に格納済のキーを指定された場合は上書きされる．
         *
         * @param[in] e 格納するデータエレメント
         *
         * @return 格納されたエレメントを指すイテレータ
         *
         * @see rehash
         */
        iterator insert(const value_type& e)
        {
            iterator it = find(e.first);
            if (it != end())
            {
                (*it).second = e.second;
                return it;
            }

            if (total_size_ == bkt_cnt_)
            {
                rehash(bkt_cnt_ * 2);
            }

            buckets_[bucket(e.first)].push_back(e);
            total_size_++;
            it = find(e.first);

            return it;
        }

        /**
         * @brief テーブル再構築
         *
         * 指定されたバケット数以上の最小素数値でハッシュテーブルを拡
         * 張または縮小し再構築する．
         * エレメントの格納順，配置が変更されるためそれまでのイテレー
         * タは無効になる．
         * 現在格納されているエレメント数以下，もしくは現在のバケット
         * 数と同じ値が指定された場合は何もしない．
         *
         * @param[in] n 再構築後のバケットサイズ
         *
         * @see bucket_count
         * @see size
         */
        void rehash(size_type n)
        {
            size_type sz = next_size(n);

            if ((sz <= total_size_) || (sz == bkt_cnt_)) return;

            Bucket* new_bkt = new Bucket[sz];
            for (iterator it = begin(); it != end(); ++it)
            {
                new_bkt[normalize((*it).first, sz)].push_back(*it);
            }

            delete[] buckets_;
            buckets_ = new_bkt;
            bkt_cnt_ = sz;
        }

        /**
         * @brief エレメント検索
         *
         * @param[in] k 取得する値のキー
         *
         * @return 見つかったエレメントの読取専用イテレータ．見つから
         * ない場合はend()を返す
         */
        const iterator find(const K& k) const
        {
            kjsd::hash_type idx = bucket(k);

            for (local_iterator it = buckets_[idx].begin();
                 it != buckets_[idx].end(); ++it)
            {
                if (key_equal_((*it).first, k))
                {
                    return iterator(this, idx, it);
                }
            }
            return end();
        }

        /**
         * @brief エレメント削除
         *
         * @param[in] it 削除するエレメントのイテレータ
         */
        void erase(const iterator it)
        {
            if (it == end()) return;

            buckets_[it.idx_].erase(it.lit_);
            --total_size_;
        }

        /**
         * @brief エレメント削除
         *
         * 削除したエレメント数を返すが，キーの重複は無いので常に0か1
         * である．
         *
         * @param[in] k 削除するエレメントのキー
         *
         * @retval 0 削除してない
         * @retval 1 削除された
         */
        size_type erase(const K& k)
        {
            iterator it = find(k);
            erase(it);
    
            return (it == end()) ? 0: 1;
        }

        /**
         * @brief 全エレメントの削除
         */
        void clear()
        {
            for (size_type i = 0; i < bkt_cnt_; ++i)
            {
                buckets_[i].clear();
            }
            total_size_ = 0;
        }

        /**
         * @brief 先頭要素を指すイテレータ取得
         *
         * エレメントが空のときはend()が返る．
         *
         * @return 先頭要素を指す読取専用イテレータ
         */
        const iterator begin() const
        {
            for (size_type i = 0; i < bkt_cnt_; ++i)
            {
                if (!buckets_[i].empty())
                {
                    return iterator(this, i, buckets_[i].begin());
                }
            }

            return end();
        }

        /**
         * @brief 末尾の次を指すイテレータ取得
         *
         * 無効なイテレータである．中身へのアクセスやイテレータ操作し
         * た場合の挙動は未定義である．比較にのみ使われることを想定し
         * ている．
         *
         * @return 末尾の次を指す読取専用イテレータ
         */
        const iterator end() const
        {
            return iterator(this, bkt_cnt_, buckets_[bkt_cnt_ - 1].end());
        }

        /**
         * @brief ハッシュテーブル全体の使用量を取得
         *
         * @return 使用量．各バケットの総サイズ
         */
        size_type size() const
        {
            return total_size_;
        }

        /**
         * @brief ハッシュテーブル全体が空かどうかを判定
         *
         * @retval true 空
         * @retval false 空じゃない
         */
        bool empty() const
        {
            return total_size_ == 0;
        }

        /**
         * @brief エレメント数取得
         *
         * 指定されたキーで格納されているエレメント数を返すが，キーの
         * 重複は無いので常に0か1である．重複判定として使える．
         *
         * @param[in] k キー
         *
         * @retval 0 格納されていない
         * @retval 1 格納されている
         */
        size_type count(const K& k) const
        {
            return (find(k) == end()) ? 0: 1;
        }

        /**
         * @brief バケットインデックス取得
         *
         * 指定キーが格納されるバケットのインデックスを取得する．
         * insertやrehashなどで再構築が行われない限り常に同じ値である．
         *
         * @param[in] k キー
         *
         * @return バケットインデックス
         */
        size_type bucket(const K& k) const
        {
            return normalize(k, bkt_cnt_);
        }

    private:
        size_type total_size_;
        Bucket* buckets_;
        size_type bkt_cnt_;
        H hasher_;
        P key_equal_;

        /**
         * @brief ハッシュ値正規化
         *
         * ハッシュ値を使って値を0〜指定範囲内で正規化する．一意なバケッ
         * トを指すインデックスとして使用可能である．
         *
         * @param[in] k キー
         * @param[in] max 正規化範囲の最大値+1
         *
         * @return 正規化値
         */
        size_type normalize(const K& k, size_type max) const
        {
            return static_cast<size_type>(hasher_(k) % max);
        }

        /**
         * @brief 指定数を満たすバケット数取得
         *
         * バケットの数を素数個にするため，指定数以上である最小の素数
         * を返すことを確率的に保証する．素数判定にかかるコストと精度
         * はトレードオフである．
         *
         * @param[in] n 指定バケット数
         *
         * @return n以上の最小素数値
         */
        size_type next_size(size_type n)
        {
            for (; !kjsd::util::is_prime<size_type>(n); ++n);
            return n;
        }
    };
}

#endif // KJSD_HASH_TABLE_HPP
