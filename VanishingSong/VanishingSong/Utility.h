#ifndef _UTILITY_H
#define _UTILITY_H

#include<vector>

template<typename Type>
static void ReleaseVector(std::vector<Type>& vector)
{
	if (!vector.empty())
	{
		
		for (auto object : vector)
		{
			if (object)
			{
				object.reset();
				object = nullptr;
			}
		}
	}
	vector.clear();
}


#endif