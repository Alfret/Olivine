// Model transform
cbuffer Data : register(b0)
{
  float4x4 model_mat;
};

// Camera information
cbuffer Camera : register(b2)
{
  float4x4 proj_mat;
  float4x4 view_mat;
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
  float3 world_pos : POSITION;
  float3 normal : NORMAL;
  float2 uv : TEXCOORD;
};

// ==========================================================================//

float3x3
to_3x3(float4x4 mat)
{
  float3x3 o = {
    mat._m00_m01_m02,
    mat._m10_m11_m12,
    mat._m20_m21_m22,
  };
  return o;
}

// ==========================================================================//

// Main
PsInput
main(VsInput input)
{
  float4x4 mvp = mul(mul(model_mat, view_mat), proj_mat);

  PsInput output;
  output.world_pos = mul(float4(input.pos, 1.0f), model_mat).xyz;
  output.pos = mul(float4(output.world_pos, 1.0f), mul(view_mat, proj_mat));
  output.normal = mul(input.normal, to_3x3(model_mat));
  output.uv = input.uv;
  return output;
}