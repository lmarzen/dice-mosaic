# dice-mosaic
Generate dice mosaics from jpeg/png images. Written in C.

Default options will attempt to read from 'input.jpg' in the same directory, scale the image by 0.05, then convert each pixel to grayscale before generating a dice mosaic using a mix of black and white dice and saving the dice mosaic to 'output.jpg'.

For all black die(w/ white dots) use 'dice-mosaic -c b'

For all white die(w/ black dots) use 'dice-mosaic -c w'



GNU General Public License v3.0.

Options
---
-i [filename]
<ul>
Input image(jpeg/png) filename or path.
</ul><ul>
Default is 'input.jpg'
</ul>
-o [filename]
<ul>
Output image filename or path. Output format is .jpg.
</ul><ul>
Default  is 'output.jpg`
</ul>
-s [number]
<ul>
Factor to scale input image by. Must be greater than 0. After scaling, 1 pixel is equivalent to 1 dice.
</ul><ul>
Default is 0.05
</ul><ul>              
Cannot be used in conjunction with -w or -h.
</ul>
-w [number]
<ul>
Alternative scaling option, will scale an image to a specified number of dice in width while maintaining aspect ratio. Must be an integer greater than 0.
</ul><ul>
Cannot be used in conjunction with -s or -h.
</ul>
-h [number]
<ul>
Alternative scaling option, will scale an image to a specified number of dice in height while mainataining aspect ratio. Must be an integer greater than 0.
</ul><ul>
Cannot be used in conjunction with -s or -w.
</ul>
-l [filename]
<ul>
Will output a text file containing a list of dice values.
</ul><ul>
If no file is specified the list will be saved in 'output.txt'.
</ul>
-c [b,w]
<ul>
For all black die(w/ white dots) use 'dice-mosaic -c b'
</ul><ul>
For all white die(w/ black dots) use 'dice-mosaic -c w'
</ul><ul>
Default is to use a mix of both black and white to achieve 12 grayscale values.
</ul>
-f
<ul>
Invert(flip) the colors of the image.
</ul>