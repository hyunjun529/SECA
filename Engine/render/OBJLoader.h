#ifndef KATA_RENDER_OBJLOADER_H_
#define KATA_RENDER_OBJLOADER_H_

#include "tiny_obj_loader.h"

#include "stb_image.h"

#include "DrawObject.h"


namespace kata
{
	namespace render
	{
		// ref https://github.com/syoyo/tinyobjloader/blob/master/examples/viewer/viewer.cc
		class OBJLoader
		{
		private:
			std::string err;

			tinyobj::attrib_t attrib;
			std::vector<tinyobj::shape_t> shapes;
			std::vector<tinyobj::material_t> materials;
			

		public:
			DrawObject loadOBJ(const char *_file, const char *_path)
			{
				DrawObject o;
				o.vaoId = 0;
				o.vboVId = 0;
				o.vboUvId = 0;
				o.vboNorId = 0;

				std::string base_dir = _path;
				std::string inputfile = "";
				inputfile.append(_path);
				inputfile.append(_file);

				// load obj file
				bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile.c_str(), base_dir.c_str());
				if (!err.empty())
				{
					KATA_CONSOLE_ERROR(err.c_str());
				}
				if (!ret) assert(true);
				KATA_CONSOLE_INFO("# of vertices  = {}\n", (int)(attrib.vertices.size()) / 3);
				KATA_CONSOLE_INFO("# of normals   = {}\n", (int)(attrib.normals.size()) / 3);
				KATA_CONSOLE_INFO("# of texcoords = {}\n", (int)(attrib.texcoords.size()) / 2);
				KATA_CONSOLE_INFO("# of materials = {}\n", (int)materials.size());
				KATA_CONSOLE_INFO("# of shapes    = {}\n", (int)shapes.size());

				// load Texture Images from materials
				for (size_t m = 0; m < materials.size(); m++)
				{
					tinyobj::material_t* mp = &materials[m];
					
					if (o.textures.find(mp->diffuse_texname) == o.textures.end()) {
						DrawObject::Texture t;

						std::string texture_filename = mp->diffuse_texname;

						texture_filename = base_dir + mp->diffuse_texname; // need check valid

						t.image = stbi_load(texture_filename.c_str(), &t.w, &t.h, &t.comp, STBI_default);

						if (!t.image)
						{
							KATA_CONSOLE_ERROR("OBJLoader : image not found in load texture");
						}

						o.textures.insert(std::make_pair(mp->diffuse_texname, t));
					}
				}

				// load obj position, uv, normal from shapes
				DrawObject::SubMesh sm;
				sm.idxBegin = 0;
				sm.cntVertex = 0;
				sm.textureId = 0;
				sm.texname = "";

				for (size_t s = 0; s < shapes.size(); s++)
				{
					int current_material_id = 0;
					if (materials.size() > 0)
					{
						current_material_id = shapes[s].mesh.material_ids[0];
						sm.texname = materials[current_material_id].diffuse_texname;
					}

					for (size_t f = 0; f < shapes[s].mesh.indices.size() / 3; f++)
					{
						tinyobj::index_t idx0 = shapes[s].mesh.indices[3 * f + 0];
						tinyobj::index_t idx1 = shapes[s].mesh.indices[3 * f + 1];
						tinyobj::index_t idx2 = shapes[s].mesh.indices[3 * f + 2];

						// v = vertex = bufferPosition
						glm::vec3 v[3];
						for (int k = 0; k < 3; k++)
						{
							int f0 = idx0.vertex_index;
							int f1 = idx1.vertex_index;
							int f2 = idx2.vertex_index;

							v[0][k] = attrib.vertices[3 * f0 + k];
							v[1][k] = attrib.vertices[3 * f1 + k];
							v[2][k] = attrib.vertices[3 * f2 + k];
						}

						// vt = tc = uv = bufferUV
						glm::vec2 tc[3];
						if (attrib.texcoords.size() > 0) {
							if ((idx0.texcoord_index < 0) || (idx1.texcoord_index < 0) ||
								(idx2.texcoord_index < 0)) {
								// face does not contain valid uv index.
								tc[0][0] = 0.0f;
								tc[0][1] = 0.0f;
								tc[1][0] = 0.0f;
								tc[1][1] = 0.0f;
								tc[2][0] = 0.0f;
								tc[2][1] = 0.0f;
							}
							else
							{
								assert(attrib.texcoords.size() >
									size_t(2 * idx0.texcoord_index + 1));
								assert(attrib.texcoords.size() >
									size_t(2 * idx1.texcoord_index + 1));
								assert(attrib.texcoords.size() >
									size_t(2 * idx2.texcoord_index + 1));

								// Flip Y coord.
								tc[0][0] = attrib.texcoords[2 * idx0.texcoord_index];
								tc[0][1] = 1.0f - attrib.texcoords[2 * idx0.texcoord_index + 1];
								tc[1][0] = attrib.texcoords[2 * idx1.texcoord_index];
								tc[1][1] = 1.0f - attrib.texcoords[2 * idx1.texcoord_index + 1];
								tc[2][0] = attrib.texcoords[2 * idx2.texcoord_index];
								tc[2][1] = 1.0f - attrib.texcoords[2 * idx2.texcoord_index + 1];
							}
						}
						else {
							tc[0][0] = 0.0f;
							tc[0][1] = 0.0f;
							tc[1][0] = 0.0f;
							tc[1][1] = 0.0f;
							tc[2][0] = 0.0f;
							tc[2][1] = 0.0f;
						}
						
						// vn = vertex normal = bufferNomal
						glm::vec3 n[3];
						bool invalid_normal_index = false;
						int nf0 = idx0.normal_index;
						int nf1 = idx1.normal_index;
						int nf2 = idx2.normal_index;
						if (attrib.normals.size() > 0)
						{
							if ((nf0 < 0) || (nf1 < 0) || (nf2 < 0))
							{
								// normal index is missing from this face.
								invalid_normal_index = true;
							}
							else
							{
								for (int k = 0; k < 3; k++)
								{
									assert(size_t(3 * nf0 + k) < attrib.normals.size());
									assert(size_t(3 * nf1 + k) < attrib.normals.size());
									assert(size_t(3 * nf2 + k) < attrib.normals.size());
									n[0][k] = attrib.normals[3 * nf0 + k];
									n[1][k] = attrib.normals[3 * nf1 + k];
									n[2][k] = attrib.normals[3 * nf2 + k];
								}
							}
						}
						else
						{
							invalid_normal_index = true;
						}
						if (invalid_normal_index) {
							// compute geometric normal
							CalcNormal(n[0], v[0], v[1], v[2]);
							n[1][0] = n[0][0];
							n[1][1] = n[0][1];
							n[1][2] = n[0][2];
							n[2][0] = n[0][0];
							n[2][1] = n[0][1];
							n[2][2] = n[0][2];
						}

						// save to DrawObject
						for (int k = 0; k < 3; k++)
						{
							// push buffer
							o.bufferPosition.push_back(v[k]);
							o.bufferUV.push_back(tc[k]);
							o.bufferNormal.push_back(n[k]);
							o.numTriangles++;
						}

						// Make SubSet
						int material_id = shapes[s].mesh.material_ids[f];
						if (current_material_id == material_id)
						{
							sm.cntVertex += 3;
						}
						else
						{
							o.subMeshs.push_back(sm);
							
							if (materials.size() > 0)
							{
								sm.texname = materials[material_id].diffuse_texname;
							}
							sm.idxBegin += sm.cntVertex;
							sm.cntVertex = 3;

							current_material_id = material_id;
						}
					}
					o.subMeshs.push_back(sm);
				}

				return o;
			}

			void CalcNormal(glm::vec3 N, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2)
			{
				float v10[3];
				v10[0] = v1[0] - v0[0];
				v10[1] = v1[1] - v0[1];
				v10[2] = v1[2] - v0[2];

				float v20[3];
				v20[0] = v2[0] - v0[0];
				v20[1] = v2[1] - v0[1];
				v20[2] = v2[2] - v0[2];

				N[0] = v20[1] * v10[2] - v20[2] * v10[1];
				N[1] = v20[2] * v10[0] - v20[0] * v10[2];
				N[2] = v20[0] * v10[1] - v20[1] * v10[0];

				float len2 = N[0] * N[0] + N[1] * N[1] + N[2] * N[2];
				if (len2 > 0.0f)
				{
					float len = sqrtf(len2);

					N[0] /= len;
					N[1] /= len;
					N[2] /= len;
				}
			}
		};
	}
}


#endif // KATA_RENDER_OBJLOADER_H_