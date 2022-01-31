# dice-mosaic
Generate dice mosaics from jpeg/png images. Portable and written in C.

Default options will attempt to read from 'input.jpg' in the same directory, scale the image by 0.05, then convert each pixel to grayscale before generating a dice mosaic using  black die(w/ white dots) and saving the dice mosaic to 'output.png'.



For a mix of black and white die use './dice-mosaic -g m'.

GNU General Public License v3.0.

Usage
---
Example: `./dice-mosaic -i images/moon.jpg -o images/moon_black_dice.png`

![moon black dice]](images/moon_black_dice.png)

For all white die(w/ black dots) use the '-g w' flag. './dice-mosaic -g w'

Example: `./dice-mosaic -i images/moon.jpg -o images/moon_white_dice.png -g w`

![moon white dice]](images/moon_white_dice.png)



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
Output image filename or path. Default output format is .png.
</ul><ul>
Default  is 'output.png`
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
-g [m,b,w]
<ul>
For all black die(w/ white dots) use './dice-mosaic -g b'. (this is the default if -g is not specified)
</ul><ul>
For all white die(w/ black dots) use './dice-mosaic -g w'.
</ul><ul>
For a mix of black and white die use './dice-mosaic -g m'.
</ul>
-j [jpg_quality]
<ul>
Set the output image's JPEG quality setting. Integer from 0-100. High quality is 80-90.
</ul><ul>
Cannot be used in conjunction with -p.
</ul>
-p
<ul>
Set the output image format to png. (this is the default if -j or -p is not specified)
</ul><ul>
Cannot be used in conjunction with -j.
</ul>
-c [contrast_modifier]
<ul>
Change the contrast. a > 1 means more contrast, 0 < a < 1 means less contrast.
</ul><ul>
The value of a pixel is determined by the following function: f(x) = a(x) + b
</ul><ul>
Default is a=1.0
</ul>
-b [brightness_modifier]
<ul>
Change the brightness.
</ul><ul>
The value of a pixel is determined by the following function: f(x) = a(x) + b
</ul><ul>
Default is b=0.0
</ul>
-r [dice_resolution]
<ul>
Set the resolution of the dice that make up the mosaic. [dice_resolution] is the length of the sides of each dice. Must be an integer that is at least 12.
</ul><ul>
Default is 40
</ul>
-d [cost_per_dice]
<ul>
When this input is given the program will calculate the total cost of the mosaic. (Assumes USD)
</ul>
-s [dice_size]
<ul>
When this input is given the program will calculate the total size of the mural. (Assumes inches) (0.63in (16mm) is most common)
</ul>