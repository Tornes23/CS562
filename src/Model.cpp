#include "Model.h"
#include "ResourceManager.h"
#include "Utils.h"

Model::Model(tinygltf::Model* model) : mGLTF_Model(*model)
{
	if (!model) return;

	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	const tinygltf::Scene& scene = model->scenes[model->defaultScene];
	for (size_t i = 0; i < scene.nodes.size(); ++i) 
	{
		assert((scene.nodes[i] >= 0) && (scene.nodes[i] < model->nodes.size()));
		BindSceneNodes(model, model->nodes[scene.nodes[i]]);
	}

    glBindVertexArray(0);
    for (size_t i = 0; i < mVBOs.size(); i++) 
        glDeleteBuffers(1, &mVBOs[i]);
}

Model::~Model()
{
    glDeleteVertexArrays(1, &mVAO);
    delete &mGLTF_Model;
}

void Model::BindSceneNodes(tinygltf::Model* model, tinygltf::Node& node)
{
	if ((node.mesh >= 0) && (node.mesh < model->meshes.size())) {
		BindMeshes(model, model->meshes[node.mesh]);
	}

	for (size_t i = 0; i < node.children.size(); i++) {
		assert((node.children[i] >= 0) && (node.children[i] < model->nodes.size()));
		BindSceneNodes(model, model->nodes[node.children[i]]);
	}
}

void Model::BindMeshes(tinygltf::Model* model, tinygltf::Mesh& mesh)
{

    for (size_t i = 0; i < model->bufferViews.size(); ++i)
    {
        const tinygltf::BufferView& bufferView = model->bufferViews[i];

        const tinygltf::Buffer& buffer = model->buffers[bufferView.buffer];

        GLuint vbo;
        glGenBuffers(1, &vbo);
        mVBOs[i] = vbo;
        glBindBuffer(bufferView.target, vbo);

        glBufferData(bufferView.target, bufferView.byteLength,
            &buffer.data.at(0) + bufferView.byteOffset, GL_STATIC_DRAW);
    }

    for (size_t i = 0; i < mesh.primitives.size(); ++i)
    {
        tinygltf::Primitive primitive = mesh.primitives[i];
        tinygltf::Accessor indexAccessor = model->accessors[primitive.indices];

        for (auto& attrib : primitive.attributes) 
        {
            tinygltf::Accessor accessor = model->accessors[attrib.second];
            int byteStride =
                accessor.ByteStride(model->bufferViews[accessor.bufferView]);
            glBindBuffer(GL_ARRAY_BUFFER, mVBOs[accessor.bufferView]);

            int size = 1;
            if (accessor.type != TINYGLTF_TYPE_SCALAR) {
                size = accessor.type;
            }

            int vaa = -1;
            if (attrib.first.compare("POSITION") == 0) vaa = 0;
            if (attrib.first.compare("NORMAL") == 0) vaa = 1;
            if (attrib.first.compare("TEXCOORD_0") == 0) vaa = 2;
            if (vaa > -1) {
                glEnableVertexAttribArray(vaa);
                glVertexAttribPointer(vaa, size, accessor.componentType,
                    accessor.normalized ? GL_TRUE : GL_FALSE,
                    byteStride, BUFFER_OFFSET(accessor.byteOffset));
            }
        }
        //Load textures if necessary
        if (model->textures.size() > 0)
            ResourceManager.GetTextures(model);

        //if(model->materials.size() > 0)
        //    ResourceManager.GetMaterials(model);

    }
}

tinygltf::Model& Model::GetGLTFModel() { return mGLTF_Model; }

void Model::BindVAO() { glBindVertexArray(mVAO); }

void Model::SetMaterialActive(int index)
{
    if (index < 0 || index > mMaterials.size())
        return;

    //mMaterials[index].SetActive();
}
