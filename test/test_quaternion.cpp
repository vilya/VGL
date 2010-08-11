#include "vgl_quaternion.h"

#include "vgl_utils.h"
#include "vgl_vec3.h"

#include <cppunit/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cmath>


//
// HELPER METHODS
//

template <typename Num>
bool operator == (const vgl::Vec3<Num>& a, const vgl::Vec3<Num>& b)
{
  return vgl::equal(a.x, b.x) && vgl::equal(a.y, b.y) && vgl::equal(a.z, b.z);
}


template <typename Num>
bool operator == (const vgl::Quaternion<Num>& a, const vgl::Quaternion<Num>& b)
{
  return (a.v == b.v) && vgl::equal(a.s, b.s);
}


//
// TESTS
//

class TestQuaternion : public CPPUNIT_NS::TestCase
{
  CPPUNIT_TEST_SUITE(TestQuaternion);
  CPPUNIT_TEST(testAddZero);
  CPPUNIT_TEST(testAdd);
  CPPUNIT_TEST(testMultiply);
  CPPUNIT_TEST(testLengthSqr);
  CPPUNIT_TEST(testConjugate);
  CPPUNIT_TEST(testInverse);
  CPPUNIT_TEST(testRotate);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp() {}
  void tearDown() {} 

protected:
  void testAddZero() {
    vgl::Quaternionf zero;
    vgl::Quaternionf a(1, 2, 3, 4);

    vgl::Quaternionf r = a + zero;
    CPPUNIT_ASSERT(r == a);
  }


  void testAdd() {
    vgl::Quaternionf a(1, 2, 3, 4);
    vgl::Quaternionf b(5, 6, 7, 8);

    vgl::Quaternionf r = a + b;
    CPPUNIT_ASSERT(r.v.x == a.v.x + b.v.x);
    CPPUNIT_ASSERT(r.v.y == a.v.y + b.v.y);
    CPPUNIT_ASSERT(r.v.z == a.v.z + b.v.z);
    CPPUNIT_ASSERT(r.s == a.s + b.s);
  }


  void testMultiply() {
    vgl::Quaternionf I(1, 0, 0, 0);
    vgl::Quaternionf J(0, 1, 0, 0);
    vgl::Quaternionf K(0, 0, 1, 0);
    vgl::Quaternionf W(0, 0, 0, 1);

    CPPUNIT_ASSERT( (I * I).s == -1 );
    CPPUNIT_ASSERT( (J * J).s == -1 );
    CPPUNIT_ASSERT( (K * K).s == -1 );
    CPPUNIT_ASSERT( (W * W).s == 1 );

    CPPUNIT_ASSERT( (I * J) == (-J * I) );
    CPPUNIT_ASSERT( (I * J) == K );

    CPPUNIT_ASSERT( (J * K) == (-K * J) );
    CPPUNIT_ASSERT( (J * K) == I );

    CPPUNIT_ASSERT( (K * I) == (-I * K) );
    CPPUNIT_ASSERT( (K * I) == J );
  }


  void testLengthSqr() {
    vgl::Quaternionf a(1, 2, 3, 4);
    vgl::Quaternionf b(5, 6, 7, 8);

    CPPUNIT_ASSERT( lengthSqr(a) == vgl::sqr(a.v.x) + vgl::sqr(a.v.y) + vgl::sqr(a.v.z) + vgl::sqr(a.s) );
    CPPUNIT_ASSERT( lengthSqr(a) == lengthSqr(conjugate(a)) );
    CPPUNIT_ASSERT( lengthSqr(a * b) == lengthSqr(a) * lengthSqr(b) );
  }

  
  void testConjugate() {
    vgl::Quaternionf a(1, 2, 3, 4);
    vgl::Quaternionf b(5, 6, 7, 8);

    CPPUNIT_ASSERT( conjugate(a) == vgl::Quaternionf(-a.v.x, -a.v.y, -a.v.z, a.s) );
    CPPUNIT_ASSERT( conjugate(conjugate(a)) == a );
    CPPUNIT_ASSERT( conjugate(a * b) == conjugate(b) * conjugate(a) );
  }


  void testInverse() {
    vgl::Quaternionf a(1, 2, 3, 4);

    vgl::Quaternionf invA = inverse(a);
    vgl::Quaternionf invInvA = inverse(inverse(a));

    CPPUNIT_ASSERT( inverse(inverse(a)) == a );
    CPPUNIT_ASSERT( a * inverse(a) == inverse(a) * a );
    CPPUNIT_ASSERT( a * inverse(a) == vgl::Quaternionf(0, 0, 0, 1) );
  }


  void testRotate() {
    vgl::Quaternionf xAxis = vgl::rotation(vgl::Vec3f(1, 0, 0), (float)M_PI_2); // a rotation of 90 degrees around the x axis
    vgl::Quaternionf yAxis = vgl::rotation(vgl::Vec3f(0, 1, 0), (float)M_PI_2); // a rotation of 90 degrees around the y axis
    vgl::Quaternionf zAxis = vgl::rotation(vgl::Vec3f(0, 0, 1), (float)M_PI_2); // a rotation of 90 degrees around the z axis

    vgl::Vec3f xPoint(1, 0, 0);
    vgl::Vec3f yPoint(0, 1, 0);
    vgl::Vec3f zPoint(0, 0, 1);

    vgl::Vec3f xRot = rotate(xAxis, xPoint);
    vgl::Vec3f yRot = rotate(yAxis, yPoint);
    vgl::Vec3f zRot = rotate(zAxis, zPoint);

    CPPUNIT_ASSERT( xRot == xPoint );
    CPPUNIT_ASSERT( yRot == yPoint );
    CPPUNIT_ASSERT( zRot == zPoint );
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestQuaternion);


int main( int ac, char **av )
{
  //--- Create the event manager and test controller
  CPPUNIT_NS::TestResult controller;

  //--- Add a listener that colllects test result
  CPPUNIT_NS::TestResultCollector result;
  controller.addListener( &result );        

  //--- Add a listener that print dots as test run.
  CPPUNIT_NS::BriefTestProgressListener progress;
  controller.addListener( &progress );      

  //--- Add the top suite to the test runner
  CPPUNIT_NS::TestRunner runner;
  runner.addTest( CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest() );
  runner.run( controller );

  return result.wasSuccessful() ? 0 : 1;
}

