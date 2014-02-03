 /*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * Main file author: Rickard Englund
 *
 *********************************************************************************/

#include <inviwo/core/util/urlparser.h>

#ifndef IVW_URLPARSERTEST_TEST_H
#define IVW_URLPARSERTEST_TEST_H


TEST(URLParserTest,fileExistsTest) {
#ifdef __FILE__
    EXPECT_TRUE(URLParser::fileExists(__FILE__));
#endif
    EXPECT_TRUE(URLParser::fileExists(global_argv[0]));//Cant find current executable
}

TEST(URLParserTest,fileExtensionTest) {
    EXPECT_STREQ("",URLParser::getFileExtension("").c_str());
    EXPECT_STREQ("txt",URLParser::getFileExtension("test.txt").c_str());
    EXPECT_STREQ("txt",URLParser::getFileExtension("test.dobule.txt").c_str());
    EXPECT_STREQ("",URLParser::getFileExtension("noExtensions").c_str());
    EXPECT_STREQ("",URLParser::getFileExtension("C:/a/directory/for/test/noExtensions").c_str());
    EXPECT_STREQ("", URLParser::getFileExtension("C:/a/directory/for/test.test/noExtensions").c_str());
#ifdef __FILE__
    EXPECT_STREQ("h",URLParser::getFileExtension(__FILE__).c_str());
#endif
}



TEST(URLParserTest,FileDirectoryTest) {
    EXPECT_STREQ("C:/a/directory/for/test/",URLParser::getFileDirectory("C:/a/directory/for/test/file.txt").c_str());
    EXPECT_STREQ("C:\\a\\directory\\for\\test\\",URLParser::getFileDirectory("C:\\a\\directory\\for\\test\\file.txt").c_str());
    EXPECT_STREQ("",URLParser::getFileDirectory("justafile.txt").c_str());
    // EXPECT_STREQ("C:/a/directory/for/test/",URLParser::getFileDirectory("C:/a/directory/for/test//withdoubleslahs.txt").c_str());
}

#endif