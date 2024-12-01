#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Nodes/NodeReader.h"
#include "Nodes/Geometry.h"
#include "Nodes/Geode.h"
#include "Nodes/Group.h"

Node *NodeReader::read(const std::string_view &file)
{
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(file.data(), aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return nullptr;
    }

    Geometry *geometry = new Geometry;

    std::vector<Vertex> vertices;
    std::vector<uint16_t> indexes;
    for (unsigned int i = 0; i < scene->mRootNode->mNumChildren; i++)
    {
        aiNode *node = scene->mRootNode->mChildren[i];
        for (uint32_t meshIndex = 0; meshIndex < node->mNumMeshes; ++meshIndex)
        {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[meshIndex]];

            for (uint32_t j = 0; j < mesh->mNumVertices; ++j)
            {
                const aiVector3D &position = mesh->mVertices[j];
                Vertex vertex;
                vertex.pos = glm::vec3(position.x, position.y, position.z);

                const aiVector3D &textureCoord = mesh->mTextureCoords[0][j];
                vertex.texCoord = glm::vec2(textureCoord.x, textureCoord.y);

                vertices.push_back(vertex);
            }

            for (uint32_t j = 0; j < mesh->mNumFaces; ++j)
            {

                const aiFace &face = mesh->mFaces[j];
                for (uint32_t k = 0; k < face.mNumIndices; ++k)
                {
                    indexes.push_back(face.mIndices[k]);
                }
            }
        }
    }

    for (uint32_t i = 0; i < scene->mNumTextures; ++i)
    {
        aiTexture *texture = scene->mTextures[i];
        geometry->setTextureFile(texture->mFilename.C_Str());
    }

    geometry->setVertices(vertices);
    geometry->setIndices(indexes);

    Geode *geode = new Geode();
    geode->addGeometry(geometry);
    Group *group = new Group();
    group->addChildren(geode);

    return group;
}