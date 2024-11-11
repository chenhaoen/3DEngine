#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Nodes/NodeReader.h"

Node *NodeReader::read(const std::string_view &file)
{
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(file.data(), aiProcess_Triangulate | aiProcess_FlipUVs); 

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return nullptr;
    }

     for(unsigned int i = 0; i < scene->mRootNode->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[scene->mRootNode->mMeshes[i]]; 
              
    }

    return nullptr;
}