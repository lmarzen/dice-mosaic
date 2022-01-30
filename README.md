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
-s [x,w,h] [number]
<ul>
Scaling option determines how to (down)scale the input image before it is converted to dice. Aspect ratio is always maintained. After scaling, each pixel will correspond to a single dice.
</ul><ul>
-s x [scale_factor] will scale the input image by a given factor. Must be greater than 0. Default is 0.05 (this is the default scaling behavior if -s is not specified)
</ul><ul>
-s w [width] will scale the input image to the specified width while maintaining the aspect ratio. Must be an integer greater than 0.
</ul><ul>
-s h [height] will scale the input image to the specified height while maintaining the aspect ratio. Must be an integer greater than 0.
</ul><ul>
-s d [max_number_of_die] will scale the input image as large as possible while not exceed the specified number of die. Must be an integer greater than 0.
</ul>
-l [filename]
<ul>
Will output a text file containing a list of dice values.
</ul><ul>
If no file is specified the list will be saved in 'output.txt'.
</ul>
-c [m,b,w]
<ul>
For mix of black and white die use 'dice-mosaic -c m'. (this is the default if -c is not specified)
</ul><ul>
For all black die(w/ white dots) use 'dice-mosaic -c b'.
</ul><ul>
For all white die(w/ black dots) use 'dice-mosaic -c w'.
</ul>
-f
<ul>
Invert(flip) the colors of the image.
</ul>