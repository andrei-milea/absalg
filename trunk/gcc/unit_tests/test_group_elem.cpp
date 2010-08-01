#include "../group.h"


#include "boost/test/included/unit_test.hpp"

using namespace boost::unit_test;


void test_trivial()
{
	//test perm element
	cPermElem<3> s1;
	cPermElem<3> s2({2,3});
	cPermElem<3> s3({1,2});
	cGroupElem< cPermElem<3>, Multiplication> elem1(s1);
	cGroupElem< cPermElem<3>, Multiplication> elem2(s2);
	cGroupElem< cPermElem<3>, Multiplication> elem3(s3);
	BOOST_CHECK(elem2 * elem2 == elem1);
	BOOST_CHECK(elem1 == elem3 * elem3);
	
	//test copy constructor and assignment operator
	cGroupElem< cPermElem<3>, Multiplication> elem4(elem1);
	cGroupElem< cPermElem<3>, Multiplication> elem5 = elem2;
	BOOST_CHECK(elem5 * elem5 == elem4);
}

void test_order_power()
{
	cPermElem<3> s1;
	cPermElem<3> s2({2,3});
	cPermElem<3> s3({1,2});
	cGroupElem< cPermElem<3>, Multiplication> elem1(s1);
	cGroupElem< cPermElem<3>, Multiplication> elem2(s2);
	cGroupElem< cPermElem<3>, Multiplication> elem3(s3);

	//test perm elem power
	BOOST_CHECK(elem1.GetNthPower(2) == elem1);
	BOOST_CHECK(elem2.GetNthPower(2) == elem1);
	BOOST_CHECK(elem2.GetNthPower(3) == elem2);
	BOOST_CHECK(elem3.GetNthPower(3) == elem3);
	BOOST_CHECK(elem1.GetNthPower(4) == elem1);

	//test order
	BOOST_CHECK(elem2.GetOrder() == 2);
	BOOST_CHECK(elem2.GetOrder() == elem3.GetOrder());
	BOOST_CHECK(elem1.GetOrder() == 1);
	BOOST_CHECK(elem2.GetOrder(cGroupElem< cPermElem<3>, Multiplication>::GroupSize)
		   	== elem2.GetOrder());


	//test inverse
	//BOOST_CHECK(elem1.GetInverse() == elem1);
	//BOOST_CHECK(elem2.GetInverse() == elem2);
	//BOOST_CHECK(elem3.GetInverse() == elem3);

	}


void test_properties()
{
	//test commutes with
	cPermElem<3> s1;
	cPermElem<3> s2({2,3});
	cPermElem<3> s3({1,2});
	cGroupElem< cPermElem<3>, Multiplication> elem1(s1);
	cGroupElem< cPermElem<3>, Multiplication> elem2(s2);
	cGroupElem< cPermElem<3>, Multiplication> elem3(s3);
	BOOST_CHECK(elem1.CommutesWith(elem1));
	BOOST_CHECK(elem1.CommutesWith(elem2));
	BOOST_CHECK(elem1.CommutesWith(elem3));
	BOOST_CHECK(!elem2.CommutesWith(elem3));
	BOOST_CHECK(!elem3.CommutesWith(elem2));

}



test_suite* init_unit_test_suite( int argc, char* argv[] ) 
{
    framework::master_test_suite().add( BOOST_TEST_CASE( &test_trivial ) );
    framework::master_test_suite().add( BOOST_TEST_CASE( &test_order_power ) );
    framework::master_test_suite().add( BOOST_TEST_CASE( &test_properties ) );

    return 0;
};