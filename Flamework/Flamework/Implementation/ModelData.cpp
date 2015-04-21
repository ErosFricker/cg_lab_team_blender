//
//  ModelData.cpp
//  Framework
//
//  Created by David Steiner on 4/11/13.
//
//

#include <map>
#include <memory>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <boost/bind.hpp>

#include "vmmlib/matrix.hpp"
#include "ModelData.h"
#include "GeometryData.h"
#include "Texture.h"
#include "TextureData.h"
#include "obj/obj_parser.hpp"
#include "Util.h"

class ModelDataImpl
{
public:
    typedef std::map< std::string, MaterialData >   Materials;
    
    struct FaceData
    {
        Index       v1;
        Index       v2;
        Index       v3;
        vmml::vec3f normal;
        vmml::vec3f tangent;
        vmml::vec3f bitangent;
    };
    
    struct VertexData
    {
        std::vector<Index>  faces;
        vmml::vec3f         position;
        vmml::vec3f         normal;
        vmml::vec3f         tangent;
        vmml::vec3f         bitangent;
        
        float x()   { return position.x();  }
        float y()   { return position.y();  }
        float z()   { return position.z();  }
        
        VertexData(const vmml::vec3f &position)
        :   position(position)
        {}
    };
    
    ModelDataImpl(ModelData *data, obj::obj_parser::flags_type flags)
    :   _data(data)
    ,   _flags(flags)
    {}
    
    bool load(std::istream& istream);
    
    void createFaceNormals();
    
    void createVertexNormals();
    
    ModelData::GroupMap getData() { return _groups; }
    
private:
    void createGroup(const std::string& name)
    {
        _groupName = name;
        auto &group = _groups[_groupName];
        if (!group)
        {
            group = GeometryDataPtr(new GeometryData);
        }
        _group = group;
    }
    
    void info_callback(const std::string& filename, std::size_t line_number, const std::string& message);
    void warning_callback(const std::string& filename, std::size_t line_number, const std::string& message);
    void error_callback(const std::string& filename, std::size_t line_number, const std::string& message);
    void geometric_vertex_callback(obj::float_type x, obj::float_type y, obj::float_type z);
    void texture_vertex_callback(obj::float_type u, obj::float_type v);
    void vertex_normal_callback(obj::float_type x, obj::float_type y, obj::float_type z);
    void triangular_face_geometric_vertices_callback(obj::index_type v1, obj::index_type v2, obj::index_type v3);
    void triangular_face_geometric_vertices_texture_vertices_callback(const obj::index_2_tuple_type& v1_vt1, const obj::index_2_tuple_type& v2_vt2, const obj::index_2_tuple_type& v3_vt3);
    void triangular_face_geometric_vertices_vertex_normals_callback(const obj::index_2_tuple_type& v1_vn1, const obj::index_2_tuple_type& v2_vn2, const obj::index_2_tuple_type& v3_vn3);
    void triangular_face_geometric_vertices_texture_vertices_vertex_normals_callback(const obj::index_3_tuple_type& v1_vt1_vn1, const obj::index_3_tuple_type& v2_vt2_vn2, const obj::index_3_tuple_type& v3_vt3_vn3);
    void polygonal_face_geometric_vertices_begin_callback(obj::index_type v1, obj::index_type v2, obj::index_type v3);
    void polygonal_face_geometric_vertices_vertex_callback(obj::index_type v);
    void polygonal_face_geometric_vertices_end_callback();
    void polygonal_face_geometric_vertices_texture_vertices_begin_callback(const obj::index_2_tuple_type& v1_vt1, const obj::index_2_tuple_type& v2_vt2, const obj::index_2_tuple_type& v3_vt3);
    void polygonal_face_geometric_vertices_texture_vertices_vertex_callback(const obj::index_2_tuple_type& v_vt);
    void polygonal_face_geometric_vertices_texture_vertices_end_callback();
    void polygonal_face_geometric_vertices_vertex_normals_begin_callback(const obj::index_2_tuple_type& v1_vn1, const obj::index_2_tuple_type& v2_vn2, const obj::index_2_tuple_type& v3_vn3);
    void polygonal_face_geometric_vertices_vertex_normals_vertex_callback(const obj::index_2_tuple_type& v_vn);
    void polygonal_face_geometric_vertices_vertex_normals_end_callback();
    void polygonal_face_geometric_vertices_texture_vertices_vertex_normals_begin_callback(const obj::index_3_tuple_type& v1_vt1_vn1, const obj::index_3_tuple_type& v2_vt2_vn2, const obj::index_3_tuple_type& v3_vt3_vn3);
    void polygonal_face_geometric_vertices_texture_vertices_vertex_normals_vertex_callback(const obj::index_3_tuple_type& v_vt_vn);
    void polygonal_face_geometric_vertices_texture_vertices_vertex_normals_end_callback();
    void group_name_callback(const std::string& group_name);
    void smoothing_group_callback(obj::size_type group_number);
    void object_name_callback(const std::string& object_name);
    void material_library_callback(const std::string& filename);
    void material_name_callback(const std::string& material_name);
    void comment_callback(const std::string& comment);
    
    void loadObjMtl(const std::string &mtlFile);
    TexturePtr createTexture(const std::string &fileName);
    
    template< bool POSITION, bool TEX_COORD, bool NORMAL >
    void genVertex(const IndexData &d);
    
    template< bool NORMAL >
    void genFace(const IndexData &d1, const IndexData &d2, const IndexData &d3);
    
    obj::obj_parser::flags_type _flags;
    ModelData                   *_data;
    Materials                   _materials;
    std::string                 _groupName;
    ModelData::GroupMap         _groups;
    GeometryDataPtr             _group;
    
    std::vector<FaceData>       _faces;
    std::vector<VertexData>     _vertices;
    std::vector<vmml::vec2f>    _texCoords;
    std::vector<vmml::vec3f>    _normals;
    std::vector<vmml::vec3f>    _tangents;
    std::vector<vmml::vec3f>    _bitangents;
};

ModelData::ModelData(const std::string &fileName, bool flipT, bool flipZ)
:   _impl(new ModelDataImpl(this,
                            obj::obj_parser::parse_blank_lines_as_comment |
                            obj::obj_parser::triangulate_faces            |
                            obj::obj_parser::translate_negative_indices))
,   _flipT(flipT)
,   _flipZ(flipZ)
{
    load(fileName);
}

ModelData::ModelData(bool flipT, bool flipZ)
:   _impl(new ModelDataImpl(this,
                            obj::obj_parser::parse_blank_lines_as_comment |
                            obj::obj_parser::triangulate_faces            |
                            obj::obj_parser::translate_negative_indices))
,   _flipT(flipT)
,   _flipZ(flipZ)
{}

ModelData::~ModelData()
{
    delete _impl;
}

ModelData::GroupMap ModelData::getData() const
{
    return _impl->getData();
}

ModelData &ModelData::load(const std::string &fileName)
{
    std::ifstream file(util::getFilePath(fileName), std::ifstream::in);
    _impl->load(file);
    return *this;
}

void ModelDataImpl::info_callback(const std::string& filename, std::size_t line_number, const std::string& message)
{
    std::cerr << filename << ":" << line_number << ": " << "info: " << message << std::endl;
}

void ModelDataImpl::warning_callback(const std::string& filename, std::size_t line_number, const std::string& message)
{
    std::cerr << filename << ":" << line_number << ": " << "warning: " << message << std::endl;
}

void ModelDataImpl::error_callback(const std::string& filename, std::size_t line_number, const std::string& message)
{
    std::cerr << filename << ":" << line_number << ": " << "error: " << message << std::endl;
}

void ModelDataImpl::geometric_vertex_callback(obj::float_type x, obj::float_type y, obj::float_type z)
{
    _vertices.push_back(vmml::vec3f(x, y, z));
}

void ModelDataImpl::texture_vertex_callback(obj::float_type u, obj::float_type v)
{
    _texCoords.push_back(vmml::vec2f(u, v));
}

void ModelDataImpl::vertex_normal_callback(obj::float_type x, obj::float_type y, obj::float_type z)
{
    _normals.push_back(vmml::vec3f(x, y, z));
}

template< bool POSITION, bool TEX_COORD, bool NORMAL >
void ModelDataImpl::genVertex(const IndexData &d)
{
    Vertex v;
    
    if (POSITION)
    {
        v.position.x = _vertices[d.vertexIndex].x();
        v.position.y = _vertices[d.vertexIndex].y();
        v.position.z = _vertices[d.vertexIndex].z();
        if (_data->_flipZ)
        {
            v.position.z *= -1.0f;
        }
    }
    else
    {
        v.position.x = 0;
        v.position.y = 0;
        v.position.z = 0;
    }
    
    if (TEX_COORD)
    {
        v.texCoord.s = _texCoords[d.texCoordsIndex].x();
        v.texCoord.t = _texCoords[d.texCoordsIndex].y();
        if (_data->_flipT)
        {
            v.texCoord.t = 1.0f - v.texCoord.t;
        }
    }
    else
    {
        v.texCoord.s = 0;
        v.texCoord.t = 0;
    }
    
    if (NORMAL)
    {
        auto index = d.vertexIndex;
        
        v.normal.x = _normals[index].x();
        v.normal.y = _normals[index].y();
        v.normal.z = _normals[index].z();
        
        v.tangent.x = _tangents[index].x();
        v.tangent.y = _tangents[index].y();
        v.tangent.z = _tangents[index].z();
        
        v.bitangent.x = _bitangents[index].x();
        v.bitangent.y = _bitangents[index].y();
        v.bitangent.z = _bitangents[index].z();
    }
    else
    {
        v.normal.x = 0;
        v.normal.y = 0;
        v.normal.z = 0;
        
        v.tangent.x = 0.0;
        v.tangent.y = 0.0;
        v.tangent.z = 0.0;
        
        v.bitangent.x = 0.0;
        v.bitangent.y = 0.0;
        v.bitangent.z = 0.0;
    }
    
    _group->vboVertices.push_back(v);
    _group->vboIndices.push_back(_group->vboIndices.size());
}

template< bool NORMAL >
void ModelDataImpl::genFace(const IndexData &d1, const IndexData &d2, const IndexData &d3)
{
    FaceData f;
    f.v1 = d1.vertexIndex;
    f.v2 = d2.vertexIndex;
    f.v3 = d3.vertexIndex;
    f.normal = vmml::vec3f::ZERO;
    
    _faces.push_back(f);
}

void ModelDataImpl::triangular_face_geometric_vertices_callback(obj::index_type v1, obj::index_type v2, obj::index_type v3)
{
    IndexData d1, d2, d3;
    d1.vertexIndex = v1 - 1;
    d2.vertexIndex = v2 - 1;
    d3.vertexIndex = v3 - 1;
    
    _group->indices.push_back(d1);
    _group->indices.push_back(d2);
    _group->indices.push_back(d3);
    
    genVertex< true, false, false >(d1);
    genVertex< true, false, false >(d2);
    genVertex< true, false, false >(d3);
    
    genFace< false >(d1, d2, d3);
}

void ModelDataImpl::triangular_face_geometric_vertices_texture_vertices_callback(const obj::index_2_tuple_type& v1_vt1, const obj::index_2_tuple_type& v2_vt2, const obj::index_2_tuple_type& v3_vt3)
{
    IndexData d1, d2, d3;
    d1.vertexIndex = boost::get<0>(v1_vt1) - 1;
    d1.texCoordsIndex = boost::get<1>(v1_vt1) - 1;
    d2.vertexIndex = boost::get<0>(v2_vt2) - 1;
    d2.texCoordsIndex = boost::get<1>(v2_vt2) - 1;
    d3.vertexIndex = boost::get<0>(v3_vt3) - 1;
    d3.texCoordsIndex = boost::get<1>(v3_vt3) - 1;
    
    _group->indices.push_back(d1);
    _group->indices.push_back(d2);
    _group->indices.push_back(d3);
    
    genVertex< true, true, false >(d1);
    genVertex< true, true, false >(d2);
    genVertex< true, true, false >(d3);
    
    genFace< false >(d1, d2, d3);
}

void ModelDataImpl::triangular_face_geometric_vertices_vertex_normals_callback(const obj::index_2_tuple_type& v1_vn1, const obj::index_2_tuple_type& v2_vn2, const obj::index_2_tuple_type& v3_vn3)
{
    IndexData d1, d2, d3;
    d1.vertexIndex = boost::get<0>(v1_vn1) - 1;
    d1.normalIndex = boost::get<1>(v1_vn1) - 1;
    d2.vertexIndex = boost::get<0>(v2_vn2) - 1;
    d2.normalIndex = boost::get<1>(v2_vn2) - 1;
    d3.vertexIndex = boost::get<0>(v3_vn3) - 1;
    d3.normalIndex = boost::get<1>(v3_vn3) - 1;
    
    _group->indices.push_back(d1);
    _group->indices.push_back(d2);
    _group->indices.push_back(d3);
    
    genVertex< true, true, true >(d1);
    genVertex< true, true, true >(d2);
    genVertex< true, true, true >(d3);
}

void ModelDataImpl::triangular_face_geometric_vertices_texture_vertices_vertex_normals_callback(const obj::index_3_tuple_type& v1_vt1_vn1, const obj::index_3_tuple_type& v2_vt2_vn2, const obj::index_3_tuple_type& v3_vt3_vn3)
{
    IndexData d1, d2, d3;
    d1.vertexIndex = boost::get<0>(v1_vt1_vn1) - 1;
    d1.texCoordsIndex = boost::get<1>(v1_vt1_vn1) - 1;
    d1.normalIndex = boost::get<2>(v1_vt1_vn1) - 1;
    d2.vertexIndex = boost::get<0>(v2_vt2_vn2) - 1;
    d2.texCoordsIndex = boost::get<1>(v2_vt2_vn2) - 1;
    d2.normalIndex = boost::get<2>(v2_vt2_vn2) - 1;
    d3.vertexIndex = boost::get<0>(v3_vt3_vn3) - 1;
    d3.texCoordsIndex = boost::get<1>(v3_vt3_vn3) - 1;
    d3.normalIndex = boost::get<2>(v3_vt3_vn3) - 1;
    
    _group->indices.push_back(d1);
    _group->indices.push_back(d2);
    _group->indices.push_back(d3);
    
    genVertex< true, true, true >(d1);
    genVertex< true, true, true >(d2);
    genVertex< true, true, true >(d3);
}

void ModelDataImpl::polygonal_face_geometric_vertices_begin_callback(obj::index_type v1, obj::index_type v2, obj::index_type v3)
{
    // (*ostream_) << "f " << v1 << " " << v2 << " " << v3;
}

void ModelDataImpl::polygonal_face_geometric_vertices_vertex_callback(obj::index_type v)
{
    // (*ostream_) << " " << v;
}

void ModelDataImpl::polygonal_face_geometric_vertices_end_callback()
{
    // (*ostream_) << "\n";
}

void ModelDataImpl::polygonal_face_geometric_vertices_texture_vertices_begin_callback(const obj::index_2_tuple_type& v1_vt1, const obj::index_2_tuple_type& v2_vt2, const obj::index_2_tuple_type& v3_vt3)
{
    // (*ostream_) << "f " << boost::get<0>(v1_vt1) << "/" << boost::get<1>(v1_vt1) << " " << boost::get<0>(v2_vt2) << "/" << boost::get<1>(v2_vt2) << " " << boost::get<0>(v3_vt3) << "/" << boost::get<1>(v3_vt3);
}

void ModelDataImpl::polygonal_face_geometric_vertices_texture_vertices_vertex_callback(const obj::index_2_tuple_type& v_vt)
{
    // (*ostream_) << " " << boost::get<0>(v_vt) << "/" << boost::get<1>(v_vt);
}

void ModelDataImpl::polygonal_face_geometric_vertices_texture_vertices_end_callback()
{
    // (*ostream_) << "\n";
}

void ModelDataImpl::polygonal_face_geometric_vertices_vertex_normals_begin_callback(const obj::index_2_tuple_type& v1_vn1, const obj::index_2_tuple_type& v2_vn2, const obj::index_2_tuple_type& v3_vn3)
{
    // (*ostream_) << "f " << boost::get<0>(v1_vn1) << "//" << boost::get<1>(v1_vn1) << " " << boost::get<0>(v2_vn2) << "//" << boost::get<1>(v2_vn2) << " " << boost::get<0>(v3_vn3) << "//" << boost::get<1>(v3_vn3);
}

void ModelDataImpl::polygonal_face_geometric_vertices_vertex_normals_vertex_callback(const obj::index_2_tuple_type& v_vn)
{
    // (*ostream_) << " " << boost::get<0>(v_vn) << "//" << boost::get<1>(v_vn);
}

void ModelDataImpl::polygonal_face_geometric_vertices_vertex_normals_end_callback()
{
    // (*ostream_) << "\n";
}

void ModelDataImpl::polygonal_face_geometric_vertices_texture_vertices_vertex_normals_begin_callback(const obj::index_3_tuple_type& v1_vt1_vn1, const obj::index_3_tuple_type& v2_vt2_vn2, const obj::index_3_tuple_type& v3_vt3_vn3)
{
    // (*ostream_) << "f " << boost::get<0>(v1_vt1_vn1) << "/" << boost::get<1>(v1_vt1_vn1) << "/" << boost::get<2>(v1_vt1_vn1) << " " << boost::get<0>(v2_vt2_vn2) << "/" << boost::get<1>(v2_vt2_vn2) << "/" << boost::get<2>(v2_vt2_vn2) << " " << boost::get<0>(v3_vt3_vn3) << "/" << boost::get<1>(v3_vt3_vn3) << "/" << boost::get<2>(v3_vt3_vn3);
}

void ModelDataImpl::polygonal_face_geometric_vertices_texture_vertices_vertex_normals_vertex_callback(const obj::index_3_tuple_type& v_vt_vn)
{
    // (*ostream_) << " " << boost::get<0>(v_vt_vn) << "/" << boost::get<1>(v_vt_vn) << "/" << boost::get<2>(v_vt_vn);
}

void ModelDataImpl::polygonal_face_geometric_vertices_texture_vertices_vertex_normals_end_callback()
{
    // (*ostream_) << "\n";
}

void ModelDataImpl::group_name_callback(const std::string& group_name)
{
    std::cout << "g " << group_name << "\n";
    createGroup(group_name);
}

void ModelDataImpl::object_name_callback(const std::string& object_name)
{
    std::cout << "o " << object_name << "\n";
}

void ModelDataImpl::material_library_callback(const std::string& filename)
{
    std::cout << "mtllib " << filename << "\n";
    loadObjMtl(filename);
}

void ModelDataImpl::material_name_callback(const std::string& material_name)
{
    std::cout << "usemtl " << material_name << "\n";
    MaterialData &material = _materials[material_name];
    
    material.name = material_name;
    _group->materialData = material;
}

void ModelDataImpl::comment_callback(const std::string& comment)
{
    std::cout << comment << std::endl;
}

bool ModelDataImpl::load(std::istream& istream)
{
    createGroup("default");
    obj::obj_parser obj_parser(_flags);
    
    std::string ifilename;
    obj_parser.info_callback(boost::bind(&ModelDataImpl::info_callback, this, ifilename, _1, _2));
    obj_parser.warning_callback(boost::bind(&ModelDataImpl::warning_callback, this, ifilename, _1, _2));
    obj_parser.error_callback(boost::bind(&ModelDataImpl::error_callback, this, ifilename, _1, _2));
    
    obj_parser.geometric_vertex_callback(boost::bind(&ModelDataImpl::geometric_vertex_callback, this, _1, _2, _3));
    obj_parser.texture_vertex_callback(boost::bind(&ModelDataImpl::texture_vertex_callback, this, _1, _2));
    obj_parser.vertex_normal_callback(boost::bind(&ModelDataImpl::vertex_normal_callback, this, _1, _2, _3));
    obj_parser.face_callbacks(
                              boost::bind(&ModelDataImpl::triangular_face_geometric_vertices_callback, this, _1, _2, _3),
                              boost::bind(&ModelDataImpl::triangular_face_geometric_vertices_texture_vertices_callback, this, _1, _2, _3),
                              boost::bind(&ModelDataImpl::triangular_face_geometric_vertices_vertex_normals_callback, this, _1, _2, _3),
                              boost::bind(&ModelDataImpl::triangular_face_geometric_vertices_texture_vertices_vertex_normals_callback, this, _1, _2, _3),
                              boost::bind(&ModelDataImpl::polygonal_face_geometric_vertices_begin_callback, this, _1, _2, _3),
                              boost::bind(&ModelDataImpl::polygonal_face_geometric_vertices_vertex_callback, this, _1),
                              boost::bind(&ModelDataImpl::polygonal_face_geometric_vertices_end_callback, this),
                              boost::bind(&ModelDataImpl::polygonal_face_geometric_vertices_texture_vertices_begin_callback, this, _1, _2, _3),
                              boost::bind(&ModelDataImpl::polygonal_face_geometric_vertices_texture_vertices_vertex_callback, this, _1),
                              boost::bind(&ModelDataImpl::polygonal_face_geometric_vertices_texture_vertices_end_callback, this),
                              boost::bind(&ModelDataImpl::polygonal_face_geometric_vertices_vertex_normals_begin_callback, this, _1, _2, _3),
                              boost::bind(&ModelDataImpl::polygonal_face_geometric_vertices_vertex_normals_vertex_callback, this, _1),
                              boost::bind(&ModelDataImpl::polygonal_face_geometric_vertices_vertex_normals_end_callback, this),
                              boost::bind(&ModelDataImpl::polygonal_face_geometric_vertices_texture_vertices_vertex_normals_begin_callback, this, _1, _2, _3),
                              boost::bind(&ModelDataImpl::polygonal_face_geometric_vertices_texture_vertices_vertex_normals_vertex_callback, this, _1),
                              boost::bind(&ModelDataImpl::polygonal_face_geometric_vertices_texture_vertices_vertex_normals_end_callback, this)
                              );
    obj_parser.group_name_callback(boost::bind(&ModelDataImpl::group_name_callback, this, _1));
    obj_parser.object_name_callback(boost::bind(&ModelDataImpl::object_name_callback, this, _1));
    obj_parser.material_library_callback(boost::bind(&ModelDataImpl::material_library_callback, this, _1));
    obj_parser.material_name_callback(boost::bind(&ModelDataImpl::material_name_callback, this, _1));
    obj_parser.comment_callback(boost::bind(&ModelDataImpl::comment_callback, this, _1));
    
    bool ret = obj_parser.parse(istream);
    
    // if there is not only vertex- but also face data, update normals accordingly
    if (_faces.size() > 0)
    {
        createFaceNormals();
        createVertexNormals();
        
        for (auto i = _groups.begin(); i != _groups.end(); ++i)
        {
            for (auto j = i->second->vboIndices.begin(); j != i->second->vboIndices.end(); ++j)
            {
                auto idx = i->second->indices[*j].vertexIndex;
                
                vmml::vec3f cNormal = _vertices[idx].normal;
                Vector3 &normal = i->second->vboVertices[*j].normal;
                normal.x = cNormal.x();
                normal.y = cNormal.y();
                normal.z = cNormal.z();
                
                vmml::vec3f cTangent = _vertices[idx].tangent;
                Vector3 &tangent = i->second->vboVertices[*j].tangent;
                tangent.x = cTangent.x();
                tangent.y = cTangent.y();
                tangent.z = cTangent.z();
                
                vmml::vec3f cBitangent = _vertices[idx].bitangent;
                Vector3 &bitangent = i->second->vboVertices[*j].bitangent;
                bitangent.x = cBitangent.x();
                bitangent.y = cBitangent.y();
                bitangent.z = cBitangent.z();
            }
        }
    }
    
    return ret;
}

void ModelDataImpl::loadObjMtl(const std::string &mtlFile)
{
    std::string file = util::getFilePath(mtlFile);
    std::ifstream inFile(file.c_str(), std::ifstream::in);
    
    std::string line, key;
    std::string materialName;
    
    while(inFile.good() && !inFile.eof() && std::getline(inFile, line))
    {
        key = "";
        std::stringstream ss(line);
        ss>>key>>std::ws;
        
        if (key  == "newmtl")
        {
            ss >> materialName >> std::ws;
        }
        else if (key  == "Ka")
        {
            vmml::vec3f kA;
            ss >> kA[0] >> std::ws >> kA[1] >> std::ws >> kA[2] >> std::ws;
            _materials[materialName].vectors["Ka"] = kA;
        }
        else if (key  == "Kd")
        {
            vmml::vec3f kD;
            ss >> kD[0] >> std::ws >> kD[1] >> std::ws >> kD[2] >> std::ws;
            _materials[materialName].vectors["Kd"] = kD;
        }
        else if (key  == "Ks")
        {
            vmml::vec3f kS;
            ss >> kS[0] >> std::ws >> kS[1] >> std::ws >> kS[2] >> std::ws;
            _materials[materialName].vectors["Ks"] = kS;
        }
        else if (key  == "Tf")
        {
            vmml::vec3f tF;
            ss >> tF[0] >> std::ws >> tF[1] >> std::ws >> tF[2] >> std::ws;
            _materials[materialName].vectors["Tf"] = tF;
        }
        else if (key  == "Ns")
        {
            float nS;
            ss >> nS >> std::ws;
            _materials[materialName].scalars["Ns"] = nS;
        }
        else if (key  == "Ni")
        {
            float nI;
            ss >> nI >> std::ws;
            _materials[materialName].scalars["Ni"] = nI;
        }
        else if (key  == "illum")
        {
            float illum;
            ss >> illum >> std::ws;
            _materials[materialName].scalars["Illum"] = illum;
        }
        else if (key  == "map_Kd")
        {
            auto &mat = _materials[materialName].textures["DiffuseMap"];
            ss >> mat >> std::ws;
        }
        else if (key  == "map_Ks")
        {
            auto &mat = _materials[materialName].textures["SpecularMap"];
            ss >> mat >> std::ws;
        }
        else if (key  == "map_Bump")
        {
            auto &mat = _materials[materialName].textures["NormalMap"];
            ss >> mat >> std::ws;
        }
    }
}

void ModelDataImpl::createFaceNormals()
{
    for (int i = 0; i < _faces.size(); ++i)
    {
        // obtain reference to face
        FaceData &face = _faces[i];
        
        // obtain indices of this face's three vertices
        Index indexV1 = face.v1;
        Index indexV2 = face.v2;
        Index indexV3 = face.v3;
        
        // add this face's index to list of adjacent faces of each vertex
        _vertices[indexV1].faces.push_back(i);
        _vertices[indexV2].faces.push_back(i);
        _vertices[indexV3].faces.push_back(i);
        
        // obtain each of this face's vertex positions
        const vmml::vec3f &p1 = _vertices[indexV1].position;
        const vmml::vec3f &p2 = _vertices[indexV2].position;
        const vmml::vec3f &p3 = _vertices[indexV3].position;
        
        vmml::vec3f e = p2 - p1;
        vmml::vec3f f = p3 - p1;
        face.normal = vmml::normalize(e.cross(f));
        
        // set normal of vertices to face normal
        _vertices[indexV1].normal = face.normal;
        _vertices[indexV2].normal = face.normal;
        _vertices[indexV3].normal = face.normal;
        
        // obtain each of this face's texture coordinates
        const vmml::vec2f &t1 = _texCoords[indexV1];
        const vmml::vec2f &t2 = _texCoords[indexV2];
        const vmml::vec2f &t3 = _texCoords[indexV3];
        
        vmml::matrix<2, 3, float> mat ;
        mat.set_row(0, e);
        mat.set_row(1, f);
        vmml::matrix<2, 2, float> mat2 ;
        mat2.set_row(0, t2-t1);
        mat2.set_row(1, t3-t1);
        vmml::matrix<2, 2, float> inverse;
        vmml::compute_inverse(mat2, inverse);
        
        vmml::matrix<2, 2, float> tb_matrix = inverse*mat;
        vmml::vec3f tangent = tb_matrix.get_row(0);
        face.tangent = normalize(tangent - face.normal*dot(face.normal, tangent));
        face.bitangent = normalize(face.tangent.cross(face.normal));

        
    }
}

void ModelDataImpl::createVertexNormals()
{
    for (VertexData &vertex : _vertices)
    {
        vmml::vec3f normalSum = vmml::vec3f::ZERO;
        vmml::vec3f tangentSum = vmml::vec3f::ZERO;
        vmml::vec3f bitangentSum = vmml::vec3f::ZERO;
        for (Index &face : vertex.faces)
        {
            normalSum += _faces[face].normal;
            tangentSum += _faces[face].tangent;
            bitangentSum += _faces[face].bitangent;
        }
        
        vertex.normal = vmml::normalize(normalSum);
        
        vmml::vec3f normTangent = normalize(tangentSum);
        vmml::vec3f orthogonalTangent = vmml::normalize(normTangent - vertex.normal*dot(normTangent, vertex.normal));
        
        vertex.tangent = orthogonalTangent;
        vertex.bitangent = normalize(bitangentSum);
    }
}
