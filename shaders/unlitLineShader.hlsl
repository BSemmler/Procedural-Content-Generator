struct VertexOut
{
    float4 position : SV_POSITION;
    float4 worldPosition : WORLD_POSITION;
    float4 normal : NORMAL;
    float2 texcoord : TEXCOORD0;
};

struct Material {
     float4 ambient;
     float4 diffuse;
     float4 specular;
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

float4 main(VertexOut input) : SV_TARGET {
    return gMaterial.diffuse;
}