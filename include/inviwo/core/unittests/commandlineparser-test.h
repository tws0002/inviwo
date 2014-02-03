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

#include <inviwo/core/util/commandlineparser.h>
#ifndef IVW_COMMANDLINEPARSER_TEST_H
#define IVW_COMMANDLINEPARSER_TEST_H



TEST(CommandLineParserTest,DefaultTest) {
    const int argc = 1;
    char* argv[argc] = {"unittests.exe"};
    CommandLineParser clp(argc,argv);
    clp.initialize();
    clp.parse();
    EXPECT_STREQ("",clp.getOutputPath().c_str());
    EXPECT_STREQ("",clp.getWorkspacePath().c_str());
    EXPECT_STREQ("",clp.getSnapshotName().c_str());
    EXPECT_STREQ("",clp.getScreenGrabName().c_str());
    EXPECT_STREQ("",clp.getPythonScirptName().c_str());
    EXPECT_FALSE(clp.getCaptureAfterStartup());
    EXPECT_FALSE(clp.getScreenGrabAfterStartup());
    EXPECT_FALSE(clp.getRunPythonScriptAfterStartup());
    EXPECT_FALSE(clp.getQuitApplicationAfterStartup());
    EXPECT_FALSE(clp.getLoadWorkspaceFromArg());
    EXPECT_TRUE(clp.getShowSplashScreen());
}


TEST(CommandLineParserTest,InviwoApplicationsParserTest) {
    InviwoApplication* app = InviwoApplication::getPtr();
    const CommandLineParser* clp = app->getCommandLineParser();
    ASSERT_TRUE(clp != 0);
    EXPECT_STREQ("",clp->getOutputPath().c_str());
    EXPECT_STREQ("",clp->getWorkspacePath().c_str());
    EXPECT_STREQ("",clp->getSnapshotName().c_str());
    EXPECT_STREQ("",clp->getScreenGrabName().c_str());
    EXPECT_STREQ("",clp->getPythonScirptName().c_str());
    EXPECT_FALSE(clp->getCaptureAfterStartup());
    EXPECT_FALSE(clp->getScreenGrabAfterStartup());
    EXPECT_FALSE(clp->getRunPythonScriptAfterStartup());
    EXPECT_FALSE(clp->getQuitApplicationAfterStartup());
    EXPECT_FALSE(clp->getLoadWorkspaceFromArg());
    //   EXPECT_TRUE(clp->getShowSplashScreen());
}



TEST(CommandLineParserTest,CommandLineParserTest) {
    const int argc = 3;
    char* argv[argc] = {"unittests.exe","-w","C:/Just/A/Path/"};
    CommandLineParser clp(argc,argv);
    clp.initialize();
    clp.parse();
    EXPECT_STREQ("",clp.getOutputPath().c_str());
    EXPECT_STREQ("C:/Just/A/Path/",clp.getWorkspacePath().c_str());
    EXPECT_STREQ("",clp.getSnapshotName().c_str());
    EXPECT_STREQ("",clp.getScreenGrabName().c_str());
    EXPECT_STREQ("",clp.getPythonScirptName().c_str());
    EXPECT_FALSE(clp.getCaptureAfterStartup());
    EXPECT_FALSE(clp.getScreenGrabAfterStartup());
    EXPECT_FALSE(clp.getRunPythonScriptAfterStartup());
    EXPECT_FALSE(clp.getQuitApplicationAfterStartup());
    EXPECT_TRUE(clp.getLoadWorkspaceFromArg());
    EXPECT_TRUE(clp.getShowSplashScreen());
}





#endif