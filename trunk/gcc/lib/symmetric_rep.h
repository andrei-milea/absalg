#ifndef SYMMETRIC_REP_
#define SYMMETRIC_REP_

#include <list>
#include <algorithm>
#include <iostream>
#include <boost/random.hpp>
#include <ctime>
#include <initializer_list>
#include <cassert>
#include "std_ex.h"


/*!
  symmetric group internal representation class
  used only from cGroup
*/
template <typename T>
class cSymmetricRep
{
public:
	typedef cSymmetricRep<T> SelfType;

public:

	//constructors
	cSymmetricRep()
	{};

	cSymmetricRep(std::vector<T> &generators_set)
		:m_GenSet(generators_set)
	{
	};

	cSymmetricRep(std::initializer_list<T> perm_list)
	{
		for(auto iter = perm_list.begin(); iter < perm_list.end()-1; iter++)
		{
			m_GenSet.push_back(*iter);
		}
	};

	//copy constructor and assign operator
	cSymmetricRep(const SelfType &sym_rep)		
	{
		m_GenSet = sym_rep.GetGeneratorsSet();
	};
	cSymmetricRep& operator=(const SelfType& sym_rep)
	{
		m_GenSet = sym_rep.GetGeneratorsSet();
		return *this;
	};
	
	~cSymmetricRep()
	{};

	friend std::ostream& operator<<(std::ostream& out,const SelfType &group_rep)
	{
		out<<"< \n";
		for(auto iter = group_rep.m_GenSet.begin(); iter != group_rep.m_GenSet.end(); iter++)
		{
			out<<*iter<<"\n";
		}
		out<<" \n>";
		return out;
	};

	bool Contains(const T &element)const
	{
		std::vector<T> elements = GetElementsDimino();
		return (std::find(elements.begin(), elements.end(), element) != elements.end());
	};

	std::vector<T> GetElementsNaive()const
	{
		std::vector<T> elements;
		elements.push_back(GetIdentity());
		for( std::size_t index = 0; index < elements.size(); index++)
		{
			for(auto set_iter = m_GenSet.begin(); set_iter != m_GenSet.end(); set_iter++)
			{
				T element = (*set_iter) * elements[index];
				if(find(elements.begin(), elements.end(), element) == elements.end())
				{
					elements.push_back(element);
				}
			}

		}
		return elements;
	};

	std::vector<T> GetElementsDimino()const
	{
		assert(!m_GenSet.empty());
		if(m_GenSet.empty())
			throw;
		if(m_GenSet.size() == 1)
			return GetCyclicSubgroup(*m_GenSet.begin());

		//generate cyclic group of the first generator	
		std::vector<T> elements = GetCyclicSubgroup(*m_GenSet.begin());
	
		//inductive step
		for(auto it = (m_GenSet.begin() + 1); it != m_GenSet.end(); it++)
		{
			if(find(elements.begin(),elements.end(),*it) == elements.end())
			{
				std::size_t prev_order = elements.size() - 1;
				elements.push_back(*it);
				for(std::size_t index = 1; index <= prev_order; index++)
				{
					if(find(elements.begin(), elements.end(), elements[index] *
								(*it)) != elements.end())
						continue;
					elements.push_back(elements[index] * (*it));
				}
				std::size_t rep_pos = prev_order + 1;	//cosset rep position
				while(true)
				{
					for(auto it1 = m_GenSet.begin(); it1 != m_GenSet.end(); it1++)
					{
						T element = elements[rep_pos] * (*it1);
						if(find(elements.begin(), elements.end(), element) == elements.end())
						{
							elements.push_back(element);
							for(std::size_t index = 1; index <= prev_order; index++)
							{
								if(find(elements.begin(), elements.end(), elements[index] *
											element) != elements.end())
									continue;
								elements.push_back(elements[index] * element);
							}
						}
					}
					rep_pos += prev_order;
					if( (prev_order == 0) || (rep_pos > elements.size() - 1) )
						break;
				}
			}
		}
		return elements;
	};

	std::vector<std::size_t> GetOrbit(const std::size_t &set_element)const
	{
		std::vector<std::size_t> orbit;
		orbit.push_back(set_element);
		std::vector<T> elements = GetElementsDimino();

		for(std::size_t index = 0; index < orbit.size(); index++)
		{
			for(auto it1 = elements.begin(); it1 != elements.end(); it1++)
			{
				std::size_t image = it1->GetImage(orbit[index]);
				if(find(orbit.begin(), orbit.end(), image) == orbit.end())
				{
					orbit.push_back(image);
				}
			}
		}
		return orbit;
	};

	//getter, setter
	const std::vector<T> &GetGeneratorsSet()const
	{
		return m_GenSet;
	};
	
	void SetGeneratorsSet(const std::vector<T> &gen_set)
	{
		m_GenSet = gen_set;
	};

	void AddGenerator(const T &element)
	{
		m_GenSet.push_back(element);
	};

	void ClearGenerators()
	{
		m_GenSet.clear();
	};

	bool operator==(const SelfType &symgrp)const
	{
		return m_GenSet == symgrp.GetGeneratorsSet();
	};

	bool operator!=(const SelfType &symgrp)const
	{
		return !(*this == symgrp);
	};

	T GetIdentity()const
	{
		assert(m_GenSet.size() != 0);
		return m_GenSet.begin()->GetIdentity();
	};

	std::vector<T> GetCyclicSubgroup(const T& element)const
	{
		T temp_el = element;
		std::vector<T> cyclic_group;
	    cyclic_group.push_back(GetIdentity());
		while(temp_el != GetIdentity())
		{
			cyclic_group.push_back(temp_el);
			temp_el = temp_el * element;
		}
		return cyclic_group;
	};

private:
	std::vector<T> m_GenSet;
};

#endif
