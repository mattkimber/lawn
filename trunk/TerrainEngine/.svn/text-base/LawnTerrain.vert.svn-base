// Values passed in to the shader
uniform mat4 g_WorldViewProjectionMatrix;

uniform sampler2D m_HeightMapV;

// Height scale
// No longer used - scaling done perfectly
// well by worldview matrix
//uniform float m_Scale;

//attribute vec3 inPosition;
//attribute vec2 inTexCoord;

varying vec4 position;
varying vec4 worldPosition;
varying vec2 texCoord;


//vec4 tex2D_bilinear( in sampler2D tex, vec2 t )
//{
	//vec2 f = fract( t.xy * textureSize );
	//vec4 t00 = texture2D( tex, t );
	//vec4 t10 = texture2D( tex, t + vec2( texelSize, 0.0 ));
	//vec4 tA = mix( t00, t10, f.x );
	//vec4 t01 = texture2D( tex, t + vec2( 0.0, texelSize ) );
	//vec4 t11 = texture2D( tex, t + vec2( texelSize, texelSize ) );
	//vec4 tB = mix( t01, t11, f.x );
	//return mix( tA, tB, f.y );
//}	

float sampleHeightmap( in vec2 uv )
{
	vec4 col = texture2DLod( m_HeightMapV, uv, 0 );
	return col.g + (col.r / 255.0); 
}

void main(){
	
	texCoord = gl_MultiTexCoord0;
	worldPosition = gl_Vertex;

	position = g_WorldViewProjectionMatrix * worldPosition;
    
	worldPosition = vec4(gl_Vertex);
	worldPosition.y += sampleHeightmap( texCoord ) ;

	position = g_WorldViewProjectionMatrix * worldPosition;
	gl_Position = position;
}
