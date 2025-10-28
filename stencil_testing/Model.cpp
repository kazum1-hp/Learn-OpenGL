#include "Model.h"
#include <iostream>

Model::Model(const std::string& path)
{
	loadModel(path);
}

void Model::draw() const
{
	for (const auto& mesh : meshes)
		mesh -> draw();
}

void Model::loadModel(const std::string& path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path,
		aiProcess_Triangulate |
		aiProcess_FlipUVs |
		aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}

	directory = path.substr(0, path.find_last_of("/"));

	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		meshes.push_back(processMesh(scene->mMeshes[i], scene));
	}
}

std::unique_ptr<Mesh> Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	std::vector<VertexAttribute> attributes;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		vertices.push_back(mesh->mVertices[i].x);
		vertices.push_back(mesh->mVertices[i].y);
		vertices.push_back(mesh->mVertices[i].z);
		if (mesh->HasNormals()) {
			vertices.push_back(mesh->mNormals[i].x);
			vertices.push_back(mesh->mNormals[i].y);
			vertices.push_back(mesh->mNormals[i].z);
		}

		if (mesh->mTextureCoords[0]) 
		{
			vertices.push_back(mesh->mTextureCoords[0][i].x);
			vertices.push_back(mesh->mTextureCoords[0][i].y);
		}

		else {
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
		}
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	GLuint index = 0;
	attributes.push_back({ index++, 3, GL_FLOAT, GL_FALSE }); // pos	
	attributes.push_back({ index++, 3, GL_FLOAT, GL_FALSE }); // normal
	attributes.push_back({ index++, 2, GL_FLOAT, GL_FALSE }); // uv
	
	Geometry geometry(vertices, indices, attributes);

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	std::vector<std::shared_ptr<Texture>> meshTextures;  // 每个 Mesh 独立纹理
	for (int type = aiTextureType_NONE + 1; type <= aiTextureType_UNKNOWN; ++type) {
		auto textures = loadMaterialTextures(material, static_cast<aiTextureType>(type), std::to_string(type));
		meshTextures.insert(meshTextures.end(), textures.begin(), textures.end());
	}

	return std::make_unique<Mesh>(geometry, meshTextures);
}

std::vector<std::shared_ptr<Texture>> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName)
{
	std::vector<std::shared_ptr<Texture>> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) 
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		std::string path = directory + "/" + str.C_Str();
		//std::cout << "[Assimp] Found texture: " << path << " | Type: " << typeName << std::endl;

		if (loadedTextures.find(path) == loadedTextures.end()) 
		{
			loadedTextures[path] = std::make_shared<Texture>(path, type == aiTextureType_DIFFUSE ? 0 : 1);
		}

		textures.push_back(loadedTextures[path]);
	}
	return textures;
}