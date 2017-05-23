

cbuffer cbPerObject
{
	float4x4 WVP;
};

void VS(float3 iPosL: POSITION, float3 iColor : COLOR, out float4 oPosH:SV_POSITION, out float3 oColor:COLOR)
{
	oPosH = mul(float4(iPosL, 1.0f), WVP);
	oColor = iColor;
}


float3 PS(float4 posH: SV_POSITION, float3 color:COLOR):SV_Target
{
	return color;
}



technique10 ColorTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_4_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS()));
	}
}