
#include "pch.h"
#include "framework.h"

#include "Scene.h"
#include "ResourceManager.h"
#include <algorithm>

Scene::Node::Node(NodeID id, type nodeType)
{
	ID = id;
	this->nodeType = nodeType;
}

Scene::Node::~Node()
{
	for (auto c : childs)
		delete c;
	childs.clear();
}

void Scene::Node::addChild(Node* n)
{
	childs.push_back(n);
	n->parent = this;
}

Scene::Scene()
{
	root = new Scene::RootNode(nextId++);
	root->scene = this;

	nodes.reserve(100);
	nodes.resize(1);
	nodes[0] = root;
}

Scene::~Scene()
{
	delete root;
}

Scene::Node* Scene::Node::findNodeByID(NodeID id)
{
	if (ID == id)
		return this;
	for (auto n : childs)
	{
		Scene::Node* node = n->findNodeByID(id);
		if (node)
			return node;
	}
	return nullptr;
}

flt4x4& Scene::Node::getPos4x4()
{
	if (!bCached)
	{
		cachedPosition4x4 = parent ? parent->getPos4x4() : flt4x4();
		cachedPosition3 = flt3(
			cachedPosition4x4.m30(),
			cachedPosition4x4.m31(),
			cachedPosition4x4.m32()
		);
		cachedPosition4 = flt4(cachedPosition3, 1);
		cachedAxisZ = flt3(
			cachedPosition4x4.m20(),
			cachedPosition4x4.m21(),
			cachedPosition4x4.m22()
		);
		cachedInverseTransposePosition = cachedPosition4x4.inverse().transpose();
		bCached = true;
	}
	return cachedPosition4x4;
}

flt3& Scene::Node::getPos3()
{
	if (!bCached)
	{
		cachedPosition4x4 = parent ? parent->getPos4x4() : flt4x4();
		cachedPosition3 = flt3(
			cachedPosition4x4.m30(),
			cachedPosition4x4.m31(),
			cachedPosition4x4.m32()
		);
		cachedPosition4 = flt4(cachedPosition3, 1);
		cachedAxisZ = flt3(
			cachedPosition4x4.m20(),
			cachedPosition4x4.m21(),
			cachedPosition4x4.m22()
		);
		cachedInverseTransposePosition = cachedPosition4x4.inverse().transpose();
		bCached = true;
	}
	return cachedPosition3;
}

flt4& Scene::Node::getPos4()
{
	if (!bCached)
	{
		cachedPosition4x4 = parent ? parent->getPos4x4() : flt4x4();
		cachedPosition3 = flt3(
			cachedPosition4x4.m30(),
			cachedPosition4x4.m31(),
			cachedPosition4x4.m32()
		);
		cachedPosition4 = flt4(cachedPosition3, 1);
		cachedAxisZ = flt3(
			cachedPosition4x4.m20(),
			cachedPosition4x4.m21(),
			cachedPosition4x4.m22()
		);
		cachedInverseTransposePosition = cachedPosition4x4.inverse().transpose();
		bCached = true;
	}
	return cachedPosition4;
}

flt3& Scene::Node::getAxisZ()
{
	if (!bCached)
	{
		cachedPosition4x4 = parent ? parent->getPos4x4() : flt4x4();
		cachedPosition3 = flt3(
			cachedPosition4x4.m30(),
			cachedPosition4x4.m31(),
			cachedPosition4x4.m32()
		);
		cachedPosition4 = flt4(cachedPosition3, 1);
		cachedAxisZ = flt3(
			cachedPosition4x4.m20(),
			cachedPosition4x4.m21(),
			cachedPosition4x4.m22()
		);
		cachedInverseTransposePosition = cachedPosition4x4.inverse().transpose();
		bCached = true;
	}
	return cachedAxisZ;
}

flt4x4& Scene::Node::getPosInvTr()
{
	if (!bCached)
	{
		cachedPosition4x4 = parent ? parent->getPos4x4() : flt4x4();
		cachedPosition3 = flt3(
			cachedPosition4x4.m30(),
			cachedPosition4x4.m31(),
			cachedPosition4x4.m32()
		);
		cachedPosition4 = flt4(cachedPosition3, 1);
		cachedAxisZ = flt3(
			cachedPosition4x4.m20(),
			cachedPosition4x4.m21(),
			cachedPosition4x4.m22()
		);
		cachedInverseTransposePosition = cachedPosition4x4.inverse().transpose();
		bCached = true;
	}
	return cachedInverseTransposePosition;
}

flt4x4& Scene::TransformNode::getPos4x4()
{
	if (!bCached)
	{
		cachedPosition4x4 = parent ? parent->getPos4x4() * pos : pos;
		cachedPosition3 = flt3(
			cachedPosition4x4.m30(),
			cachedPosition4x4.m31(),
			cachedPosition4x4.m32()
		);
		cachedPosition4 = flt4(cachedPosition3, 1);
		cachedAxisZ = flt3(
			cachedPosition4x4.m20(),
			cachedPosition4x4.m21(),
			cachedPosition4x4.m22()
		);
		cachedInverseTransposePosition = cachedPosition4x4.inverse().transpose();
		bCached = true;
	}
	return cachedPosition4x4;
}

flt3& Scene::TransformNode::getPos3()
{
	if (!bCached)
	{
		cachedPosition4x4 = parent ? parent->getPos4x4() * pos : pos;
		cachedPosition3 = flt3(
			cachedPosition4x4.m30(),
			cachedPosition4x4.m31(),
			cachedPosition4x4.m32()
		);
		cachedPosition4 = flt4(cachedPosition3, 1);
		cachedAxisZ = flt3(
			cachedPosition4x4.m20(),
			cachedPosition4x4.m21(),
			cachedPosition4x4.m22()
		);
		cachedInverseTransposePosition = cachedPosition4x4.inverse().transpose();
		bCached = true;
	}
	return cachedPosition3;
}

flt4& Scene::TransformNode::getPos4()
{
	if (!bCached)
	{
		cachedPosition4x4 = parent ? parent->getPos4x4() * pos : pos;
		cachedPosition3 = flt3(
			cachedPosition4x4.m30(),
			cachedPosition4x4.m31(),
			cachedPosition4x4.m32()
		);
		cachedPosition4 = flt4(cachedPosition3, 1);
		cachedAxisZ = flt3(
			cachedPosition4x4.m20(),
			cachedPosition4x4.m21(),
			cachedPosition4x4.m22()
		);
		cachedInverseTransposePosition = cachedPosition4x4.inverse().transpose();
		bCached = true;
	}
	return cachedPosition4;
}

flt3& Scene::TransformNode::getAxisZ()
{
	if (!bCached)
	{
		cachedPosition4x4 = parent ? parent->getPos4x4() * pos : pos;
		cachedPosition3 = flt3(
			cachedPosition4x4.m30(),
			cachedPosition4x4.m31(),
			cachedPosition4x4.m32()
		);
		cachedPosition4 = flt4(cachedPosition3, 1);
		cachedAxisZ = flt3(
			cachedPosition4x4.m20(),
			cachedPosition4x4.m21(),
			cachedPosition4x4.m22()
		);
		cachedInverseTransposePosition = cachedPosition4x4.inverse().transpose();
		bCached = true;
	}
	return cachedAxisZ;
}

flt4x4& Scene::TransformNode::getPosInvTr()
{
	if (!bCached)
	{
		cachedPosition4x4 = parent ? parent->getPos4x4() * pos : pos;
		cachedPosition3 = flt3(
			cachedPosition4x4.m30(),
			cachedPosition4x4.m31(),
			cachedPosition4x4.m32()
		);
		cachedPosition4 = flt4(cachedPosition3, 1);
		cachedAxisZ = flt3(
			cachedPosition4x4.m20(),
			cachedPosition4x4.m21(),
			cachedPosition4x4.m22()
		);
		cachedInverseTransposePosition = cachedPosition4x4.inverse().transpose();
		bCached = true;
	}
	return cachedInverseTransposePosition;
}

void Scene::TransformNode::accept(Visitor* visitor) const
{
	visitor->startVisit(this);

	for (auto& child : childs)
		child->accept(visitor);

	visitor->finishVisit(this);
}

void Scene::MeshNode::accept(Visitor* visitor) const
{
	visitor->startVisit(this);

	for (auto& child : childs)
		child->accept(visitor);

	visitor->finishVisit(this);
}

NodeID Scene::addTransformNode(const flt4x4& pos, NodeID id)
{
	Scene::Node* node = nodes[id];
	if (node == nullptr)
		return NodeID(-1);

	id = nextId;
	Node* addedNode = new Scene::TransformNode(nextId++, pos);
	addedNode->scene = this;
	node->addChild(addedNode);

	if (nodes.size() <= id)
	{
		nodes.resize(id + 1, nullptr);
		paramsLocations.resize(id + 1);
	}
	nodes[id] = addedNode;

	return id;
}

NodeID Scene::addMeshNode(Mesh* mesh, NodeID id)
{
	Scene::Node* node = nodes[id];
	if (node == nullptr)
		return NodeID(-1);

	id = nextId;
	Node* addedNode = new Scene::MeshNode(nextId++, mesh);
	addedNode->scene = this;

	addedNode->params.clear();

	string_id technique_name_id = mesh->getTechnique();

	std::map<string_id, Float4Resource>& flt4s =
		ResourceManager::instance()->getFloat4s(technique_name_id);
	for (auto& f4 : flt4s)
	{
		ParamKey pk{ f4.first,-1,string_id(-1) };
		ParamValue pv;
		addedNode->params.push_back({ pk,pv });
	}

	std::map<string_id, Float3Resource>& flt3s =
		ResourceManager::instance()->getFloat3s(technique_name_id);
	for (auto& f3 : flt3s)
	{
		ParamKey pk{ f3.first,-1,string_id(-1) };
		ParamValue pv;
		addedNode->params.push_back({ pk,pv });
	}

	std::map<string_id, Float2Resource>& flt2s =
		ResourceManager::instance()->getFloat2s(technique_name_id);
	for (auto& f2 : flt2s)
	{
		ParamKey pk{ f2.first,-1,string_id(-1) };
		ParamValue pv;
		addedNode->params.push_back({ pk,pv });
	}

	std::map<string_id, Float1Resource>& flt1s =
		ResourceManager::instance()->getFloat1s(technique_name_id);
	for (auto& f1 : flt1s)
	{
		ParamKey pk{ f1.first,-1,string_id(-1) };
		ParamValue pv;
		addedNode->params.push_back({ pk,pv });
	}

	std::map<string_id, Float4x4Resource>& flt4x4s =
		ResourceManager::instance()->getFloat4x4s(technique_name_id);
	for (auto& f4x4 : flt4x4s)
	{
		ParamKey pk{ f4x4.first,-1,string_id(-1) };
		ParamValue pv;
		addedNode->params.push_back({ pk,pv });
	}

	std::map<string_id, StructResource>& structs =
		ResourceManager::instance()->getStructures(technique_name_id);
	for (auto& s : structs)
	{
		string_id var = s.first;
		const StructResource& sr = s.second;
		for (int e = 0; e < sr.elementsCount; ++e)
		{
			for (int f = 0; f < sr.fieldsCount; ++f)
			{
				auto& fr = sr.fields[f];

				ParamKey pk{ var,e,fr.name };
				ParamValue pv;
				addedNode->params.push_back({ pk,pv });
			}
		}
	}

	std::map<string_id, Texture2dResource>& textures =
		ResourceManager::instance()->getTextures(technique_name_id);
	for (auto& t : textures)
	{
		ParamKey pk{ t.first,-1,string_id(-1) };
		ParamValue pv;
		addedNode->params.push_back({ pk,pv });
	}

	std::map<string_id, Texture2dArrayResource>& texArrs =
		ResourceManager::instance()->getTexturesArrays(technique_name_id);
	for (auto& ta : texArrs)
	{
		ParamKey pk{ ta.first,-1,string_id(-1) };
		ParamValue pv;
		addedNode->params.push_back({ pk,pv });
	}

	node->addChild(addedNode);

	mesh->scene = (void*)this;
	mesh->nodeID = (int)id;

	if (nodes.size() <= id)
	{
		nodes.resize(id + 1, nullptr);
		paramsLocations.resize(id + 1);
	}
	
	nodes[id] = addedNode;

	paramsLocations[id].location.clear();
	for (auto& p : addedNode->params)
		paramsLocations[id].location.push_back({ p.first, id });

	return id;
}

string_id* Scene::Node::getStringID(int param_index)
{
	if (!params[param_index].second.valid)
		return nullptr;
	return &params[param_index].second.s;
}

float* Scene::Node::getFloat(int param_index)
{
	if (!params[param_index].second.valid)
		return nullptr;
	return &params[param_index].second.f;
}

flt2* Scene::Node::getFlt2(int param_index)
{
	if (!params[param_index].second.valid)
		return nullptr;
	return &params[param_index].second.f2;
}

flt3* Scene::Node::getFlt3(int param_index)
{
	if (!params[param_index].second.valid)
		return nullptr;
	return &params[param_index].second.f3;
}

flt4* Scene::Node::getFlt4(int param_index)
{
	if (!params[param_index].second.valid)
		return nullptr;
	return &params[param_index].second.f4;
}

flt4x4* Scene::Node::getFlt4x4(int param_index)
{
	if (!params[param_index].second.valid)
		return nullptr;
	return &params[param_index].second.f4x4;
}

int* Scene::Node::getInt(int param_index)
{
	if (!params[param_index].second.valid)
		return nullptr;
	return &params[param_index].second.i;
}

Vec2d<int>* Scene::Node::getInt2(int param_index)
{
	if (!params[param_index].second.valid)
		return nullptr;
	return &params[param_index].second.i2;
}

bool Scene::Node::getParam(int param_index, string_id& s) const
{
	if (!params[param_index].second.valid)
		return false;
	s = params[param_index].second.s;
	return true;
}

bool Scene::Node::getParam(int param_index, float& f) const
{
	if (!params[param_index].second.valid)
		return false;
	f = params[param_index].second.f;
	return true;
}

bool Scene::Node::getParam(int param_index, flt2& f2) const
{
	if (!params[param_index].second.valid)
		return false;
	f2 = params[param_index].second.f2;
	return true;
}

bool Scene::Node::getParam(int param_index, flt3& f3) const
{
	if (!params[param_index].second.valid)
		return false;
	f3 = params[param_index].second.f3;
	return true;
}

bool Scene::Node::getParam(int param_index, flt4& f4) const
{
	if (!params[param_index].second.valid)
		return false;
	f4 = params[param_index].second.f4;
	return true;
}

bool Scene::Node::getParam(int param_index, flt4x4& f4x4) const
{
	if (!params[param_index].second.valid)
		return false;
	f4x4 = params[param_index].second.f4x4;
	return true;
}

bool Scene::Node::getParam(int param_index, int& i) const
{
	if (!params[param_index].second.valid)
		return false;
	i = params[param_index].second.i;
	return true;
}

bool Scene::Node::getParam(int param_index, Vec2d<int>& i2) const
{
	if (!params[param_index].second.valid)
		return false;
	i2 = params[param_index].second.i2;
	return true;
}

bool Scene::Node::getParam(const ParamKey& paramName, string_id& s) const
{
	if (scene)
		return scene->getNodeParam(ID, paramName, s);

	auto it = std::find_if(params.begin(), params.end(), [&paramName](auto& p)
	{
		return p.first == paramName;
	}
	);
	if (it == params.end())
		return false;

	s = it->second.s;
	
	return it->second.valid;
}

bool Scene::Node::getParam(const ParamKey& paramName, float& f) const
{
	if (scene)
		return scene->getNodeParam(ID, paramName, f);

	auto it = std::find_if(params.begin(), params.end(), [&paramName](auto& p)
	{
		return p.first == paramName;
	}
	);
	if (it == params.end())
		return false;

	f = it->second.f;

	return it->second.valid;
}

bool Scene::Node::getParam(const ParamKey& paramName, flt2& f2) const
{
	if (scene)
		return scene->getNodeParam(ID, paramName, f2);

	auto it = std::find_if(params.begin(), params.end(), [&paramName](auto& p)
	{
		return p.first == paramName;
	}
	);
	if (it == params.end())
		return false;

	f2 = it->second.f2;

	return it->second.valid;
}

bool Scene::Node::getParam(const ParamKey& paramName, flt3& f3) const
{
	if (scene)
		return scene->getNodeParam(ID, paramName, f3);

	auto it = std::find_if(params.begin(), params.end(), [&paramName](auto& p)
	{
		return p.first == paramName;
	}
	);
	if (it == params.end())
		return false;

	f3 = it->second.f3;

	return it->second.valid;
}

bool Scene::Node::getParam(const ParamKey& paramName, flt4& f4) const
{
	if (scene)
		return scene->getNodeParam(ID, paramName, f4);

	auto it = std::find_if(params.begin(), params.end(), [&paramName](auto& p)
	{
		return p.first == paramName;
	}
	);
	if (it == params.end())
		return false;

	f4 = it->second.f4;

	return it->second.valid;
}

bool Scene::Node::getParam(const ParamKey& paramName, flt4x4& f4x4) const
{
	if (scene)
		return scene->getNodeParam(ID, paramName, f4x4);

	auto it = std::find_if(params.begin(), params.end(), [&paramName](auto& p)
	{
		return p.first == paramName;
	}
	);
	if (it == params.end())
		return false;

	f4x4 = it->second.f4x4;

	return it->second.valid;
}

bool Scene::Node::getParam(const ParamKey& paramName, int& i) const
{
	if (scene)
		return scene->getNodeParam(ID, paramName, i);

	auto it = std::find_if(params.begin(), params.end(), [&paramName](auto& p)
	{
		return p.first == paramName;
	}
	);
	if (it == params.end())
		return false;

	i = it->second.i;

	return it->second.valid;
}

bool Scene::Node::getParam(const ParamKey& paramName, Vec2d<int>& i2) const
{
	if (scene)
		return scene->getNodeParam(ID, paramName, i2);

	auto it = std::find_if(params.begin(), params.end(), [&paramName](auto& p)
	{
		return p.first == paramName;
	}
	);
	if (it == params.end())
		return false;

	i2 = it->second.i2;

	return it->second.valid;
}

void Scene::Node::setParam(const ParamKey& paramName, const string_id& s)
{
	auto it = std::find_if(params.begin(), params.end(), [&paramName](auto& p)
	{
		return p.first == paramName;
	}
	);
	if (it == params.end())
	{
		params.push_back({ paramName,ParamValue(s) });
		return;
	}

	it->second.s = s;
	it->second.valid = true;
}

void Scene::Node::setParam(const ParamKey& paramName, const float& f)
{
	auto it = std::find_if(params.begin(), params.end(), [&paramName](auto& p)
	{
		return p.first == paramName;
	}
	);
	if (it == params.end())
	{
		params.push_back({ paramName,ParamValue(f) });
		return;
	}

	it->second.f = f;
	it->second.valid = true;
}

void Scene::Node::setParam(const ParamKey& paramName, const flt2& f2)
{
	auto it = std::find_if(params.begin(), params.end(), [&paramName](auto& p)
	{
		return p.first == paramName;
	}
	);
	if (it == params.end())
	{
		params.push_back({ paramName,ParamValue(f2) });
		return;
	}

	it->second.f2 = f2;
	it->second.valid = true;
}

void Scene::Node::setParam(const ParamKey& paramName, const flt3& f3)
{
	auto it = std::find_if(params.begin(), params.end(), [&paramName](auto& p)
	{
		return p.first == paramName;
	}
	);
	if (it == params.end())
	{
		params.push_back({ paramName,ParamValue(f3) });
		return;
	}

	it->second.f3 = f3;
	it->second.valid = true;
}

void Scene::Node::setParam(const ParamKey& paramName, const flt4& f4)
{
	auto it = std::find_if(params.begin(), params.end(), [&paramName](auto& p)
	{
		return p.first == paramName;
	}
	);
	if (it == params.end())
	{
		params.push_back({ paramName,ParamValue(f4) });
		return;
	}

	it->second.f4 = f4;
	it->second.valid = true;
}

void Scene::Node::setParam(const ParamKey& paramName, const flt4x4& f4x4)
{
	auto it = std::find_if(params.begin(), params.end(), [&paramName](auto& p)
	{
		return p.first == paramName;
	}
	);
	if (it == params.end())
	{
		params.push_back({ paramName,ParamValue(f4x4) });
		return;
	}

	it->second.f4x4 = f4x4;
	it->second.valid = true;
}

void Scene::Node::setParam(const ParamKey& paramName, const int& i)
{
	auto it = std::find_if(params.begin(), params.end(), [&paramName](auto& p)
	{
		return p.first == paramName;
	}
	);
	if (it == params.end())
	{
		params.push_back({ paramName,ParamValue(i) });
		return;
	}

	it->second.i = i;
	it->second.valid = true;
}

void Scene::Node::setParam(const ParamKey& paramName, const Vec2d<int>& i2)
{
	auto it = std::find_if(params.begin(), params.end(), [&paramName](auto& p)
	{
		return p.first == paramName;
	}
	);
	if (it == params.end())
	{
		params.push_back({ paramName,ParamValue(i2) });
		return;
	}

	it->second.i2 = i2;
	it->second.valid = true;
}

void Scene::Node::accept(Visitor* visitor) const
{
	visitor->startVisit(this);

	for (auto& child : childs)
		child->accept(visitor);

	visitor->finishVisit(this);
}

void Scene::RootNode::accept(Visitor* visitor) const
{
	visitor->startVisit(this);

	for (auto& child : childs)
		child->accept(visitor);

	visitor->finishVisit(this);
}

string_id* Scene::getStringID(NodeID id, int param_index)
{
	id = paramsLocations[id].location[param_index].second;

	Node* node = nodes[id];
	if (!node->params[param_index].second.valid)
		return nullptr;

	return &node->params[param_index].second.s;
}

float* Scene::getFloat(NodeID id, int param_index)
{
	id = paramsLocations[id].location[param_index].second;

	Node* node = nodes[id];
	if (!node->params[param_index].second.valid)
		return nullptr;

	return &node->params[param_index].second.f;
}

flt2* Scene::getFlt2(NodeID id, int param_index)
{
	id = paramsLocations[id].location[param_index].second;

	Node* node = nodes[id];
	if (!node->params[param_index].second.valid)
		return nullptr;

	return &node->params[param_index].second.f2;
}

flt3* Scene::getFlt3(NodeID id, int param_index)
{
	id = paramsLocations[id].location[param_index].second;

	Node* node = nodes[id];
	if (!node->params[param_index].second.valid)
		return nullptr;

	string_id s = node->params[param_index].second.s;

	if (s == node_position_id)
		return &node->getPos3();
	if (s == node_axis_z_id)
		return &node->getAxisZ();
	return &node->params[param_index].second.f3;
}

flt4* Scene::getFlt4(NodeID id, int param_index)
{
	id = paramsLocations[id].location[param_index].second;

	Node* node = nodes[id];
	if (!node->params[param_index].second.valid)
		return nullptr;

	return &node->params[param_index].second.f4;
}

flt4x4* Scene::getFlt4x4(NodeID id, int param_index)
{
	id = paramsLocations[id].location[param_index].second;

	Node* node = nodes[id];
	if (!node->params[param_index].second.valid)
		return nullptr;

	return &node->params[param_index].second.f4x4;
}

int* Scene::getInt(NodeID id, int param_index)
{
	id = paramsLocations[id].location[param_index].second;

	Node* node = nodes[id];
	if (!node->params[param_index].second.valid)
		return nullptr;

	return &node->params[param_index].second.i;
}

Vec2d<int>* Scene::getInt2(NodeID id, int param_index)
{
	id = paramsLocations[id].location[param_index].second;

	Node* node = nodes[id];
	if (!node->params[param_index].second.valid)
		return nullptr;

	return &node->params[param_index].second.i2;
}

bool Scene::getNodeParam(NodeID id, int param_index, string_id& s) const
{
	id = paramsLocations[id].location[param_index].second;

	Node* node = nodes[id];
	if (!node->params[param_index].second.valid)
		return false;

	s = node->params[param_index].second.s;

	return true;
}

bool Scene::getNodeParam(NodeID id, int param_index, float& f) const
{
	id = paramsLocations[id].location[param_index].second;

	Node* node = nodes[id];
	if (!node->params[param_index].second.valid)
		return false;

	f = node->params[param_index].second.f;

	return true;
}

bool Scene::getNodeParam(NodeID id, int param_index, flt2& f2) const
{
	id = paramsLocations[id].location[param_index].second;

	Node* node = nodes[id];
	if (!node->params[param_index].second.valid)
		return false;

	f2 = node->params[param_index].second.f2;

	return true;
}

bool Scene::getNodeParam(NodeID id, int param_index, flt3& f3) const
{
	id = paramsLocations[id].location[param_index].second;

	Node* node = nodes[id];
	if (!node->params[param_index].second.valid)
		return false;

	string_id s = node->params[param_index].second.s;

	if (s == node_position_id)
	{
		f3 = node->getPos3();
		return true;
	}
	if (s == node_axis_z_id)
	{
		f3 = node->getAxisZ();
		return true;
	}

	f3 = node->params[param_index].second.f3;

	return true;
}

bool Scene::getNodeParam(NodeID id, int param_index, flt4& f4) const
{
	id = paramsLocations[id].location[param_index].second;

	Node* node = nodes[id];
	if (!node->params[param_index].second.valid)
		return false;

	f4 = node->params[param_index].second.f4;

	return true;
}

bool Scene::getNodeParam(NodeID id, int param_index, flt4x4& f4x4) const
{
	id = paramsLocations[id].location[param_index].second;

	Node* node = nodes[id];
	if (!node->params[param_index].second.valid)
		return false;

	f4x4 = node->params[param_index].second.f4x4;

	return true;
}

bool Scene::getNodeParam(NodeID id, int param_index, int& i) const
{
	id = paramsLocations[id].location[param_index].second;

	Node* node = nodes[id];
	if (!node->params[param_index].second.valid)
		return false;

	i = node->params[param_index].second.i;

	return true;
}

bool Scene::getNodeParam(NodeID id, int param_index, Vec2d<int>& i2) const
{
	id = paramsLocations[id].location[param_index].second;

	Node* node = nodes[id];
	if (!node->params[param_index].second.valid)
		return false;

	i2 = node->params[param_index].second.i2;

	return true;
}

void Scene::setNodeParam(NodeID id, const ParamKey& paramName, const string_id& s)
{
	Node* node = nodes[id];
	if (node == nullptr)
		return;
	node->setParam(paramName, s);
}

void Scene::setNodeParam(NodeID id, const ParamKey& paramName, const float& f)
{
	Node* node = nodes[id];
	if (node == nullptr)
		return;
	node->setParam(paramName, f);
}

void Scene::setNodeParam(NodeID id, const ParamKey& paramName, const flt2& f2)
{
	Node* node = nodes[id];
	if (node == nullptr)
		return;
	node->setParam(paramName, f2);
}

void Scene::setNodeParam(NodeID id, const ParamKey& paramName, const flt3& f3)
{
	Node* node = nodes[id];
	if (node == nullptr)
		return;
	node->setParam(paramName, f3);
}

void Scene::setNodeParam(NodeID id, const ParamKey& paramName, const flt4& f4)
{
	Node* node = nodes[id];
	if (node == nullptr)
		return;
	node->setParam(paramName, f4);
}

void Scene::setNodeParam(NodeID id, const ParamKey& paramName, const flt4x4& f4x4)
{
	Node* node = nodes[id];
	if (node == nullptr)
		return;
	node->setParam(paramName, f4x4);
}

void Scene::setNodeParam(NodeID id, const ParamKey& paramName, const int& i)
{
	Node* node = nodes[id];
	if (node == nullptr)
		return;
	node->setParam(paramName, i);
}

void Scene::setNodeParam(NodeID id, const ParamKey& paramName, const Vec2d<int>& i2)
{
	Node* node = nodes[id];
	if (node == nullptr)
		return;
	node->setParam(paramName, i2);
}

void Scene::createNodeParamReference(NodeID reference, NodeID referenced, const ParamKey& param)
{
	auto& l = paramsLocations[reference].location;
	auto it = std::find_if(l.begin(), l.end(), [&param](auto& p)
	{
		return p.first == param;
	}
	);
	it->second = referenced;

	bool updateReferencedNodeParams = false;
	updateReferencedNodeParams = nodes[reference]->params.size() !=
		nodes[referenced]->params.size();
	if (!updateReferencedNodeParams)
	{
		for (int i = 0; i < nodes[reference]->params.size(); ++i)
		{
			if (nodes[reference]->params[i].first ==
				nodes[referenced]->params[i].first)
			{
				continue;
			}

			updateReferencedNodeParams = true;
			break;
		}
	}

	if (!updateReferencedNodeParams)
		return;

	std::vector<std::pair<ParamKey, ParamValue>> params(nodes[reference]->params);
	for (auto& p : params)
		p.second = ParamValue();
	for (auto& param : nodes[referenced]->params)
	{
		auto it = std::find_if(params.begin(), params.end(), [&param](auto& p)
		{
			return p.first == param.first;
		});
		if (it == params.end())
			continue;
		it->second = param.second;
	}
	nodes[referenced]->params.swap(params);
}

bool Scene::getNodeParam(NodeID id, const ParamKey& paramName, string_id& s) const
{
	auto it = std::find_if(nodes[id]->params.begin(), nodes[id]->params.end(), [&paramName](auto& p)
	{
		return paramName == p.first;
	});

	int param_index = it - nodes[id]->params.begin();

	id = paramsLocations[id].location[param_index].second;
	Node* node = nodes[id];
	if (!node->params[param_index].second.valid)
		return false;

	s = node->params[param_index].second.s;

	return true;
}

bool Scene::getNodeParam(NodeID id, const ParamKey& paramName, float& f) const
{
	auto it = std::find_if(nodes[id]->params.begin(), nodes[id]->params.end(), [&paramName](auto& p)
	{
		return paramName == p.first;
	});

	int param_index = it - nodes[id]->params.begin();

	id = paramsLocations[id].location[param_index].second;
	Node* node = nodes[id];
	if (!node->params[param_index].second.valid)
		return false;

	f = node->params[param_index].second.f;

	return true;
}

bool Scene::getNodeParam(NodeID id, const ParamKey& paramName, flt2& f2) const
{
	auto it = std::find_if(nodes[id]->params.begin(), nodes[id]->params.end(), [&paramName](auto& p)
	{
		return paramName == p.first;
	});

	int param_index = it - nodes[id]->params.begin();

	id = paramsLocations[id].location[param_index].second;
	Node* node = nodes[id];
	if (!node->params[param_index].second.valid)
		return false;

	f2 = node->params[param_index].second.f2;

	return true;
}

bool Scene::getNodeParam(NodeID id, const ParamKey& paramName, flt3& f3) const
{
	auto it = std::find_if(nodes[id]->params.begin(), nodes[id]->params.end(), [&paramName](auto& p)
	{
		return paramName == p.first;
	});

	int param_index = it - nodes[id]->params.begin();

	id = paramsLocations[id].location[param_index].second;
	Node* node = nodes[id];
	if (!node->params[param_index].second.valid)
		return false;

	auto& param = node->params[param_index].second;
	string_id s = param.s;

	if (s == node_position_id)
	{
		f3 = node->getPos3();
		return true;
	}
	if (s == node_axis_z_id)
	{
		f3 = node->getAxisZ();
		return true;
	}

	f3 = param.f3;

	return true;
}

bool Scene::getNodeParam(NodeID id, const ParamKey& paramName, flt4& f4) const
{
	auto it = std::find_if(nodes[id]->params.begin(), nodes[id]->params.end(), [&paramName](auto& p)
	{
		return paramName == p.first;
	});

	int param_index = it - nodes[id]->params.begin();

	id = paramsLocations[id].location[param_index].second;
	Node* node = nodes[id];
	if (!node->params[param_index].second.valid)
		return false;

	f4 = node->params[param_index].second.f4;

	return true;
}

bool Scene::getNodeParam(NodeID id, const ParamKey& paramName, flt4x4& f4x4) const
{
	auto it = std::find_if(nodes[id]->params.begin(), nodes[id]->params.end(), [&paramName](auto& p)
	{
		return paramName == p.first;
	});

	int param_index = it - nodes[id]->params.begin();

	id = paramsLocations[id].location[param_index].second;
	Node* node = nodes[id];
	if (!node->params[param_index].second.valid)
		return false;

	f4x4 = node->params[param_index].second.f4x4;

	return true;
}

bool Scene::getNodeParam(NodeID id, const ParamKey& paramName, int& i) const
{
	auto it = std::find_if(nodes[id]->params.begin(), nodes[id]->params.end(), [&paramName](auto& p)
	{
		return paramName == p.first;
	});

	int param_index = it - nodes[id]->params.begin();

	id = paramsLocations[id].location[param_index].second;
	Node* node = nodes[id];
	if (!node->params[param_index].second.valid)
		return false;

	i = node->params[param_index].second.i;

	return true;
}

bool Scene::getNodeParam(NodeID id, const ParamKey& paramName, Vec2d<int>& i2) const
{
	auto it = std::find_if(nodes[id]->params.begin(), nodes[id]->params.end(), [&paramName](auto& p)
	{
		return paramName == p.first;
	});

	int param_index = it - nodes[id]->params.begin();

	id = paramsLocations[id].location[param_index].second;
	Node* node = nodes[id];
	if (!node->params[param_index].second.valid)
		return false;

	i2 = node->params[param_index].second.i2;

	return true;
}

flt4x4& Scene::getNodePosition4x4(NodeID id)
{
	Node* node = nodes[id];
	return node->getPos4x4();
}

flt4& Scene::getNodePosition4(NodeID id)
{
	Node* node = nodes[id];
	return node->getPos4();
}

flt3& Scene::getNodePosition3(NodeID id)
{
	Node* node = nodes[id];
	return node->getPos3();
}

flt3& Scene::getNodeAxisZ(NodeID id)
{
	Node* node = nodes[id];
	return node->getAxisZ();
}

flt4x4& Scene::getNodeInvTrPosition(NodeID id)
{
	Node* node = nodes[id];
	return node->getPosInvTr();
}

void Scene::accept(Visitor* visitor) const
{
	if (root == nullptr)
		return;
	root->accept(visitor);
}

void* Scene::getNode(NodeID id)
{
	return nodes[id];
}

void Scene::updateTransformWithTranslation(NodeID id, flt4x4& translation)
{
	auto currentNode = nodes[id];
	while (currentNode->nodeType != Node::type::TRANSFORM &&
		currentNode->nodeType != Node::type::ROOT)
	{
		currentNode->bCached = false;
		currentNode = currentNode->parent;
	}
	currentNode->bCached = false;

	if (currentNode->nodeType == Node::type::ROOT)
		return;

	auto transformNode = static_cast<TransformNode*>(currentNode);
	transformNode->pos *= translation;
}

void Scene::updateTransformWithRotation(NodeID id, flt4x4& rotation)
{
	auto currentNode = nodes[id];
	while (currentNode->nodeType != Node::type::TRANSFORM &&
		currentNode->nodeType != Node::type::ROOT)
	{
		currentNode->bCached = false;
		currentNode = currentNode->parent;
	}
	currentNode->bCached = false;

	if (currentNode->nodeType == Node::type::ROOT)
		return;

	auto transformNode = static_cast<TransformNode*>(currentNode);
	transformNode->pos *= rotation;
}

void Scene::updateTransformWithScaling(NodeID id, flt4x4& scaling)
{
	auto currentNode = nodes[id];
	while (currentNode->nodeType != Node::type::TRANSFORM &&
		currentNode->nodeType != Node::type::ROOT)
	{
		currentNode->bCached = false;
		currentNode = currentNode->parent;
	}
	currentNode->bCached = false;

	if (currentNode->nodeType == Node::type::ROOT)
		return;

	auto transformNode = static_cast<TransformNode*>(currentNode);
	transformNode->pos *= scaling;
}

class SaveVisitor :public Scene::Visitor
{
public:
	SaveVisitor(std::ofstream& s) :s(s) {}

	void startVisit(const Scene::RootNode* node)
	{
		s << "root node:" << std::endl;
		s << "node id: " << node->ID << std::endl;

		s << "params: " << std::endl;

		int n = node->params.size();
		s << "params count: " << n << std::endl;
		for (auto& param : node->params)
		{
			s << "param key: " << std::endl;

			auto& k = param.first;

			s << "name: ";
			StringManager::save(s, k.name);
			s << std::endl;

			s << "index: " << k.index << std::endl;

			s << "field: ";
			StringManager::save(s, k.field);
			s << std::endl;

			s << "param value: " << std::endl;

			auto& pv = param.second;

			s << "string: ";
			StringManager::save(s, pv.s);
			s << std::endl;

			s << "float: " << pv.f << std::endl;
			s << "float2: " << std::string(pv.f2) << std::endl;
			s << "float3: " << std::string(pv.f3) << std::endl;
			s << "float4: " << std::string(pv.f4) << std::endl;
			s << "float4x4: " << std::string(pv.f4x4) << std::endl;
			s << "int: " << pv.i << std::endl;
			s << "int2: " << std::string(pv.i2) << std::endl;
			s << "validity: " << pv.valid << std::endl;
		}

		s << "childs:" << std::endl;
		s << "childs count: " << node->childs.size() << std::endl;
	}

	void startVisit(const Scene::TransformNode* node)
	{
		s << "transform node:" << std::endl;
		s << "node id: " << node->ID << std::endl;

		s << "params: " << std::endl;

		int n = node->params.size();
		s << "params count: " << n << std::endl;
		for (auto& param : node->params)
		{
			s << "param key: " << std::endl;

			auto& k = param.first;

			s << "name: ";
			StringManager::save(s, k.name);
			s << std::endl;

			s << "index: " << k.index << std::endl;

			s << "field: ";
			StringManager::save(s, k.field);
			s << std::endl;

			s << "param value: " << std::endl;

			auto& pv = param.second;

			s << "string: ";
			StringManager::save(s, pv.s);
			s << std::endl;

			s << "float: " << pv.f << std::endl;
			s << "float2: " << std::string(pv.f2) << std::endl;
			s << "float3: " << std::string(pv.f3) << std::endl;
			s << "float4: " << std::string(pv.f4) << std::endl;
			s << "float4x4: " << std::string(pv.f4x4) << std::endl;
			s << "int: " << pv.i << std::endl;
			s << "int2: " << std::string(pv.i2) << std::endl;
			s << "validity: " << pv.valid << std::endl;
		}

		s << "transform matrix: " << std::string(node->pos) << std::endl;
		s << "childs:" << std::endl;
		s << "childs count: " << node->childs.size() << std::endl;
	}

	void startVisit(const Scene::MeshNode* node) 
	{
		s << "mesh node:" << std::endl;
		s << "node id: " << node->ID << std::endl;

		s << "params: " << std::endl;

		int n = node->params.size();
		s << "params count: " << n << std::endl;
		for (auto& param : node->params)
		{
			s << "param key: " << std::endl;

			auto& k = param.first;

			s << "name: ";
			StringManager::save(s, k.name);
			s << std::endl;

			s << "index: " << k.index << std::endl;
			
			s << "field: ";
			StringManager::save(s, k.field);
			s << std::endl;
			
			s << "param value: " << std::endl;

			auto& pv = param.second;
			
			s << "string: ";
			StringManager::save(s, pv.s);
			s << std::endl;
			
			s << "float: " << pv.f << std::endl;
			s << "float2: " << std::string(pv.f2) << std::endl;
			s << "float3: " << std::string(pv.f3) << std::endl;
			s << "float4: " << std::string(pv.f4) << std::endl;
			s << "float4x4: " << std::string(pv.f4x4) << std::endl;
			s << "int: " << pv.i << std::endl;
			s << "int2: " << std::string(pv.i2) << std::endl;
			s << "validity: " << pv.valid << std::endl;
		}

		auto it = mapMeshToIndex.find(node->mesh);
		if (it == mapMeshToIndex.end())
			mapMeshToIndex[node->mesh] = this->meshIndex++;
		s << "mesh index: " << mapMeshToIndex[node->mesh] << std::endl;
	}

	std::ofstream& s;
	std::map<const Mesh*, int> mapMeshToIndex;
	int meshIndex = 0;
};

void Scene::save(std::ofstream& s) const
{
	s << "nodes count: " << nodes.size() << std::endl;

	SaveVisitor saveVisitor{s};
	accept(&saveVisitor);

	s << "meshes: " << std::endl;

	int n = saveVisitor.mapMeshToIndex.size();
	s << "meshes count: " << n << std::endl;

	std::vector<const Mesh*> meshes(n);
	for (auto& m : saveVisitor.mapMeshToIndex)
	{
		meshes[m.second] = m.first;
	}
	
	for (auto mesh : meshes)
	{
		mesh->save(s);
	}

	s << "params locations:" << std::endl;

	n = paramsLocations.size();
	s << "params locations count: " << n << std::endl;
	for (int i = 0; i < n; i++)
	{
		s << "referencing node id: " << i << std::endl;

		int m = paramsLocations[i].location.size();
		s << "params count: " << m << std::endl;
		for (int j = 0; j < m; j++)
		{
			auto& k = paramsLocations[i].location[j].first;
			s << "param key: " << std::endl;

			s << "name: ";
			StringManager::save(s, k.name);
			s << std::endl;

			s << "index: " << k.index << std::endl;
			
			s << "field: ";
			StringManager::save(s, k.field);
			s << std::endl;

			s << "referenced node id: " << paramsLocations[i].location[j].second << std::endl;
		}
	}
}

void Scene::clear()
{
	nextId = 0;

	nodes.clear();

	delete root;
	root = nullptr;

	paramsLocations.clear();

	for (auto m : meshes)
		delete m;
	meshes.clear();
}

void Scene::loadNode(std::ifstream& s, NodeID parent, int nodesCount, int& evaluatedNodesCount)
{
	if (nodesCount == evaluatedNodesCount)
		return;

	std::string tmp;
	std::string nodeType;
	s >> nodeType >> tmp;

	NodeID id = 0;
	s >> tmp >> tmp >> id;

	// params
	s >> tmp;

	int paramsCount;
	s >> tmp >> tmp >> paramsCount;

	std::vector<std::pair<ParamKey, ParamValue>> params(paramsCount);
	for (int pi = 0; pi < paramsCount; pi++)
	{
		// key
		s >> tmp >> tmp;
		
		s >> tmp; 
		params[pi].first.name = StringManager::load(s);
		
		s >> tmp >> params[pi].first.index;

		s >> tmp;
		params[pi].first.field = StringManager::load(s);

		// value
		s >> tmp >> tmp;

		s >> tmp;
		params[pi].second.s = StringManager::load(s);

		s >> tmp >> params[pi].second.f;

		s >> tmp >> tmp;
		params[pi].second.f2 = tmp;

		s >> tmp >> tmp;
		params[pi].second.f3 = tmp;

		s >> tmp >> tmp;
		params[pi].second.f4 = tmp;

		s >> tmp >> tmp;
		params[pi].second.f4x4 = tmp;

		s >> tmp >> params[pi].second.i;

		s >> tmp >> tmp;
		params[pi].second.i2 = tmp;

		s >> tmp >> params[pi].second.valid;
	}

	if (nodeType == std::string("transform"))
	{
		s >> tmp >> tmp >> tmp;
		auto node = new TransformNode(parent, flt4x4(tmp));
		node->ID = id;
		node->scene = this;
		node->params = std::move(params);

		nodes[id] = node;
		nodes[parent]->addChild(node);

		s >> tmp;

		int childsCount;
		s >> tmp >> tmp >> childsCount;
		for (int chi = 0; chi < childsCount; chi++)
		{
			loadNode(s, id, nodesCount, evaluatedNodesCount);
		}
	}
	if (nodeType == std::string("root"))
	{
		auto node = new RootNode(parent);
		node->ID = id;
		node->scene = this;
		node->params = std::move(params);

		nodes[id] = node;

		s >> tmp;

		int childsCount;
		s >> tmp >> tmp >> childsCount;
		for (int chi = 0; chi < childsCount; chi++)
		{
			loadNode(s, id, nodesCount, evaluatedNodesCount);
		}
	}
	if (nodeType == std::string("mesh"))
	{
		int meshIndex;
		s >> tmp >> tmp >> meshIndex;

		auto node = new MeshNode(parent, (const Mesh*)meshIndex);
		node->ID = id;
		node->scene = this;
		node->params = std::move(params);

		nodes[id] = node;
		nodes[parent]->addChild(node);
	}
	evaluatedNodesCount++;
}

void Scene::load(std::ifstream& s)
{
	clear();

	std::string tmp;

	int nodesCount;
	s >> tmp >> tmp >> nodesCount;

	nodes.resize(nodesCount);

	int evaluatedNodesCount = 0;
	loadNode(s, -1, nodesCount, evaluatedNodesCount);
	root = dynamic_cast<RootNode*>(nodes[0]);

	s >> tmp;

	int meshesCount;
	s >> tmp >> tmp >> meshesCount;

	meshes.resize(meshesCount);
	for (int mi = 0; mi < meshesCount; mi++)
	{
		meshes[mi] = new Mesh();
		meshes[mi]->load(s);
		meshes[mi]->scene = this;
	}

	for (auto n : nodes)
	{
		if (n->nodeType != Node::type::MESH)
			continue;
		auto meshNode = (MeshNode*)n;
		meshNode->mesh = meshes[(int)meshNode->mesh];
	}

	s >> tmp >> tmp;

	int paramsLocationsCount;
	s >> tmp >> tmp >> tmp >> paramsLocationsCount;
	paramsLocations.resize(paramsLocationsCount);

	for (int pli = 0; pli < paramsLocationsCount; pli++)
	{
		int referencingNodeId;
		s >> tmp >> tmp >> tmp >> referencingNodeId;

		int paramsCount;
		s >> tmp >> tmp >> paramsCount;
		paramsLocations[pli].location.resize(paramsCount);

		for (int pi = 0; pi < paramsCount; pi++)
		{
			auto& k = paramsLocations[pli].location[pi].first;
			
			s >> tmp >> tmp;

			// name
			s >> tmp;
			k.name = StringManager::load(s);

			// index
			s >> tmp >> k.index;

			// field
			s >> tmp;
			k.field = StringManager::load(s);

			// referenced node id
			s >> tmp >> tmp >> tmp >> paramsLocations[pli].location[pi].second;
		}
	}

}