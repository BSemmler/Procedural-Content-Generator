Texture2D gTexture : register(t0);
SamplerState gTextureSampler : register(s0);

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

float4 main(VertexOut input) : SV_TARGET {
    return gTexture.Sample(gTextureSampler, input.texcoord);
//     return float4(1, 0.1, 0.1, 1);
}