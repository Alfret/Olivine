// Textures
Texture2D tex_albedo : register(t0);
Texture2D tex_roughness : register(t1);
Texture2D tex_metallic : register(t2);
Texture2D tex_normal : register(t3);
SamplerState samp : register(s0);

// Camera information
cbuffer Camera : register(b2)
{
  float4x4 proj_mat;
  float4x4 view_mat;
};

// Light data (Single light at the moment)
cbuffer LightData : register(b1)
{
  float4 light_pos;
  float4 light_col;
};

// ==========================================================================//

static const float PI = 3.14159265f;
static const float GAMMA = 2.2f;

// ==========================================================================//

// Input from vertex shader
struct PsInput
{
  float4 pos : SV_POSITION;
  float3 world_pos : POSITION;
  float3 normal : NORMAL;
  float2 uv : TEXCOORD;
};

// ==========================================================================//

float4
sample_tex_linear(Texture2D tex, float2 uv)
{
  float4 output = tex.Sample(samp, uv);
  output.rgb = pow(output.rgb, float3(GAMMA, GAMMA, GAMMA));
  return output;
}

float4
sample_tex(Texture2D tex, float2 uv)
{
  return tex.Sample(samp, uv);
}

// ==========================================================================//

float
distribution_ggx(float dotNH, float roughness)
{
  float alpha = roughness * roughness;
  float alpha2 = alpha * alpha;
  float denom = dotNH * dotNH * (alpha2 - 1.0) + 1.0;
  float num = alpha2;
  return num / (PI * denom * denom);
}

float
geometry_schlick__smith_ggx(float dotNL, float dotNV, float roughness)
{
  float R = (roughness + 1.0);
  float K = (R * R) / 8.0;
  float GL = dotNL / (dotNL * (1.0 - K) + K);
  float GV = dotNV / (dotNV * (1.0 - K) + K);
  return GL * GV;
}

float3
fresnel_schlick(float cosTheta, float3 albedo, float3 metallic)
{
  float3 base = 0.04;
  float3 F0 = lerp(base, albedo, metallic);
  float3 F = F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
  return F;
}

// ==========================================================================//

// Main
float4
main(PsInput input)
  : SV_TARGET
{
  // TMP: Camera pos (Calculations should just be done in view space)
  float3 camera_pos = float3(0.0f, 0.0f, 0.0f);

  // Material properties
  float3 albedo = float3(1.0, 0.0, 0.0);
  float roughness = 1.0;
  float metallic = 1.0;
  // float3 albedo = sample_tex_linear(tex_albedo, input.uv).rgb;
  // float roughness = sample_tex(tex_roughness, input.uv).r;
  // float metallic = sample_tex(tex_metallic, input.uv).r;

  float3 N = normalize(input.normal);
  float3 V = normalize(camera_pos - input.world_pos);
  float3 L = normalize(light_pos.xyz - input.world_pos);

  // Cook-Torrance BRDF
  float3 H = normalize(V + L);
  float dotNH = clamp(dot(N, H), 0.0, 1.0);
  float dotNV = clamp(dot(N, V), 0.0, 1.0);
  float dotNL = clamp(dot(N, L), 0.0, 1.0);

  float R = max(0.05, roughness);
  float D = distribution_ggx(dotNH, roughness);
  float G = geometry_schlick__smith_ggx(dotNL, dotNV, roughness);
  float3 F = fresnel_schlick(dotNV, albedo, metallic);
  float3 specular = (D * G * F) / (4.0 * dotNL * dotNV);
  float3 Lo = 0.0;
  if (dotNL > 0.0) {
    Lo += (specular * dotNL * light_col.xyz);
  }
  float3 ambient = (albedo * 0.02);

  // Output gamma corrected
  float3 output = ambient + Lo;
  output = output / (output + 1.0);
  output = pow(output, float3(1.0 / GAMMA, 1.0 / GAMMA, 1.0 / GAMMA));
  return float4(output, 1.0f);
}