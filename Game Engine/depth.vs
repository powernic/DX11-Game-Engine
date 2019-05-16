cbuffer MatrixBuffer
{
	matrix WVP;
};

struct VertexInputType
{
	float4 position : POSITION;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 depthPosition : TEXTURE0;
};

PixelInputType VS(VertexInputType input)
{
	PixelInputType output;
	
	input.position.w = 1.0f;
	output.position = mul(input.position, WVP);
	// Пишем позицию в depthPosition
	output.depthPosition = output.position;
	
	return output;
}