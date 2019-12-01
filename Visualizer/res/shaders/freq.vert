#version 330 core

layout(location = 0) in vec4 a_Vertex;
layout(location = 1) in vec4 a_Dims;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in float a_Rot;

uniform mat4 u_ViewProjection;

out vec4 v_Color;

mat4 rotateZ(in float angle);
mat4 scale(in vec2 dims);
mat4 translate(in vec2 pos);

void main(){
	gl_Position = u_ViewProjection * translate(a_Dims.xy) * rotateZ(a_Rot) * scale(a_Dims.zw) * a_Vertex;
	v_Color = a_Color;
}

mat4 rotateZ( in float angle ){
	return mat4(cos(angle),	-sin(angle), 0,	0,
			 	sin(angle),	 cos(angle), 0,	0,
						 0,			  0, 1,	0,
						 0,			  0, 0,	1);
}

mat4 scale(in vec2 dims){
	return mat4(dims.x,		 0, 0, 0,
			 		 0,	dims.y, 0, 0,
					 0,		 0, 1, 0,
					 0,		 0, 0, 1);
}

mat4 translate(in vec2 dims){
    return mat4(
        1.0,	   0.0, 0.0, 0.0,
        0.0,	   1.0, 0.0, 0.0,
        0.0,	   0.0, 1.0, 0.0,
        dims.x, dims.y,   0, 1.0
    );
}

