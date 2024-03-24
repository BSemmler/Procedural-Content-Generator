Texture2D gTexture : register(t0);
SamplerState gTextureSampler : register(s0);

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

cbuffer ObjectConstants : register(b0) {
    matrix gWorldMatrix;
    matrix gWorldInvTranspose;
};

cbuffer CameraConstants : register(b1) {
    matrix gViewProjectionMatrix;
    float4 gCameraPos;
};

VertexOut VS(VertexIn input) {
    VertexOut output;

    output.worldPosition = mul(float4(input.position, 1.0f), gWorldMatrix);
    output.position = mul(output.worldPosition, gViewProjectionMatrix);

    // Transform the normals to homogeneous clip space.
    output.normal = mul(input.normal, gWorldInvTranspose);
    output.texcoord = input.texcoord;

    return output;
}

float4 PS(VertexOut input) : SV_TARGET {
    return gTexture.Sample(gTextureSampler, input.texcoord);
//     return float4(1, 0.1, 0.1, 1);
}