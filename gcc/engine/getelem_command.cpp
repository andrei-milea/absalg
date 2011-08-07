
#include "getelem_command.h"
#include "group_factory.h"
#include "estimate.h"
#include "serializer.h"


cGetElemCommand::cGetElemCommand(const std::string &params, cResult* result)
	:cGroupGenCommand(params, result)
{
};

cGetElemCommand::~cGetElemCommand()
{
};

void cGetElemCommand::Execute()
{
	if(SYMMETRIC_GROUP == GetGroupType())
	{
		SymmGrp symmetric_group;
		for(unsigned int i = 0; i < m_Generators.size(); i++)
		{
			SymmGrpElem generator = boost::any_cast<SymmGrpElem>(m_Generators[i]);
			symmetric_group.AddGenerator(generator);
		}

		std::vector<SymmGrpElem> group_elements = symmetric_group.GetElementsDimino();
		m_Result->SetResult(group_elements);
	}
};

unsigned int cGetElemCommand::EstimateRunTime(const cEstimator &estimator)const
{
	return estimator.Estimate(*this);
};


