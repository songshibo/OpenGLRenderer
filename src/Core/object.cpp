#include "object.h"
#include "Config.h"

Object::Object(string const &path, float3 worldPos, quaternion localRot, float3 localScale)
{
    position = worldPos;
    scale = localScale;
    rotation = localRot;

    loadModel(SOURCE_DIR + path);
}

void Object::Draw(Shader &drawShader, GLenum drawMode)
{
    if (shader != nullptr)
        DEBUG_INFO("Object:{} has changed shader!", name);
    shader = &drawShader;
    shader->use();
    // Generate Model Matrix
    float4x4 model = this->model();
    shader->SetFloat4x4("model", model);

    for (int i = 0; i < meshes.size(); i++)
    {
        meshes[i].Draw(*shader, drawMode);
    }
}

void Object::Draw(GLenum drawMode)
{
    if (shader == nullptr)
    {
        DEBUG_ERROR("Object:{} has no shader for drawing!", name);
        return;
    }
    shader->use();
    // Generate Model Matrix
    float4x4 model = this->model();
    shader->SetFloat4x4("model", model);

    for (int i = 0; i < meshes.size(); i++)
    {
        meshes[i].Draw(*shader, drawMode);
    }
}

float4x4 Object::model()
{
    using namespace Eigen;
    Transform<float, 3, Affine> translation = Translation3f(position) * rotation.toRotationMatrix() * Scaling(scale);
    return translation.matrix(); // this should be a 4x4 translation matrix
}

void Object::loadModel(string const &path)
{
    // read file via ASSIMP
    Assimp::Importer importer;
    // remember macos don't need to aiProcess_FlipUVs
#ifdef __APPLE__
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);
#elif _WIN32
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
#endif

    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        DEBUG_ERROR("ERROR::ASSIMP:: {}", importer.GetErrorString());
        return;
    }
    // retrieve the directory path of the filepath
    name = path.substr(path.find_last_of('/') + 1);
    DEBUG_INFO("Model:{} loaded.", name);

    // process ASSIMP's root node recursively
    processNode(scene->mRootNode, scene);
}
void Object::processNode(aiNode *node, const aiScene *scene)
{
    // process each mesh located at the current node
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene.
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        Mesh submesh = processMesh(mesh, scene);
        meshes.push_back(submesh);
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for (uint32_t i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Object::processMesh(aiMesh *mesh, const aiScene *scene)
{
    // data to fill
    vector<Vertex> vertices;
    vector<uint32_t> indices;

    // Walk through each of the mesh's vertices
    for (int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        float3 vector;
        // positions
        vector << mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z;
        vertex.position = vector;
        // normals
        vector << mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z;
        vertex.normal = vector;
        // texture coordinates
        if (mesh->HasTextureCoords(0))
        {
            float2 vec;
            vec << mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y;
            vertex.texcoords = vec;
        }
        else
            // No texture coords
            vertex.texcoords = float2(0.0f, 0.0f);

        if (mesh->HasTangentsAndBitangents())
        {
            // tangent
            vector << mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z;
            vertex.tangent = vector;
            // bitangent
            vector << mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z;
            vertex.bitangent = vector;
        }
        else
        {
            vertex.tangent = float3::Zero();
            vertex.bitangent = float3::Zero();
        }
        vertices.push_back(vertex);
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (uint32_t j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // No Textures

    // return a mesh object created from the extracted mesh data
    return Mesh(vertices, indices);
}