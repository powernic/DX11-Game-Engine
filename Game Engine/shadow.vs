cbuffer MatrixBuffer
{
	matrix world;
	matrix wvp;
	matrix wvplight;
};

cbuffer LightBuffer2
{
	float3 lightPosition;
	float padding;
};

struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 lightViewPosition : TEXCOORD1;
	float3 lightPos : TEXCOORD2;
};

PixelInputType VS(VertexInputType input)
{
	PixelInputType output;
        
	input.position.w = 1.0f;

	output.position = mul(input.position, wvp); 
	output.lightViewPosition = mul(input.position, wvplight);

	output.tex = input.tex;
    
	output.normal = mul(input.normal, (float3x3)world);
	output.normal = normalize(output.normal);

	// Вычисление позиции вершины в мире
	float4 worldPosition = mul(input.position, world);

	// Определение позиции света на основе позиции света и позиции вершины в мире
	output.lightPos = lightPosition.xyz - worldPosition.xyz;
	output.lightPos = normalize(output.lightPos);

	return output;
}