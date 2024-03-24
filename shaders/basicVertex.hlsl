struct vsInput
{
    float4 position : POSITION0;
    float4 normal : NORMAL;
    float2 texcoord : TEXCOORD0;
};

struct psInput
{
    float4 position : SV_POSITION;
    float4 worldPosition : WORLD_POSITION;
    float3 normal : NORMAL;
};

cbuffer CameraConstants : register(b1) {
    matrix viewProjectionMatrix;
};

cbuffer ObjectConstants : register(b0) {
    matrix worldMatrix;
};

cbuffer FrameConstantsDef : register(b2) {
    float deltaTime;
    float pad1;
    float pad2;
    float pad3;
};

psInput main(vsInput input)
{
    psInput output;
//     output.position = input.position;
    output.worldPosition = mul(input.position, worldMatrix);
    output.position = mul(output.worldPosition, viewProjectionMatrix);

    output.normal = mul(input.normal, worldMatrix);

	return output;
}