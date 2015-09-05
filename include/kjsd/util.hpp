/**
 * @file util.hpp
 *
 * @version $Id:$
 *
 * @brief
 *
 * @author Kenji MINOURA / kenji@kandj.org
 *
 * Copyright (c) 2012 The KJSD Project. All rights reserved.
 *
 * @see <related_items>
 ***********************************************************************/
#ifndef KJSD_UTIL_HPP
#define KJSD_UTIL_HPP

#include <string>
#include <cstdlib>
#include <climits>

namespace kjsd
{
    namespace util
    {
        /**
         * 最大公約数(ユークリッド互除法)
         */
        template<typename T> inline T gcd(T a, T b)
        {
            if (a < b)
            {
                T n = a;
                a = b;
                b = n;
            }
            if (b == 0) return 0;

            do
            {
                T r = a % b;
                a = b;
                b = r;
            }
            while (b != 0);

            return a;
        }

        /**
         * 法nのべき乗計算( aのk乗をnで割った余りを求める )
         *
         * @param[in] a 底
         * @param[in] k 指数
         * @param[in] n 法
         */
        template<typename T> inline T power(T a, T k, T n)
        {
            if (a == 0 || n == 0) return 0;
            if (k == 0) return 1 % n;

            T currentMod = a % n;
            T currentValue = ((k & 1) > 0) ? currentMod : 1;

            for (k >>= 1; k > 0; k >>= 1)
            {
                currentMod = (currentMod * currentMod) % n;
                if ((k & 1) > 0)
                    currentValue = (currentValue * currentMod) % n;
            }

            return currentValue;
        }

        /**
         * 確率的素数判定(フェルマーテスト)
         */
        template<typename T> inline bool is_prime(T p)
        {
            if (p == 1) return false;
            if (p == 2) return true;
            if ((p & 1) == 0) return false;

            T a(2U);
            T gap(p);
            gap /= 100;
            if (gap == 0) gap += 1;
            T pm(p - 1);

            while (a < p)
            {
                T g(gcd(a, p));
                if (g != 1) return false;

                T pow = power(a, pm, p);
                if (pow != 1) return false;
                a += gap;
            }

            return true;
        }

        inline bool is_le()
        {
#if defined(__LITTLE_ENDIAN__)
            return true;
#elif defined(__BIG_ENDIAN__)
            return false;
#else
            int i = 1;
            return static_cast<bool>(*reinterpret_cast<char*>(&i));
#endif
        }

        template<typename T> inline T change_endian(T val)
        {
            const size_t bitT = sizeof(T) * CHAR_BIT;
            const size_t cmask = (1U << CHAR_BIT) - 1;
            T v = 0;
            for(size_t i = 0; i < sizeof(T); ++i)
            {
                v |= (val & cmask) << (bitT - (CHAR_BIT * (i + 1)));
                val >>= CHAR_BIT;
            }
            return v;
        }

        template<typename T> inline T htobe(T val)
        {
            return is_le() ? change_endian<T>(val): val;
        }

        template<typename T> inline T htole(T val)
        {
            return is_le() ? val: change_endian<T>(val);
        }

        template<typename T> inline T betoh(T val)
        {
            return is_le() ? change_endian<T>(val): val;
        }

        template<typename T> inline T letoh(T val)
        {
            return is_le() ? val: change_endian<T>(val);
        }

        inline std::string narrow(const std::wstring& input)
        {
            char* buffer = new char[input.size() * MB_CUR_MAX + 1];
#ifdef _WIN32
            size_t ret_val = 0;
            wcstombs_s(&ret_val, buffer, input.size() * MB_CUR_MAX,
                       input.c_str(), _TRUNCATE);
#else
            wcstombs(buffer, input.c_str(), input.size() * MB_CUR_MAX);
#endif // _WIN32
            std::string result = buffer;
            delete[] buffer;
            return result;
        }

        inline std::wstring widen(const std::string& input)
        {
            wchar_t* buffer = new wchar_t[input.size() + 1];
#ifdef _WIN32
            size_t ret_val = 0;
            mbstowcs_s(&ret_val, buffer, input.size() * MB_CUR_MAX,
                       input.c_str(), _TRUNCATE);
#else
            mbstowcs(buffer, input.c_str(), input.size());
#endif // _WIN32
            std::wstring result = buffer;
            delete[] buffer;
            return result;
        }
    }
}

#endif /* KJSD_UTIL_HPP */
