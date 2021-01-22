/*
 * Copyright 2017-2018 Duke Energy Corporation and Open Energy Solutions, Inc.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>

//namespace trompeloeil
//{
//    template <>
//    inline void reporter<specialized>::send(      //** 3 **//
//        severity s,
//        const char* file,
//        unsigned long line,
//        const char* msg)
//    {
//        std::ostringstream os;
//        if (line) os << file << ':' << line << '\n';
//        os << msg;
//        auto failure = os.str();
//        if (s == severity::fatal)
//        {
//            FAIL(failure);
//        }
//        else
//        {
//            CAPTURE(failure);
//            CHECK(failure.empty());
//        }
//    }
//
//} // namespace trompeloeil
