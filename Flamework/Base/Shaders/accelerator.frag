uniform mediump float deltaT;
uniform mediump vec4 ColorVector;

uniform mediump mat4 ViewMatrix;
uniform mediump mat4 ModelMatrix;
uniform mediump mat4 ProjectionMatrix;

uniform mediump mat3 NormalMatrix;

uniform mediump vec4 LightPos;
uniform mediump vec4 EyePos;

uniform lowp vec3 Ka;   // ambient material coefficient
uniform lowp vec3 Kd;   // diffuse material coefficient
uniform lowp vec3 Ks;   // specular material coefficient

uniform mediump float Ns;   // specular material exponent (shininess)

uniform lowp vec3 Ia;   // ambient light intensity
uniform lowp vec3 Id;   // diffuse light intensity
uniform lowp vec3 Is;   // specular light intensity

uniform sampler2D DiffuseMap;
uniform sampler2D SpecularMap;
uniform sampler2D NormalMap;

varying lowp vec4 ambientVarying;
varying lowp vec4 diffuseVarying;
varying lowp vec4 specularVarying;
varying lowp vec4 texCoordVarying;

varying mediump vec4 posVarying;        // pos in world space
varying mediump vec3 normalVarying;     // normal in world space
varying mediump vec3 tangentVarying;    // tangent in world space
varying lowp vec4 colorVarying;

void main()
{
    // TBN-Matrix
    mediump vec3 n = normalize(normalVarying);
    mediump vec3 t = normalize(tangentVarying - dot(tangentVarying, n));
    mediump vec3 b = cross(n, t);
    mediump mat3 tbn = mat3(t, b, n);
    
    // Bumpy normal
    mediump vec3 bumpNormal = texture2D(NormalMap, texCoordVarying.xy).xyz;
    bumpNormal = 2.0*bumpNormal - vec3(1.0);
    n = normalize(tbn*bumpNormal);
    
    // Ambient
    lowp vec4 ambientResult = vec4(Ka * Ia, 1.0);
    
    // Diffuse
    mediump vec3 l = normalize(LightPos - posVarying).xyz;
    lowp float intensity = dot(n, l);
    lowp vec3 diffuse = Kd * clamp(intensity, 0.0, 1.0) * Id;
    lowp vec4 diffuseResult = vec4(clamp(diffuse, 0.0, 1.0), 1.0);
    
    // Specular (Blinn-Phong)
    lowp vec4 specularResult = vec4(0.0);
    if (intensity > 0.0)
    {
        mediump vec3 eyeVec = normalize(EyePos.xyz - posVarying.xyz);
        mediump vec3 h = normalize(l + eyeVec);
        mediump float specIntensity = pow(max(dot(h,n), 0.0), Ns);
        mediump vec3 spec = texture2D(SpecularMap, texCoordVarying.st).rgb;
        mediump vec3 specular = Ks * spec * clamp(specIntensity, 0.0, 1.0) * Is;
        specularResult = vec4(clamp(specular, 0.0, 1.0), 1.0);
    }
    
    // Make the texture move
    lowp vec4 color = texture2D(DiffuseMap, texCoordVarying.xy + vec2(0,-deltaT));
    gl_FragColor = (ambientResult + diffuseResult)*color;
}
