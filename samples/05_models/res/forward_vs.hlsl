// Model transform
cbuffer Data : register(b0)
{
  float4x4 mvp;
};

// Light information
cbuffer LightData : register(b1)
{
  float4 pos;
  float4 col;
};

// ==========================================================================//

// Input to vertex shader
struct VsInput
{
  float3 pos : POSITION;
  float3 normal : NORMAL;
  float2 uv : TEXCOORD;
};

// Input to pixel shader
struct PsInput
{
  float4 pos : SV_POSITION;
  float4 normal : NORMAL;
  float2 uv : TEXCOORD;
};

// ==========================================================================//

// Main
PsInput
main(VsInput input)
{
  PsInput output;
  output.pos = mul(float4(input.pos, 1.0f), mvp);
  output.normal = float4(input.normal, 0.0f);
  output.uv = input.uv;
  return output;
}