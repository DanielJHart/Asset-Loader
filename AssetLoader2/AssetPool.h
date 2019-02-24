#pragma once
#include <tuple>
#include <map>
#include <vector>
#include <string>
#include <functional>

using namespace std;
template<typename T>
class AssetFile
{
public:
	void SetFile(T* data, const string& fileName)
	{
		m_sFileName = fileName;
		m_tFileData = data;
		m_iHashsedFileName = hash<std::string>()(fileName);
	}

	int GetHashedFileName() const { return m_iHashsedFileName; } 
	string GetFileName() const { return m_sFileName; }
	T* GetFileData() const { return m_tFileData; }

private:
	string m_sFileName = "";
	int m_iHashsedFileName = 0;
	T* m_tFileData = nullptr;
};

template <typename T>
class AssetPool
{
public:
	int AddFileToPool(const AssetFile<T>& file)
	{
		if (DoesFileExistInPool(file))
		{
			return -1;
		}

		InsertFileIntoPool(file);

		return 1;
	}

private:
	bool DoesFileExistInPool(const AssetFile<T>& file)
	{
		for (auto const& x : m_mPool)
		{
			if (x.second.GetHashedFileName() == file.GetHashedFileName())
			{
				return true;
			}
		}

		return false;
	}

	void InsertFileIntoPool(const AssetFile<T>& file)
	{
		m_mPool.insert(pair<unsigned char, AssetFile<T>>(idTracker, file));
		++idTracker;
	}

	int idTracker = 0;
	map<unsigned char, AssetFile<T>> m_mPool;
};