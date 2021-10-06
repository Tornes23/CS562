#pragma once
#include <string>

class Resource
{
public:
	Resource() {};
	virtual ~Resource() {};
	void SetName(std::string name);
	std::string GetName() const;

protected:

	std::string mName{};
};


template<typename T>
class TResource : public Resource
{
public:

	//returns actual texture etc.
	T* Get() { return mActualResource; }
	void SetResource(T* _resource) { mActualResource = _resource; }
	void Release() { if (mActualResource) delete mActualResource; }
	~TResource() { Release(); }

private:
	T* mActualResource = nullptr;
};
