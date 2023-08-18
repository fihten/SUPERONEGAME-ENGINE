
#include "pch.h"
#include "framework.h"

#include "Scene.h"
#include "ResourceManager.h"
#include <algorithm>

Scene::Node::Node(NodeID id)
{
	ID = id;
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
	root->accept(visitor);
}