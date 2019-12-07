// Textures
Texture2D tex_albedo : register(t0);
Texture2D tex_roughness : register(t1);
Texture2D tex_metallic : register(t2);
Texture2D tex_normal : register(t3);
SamplerState samp : register(s0);

// Light data (Single light at the moment)
cbuffer LightData : register(b1)
{
  float4 light_pos;
  float4 light_col;
};

// ==========================================================================//

static float GAMMA = 2.2f;

// ==========================================================================//

// Input from vertex shader
struct PsInput
{
  float4 pos : SV_POSITION;
  float4 normal : NORMAL;
  float2 uv : TEXCOORD;
};

// ==========================================================================//

float4
sample_to_linear(Texture2D tex, float2 uv)
{
  float4 output = tex.Sample(samp, uv);
  output.rgb = pow(output.rgb, float3(GAMMA, GAMMA, GAMMA));
  return output;
}

// ==========================================================================//

// Main
float4
main(PsInput input)
  : SV_TARGET
{
  // TMP: We don't want the textures to be optimized away while testing
  float4 roughness = sample_to_linear(tex_roughness, input.uv);
  float4 metallic = sample_to_linear(tex_metallic, input.uv);
  float4 normal = sample_to_linear(tex_normal, input.uv);
  float4 extra =
    float4(0.001, 0.001, 0.001, 0.001) * (roughness + metallic + normal);
  extra *= light_pos;

  // Output gamma corrected
  float4 output = sample_to_linear(tex_albedo, input.uv) + extra;
  output.rgb = pow(output.rgb, float3(1.0 / GAMMA, 1.0 / GAMMA, 1.0 / GAMMA));
  return output;
}