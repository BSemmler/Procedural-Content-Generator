struct psInput
{
    float4 position : SV_POSITION;
};

// cbuffer MaterialConstants : register(b3)
// {
//     float4 color;
// };

cbuffer MaterialConstants : register(b3) {
     float4 color;
};

float4 main(psInput ps) : SV_TARGET
{
    return color;
}