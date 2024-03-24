struct VertexOut
{
    float4 position : SV_POSITION;
    float4 worldPosition : WORLD_POSITION;
    float4 normal : NORMAL;
    float2 texcoord : TEXCOORD0;
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
     float displacement;
     float pad1;
     float pad2;
     float pad3;
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

Texture2D gRockTexture : register(t0);
SamplerState gColorSampler : register(s0);
Texture2D gGrassTexture : register(t1);
Texture2D gSandTexture : register(t2);
Texture2D gSnowTexture : register(t3);

// TODO: Move these to constant buffer so they're adjustable.
float4 main(VertexOut input) : SV_TARGET {
    const float gMinRockSlope = 0.5;
    const float gMaxGrassSlope = 0.9;
    const float gMaxSnowSlope = 0.9;
    const float gMinRockSnowSlope = 0.7;

    const float gMinRockGrassHeight = 4.0f;
    const float gMaxSandHeight = 5.0f;

    const float gMinRockHeight = 79;
    const float gMaxRockHeight = 100;
    const float gMaxRockGrassHeight = 80.0f;
    const float gMinSnowRockHeight = 99.0f;
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

    float3 rockAlbedo = gRockTexture.Sample(gColorSampler, input.texcoord * 16);
    float3 grassAlbedo = gGrassTexture.Sample(gColorSampler, input.texcoord * 16);
    float3 sandAlbedo = gSandTexture.Sample(gColorSampler, input.texcoord * 16);
    float3 snowAlbedo = gSnowTexture.Sample(gColorSampler, input.texcoord * 16);

    float rock_GrassSlopeWeighting = input.normal.y;
    rock_GrassSlopeWeighting = max(gMinRockSlope, rock_GrassSlopeWeighting);
    rock_GrassSlopeWeighting = min(gMaxGrassSlope, rock_GrassSlopeWeighting);
    rock_GrassSlopeWeighting -= gMinRockSlope;
    rock_GrassSlopeWeighting /= gMaxGrassSlope - gMinRockSlope;

    float rock_RockGrassWeighting = input.worldPosition.y;
    rock_RockGrassWeighting = max(gMinRockHeight, rock_RockGrassWeighting);
    rock_RockGrassWeighting = min(gMaxRockGrassHeight, rock_RockGrassWeighting);
    rock_RockGrassWeighting -= gMinRockHeight;
    rock_RockGrassWeighting /= gMaxRockGrassHeight - gMinRockHeight;

    float snow_RockSlopeWeighting = input.normal.y;
    snow_RockSlopeWeighting = max(gMinRockSnowSlope, snow_RockSlopeWeighting);
    snow_RockSlopeWeighting = min(gMaxSnowSlope, snow_RockSlopeWeighting);
    snow_RockSlopeWeighting -= gMinRockSnowSlope;
    snow_RockSlopeWeighting /= gMaxSnowSlope - gMinRockSnowSlope;

    float rock_SnowRockWeighting = input.worldPosition.y;
    rock_SnowRockWeighting = max(gMinSnowRockHeight, rock_SnowRockWeighting);
    rock_SnowRockWeighting = min(gMaxRockHeight, rock_SnowRockWeighting);
    rock_SnowRockWeighting -= gMinSnowRockHeight;
    rock_SnowRockWeighting /= gMaxRockHeight - gMinSnowRockHeight;

    float sand_RockGrassHeightWeighting = input.worldPosition.y;
    sand_RockGrassHeightWeighting = max(gMinRockGrassHeight, sand_RockGrassHeightWeighting);
    sand_RockGrassHeightWeighting = min(gMaxSandHeight, sand_RockGrassHeightWeighting);
    sand_RockGrassHeightWeighting -= gMinRockGrassHeight;
    sand_RockGrassHeightWeighting /= gMaxSandHeight - gMinRockGrassHeight;

    float3 snowRockAlbedo = lerp(snowAlbedo, rockAlbedo, snow_RockSlopeWeighting);
    float3 rockGrassAlbedo = lerp(rockAlbedo, grassAlbedo, rock_GrassSlopeWeighting);
    float3 rock_RockGrassAlbedo = lerp(rockGrassAlbedo, rockAlbedo, rock_RockGrassWeighting);
    float3 rock_SnowRockAlbedo = lerp(rock_RockGrassAlbedo, snowRockAlbedo, rock_SnowRockWeighting);
    float3 finalColor = lerp(sandAlbedo, rock_SnowRockAlbedo, sand_RockGrassHeightWeighting);

    float3 litColor = finalColor * (ambient.rgb + diffuse.rgb) + spec.rgb;
    return float4(litColor, 1);
//     return float4(rockGrassAlbedo, 1);
//     return float4(sandAlbedo, 1.0f);
//     return float4(finalColor, 1.0f);
}