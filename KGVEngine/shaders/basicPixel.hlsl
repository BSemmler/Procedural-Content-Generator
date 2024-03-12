struct psInput
{
    float4 position : SV_POSITION;
    float4 worldPosition : WORLD_POSITION;
    float3 normal : NORMAL;
};

// cbuffer MaterialConstants : register(b3)
// {
//     float4 color;
// };

cbuffer MaterialConstants : register(b3) {
     float4 objectColor;
};

cbuffer LightConstants : register(b4) {
    float4 lightColor;
    float4 lightPos;
    float4 gLightDir;
}

float4 main(psInput input) : SV_TARGET
{
    float3 normal = normalize(input.normal);
    float3 lightDir = normalize(lightPos - input.worldPosition);
    float diff = max(dot(normal, lightDir), 0.0); // Neg light direction because its direction to the light source.
    float3 diffuse = diff * lightColor;
    float ambientStrength = 0.2f;
    float3 ambient = ambientStrength * lightColor;
    float3 result = (ambient + diffuse) * objectColor;

//     return color;

    return float4(result, 1.0f);
}