uniform sampler2D m_HeightMapV;
uniform sampler2D m_ColourMapP;

varying vec4 position; //for projection
varying vec4 worldPosition;
varying vec2 texCoord;

// Height scale
uniform float m_TileScale;

    
void main(){

	vec4 height = texture2D(m_HeightMapV, texCoord);
	
	texCoord.x = 0.01 + (fract(texCoord.x * m_TileScale * 4.0)) * 0.23;
	texCoord.y = fract(texCoord.y * m_TileScale * 4.0);
	
	
	// Red channel contains the material nbr.
	texCoord.x += (height.b * 64.0);
	
	vec4 color = texture2D(m_ColourMapP, texCoord);
	
	float dist = exp(-position.z /  10000.0);
	
	gl_FragColor =  mix(vec4(0.9, 0.95, 1.0, 1.0), color, dist);
}