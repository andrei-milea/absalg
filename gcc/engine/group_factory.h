#ifndef _GROUP_FACTORY
#define _GROUP_FACTORY

#include <pair>
#include <algorithm>

typedef cGroup<cGroupElem<cPermElem, Multiplication>, cSymmetricRep> SymmGrp;
typedef std::vector< cGroupElem<cPermElem, Multiplication> >  SymmGrpGen;

//singleton class used to facilitate creation of groups
//contains a pool of groups for optimization
class cGroupFactory
{
public://methods

	static cGroupFact* GetInstance()
	{
		if(NULL == s_Instance)
			s_Instance = new cGroupFact;
		return s_Instance;
	};


	SymmetricGrp* GetSymmGrp(SymmGrpGen &generators)
	{
		for(m_SymmGrps::iterator it = m_SymmGrps.begin(); it != m_SymmGrps.end(); it++)
		{
			if(false == it->second)
			{
				it->first.SetGeneratorsSet(generators);
				return it->first;
			}
		}

		//if no free group is found create new one and add it to the vector
		SymmGrp symm_grp = new SymmGrp(generators);
		std::pair<SymmGrp*, bool> *grp_entry = new std::pair<SymmGrp*, bool>();
		grp_entry->first = symm_grp;
		grp_entry->second = m_SymmGrps.size() + 1;
		m_SymmGrps.push_back(grp_entry);
	};


	void ReleaseSymmGrp(SymmGrp *group)
	{
		for(m_SymmGrps::iterator it = m_SymmGrps.begin(); it != m_SymmGrps.end(); it++)
		{
			if(group == it->first)
			{
				it->first->ClearGenerators();
				it->second = false;
			}
		}
	};

	void CleanUp()
	{
		m_SymmGrps.clear();
	};


private://methods
	cGroupFactory()
	{};

private://members
	std::vector< std::pair<SymmGrp*, bool> *> m_SymmGrps;

private://static member
	static cGroupFactory* s_Instance;
};


cGroupFactory::s_Instance = NULL;


#endif


