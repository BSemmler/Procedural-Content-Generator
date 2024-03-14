//
// Created by Brett on 2024-03-12.
//

#include "GeometryFactory.h"
using namespace DirectX;
// z+ is front (looking away from camera)
constexpr S32 gFlatCubeNumVerts = 24;

constexpr XMFLOAT3 gCubeFlatNormals[] = {
        // Front
        {  0,  0,  1 },
        {  0,  0,  1 },
        {  0,  0,  1 },
        {  0,  0,  1 },
        // Back
        {  0,  0, -1 },
        {  0,  0, -1 },
        {  0,  0, -1 },
        {  0,  0, -1 },
        // Top
        {  0,  1,  0 },
        {  0,  1,  0 },
        {  0,  1,  0 },
        {  0,  1,  0 },
        // Bottom
        {  0, -1,  0 },
        {  0, -1,  0 },
        {  0, -1,  0 },
        {  0, -1,  0 },
        // Right
        {  1,  0,  0 },
        {  1,  0,  0 },
        {  1,  0,  0 },
        {  1,  0,  0 },
        // Left
        { -1,  0,  0 },
        { -1,  0,  0 },
        { -1,  0,  0 },
        { -1,  0,  0 },
};

constexpr XMFLOAT3 gCubeFlatVertices[] = {
    // Front
    {  0.5f,  0.5f, 0.5f}, // top left
    {  0.5f, -0.5f, 0.5f}, // bottom left
    { -0.5f,  0.5f, 0.5f}, // top right
    { -0.5f, -0.5f, 0.5f}, // bottom right

    // Back
    { -0.5f,  0.5f,  -0.5f}, // top  left
    { -0.5f, -0.5f,  -0.5f}, // bottom  left
    {  0.5f,  0.5f,  -0.5f}, // top  right
    {  0.5f, -0.5f,  -0.5f}, // bottom  right

    // Top
    {  0.5f, 0.5f,  -0.5f}, // top left
    {  0.5f, 0.5f, 0.5f}, // bottom left
    { -0.5f, 0.5f,  -0.5f}, // top right
    { -0.5f, 0.5f, 0.5f}, // bottom right

    // Bottom
    {  0.5f,  -0.5f,  0.5f}, // top left
    {  0.5f,  -0.5f, -0.5f}, // bottom left
    { -0.5f,  -0.5f,  0.5f}, // top right
    { -0.5f,  -0.5f, -0.5f}, // bottom right
    // right
    {  0.5f,  0.5f, -0.5f}, // top left
    {  0.5f, -0.5f, -0.5f}, // bottom left
    {  0.5f,  0.5f,  0.5f}, // top right
    {  0.5f, -0.5f,  0.5f}, // bottom  right
    // left
    { -0.5f,  0.5f,  0.5f}, // top left
    { -0.5f, -0.5f,  0.5f}, // bottom left
    { -0.5f,  0.5f,  -0.5f}, // top right
    { -0.5f, -0.5f,  -0.5f}, // bottom right
};

constexpr S32 gCubeNumIndices = 36;
constexpr U32 gCubeFlatIndices[] = {
        // Front
    0,2,1,
    2,3,1,
    // Back
    4,6,5,
    6,7,5,
    // Top
    8,10,9,
    10,11,9,
    // Bottom
    12,14,13,
    14,15,13,
    // Left
    16,18,17,
    18,19,17,
    // Right
    20,22,21,
    22,23,21
};

void KGV::Engine::GeometryFactory::getCube(bool hasFlatNormalGeometry, std::vector<XMFLOAT3> &vertices,
                                           std::vector<XMFLOAT3> &normals, std::vector<U32> &indices) {
    vertices.resize(gFlatCubeNumVerts);
    memcpy(vertices.data(), gCubeFlatVertices, sizeof(XMFLOAT3) * gFlatCubeNumVerts);

    normals.resize(gFlatCubeNumVerts);
    memcpy(normals.data(), gCubeFlatNormals, sizeof(XMFLOAT3) * gFlatCubeNumVerts);

    indices.resize(gCubeNumIndices);
    memcpy(indices.data(), gCubeFlatIndices, sizeof(U32) * gCubeNumIndices);
}

void KGV::Engine::GeometryFactory::getVertexGridU16(U32 width, U32 height, std::vector<DirectX::XMFLOAT3> &vertices,
                                                    std::vector<DirectX::XMFLOAT3> &normals, std::vector<U16> &indices, float step) {

}

void KGV::Engine::GeometryFactory::getVertexGridU32(U32 widthNodes, U32 heightNodes, std::vector<DirectX::XMFLOAT3> &vertices,
                                                 std::vector<DirectX::XMFLOAT3> &normals, std::vector<U32> &indices, float step) {
    const U32 numNodes = widthNodes * heightNodes;
    vertices.reserve(numNodes);
    normals.reserve(numNodes);

    // The number of squares/quads is always 1 less than the number of nodes in a direction
    // Since each quad will be divided into two triangles,
    // with each triangle having 3 vertices, this gives us the num of indices
    const U32 numQuads = (widthNodes - 1) * (heightNodes - 1);
    const U32 numIndices = numQuads * 2 * 3;
    indices.reserve(numIndices);

    float currentZPos = -(static_cast<float>(heightNodes) / 2.0f);
    float currentXPos = -(static_cast<float>(widthNodes) / 2.0f);


    for (int i = 0; i < widthNodes; ++i) {
        for (int j = 0; j < heightNodes; ++j) {
//            vertices.emplace_back({})
        }
    }
}
