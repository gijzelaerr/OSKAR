/*
 * Copyright (c) 2015, The University of Oxford
 * All rights reserved.
 *
 * This file is part of the OSKAR package.
 * Contact: oskar at oerc.ox.ac.uk
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the University of Oxford nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <gtest/gtest.h>

#include "oskar_SettingsKey.hpp"
#include <vector>
#include <string>

using oskar::SettingsKey;
using namespace std;

TEST(SettingsKey, create_simple)
{
    const char* key = "foo";
    SettingsKey k(key);
    ASSERT_STREQ(key, k);
    ASSERT_TRUE(k == SettingsKey(key));
    ASSERT_EQ(0, k.depth());
    vector<string> tokens = k.tokens();
    ASSERT_EQ((size_t)1, tokens.size());
    ASSERT_STREQ("foo", k.leaf().c_str());
}

TEST(SettingsKey, create_type_1)
{
    const char* key = "foo/bar/key";
    SettingsKey k(key);
    ASSERT_STREQ(key, k);
    ASSERT_TRUE(k == SettingsKey(key));
    ASSERT_EQ(2, k.depth());
    vector<string> tokens = k.tokens();
    ASSERT_EQ((size_t)3, tokens.size());
    ASSERT_STREQ("key", k.leaf().c_str());
}

TEST(SettingsKey, create_type_2)
{
    const char* key = "foo\\bar\\key";
    SettingsKey k(key, '\\');
    ASSERT_STREQ(key, k);
    ASSERT_TRUE(k == SettingsKey(key,'\\'));
    ASSERT_EQ(2, k.depth());
    vector<string> tokens = k.tokens();
    ASSERT_EQ((size_t)3, tokens.size());
    ASSERT_STREQ("key", k.leaf().c_str());
}

TEST(SettingsKey, create_type_3)
{
    const char* key = "A:B:C";
    SettingsKey k(key, ':');
    ASSERT_STREQ(key, k);
    ASSERT_TRUE(k == SettingsKey(key,':'));
    ASSERT_EQ(2, k.depth());
    vector<string> tokens = k.tokens();
    ASSERT_EQ((size_t)3, tokens.size());
    ASSERT_STREQ("C", k.leaf().c_str());
}



