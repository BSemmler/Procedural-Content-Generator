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

Texture1D<float> gHeightMap : register(t0);
SamplerState gHeightMapSampler : register(s0);

VertexOut main(VertexIn input)
{
    VertexOut output;
    input.position.y = gHeightMap.SampleLevel(gHeightMapSampler, input.texcoord.x, 0) * max(gMaterial.displacement, 1.0f);
    output.worldPosition = mul(float4(input.position, 1.0f), gWorldMatrix);
    output.position = mul(output.worldPosition, gViewProjectionMatrix);
    output.texcoord = input.texcoord;
	return output;
}
