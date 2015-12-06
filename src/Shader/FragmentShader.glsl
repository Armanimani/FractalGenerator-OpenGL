#version 430

uniform float x_min;
uniform float x_max;
uniform float y_min;
uniform float y_max;
uniform float maxItteration; 
uniform vec2 C;
uniform float bound;

in vec2 position;

out vec4 dacolor;

void main(){

	vec2 newPosition = vec2(x_min + (x_max - x_min) * 0.5 * (position.x + 1),
							  y_min + (y_max - y_min) * 0.5 * (position.y + 1));
	
	vec2 temp = vec2(1.0,1.0);

	float it = 0;

	for (it = 0; it != 1000; ++it){
				
		temp = vec2(newPosition.x * newPosition.x - newPosition.y * newPosition.y + C.x ,
					  2 * newPosition.x * newPosition.y + C.y);
		newPosition = temp;
		if (length(temp) > bound) break;

	}

	it = mod(it,255);

	vec4 color;
	
	if( it < 63){
		
		color = vec4((0 + it / 63 * 32) / 255, (7 + it / 63 * 100) / 255, (100 + it / 63 * 103) / 255 , 1.0 );
	}
	else if (it >=63 && it < 127){

		color = vec4((32 + (it - 63) / 63 * 205) / 255, (107 + (it - 63) / 63 * 148) / 255, (203 + (it - 63) / 63 * 52) / 255 , 1.0 );
	}
	else if(it >=127 && it < 191 ){
		
		color = vec4((237 + (it - 127) / 63 * 18) / 255, (255 - (it - 127) / 63 * 85) / 255, (255 - (it - 127) / 63 * 255) / 255 , 1.0 );
	}
	else{
		
		color = vec4((255 - (it - 191) / 63 * 255) / 255, (170 - (it - 191) / 63 * 168) / 255, 0.0 , 1.0 );
	}

	dacolor = color;
};