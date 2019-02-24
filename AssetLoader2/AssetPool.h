#pragma once
#include <map>
#include <vector>
#include <string>
#include <functional>

using namespace std;
template<class T>
struct AssetFile
{
	string filename;
	T* acutalFile;

	bool operator== (const AssetFile<T>& rhs) 
	{ 
		return (hash<std::string>()(filename) == hash<std::string>()(rhs.filename));
	};
};

template <class T>
class AssetPool
{
public:
	int AddFileToPool(const AssetFile<T>& file)
	{
		if (DoesFileExistInPool(file))
		{
			return -1;
		}

		////m_mPool.insert(pair<unsigned char, int>(0, hash<std::string>()(file.filename)));

		return 1;
	}


private:
	bool DoesFileExistInPool(const AssetFile<T>& file)
	{
		bool isFileInPool = false;

		for (auto const& x : m_mPool)
		{
			if (x.second == file)
			{
				isFileInPool = true;
				break;
			}
		}

		return isFileInPool;
	}

	map<unsigned char, AssetFile<T>> m_mPool;
};