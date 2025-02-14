

#ifndef GRAPHICEDITOR_SHADERSSOURCE_H
#define GRAPHICEDITOR_SHADERSSOURCE_H

#include "../structs.h"


const GLchar* vertexShaderSource = R"(
#version 120

uniform mat3 normalMatrix;
varying vec4 diffuse;
varying vec3 objectColor;

void main()
{
    gl_Position = ftransform();
    //objectColor = gl_Vertex.xyz;
    vec3 normal = gl_Normal * gl_NormalMatrix;
    vec3 lightVector = gl_LightSource[0].position.xyz - gl_Vertex.xyz;
    float dist = length(lightVector);

    float attenuation = 1.0 / (gl_LightSource[0].constantAttenuation +
                                gl_LightSource[0].linearAttenuation * dist +
                                gl_LightSource[0].quadraticAttenuation * dist * dist);
    lightVector = normalize(lightVector);
    float nxDir = max(0.0, dot(normal, lightVector));

    diffuse = gl_LightSource[0].diffuse * nxDir * attenuation;
}
)";

const GLchar* fragmentShaderSource = R"(
#version 120

varying vec3 objectColor;
uniform float lightIntensity;

varying vec4 diffuse;

void main() {
    vec3 color = vec3(1.0, 0.2, 0.2);

    gl_FragColor = ((diffuse * vec4(color, 1.0)));
}
)";


#endif //GRAPHICEDITOR_SHADERSSOURCE_H
