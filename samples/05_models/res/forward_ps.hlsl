Texture2D tex : register(t0);
Texture2D tex_roughness : register(t1);
Texture2D tex_metallic : register(t2);
Texture2D tex_normal : register(t3);
SamplerState samp : register(s0);

struct PsInput
{
  float4 pos : SV_POSITION;
  float4 normal : NORMAL;
  float2 uv : TEXCOORD;
};

float4
main(PsInput input)
  : SV_TARGET
{
  float4 roughness = tex_roughness.Sample(samp, input.uv);
  float4 metallic = tex_metallic.Sample(samp, input.uv);
  float4 normal = tex_normal.Sample(samp, input.uv);

  float4 extra =
    float4(0.001, 0.001, 0.001, 0.001) * (roughness + metallic + normal);

  return tex.Sample(samp, input.uv) + extra;
}