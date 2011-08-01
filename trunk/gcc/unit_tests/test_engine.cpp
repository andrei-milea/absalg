
#include "../engine/session.h"
#include "../engine/getelem_command.h"
#include "../engine/getsubgrp_command.h"
#include "../engine/serializer.h"

#define BOOST_TEST_MODULE "test_engine"
#include "boost/test/included/unit_test.hpp"

using namespace boost::unit_test;

typedef cGroupElem< cPermElem, Multiplication> SymmetricGrpGen;

class cSessionFix
{
public:
	cSessionFix()
	{
		
	};
	~cSessionFix()
	{

	};

protected:
	cSession m_Session;
};

BOOST_FIXTURE_TEST_SUITE(test_session, cSessionFix)////////////////////////////////////////

BOOST_AUTO_TEST_CASE(ExecuteGetElemTest)
{
	std::string result;
	std::string command_txt("SYMMETRIC_GROUP {1,2,3} {1,3,2} {3,2,1}");
	cGetElemCommand command(command_txt, &result);
	//m_Session.RunCommand(&command);
};

BOOST_AUTO_TEST_SUITE_END()////////////////////////////////////////////////////


BOOST_AUTO_TEST_SUITE(test_infrastructure)/////////////////////////////////////

BOOST_AUTO_TEST_CASE(test_thread_pool_basic)
{
	cThreadPool	threadPool(20);
	BOOST_ASSERT(false == threadPool.isStarted());
	threadPool.StartPool();
	BOOST_ASSERT(true == threadPool.isStarted());
	threadPool.StopPool();
	BOOST_ASSERT(false == threadPool.isStarted());
}

BOOST_AUTO_TEST_CASE(test_command_queue)
{
	std::string result;
	cCommandQueue commad_queue;
	BOOST_ASSERT(commad_queue.Empty());
	for(unsigned int i = 1; i<= 100; i++)
	{
		cGetElemCommand *getelem_command = new cGetElemCommand("SYMMETRIC_GROUP {1,2,3} {1,3,2} {3,2,1}", &result);
		commad_queue.Put(getelem_command);
		BOOST_ASSERT(commad_queue.GetSize() == i);
	}
	BOOST_ASSERT(!commad_queue.Empty());

	for(unsigned int i = 1; i<= 100; i++)
	{
		commad_queue.Remove();
		BOOST_ASSERT(commad_queue.GetSize() == 100 - i);
	}
	BOOST_ASSERT(commad_queue.Empty());
}

BOOST_AUTO_TEST_CASE(test_serializer)
{
	cSerializer<SymmGrpElem> symm_grp_serializer;

	//basic test with s3
	SymmGrpElem elem1({1,2,3});
	SymmGrpElem elem2({1,3,2});
	SymmGrpElem elem3({3,2,1});
	std::string generators_str1;
	generators_str1 += symm_grp_serializer.Stringify(elem1);
	generators_str1 += symm_grp_serializer.Stringify(elem2);
	generators_str1 += symm_grp_serializer.Stringify(elem3);
	std::vector< SymmGrpElem >  generators;
	generators.push_back(elem1);
	generators.push_back(elem2);
	generators.push_back(elem3);
	std::string generators_str = symm_grp_serializer.Stringify(generators);

	BOOST_CHECK(generators_str == generators_str1);
}

BOOST_AUTO_TEST_SUITE_END()////////////////////////////////////////////////////


BOOST_AUTO_TEST_SUITE(test_command)////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(test_command_getelem)
{
	std::string result;
	cGetElemCommand command( "SYMMETRIC_GROUP {1,2,3} {1,3,2} {3,2,1}", &result);
//	cPermElem s1({1,2,3});
//	cPermElem s2({1,3,2});
//	cPermElem s3({3,2,1});
//
//	cGroupElem< cPermElem, Multiplication> elem1(s1);
//	cGroupElem< cPermElem, Multiplication> elem2(s2);
//	cGroupElem< cPermElem, Multiplication> elem3(s3);
//	std::vector< cGroupElem<cPermElem, Multiplication> >  generators;
//	generators1.push_back(elem1);
//	generators1.push_back(elem2);
//	generators1.push_back(elem3);
//	S3 group_s3(generators1);
//	std::vector< cGroupElem<cPermElem, Multiplication> > elements = group_s3.GetElementsNaive();
//	TODO
}

BOOST_AUTO_TEST_CASE(test_command_getsubgroup)
{
	std::string result;
	cGetSubgrpCommand command("SYMMETRIC_GROUP {1,2,3} {1,3,2} {3,2,1}", &result);
	//TODO
}

BOOST_AUTO_TEST_CASE(test_command_parsing)
{
	SymmGrpElem elem1({1,2,3});
	SymmGrpElem elem2({1,3,2});
	SymmGrpElem elem3({3,2,1});
	std::vector<SymmGrpElem>  generators;
	generators.push_back(elem1);
	generators.push_back(elem2);
	generators.push_back(elem3);

	std::string command_txt("SYMMETRIC_GROUP {1,2,3} {1,3,2} {3,2,1}");
	std::string result;
	cGetElemCommand command(command_txt, &result);
	std::vector<boost::any> parsed_generators = command.GetGenerators();

	BOOST_ASSERT(SYMMETRIC_GROUP == command.GetGroupType());
	for(unsigned int i = 0; i < parsed_generators.size(); i++)
	{
		SymmetricGrpGen generator = boost::any_cast<SymmetricGrpGen>(parsed_generators[i]);
		BOOST_ASSERT(generator == generators[i]);
	}
}

BOOST_AUTO_TEST_SUITE_END()//////////////////////////////////////////////////


