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

void KGV::Engine::GeometryFactory::getVertexGridU16(U32 size, std::vector<DirectX::XMFLOAT3> &vertices,
                                                    std::vector<DirectX::XMFLOAT3> &normals, std::vector<U16> &indices, float step) {

}

void KGV::Engine::GeometryFactory::getVertexGridU32(U32 size, std::vector<Render::Vertex> &vertices, std::vector<U32> &indices, float step) {
    const U32 numNodes = size * size;
    vertices.reserve(numNodes);

    // The number of squares/quads is always 1 less than the number of nodes in a direction
    // Since each quad will be divided into two triangles,
    // with each triangle having 3 vertices, this gives us the num of indices
    const U32 numQuads = (size - 1) * (size - 1);
    const U32 numIndices = numQuads * 2 * 3;
    indices.reserve(numIndices);
    const float uvStep = 1.0f / static_cast<float>(size - 1);

    // We produce the grid in a scanline pattern
    // Traverse horizontal then step vertical,
    float currentZPos = (static_cast<float>(size) / 2.0f) * step;
    for (int i = 0; i < size; ++i) {

        float currentXPos = -(static_cast<float>(size) / 2.0f) * step;
        for (int j = 0; j < size; ++j) {

            Render::Vertex v{};
            v.position = {currentXPos, 0.0f, currentZPos};
            v.normal = {0.0f, 1.0f, 0.0f};
            v.texCoord = {uvStep * static_cast<float>(j), uvStep * static_cast<float>(i)};
            vertices.emplace_back(v);

            currentXPos += step;
        }
        currentZPos -= step;
    }

    /* We produce indices from quads in the same left to right top to bottom fashion.
     * however we keep repeating the pattern below.
     *
     * 0-------1
     * |  t1 / |
     * |   /   |
     * | /  t2 |
     * 3-------2
     *
     * Indices for t1 = 0 1 3
     * Indices for t2 = 1 2 3
    */
    for (int i = 0; i < size - 1; ++i) {
        for (int j = 0; j < size - 1; ++j) {
            // top left tri (T1)
            indices.emplace_back(i * size + j);
            indices.emplace_back(i * size + j + 1);
            indices.emplace_back((i + 1) * size + j);
//            spdlog::info("{} {} {}", indices[indices.size() - 3], indices[indices.size() - 2], indices[indices.size() - 1]);

            // bottom right tri (T2)
            indices.emplace_back(i * size + j + 1);
            indices.emplace_back((i + 1) * size + j + 1);
            indices.emplace_back((i + 1) * size + j);
//            spdlog::info("{} {} {}", indices[indices.size() - 3], indices[indices.size() - 2], indices[indices.size() - 1]);
        }
    }
}

void KGV::Engine::GeometryFactory::getPlane(std::vector<Render::Vertex> &vertices, std::vector<U32> &indices) {

    vertices.emplace_back(Render::Vertex{{ 0.5, -0.5, 0.0}, {0, 0, -1}, {0.0, 0.0}}); // top left
    vertices.emplace_back(Render::Vertex{{-0.5, -0.5, 0.0}, {0, 0, -1}, {1.0, 0.0}}); // Top Right
    vertices.emplace_back(Render::Vertex{{-0.5,  0.5, 0.0}, {0, 0, -1}, {1.0, 1.0}}); // Bot Right
    vertices.emplace_back(Render::Vertex{{ 0.5,  0.5, 0.0}, {0, 0, -1}, {0.0, 1.0}}); // Bot Left

    indices.emplace_back(2);
    indices.emplace_back(0);
    indices.emplace_back(1);
    indices.emplace_back(0);
    indices.emplace_back(2);
    indices.emplace_back(3);
}
