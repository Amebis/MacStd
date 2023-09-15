/*
	SPDX-License-Identifier: MIT
	Copyright © 2023 Amebis
*/

#import <XCTest/XCTest.h>
#include "../include/MacStd/dyld.hpp"

@interface Tests : XCTestCase
@end

@implementation Tests

- (void)setUp {
	// Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
	// Put teardown code here. This method is called after the invocation of each test method in the class.
}

- (void)test_NSGetExecutablePath {
	std::string path;
	XCTAssert(_NSGetExecutablePath(path) == 0);
}

- (void)testPerformanceExample {
	// This is an example of a performance test case.
	[self measureBlock:^{
		// Put the code you want to measure the time of here.
	}];
}

@end
