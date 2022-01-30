# dice-mosaic
Generate dice mosaics from jpeg/png images. Written in C.

Default options will attempt to read from 'input.jpg' in the same directory, scale the image by 0.05, then convert each pixel to grayscale before generating a dice mosaic using a mix of black and white dice and saving the dice mosaic to 'output.jpg'.

For all black die(w/ white dots) use './dice-mosaic -c b'

For all white die(w/ black dots) use './dice-mosaic -c w'



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
-x [scale_factor]
<ul>
Factor to scale input image by. Aspect ratio is preserved. Must be greater than 0. After scaling, each pixel cooresponds to a single dice. (this is the default scaling behavior if -x, -w, -h or -m, are not specified) 
</ul><ul>
Default is 0.05
</ul><ul>              
Cannot be used in conjunction with -w, -h, or -m.
</ul>
-w [width]
<ul>
Alternative scaling option, will scale an image to a specified number of die in width while preserving aspect ratio. Must be an integer greater than 0.
</ul><ul>
Cannot be used in conjunction with -x, -h, or -m.
</ul>
-h [height]
<ul>
Alternative scaling option, will scale an image to a specified number of die in height while preserving aspect ratio. Must be an integer greater than 0.
</ul><ul>
Cannot be used in conjunction with -x, -w, or -m.
</ul>
-m [max_allowable_die]
<ul>
Alternative scaling option, will scale the input image as large as possible while not exceed the specified number of die. Aspect ratio is preserved. Must be an integer greater than 0.
</ul><ul>
Cannot be used in conjunction with -x, -w, or -h.
</ul>
-l [filename]
<ul>
Will output a text file containing a list of dice values.
</ul><ul>
If no file is specified the list will be saved in 'output.txt'.
</ul>
-c [m,b,w]
<ul>
For mix of black and white die use './dice-mosaic -c m'. (this is the default if -c is not specified) 
</ul><ul>
For all black die(w/ white dots) use './dice-mosaic -c b'
</ul><ul>
For all white die(w/ black dots) use './dice-mosaic -c w'
</ul>
-f
<ul>
Invert(flip) the colors of the image.
</ul>
-j [jpg_quality]
<ul>
Set the output image's JPEG quality setting. Integer from 0-100. Default is 85.
</ul><ul>
Cannot be used in conjunction with -p.
</ul>
-p
<ul>
Set the output image format to png.
</ul><ul>
Cannot be used in conjunction with -j.
</ul>
-d [cost_per_dice]
<ul>
When this input is given the program will calculate the total cost of the mosaic in USD.
</ul>