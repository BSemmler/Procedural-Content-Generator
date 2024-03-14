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

struct DirectionalLight
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float3 direction;
    float pad;
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

cbuffer FrameConstantsDef : register(b2) {
    DirectionalLight gDirectionalLight;
    float gDeltaTime;
    float pad1;
    float pad2;
    float pad3;
};

cbuffer MaterialConstants : register(b3) {
    Material gMaterial;
};

VertexOut VS(VertexIn input)
{
    VertexOut output;

    output.worldPosition = mul(float4(input.position, 1.0f), gWorldMatrix);
    output.position = mul(output.worldPosition, gViewProjectionMatrix);

    // Transform the normals to homogeneous clip space.
    output.normal = mul(input.normal, gWorldInvTranspose);

	return output;
}

void calcDirectionalLighting(Material mat, DirectionalLight light, float3 normal, float3 toEye,
                            out float4 ambient, out float4 diffuse, out float4 spec) {
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);

    float3 lightVec = -light.direction;

    ambient = mat.ambient * light.ambient;

    float diffuseFactor = dot(lightVec, normal);

    [flatten]
    if (diffuseFactor > 0.0f) {
        float3 v = reflect(-lightVec, normal);
        float specFactor = pow(max(dot(v, toEye), 0.0f), mat.specular.w);

        diffuse = diffuseFactor * mat.diffuse * light.diffuse;
        spec = specFactor * mat.specular * light.specular;
    }
}

float4 PS(VertexOut input) : SV_TARGET {
    input.normal = normalize(input.normal);

    float3 toEyeW = normalize(gCameraPos - input.worldPosition);

    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);

    float4 A, D, S;
    calcDirectionalLighting(gMaterial, gDirectionalLight, input.normal, toEyeW, A, D, S);
    ambient += A;
    diffuse += D;
    spec    += S;

   // TODO: Implement point and spot lights

   float4 litColor = ambient + diffuse + spec;
   litColor.a = gMaterial.diffuse.a;

   return litColor;
}

