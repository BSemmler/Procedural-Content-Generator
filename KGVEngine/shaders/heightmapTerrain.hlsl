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

Texture2D<float> gHeightMap : register(t0);
SamplerState gHeightMapSampler : register(s0);

// Function to sample heightmap texture at a given UV coordinate
float SampleHeightMap(float2 uv) {
    return gHeightMap.SampleLevel(gHeightMapSampler, uv, 0); // Sample the heightmap at the given UV coordinate
}

// Function to get the height difference between a texel and its neighboring texels
float GetHeightDifference(float2 uv, float2 offset) {
    float centerHeight = SampleHeightMap(uv).r * max(gMaterial.displacement, 1.0f);
    float neighborHeight = SampleHeightMap(uv + offset).r * max(gMaterial.displacement, 1.0f);
    return neighborHeight - centerHeight;
}

VertexOut VS(VertexIn input)
{
    VertexOut output;

    float width = 0.0f;
    float height = 0.0f;
    gHeightMap.GetDimensions(width, height);
    float2 uv = input.texcoord;
    float2 offset = float2(1.0f / width, 1.0f / height);
    input.position.y = gHeightMap.SampleLevel(gHeightMapSampler, uv, 0).r * max(gMaterial.displacement, 1.0f) - (max(gMaterial.displacement, 1.0f) / 2);

    output.worldPosition = mul(float4(input.position, 1.0f), gWorldMatrix);
    output.position = mul(output.worldPosition, gViewProjectionMatrix);

    float heightDifferenceLeft = GetHeightDifference(uv, float2(-offset.x, 0));
    float heightDifferenceRight = GetHeightDifference(uv, float2(offset.x, 0));
    float heightDifferenceUp = GetHeightDifference(uv, float2(0, offset.y));
    float heightDifferenceDown = GetHeightDifference(uv, float2(0, -offset.y));

    // Transform the normals to homogeneous clip space.
    float3 normal;
    normal.x = heightDifferenceLeft - heightDifferenceRight;
    normal.y = 2.0;
    normal.z = heightDifferenceDown - heightDifferenceUp;
    output.normal = normalize(mul(float4(normal, 0), gWorldInvTranspose));
    output.texcoord = uv;

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


Texture2D gRockTexture : register(t0);
SamplerState gColorSampler : register(s0);
Texture2D gGrassTexture : register(t1);
Texture2D gSandTexture : register(t2);

// TODO: Move these to constant buffer so they're adjustable.


float4 PS(VertexOut input) : SV_TARGET {
    const float gMinRockSlope = 0.7;
    const float gMaxGrassSlope = 0.9;
    const float gMinRockGrassHeight = 4.0f;
    const float gMaxSandHeight = 6.0f;
    const float gMaxSnowHeight = 100.0f;
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

    float3 rockAlbedo = gRockTexture.Sample(gColorSampler, input.texcoord * 512);
    float3 grassAlbedo = gGrassTexture.Sample(gColorSampler, input.texcoord * 512);
    float3 sandAlbedo = gSandTexture.Sample(gColorSampler, input.texcoord * 512);

    float rockGrassWeighting = input.normal.y;

    rockGrassWeighting = max(gMinRockSlope, rockGrassWeighting);
    rockGrassWeighting = min(gMaxGrassSlope, rockGrassWeighting);
    rockGrassWeighting -= gMinRockSlope;
    rockGrassWeighting /= gMaxGrassSlope - gMinRockSlope;

    float sandRockGrassWeighting = input.position.y;
    sandRockGrassWeighting = max(gMinRockGrassHeight, sandRockGrassWeighting);
    sandRockGrassWeighting = min(gMaxSandHeight, sandRockGrassWeighting);
    sandRockGrassWeighting -= gMinRockGrassHeight;
    sandRockGrassWeighting /= gMaxSandHeight - gMinRockGrassHeight;
//     printf("%d", sandRockGrassWeighting);

    float3 rockGrassAlbedo = lerp(rockAlbedo, grassAlbedo, rockGrassWeighting);
    float3 finalColor = lerp(sandAlbedo, rockGrassAlbedo, sandRockGrassWeighting);

    float3 litColor = finalColor * (ambient.rgb + diffuse.rgb) + spec.rgb;
    return float4(litColor, 1);
//     return float4(rockGrassAlbedo, 1);
//     return float4(sandAlbedo, 1.0f);
//     return float4(finalColor, 1.0f);
}

