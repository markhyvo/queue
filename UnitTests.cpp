/*
 * This file contains implementation for queue (container adapter).
 * It is designed to to operate in FIFO context(fisrt-in first-out)
 * manner.
 * Unit tests are provided for all member functions.
 */
#include <string>
using std::string;

#include "Queue.h"
using MH::Queue;

#include "UnitTests.h"
using MH::UnitTests;

//-----------------------------------------------------------------------------
// Helpers
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Tests
//-----------------------------------------------------------------------------
void 
MH::UnitTests::TestAll()
{
   TestConstructor();
   TestDestructor();
   TestCopyConstructor();
   TestCopyAssignmentOperator();
   TestPush();
   TestPop();
   TestFront();
   TestFrontConst();
   TestEmpty();
   TestSize();;
}

int main()
{
   MH::UnitTests::TestAll();
}
