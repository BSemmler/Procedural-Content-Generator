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
};

cbuffer ObjectConstants : register(b0) {
    matrix gWorldMatrix;
    matrix gWorldInvTranspose;
};

cbuffer CameraConstants : register(b1) {
    matrix gViewProjectionMatrix;
    float4 gCameraPos;
};

VertexOut main(VertexIn input)
{
    VertexOut output;

    output.worldPosition = mul(float4(input.position, 1.0f), gWorldMatrix);
    output.position = mul(output.worldPosition, gViewProjectionMatrix);

    // Transform the normals to homogeneous clip space.
    output.normal = mul(input.normal, gWorldInvTranspose);

	return output;
}