#include "mypch.h"
#include "entity.h"

class Scene
{
public:
	std::vector< std::shared_ptr<Entity> > m_entities;

	std::shared_ptr<Entity>  GetEntity(std::string name);
	std::shared_ptr<Entity>  GetEntity(long long uuid);
	std::shared_ptr<Entity>  GetEntity(std::string name, long long uuid);

	void AddEntity();
	void AddEntity(std::string name);
	void AddEntity(long long uuid);
	void AddEntity(std::string name, long long uuid);


	std::shared_ptr<Entity> AddEntityR();
	std::shared_ptr<Entity> AddEntityR(std::string name);
	std::shared_ptr<Entity> AddEntityR(long long uuid);
	std::shared_ptr<Entity> AddEntityR(std::string name, long long uuid);


	void RemoveEntity(std::string name);
	void RemoveEntity(long long uuid);
	void RemoveEntity(std::string name, long long uuid);

	void Clear();
};