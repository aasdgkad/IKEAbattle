uniform sampler2D texture;

void main()
{
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
    
    // If the pixel has any opacity (not fully transparent)
    if (pixel.a > 0.0)
    {
        // Change it to yellow but keep the alpha
        gl_FragColor = vec4(1.0, 1.0, 0.0, pixel.a);
    }
    else
    {
        // Keep it fully transparent
        gl_FragColor = pixel;
    }
}