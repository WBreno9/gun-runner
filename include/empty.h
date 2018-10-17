#ifndef SHiP_H
#define SHiP_H

#include <entity.h>

class Empty : public Entity
{
public:
	Empty(); 
	~Empty();

	std::string GetTypeString() { return "Empty"; }

	void Update();
private:
};

#endif
