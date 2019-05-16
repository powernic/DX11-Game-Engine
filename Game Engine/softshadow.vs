cbuffer MatrixBuffer
{
	matrix world;
	matrix wvp;
};

cbuffer LightBuffer2
{
	float3 lightPosition;
	float padding;
	float3 lightPosition2;
	float padding2;
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
	float4 lightViewPosition2 : TEXCOORD3;
	float3 lightPos2 : TEXCOORD4;
};

PixelInputType VS(VertexInputType input)
{
	PixelInputType output;
        
	input.position.w = 1.0f;

	output.position = mul(input.position, wvp); 

	output.tex = input.tex;
    
	output.normal = mul(input.normal, (float3x3)world);
	output.normal = normalize(output.normal);

	// ���������� ������� ������� � ����
	float4 worldPosition = mul(input.position, world);

	// ����������� ������� ����� �� ������ ������� ����� � ������� ������� � ����
	output.lightPos = lightPosition.xyz - worldPosition.xyz;
	output.lightPos = normalize(output.lightPos);
	output.lightPos2 = lightPosition2.xyz - worldPosition.xyz;
	output.lightPos2 = normalize(output.lightPos2);

	return output;
}