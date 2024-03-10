struct vsInput
{
    float4 position : POSITION;
    float4 color : COLOR;
};

struct psInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

cbuffer MatrixBuffer {
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
}

psInput main(vsInput input)
{
    psInput output;
    output.color = input.color;
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
	return output;
}