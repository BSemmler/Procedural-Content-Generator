struct VertexIn
{
    float3 position : POSITION0;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD0;
};

struct VertexOut
{
    float4 position : SV_POSITION;
    float4 worldPosition : WORLD_POSITION;
    float4 normal : NORMAL;
    float2 texcoord : TEXCOORD0;
};

struct Material {
     float4 pad1;
     float4 pad2;
     float4 pad3;
     float displacement;
     float pad4;
     float pad5;
     float pad6;
};

cbuffer ObjectConstants : register(b0) {
    matrix gWorldMatrix;
    matrix gWorldInvTranspose;
};

cbuffer CameraConstants : register(b1) {
    matrix gViewProjectionMatrix;
    float4 gCameraPos;
};

cbuffer MaterialConstants : register(b3) {
    Material gMaterial;
};


Texture2D<float> gHeightMap : register(t0);
SamplerState gHeightMapSampler : register(s0);

// Function to sample heightmap texture at a given UV coordinate
float SampleHeightMap(float2 uv) {
    return gHeightMap.SampleLevel(gHeightMapSampler, uv, 0); // Sample the heightmap at the given UV coordinate
}

// Function to get the height difference between a texel and its neighboring texels
float GetHeightDifference(float2 uv, float2 offset) {
    float centerHeight = SampleHeightMap(uv).r * max(gMaterial.displacement, 1.0f);
    float neighborHeight = SampleHeightMap(uv + offset).r * max(gMaterial.displacement, 1.0f);
    return neighborHeight - centerHeight;
}

VertexOut main(VertexIn input)
{
    VertexOut output;

    float width = 0.0f;
    float height = 0.0f;
    gHeightMap.GetDimensions(width, height);
    float2 uv = input.texcoord;
    float2 offset = float2(1.0f / width, 1.0f / height);
    input.position.y = gHeightMap.SampleLevel(gHeightMapSampler, uv, 0).r * max(gMaterial.displacement, 1.0f) - (max(gMaterial.displacement, 1.0f) / 2);

    output.worldPosition = mul(float4(input.position, 1.0f), gWorldMatrix);
    output.position = mul(output.worldPosition, gViewProjectionMatrix);

    float heightDifferenceLeft = GetHeightDifference(uv, float2(-offset.x, 0));
    float heightDifferenceRight = GetHeightDifference(uv, float2(offset.x, 0));
    float heightDifferenceUp = GetHeightDifference(uv, float2(0, offset.y));
    float heightDifferenceDown = GetHeightDifference(uv, float2(0, -offset.y));

    // Transform the normals to homogeneous clip space.
    float3 normal;
    normal.x = heightDifferenceLeft - heightDifferenceRight;
    normal.y = 2.0;
    normal.z = heightDifferenceDown - heightDifferenceUp;
    output.normal = normalize(mul(float4(normal, 0), gWorldInvTranspose));
    output.texcoord = uv;

	return output;
}
