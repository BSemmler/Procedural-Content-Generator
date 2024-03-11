struct psInput
{
    float4 position : SV_POSITION;
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
    float4 direction;
    float4 position;
}

float4 main(psInput input) : SV_TARGET
{
    float ambientStrength = 0.2f;
    float3 ambient = ambientStrength * lightColor;
    float3 result = ambient * objectColor;
//     return color;

    return float4(result, 1.0f);
}