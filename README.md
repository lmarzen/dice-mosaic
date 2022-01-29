# dice-mosaic
dice-mosaic  -  Generate dice mosaics from jpeg/png images. Written in C.
                GNU General Public License v3.0.

Options
---
-i [filename]
<ul>
Input image(jpeg/png) filename or path.
</ul><ul>
Default 'input.jpg'
</ul>

-o [filename]
<ul>
Output image filename or path. Output format is .jpg.
</ul><ul>
Default 'output.jpg`
</ul>
-s [number]
<ul>
Factor to scale input image by. Must be greater than 0. After scaling, 1 pixel is equivalent to 1 dice.
</ul><ul>
Default is 0.05
</ul><ul>              
Cannot be used in conjunction with `-w` or `-h`.
</ul>
-w [number]
<ul>
Alternative scaling option, will scale an image to a specified number of dice in width while maintaining aspect ratio. Must be an integer greater than 0.
</ul><ul>
Cannot be used in conjunction with `-s` or `-h`.
</ul>
-h [number]
<ul>
Alternative scaling option, will scale an image to a specified number of dice in height while mainataining aspect ratio. Must be an integer greater than 0.
</ul><ul>
Cannot be used in conjunction with `-s` or `-w`.
</ul>