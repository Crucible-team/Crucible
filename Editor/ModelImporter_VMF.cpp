#include "stdafx.h"
#include "wiScene.h"
#include "ModelImporter.h"
#include "wiRandom.h"

#include "Utility/stb_image.h"

#include <mutex>
#include <string>
#include <limits>
#include <fstream>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <sstream>


using namespace wi::graphics;
using namespace wi::scene;
using namespace wi::ecs;




struct LoaderState
{
	std::string name;
	Scene* scene;
	wi::unordered_map<int, Entity> entityMap;  // node -> entity
	Entity rootEntity = INVALID_ENTITY;

	//Export states
	wi::unordered_map<std::string, int> textureMap; // path -> textureid
	wi::unordered_map<Entity, int> nodeMap;// entity -> node
	wi::unordered_map<size_t, TransformComponent> transforms_original; // original transform states
};

struct Node {
	std::string name;
	std::vector<int> meshIndices;
};

struct Mesh {
	std::string name;
	std::vector<float> vertices;
	std::vector<int> indices;
};

struct Normals {
	std::string name;
	std::vector<float> vertices;
	std::vector<int> indices;
};

struct distances {
	std::string name;
	std::vector<float> vertices;
	std::vector<int> indices;
};

struct offsets {
	std::string name;
	std::vector<float> vertices;
	std::vector<int> indices;
};

struct offset_normals {
	std::string name;
	std::vector<float> vertices;
	std::vector<int> indices;
};

struct alphas {
	std::string name;
	std::vector<float> vertices;
	std::vector<int> indices;
};

struct triangle_tags {
	std::string name;
	std::vector<float> vertices;
	std::vector<int> indices;
};

struct allowed_verts {
	std::string name;
	std::vector<float> vertices;
	std::vector<int> indices;
};

struct dispinfo {
	std::string power;
	std::string startposition;
	std::string flags;
	std::string elevation;
	std::string subdiv;
	std::vector<Normals> normals;
	std::vector<distances> distances;
	std::vector<offsets> offsets;
	std::vector<offset_normals> offset_normals;
	std::vector<alphas> alphas;
	std::vector<triangle_tags> triangle_tags;
	std::vector<allowed_verts> allowed_verts;
};

struct Side {
	int id;
	std::string plane;
	std::string material;
	std::string uaxis;
	std::string vaxis;
	int rotation;
	int lightmapscale;
	int smoothing_groups;
	dispinfo displacment;
};

struct VMFEntity {
	int id;
	std::string classname;
	std::string targetname;
	std::string angles;
	std::string StartDisabled;
	std::string origin;
	//int lightmapscale;
	//int smoothing_groups;
};

struct VMFMAP {
	std::vector<Node> nodes;
	std::vector<Mesh> meshes;
	std::vector<Side> sides;
	std::vector<VMFEntity> ents;
};

std::map<std::string, std::string> parse_attributes(const std::string& line) {
	std::map<std::string, std::string> attributes;
	std::istringstream ss(line);
	std::string attr, value;

	ss >> attr;
	attr = attr.substr(1, attr.length() - 2);

	ss >> value;
	value = value.substr(1, value.length() - 2);

	attributes[attr] = value;

	return attributes;
}



void ImportModel_VMF(const std::string& fileName, wi::scene::Scene& scene)
{
	std::string directory = wi::helper::GetDirectoryFromPath(fileName);
	std::string name = wi::helper::GetFileNameFromPath(fileName);
	std::string extension = wi::helper::toUpper(wi::helper::GetExtensionFromFileName(name));

	//tinygltf::TinyGLTF loader;
	//std::string err;
	//std::string warn;

	//tinygltf::FsCallbacks callbacks;
	//callbacks.ReadWholeFile = tinygltf::ReadWholeFile;
	//callbacks.WriteWholeFile = tinygltf::WriteWholeFile;
	//callbacks.FileExists = tinygltf::FileExists;
	//callbacks.ExpandFilePath = tinygltf::ExpandFilePath;
	//loader.SetFsCallbacks(callbacks);

	//wi::resourcemanager::ResourceSerializer seri; // keep this alive to not delete loaded images while importing gltf
	//loader.SetImageLoader(tinygltf::LoadImageData, &seri);
	//loader.SetImageWriter(tinygltf::WriteImageData, nullptr);

	LoaderState state;
	state.scene = &scene;

	wi::vector<uint8_t> filedata;
	//std::string basedir = tinygltf::GetBaseDir(fileName);

	if (!extension.compare("VMF"))
	{
		//VMFMAP nodes = parseVMF(fileName);

		state.rootEntity = CreateEntity();
		scene.transforms.Create(state.rootEntity);
		scene.names.Create(state.rootEntity) = name;
		state.name = name;

		int entityIndex = 0;

		//for (const auto& node : nodes.ents) {
		//
		//	Scene& scene = *state.scene;
		//	Entity entity = INVALID_ENTITY;
		//
		//	if (entity == INVALID_ENTITY)
		//	{
		//		entity = CreateEntity();
		//		scene.transforms.Create(entity);
		//		scene.IOs.Create(entity);
		//		if (node.targetname == "")
		//		{
		//			scene.names.Create(entity) = node.classname + std::to_string(entityIndex);
		//		}
		//		else
		//		{
		//			scene.names.Create(entity) = node.targetname;
		//		}
		//
		//		entityIndex++;
		//
		//	}
		//
		//	state.entityMap[entityIndex] = entity;
		//
		//	TransformComponent& transform = *scene.transforms.GetComponent(entity);
		//	transform.UpdateTransform();
		//
		//	if (state.rootEntity != INVALID_ENTITY)
		//	{
		//		scene.Component_Attach(entity, state.rootEntity, true);
		//	}
		//	//Entity entity = scene.cameras.GetEntity(entityIndex);
		//	//IOComponent& io = scene.IOs[entityIndex++];
		//	//NameComponent& name = *scene.names.GetComponent(entity);
		//
		//
		//	/*if (node.classname == "light_spot")
		//	{
		//		LightComponent& light = scene.lights[entity];
		//
		//		light.type = LightComponent::LightType::SPOT;
		//
		//		light.SetCastShadow(true);
		//	}*/
		//
		//
		//
		//	//std::cout << "Entity: " << node.id << std::endl;
		//	//std::cout << "Entity_ClassName: " << node.classname << std::endl;
		//	//std::cout << "Entity_Name: " << node.targetname << std::endl;
		//	/* for (const auto& index : node.meshIndices) {
		//		 const Mesh& mesh = meshes[index];
		//		 std::cout << "  Mesh: " << mesh.name << std::endl;
		//		 // Print out vertices and indices
		//		 // ...
		//	 }*/
		//}
	}

	scene.Update(0);
}
