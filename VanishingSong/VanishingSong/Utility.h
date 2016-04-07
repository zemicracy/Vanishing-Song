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
				delete object;
				object = nullptr;
			}
		}
	}
	vector.clear();
}

/*
	��ԗp�֐�
	�������F�������
	�������F�ő�̏��
	��O�����F�S�t���[����
	��l�����F���̃t���[����
*/
template<class type>
type Split(type first, type last, const int allFrameCount, const int nowCount){
	type output;
	float allFrame = static_cast<float>(allFrameCount);
	float nowFrame = static_cast<float>(nowCount);
	float coefficient = (float)(nowFrame / allFrame);
	output = (type)(first + (last - first)*coefficient);
	return output;
}



#endif