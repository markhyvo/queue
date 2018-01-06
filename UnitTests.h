/*
* This file contains interface for unittest functions for testing
* queue (container adapter).
* Unit tests are provided for all member functions.
*/
#ifndef MH_UNITTESTS_H
#define MH_UNITTESTS_H

namespace MH {

      class UnitTests
      {
      public:
         static void TestConstructor();
         static void TestDestructor();
         static void TestCopyConstructor();
         static void TestCopyAssignmentOperator();
         static void TestPush();
         static void TestPop();
         static void TestFront();
         static void TestFrontConst();
         static void TestEmpty();
         static void TestSize();;
         static void TestAll();
      private:
         // Private constructor prevents instantiation
         UnitTests() {}
      };
}
#endif