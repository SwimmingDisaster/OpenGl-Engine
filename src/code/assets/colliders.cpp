#include "mypch.h"
#include "assets/colliders.h"
#include "core/modelImporter.h"
#include "core/physics.h"


//--------------BoxCollider--------------
REGISTERIMPL(BoxCollider);

void BoxCollider::Show() {
    ImGui::DragFloat3("scale", glm::value_ptr(scale), 0.01f);
}
void BoxCollider::Serialize(YAML::Emitter& out) const {
    out << YAML::Key << name;
    out << YAML::BeginMap;
    out << YAML::Key << "Scale" << YAML::Value << scale;
    out << YAML::EndMap;
}
void BoxCollider::Deserialize(const YAML::Node& data) {
    scale = data["Scale"].as<glm::vec3>();
}
PxGeometryHolder BoxCollider::GetGeometry() {
    return PxBoxGeometry(scale.x, scale.y, scale.z);
}
BoxCollider::BoxCollider() {
    name = "BoxCollider";
}
#ifdef SHOW_DELETED
BoxCollider::~BoxCollider() {
    Log("Deleted " << name);
}
#endif
//--------------BoxCollider--------------


//--------------SphereCollider--------------
REGISTERIMPL(SphereCollider);

void SphereCollider::Show() {
    ImGui::DragFloat("Radius", &radius, 0.01f);
}
void SphereCollider::Serialize(YAML::Emitter& out) const {
    out << YAML::Key << name;
    out << YAML::BeginMap;
    out << YAML::Key << "Radius" << YAML::Value << radius;
    out << YAML::EndMap;
}
void SphereCollider::Deserialize(const YAML::Node& data) {
    radius = data["Radius"].as<float>();
}
PxGeometryHolder SphereCollider::GetGeometry() {
    return PxSphereGeometry(PxReal(radius));
}
SphereCollider::SphereCollider() {
    name = "SphereCollider";
}
#ifdef SHOW_DELETED
SphereCollider::~SphereCollider() {
    Log("Deleted " << name);
}
#endif
//--------------SphereCollider--------------
//--------------ConcaveMeshCollider--------------
REGISTERIMPL(ConcaveMeshCollider);

void ConcaveMeshCollider::Show() {
    ImGui::InputText("File path", &filePath, ImGuiInputTextFlags_CallbackResize);
    if (ImGui::Button("Browse")) {
        nfdfilteritem_t filterItemList[1] = { { "3D model", "fbx,dae,gltf,glb,blend,3ds,ase,obj,ifc,xgl,zgl,ply,dxf,lwo,lws,lxo,stl,x,ac,ms3d,cob,scn,bvh,csm,xml,irrmesh,irr,mdl,md2,md3,pk3,mdc,md5*,smd,vta,ogex,3d,b3d,q3d,q3s,nff,nff,off,raw,ter,mdl,hmp,ndo" } };
        filePath = OpenFile(filterItemList, 1);
    }
    ImGui::SameLine();

    if (ImGui::Button("Reload")) {
        Start();
    }
}
void ConcaveMeshCollider::Start() {
    if(filePath != "")
        ModelImporter::LoadModelBasic(filePath, vertices, indices, false, false);

    transform = parentEntity->GetComponent<Transform>();
    PxTolerancesScale scale;
    scale.length = 100;
    scale.speed = 981;
    PxCookingParams params(scale);
    // disable mesh cleaning - perform mesh validation on development configurations
    params.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH;
    // disable edge precompute, edges are set for each triangle, slows contact generation
    params.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE;

    PhysicsManager::mCooking->setParams(params);

    PxTriangleMeshDesc meshDesc;
    meshDesc.points.count           = vertices.size();
    meshDesc.points.stride          = sizeof(glm::vec3);
    meshDesc.points.data            = &vertices[0][0];

    meshDesc.triangles.count        = indices.size() / 3;
    meshDesc.triangles.stride       = 3*sizeof(unsigned int);
    meshDesc.triangles.data         = &indices[0];


#ifndef RELEASE_BUILD
    // mesh should be validated before cooked without the mesh cleaning
    bool res = PhysicsManager::mCooking->validateTriangleMesh(meshDesc);
    assert(res);
#endif

    //BatchRenderer::AddObject(model->vertices, model->indices, m_materialComponent, model->transform, shaderName);
    aTriangleMesh = PhysicsManager::mCooking->createTriangleMesh(meshDesc, PhysicsManager::mPhysics->getPhysicsInsertionCallback());
}
void ConcaveMeshCollider::Serialize(YAML::Emitter& out) const {
    out << YAML::Key << name;
    out << YAML::BeginMap;
    out << YAML::Key << "File path" << YAML::Value << filePath;
    out << YAML::EndMap;
}
void ConcaveMeshCollider::Deserialize(const YAML::Node& data) {
    filePath = data["File path"].as<std::string>();
}
PxGeometryHolder ConcaveMeshCollider::GetGeometry() {
    PxMeshScale scale(PxVec3(transform->scale.x, transform->scale.y, transform->scale.z), PxQuat(PxIdentity));
    return PxTriangleMeshGeometry(aTriangleMesh, scale);
}
ConcaveMeshCollider::ConcaveMeshCollider() {
    name = "ConcaveMeshCollider";
}
#ifdef SHOW_DELETED
ConcaveMeshCollider::~ConcaveMeshCollider() {
    aTriangleMesh->release();
    Log("Deleted " << name);
}
#else
ConcaveMeshCollider::~ConcaveMeshCollider() {
    aTriangleMesh->release();
}
#endif
//--------------ConcaveMeshCollider--------------
