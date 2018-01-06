/*
 * This file contains implementation for queue (container adapter) and
 * unit tests for it.
 * Queue class is designed to to operate in FIFO context(fisrt-in first-out)
 * manner.
 * Unit tests are provided for all public member functions.
 */
#include <algorithm>
using std::copy;
#include <exception>
#include <cassert>
#include <cstring>
using std::memmove;
#include <iostream>
using std::cout;
using std::clog;
#include <new>
using std::logic_error;

#include <string>
using std::string;

#include "Queue.h"
using MH::Queue;

#include "UnitTests.h"
using MH::UnitTests;

// Constants
const int INIT_ELEM = 10;           // Initial buffer element count
const int BUF_MULTIP = 3;           // buffer size algorithm
const int BUF_ADD = 2;              // buffer size algorithm

bool destructorCalled = false;

// Helper function for class Queue
template <typename T>
T *
MH::newCopy(const T *src, size_t srcsize, rsize_t destsize)
{  
   // assert stripped out of PROD code
   assert(destsize >= srcsize); // Destination at least as big as source
   // memory allocated and default constructors ran for all T elems
   T *dest = new T[destsize];         
   try
   {
      // Throws if either an element assignment(T elems) 
      // or an operation on iterators throws.
      copy(src, src + srcsize, dest);
   }
   catch (...)
   {
      delete[] dest;            // de-alloc mem and run destructors -can't throw      
      throw;                    // Rethrow original exception to caller
   }
   return dest;
}

template <typename T>
MH::Queue<T>::Queue()
   : v_(0),
     vsize_(INIT_ELEM),
     vused_(0)
{   
   // Course info: Array version of new puts a safety net in place,
   // a for loop that will run in reverse in case one or more
   // T object contructors fail - no memory leaks.
   v_ = new T[vsize_];   
}

template <typename T>
MH::Queue<T>::~Queue()
{  
   destructorCalled = true;
   // Course Guideline: destructors never throw
   // Compiler generated for loop runs in reverse and calls destructors 
   // on every T element in the array and finally de-allocates memory.
   delete[] v_;               
}

// Copy constructor
template <typename T>
MH::Queue<T>::Queue(const Queue<T> &other)
        : v_(newCopy(other.v_,
                     other.vsize_,
                     other.vsize_)),
           vsize_(other.vsize_),
           vused_(other.vused_)
{
   //cout << "\nCOPY CONSTRUCTOR RAN!";
}

// Copy assignment operator
template <typename T>
Queue<T> &
MH::Queue<T>::operator=(const Queue<T> &other)
{
   cout << "\nCOPY ASSIGNMENT RAN!";
   if (this != &other)
   {
      T *v_new = newCopy(other.v_,
                         other.vsize_,
                         other.vsize_);      
      // run destructors in reverse order to construction and de-allocate mem 
      delete[] v_;              
      v_ = v_new;                 // take ownership of new mem block
      vsize_ = other.vsize_;
      vused_ = other.vused_;
   }
   return *this;
}

// Inserts a new element at the end of the queue,
// after its current last element. 
template <typename T>
void
MH::Queue<T>::push(const T& t)
{
   // Expand array if needed. If throws, local variable
   // scratch pad will automatically clean itself
   if (vused_ == vsize_)
   {  
      size_t vsize_new = getNewBufferSize();     // Retrieve larger buff size
      T *v_new = newCopy(v_, vsize_, vsize_new); // does not throw
      // run destructors in reverse order to construction and de-allocate mem 
      delete[] v_;              
      v_ = v_new;                 // take ownership of new block
      vsize_ = vsize_new;
   }
   else
   {  
      try
      {  
         v_[vused_] = t;          // assignment to end - can throw
      }
      catch (...)
      {
         // assignent failed, destruct elems and de-allocate memory
         size_t vsize_old = getOldBufferSize();     // Retrieve old memory block
         T *v_old = newCopy(v_, vsize_, vsize_old); // allocate old memory block
         // run destructors in reverse order to construction and de-allocate mem 
         delete[] v_;           
         v_ = v_old;              // take ownership of new block
         vsize_ = vsize_old;      
         throw;                   // re-throw exception to be exception neutral
      }                           
      ++vused_;                   // increment, elem was successfully pushed
   }
}

// Removes "oldest" element in the queue from front
// If empty queue throws exception
template <typename T>
void
MH::Queue<T>::pop()
{
   if (vused_ == 0)               // Empty stack
   {
      throw logic_error("Pop from empty stack");
   }
   else
   {  
      // shift all elems to lower indexes by an element
      memmove(v_, v_ + 1, sizeof(T) * vused_);
      --vused_;      
   }
}

// Returns a reference to the next element in the queue.
// The next element is the "oldest" element in the queue
template <typename T>
T &
MH::Queue<T>::front()
{   
   // what if empty queue?
   //cout << "\nfront() - called";
   return *v_;
}

// Returns a reference to the next element in the queue.
// The next element is the "oldest" element in the queue
template <typename T>
const T &
MH::Queue<T>::front() const  // compiler verifies function doesn't change state
{
   //cout << "\nCONST front() - called";
   return *v_;
}

// Returns whether queue is empty.
template <typename T>
bool
Queue<T>::empty() const
{
   if (!vused_)
      return true;
   else
      return false;
}

// Returns the number of elements in the queue.
template <typename T>
size_t
MH::Queue<T>::size() const
{
   return vsize_;
}

// Returns the new buffer size
template <typename T>
size_t
MH::Queue<T>::getNewBufferSize()
{
   return vsize_ * BUF_MULTIP + BUF_ADD;
}

// Returns the orig buffer size in use before exception
template <typename T>
size_t
MH::Queue<T>::getOldBufferSize()
{
   return vsize_ / BUF_MULTIP - BUF_ADD;
}




///////////////////////////////////////////////////////
//                 UNIT TESTS BEGIN
///////////////////////////////////////////////////////
void
MH::UnitTests::TestConstructor()
{
   const int ELEMS = 10;

   // Initialize queue
   Queue<int> q;   
       
   // Verify result
   if (q.size() == ELEMS)
      clog << "TestConstructor PASSED";
   else
      clog << "TestConstructor FAILED, expected " << ELEMS << ", saw " << q.size();
   clog << "\n";
}

void
MH::UnitTests::TestDestructor()
{
   destructorCalled = false;
   // Initialize queue
   Queue<int> *q = new Queue<int>;
   q->~Queue();
   // Verify result
   if (destructorCalled)
      clog << "TestDestructor PASSED";
   else
      clog << "TestDestructor FAILED, expected " << true << ", saw " << destructorCalled;
   clog << "\n";   
}

void
MH::UnitTests::TestCopyConstructor()
{

   const int INT_RES = 2;

   Queue<int> src;
   src.push(2);
   src.push(4);
   Queue<int> dest(src);

   // Verify result
   if (dest.front() == 2)
      clog << "TestCopyConstructor PASSED";
   else
      clog << "TestCopyConstructor FAILED, expected " << INT_RES << ", saw " << dest.front();
   clog << "\n";
}

void
MH::UnitTests::TestCopyAssignmentOperator()
{
   const double DOUBLE_RES = 2.2;

   Queue<double> dQ;
   dQ.push(2.2);
   dQ.push(4.4);
   Queue<double> dQOther;
   dQOther.push(dQ.front());

   // Verify result
   if (dQOther.front() == DOUBLE_RES)
      clog << "TestCopyAssignmentOperator PASSED";
   else
      clog << "TestCopyAssignmentOperator FAILED, expected " << DOUBLE_RES << ", saw " << dQOther.front();
   clog << "\n";

}

void
MH::UnitTests::TestPush()
{
   const string RESULT_STR = "UCSD";
   
   Queue<string> sQ;
   sQ.push(RESULT_STR);

   // Verify result
   if (sQ.front() == RESULT_STR)
      clog << "TestPush PASSED";
   else
      clog << "TestPush FAILED, expected " << RESULT_STR << ", saw " << sQ.front();
   clog << "\n";
}

void
MH::UnitTests::TestPop()
{
   //const string RESULT_STR = "UCSD";
   const int RESULT_INT = 7;

   Queue<int> sQ;
   sQ.push(2);
   sQ.push(RESULT_INT);
   sQ.push(10);
   sQ.pop();

   // Verify result
   if (sQ.front() == RESULT_INT)
      clog << "TestPop PASSED";
   else
      clog << "TestPop FAILED, expected " << RESULT_INT << ", saw " << sQ.front();
   clog << "\n";
}

void
MH::UnitTests::TestFront()
{
}

void
MH::UnitTests::TestFrontConst()
{
}
void
MH::UnitTests::TestEmpty()
{
   Queue<int> iQ;
   
   // Verify result
   if (iQ.empty())
      clog << "TestEmpty PASSED";
   else
      clog << "TestEmpty FAILED, expected " << true << ", saw " << false;
   clog << "\n";
}

void
MH::UnitTests::TestSize()
{
   const int ELEMS = 10;
   const double D_VAL = 2.2;

   Queue<double> dQ;
   dQ.push(D_VAL);
   dQ.push(D_VAL);   

   // Verify result
   if (dQ.size() == ELEMS)
      clog << "TestSize PASSED";
   else
      clog << "TestSize FAILED, expected " << ELEMS << ", saw " << dQ.size();
   clog << "\n";
}
